#include <math.h>
#include <string.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>

#include "Tensor.h"
#include "GINFO_VAR.h"

// 参考 http://t.zoukankan.com/ssyfj-p-13964588.html
/**
* 最大池化
* 说明: 根据输入的池化参数进行最大池化操作.
* @param[in]  conved_Tsr   -> 被池化张量结构体
* @param[in]  pool_size    -> 池化核尺寸
* @param[in]  strides      -> 池化核步长
* @param[in]  pool_mode    -> 池化模式 [VALID \ SAME]
* @param[in]  pool_result  -> 池化结果张量
* @param[out] Null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Apr. 21, 2022  
*/
void MaxPooling2D(Tensor *conved_Tsr, POOLSIZE *pool_size, POOLSTRIDE *strides, char *pool_mode, Tensor *pool_result)
{
    // 对每个张量, 对每个通道单独池化, 但应先定义结果张量和结果矩阵, 对每个通道赋值给矩阵, 然后矩阵卷积之后赋值给张量
    int i, j, k, h;
    int ext_pool_flag = 0; // 默认不使用填0张量, 当SAME模式不能整除时, 置1进行单独运算

    // 池化核的尺寸
    int ck_row = pool_size->row;
    int ck_col = pool_size->col;

    int pool_result_row; // 结果张量的行数
    int pool_result_col; // 结果张量的列数

    int ext_len_row = 0; // 延拓张量补0行宽
    int ext_len_col = 0; // 延拓张量补0列宽

    // 根据池化模式确定结果张量的尺寸, 以及补0的位宽
    if (strcmp(pool_mode, VALID) == 0)
    {
        // TODO 需确认
        // Valid模式不会补0, 因此可能会出现无法遍历所有输入数据的情况
        pool_result_row = (((conved_Tsr->row) - ck_row) / (strides->row)) + 1;
        pool_result_col = (((conved_Tsr->col) - ck_col) / (strides->col)) + 1;
        // printf("池化结果尺寸 %d, %d\n", pool_result_row, pool_result_col);

    }
    else if (strcmp(pool_mode, SAME) == 0)
    {
        // TODO 需确认
        // Same模式在除不尽的时候会补0
        pool_result_row = (((conved_Tsr->row) - ck_row - 1) / (strides->row) + 1) + 1;
        pool_result_col = (((conved_Tsr->col) - ck_col - 1) / (strides->col) + 1) + 1;

        // 补0个数, 高(宽)可以被步长整除
        int conved_Tsr_mod_stride_row = (((conved_Tsr->row) - ck_row) % (strides->row));
        if (conved_Tsr_mod_stride_row != 0) 
        {
            ext_len_row = (strides->row) - conved_Tsr_mod_stride_row;
            ext_pool_flag = 1;
        }
        int conved_Tsr_mod_stride_col = (((conved_Tsr->col) - ck_col) % (strides->col));
        if (conved_Tsr_mod_stride_col != 0) 
        {
            ext_len_col = (strides->col) - conved_Tsr_mod_stride_col;
            ext_pool_flag = 1;
        }
    }
    else
    {
        fprintf(stderr, "ERROR: MaxPooling2D: pool_mode=>[SAME, VALID]\n");
        exit(EXIT_FAILURE);
    }

    // 不适用延拓矩阵参与训练
    if (ext_pool_flag == 0)
    {
        // 定义结果张量的尺寸
        TENSOR4_SHAPE[0] = conved_Tsr->tsnum;
        TENSOR4_SHAPE[1] = conved_Tsr->chnum; 
        TENSOR4_SHAPE[2] = pool_result_row;  // 输出张量的行数为 conved_Tsr->row + 延拓半宽*2
        TENSOR4_SHAPE[3] = pool_result_col;  // 输出张量的列数为 conved_Tsr->col + 延拓半宽*2

        // printf("结果卷积\n");
        TensorInitial(pool_result, TENSOR4_STR, TENSOR4_SHAPE);
        // TensorPaddingZero(conv_result);

        // 最大池化具体过程
        int cd_i, cd_j, cd_k, cd_h;  // conved_Tsr
        int pl_k = 0, pl_h = 0;  // pool_result
        int pl_r, pl_c;  // pool operation  
        DATA pl_result_tmp = NInf;         

        //                              个    通道  行    列
        // 被池化张量索引 conved_Tsr    [cd_i][cd_j][cd_k][cd_h]
        // 池化结果索引   pool_result   [cd_i][cd_j][pl_k][pl_h]
        // int temp=0;
        for (cd_i = 0; cd_i < conved_Tsr->tsnum; cd_i++)
        {
            for (cd_j = 0; cd_j < conved_Tsr->chnum; cd_j++)
            {
                // 遍历当前通道矩阵
                // 从核末尾开始计算, 步长为池化步长, 直到边界
                pl_k = 0;
                for (cd_k = (ck_row-1); cd_k < (conved_Tsr->row); cd_k = (cd_k+(strides->row)))
                {
                    pl_h = 0;
                    for (cd_h = (ck_col-1); cd_h < (conved_Tsr->col); cd_h = (cd_h+(strides->col)))
                    {
                        pl_result_tmp = NInf;
                        // cd_k\cd_h 为池化核右下角的 行\列 坐标, 向上\左操作
                        for (pl_r = cd_k; pl_r > (cd_k-ck_row); pl_r--)
                        {
                            for (pl_c = cd_h; pl_c > (cd_h-ck_col); pl_c--)
                            {
                                if ((conved_Tsr->data[cd_i][cd_j][pl_r][pl_c]) > pl_result_tmp)
                                {
                                    
                                    pl_result_tmp = (conved_Tsr->data[cd_i][cd_j][pl_r][pl_c]);
                                }
                            }
                        }
                        pool_result->data[cd_i][cd_j][pl_k][pl_h] = pl_result_tmp;
                        pl_h++;
                    }
                    pl_k++;
                }
            }
        }
    }
    else
    {
        // 定义结果张量的尺寸
        TENSOR4_SHAPE[0] = conved_Tsr->tsnum;
        TENSOR4_SHAPE[1] = conved_Tsr->chnum; 
        TENSOR4_SHAPE[2] = pool_result_row; 
        TENSOR4_SHAPE[3] = pool_result_col;  
        // printf("结果卷积\n");
        TensorInitial(pool_result, TENSOR4_STR, TENSOR4_SHAPE);

        Tensor conved_Tsr_Tmp;
        TENSOR4_SHAPE[0] = conved_Tsr->tsnum;
        TENSOR4_SHAPE[1] = conved_Tsr->chnum; 
        TENSOR4_SHAPE[2] = conved_Tsr->row + ext_len_row; 
        TENSOR4_SHAPE[3] = conved_Tsr->col + ext_len_col;  
        // printf("%d, %d, %d, %d\n", TENSOR4_SHAPE[0], TENSOR4_SHAPE[1], TENSOR4_SHAPE[2], TENSOR4_SHAPE[3]);
        TensorInitial(&conved_Tsr_Tmp, TENSOR4_STR, TENSOR4_SHAPE);
        TensorPaddingZero(&conved_Tsr_Tmp);
        for (i = 0; i < (conved_Tsr->tsnum); i++)
        {
            for (j = 0; j < (conved_Tsr->chnum); j++)
            {
                for (k = 0; k < (conved_Tsr->row); k++)
                {
                    for (h = 0; h < (conved_Tsr->col); h++)
                    {
                        // printf("[%d][%d][%d][%d]\n", i, j, k + ext_half_len, h + ext_half_len);
                        conved_Tsr_Tmp.data[i][j][k][h] = conved_Tsr->data[i][j][k][h];
                    }
                }
            }
        }


        // 最大池化具体过程
        int cd_i, cd_j, cd_k, cd_h;  // conved_Tsr
        int pl_k = 0, pl_h = 0;  // pool_result
        int pl_r, pl_c;  // pool operation  
        DATA pl_result_tmp = NInf;    

        for (cd_i = 0; cd_i < conved_Tsr_Tmp.tsnum; cd_i++)
        {
            for (cd_j = 0; cd_j < conved_Tsr_Tmp.chnum; cd_j++)
            {
                // 遍历当前通道矩阵
                // 从核末尾开始计算, 步长为池化步长, 直到边界
                pl_k = 0;
                for (cd_k = (ck_row-1); cd_k < (conved_Tsr_Tmp.row); cd_k = (cd_k+(strides->row)))
                {
                    pl_h = 0;
                    for (cd_h = (ck_col-1); cd_h < (conved_Tsr_Tmp.col); cd_h = (cd_h+(strides->col)))
                    {
                        pl_result_tmp = NInf;
                        // cd_k\cd_h 为池化核右下角的 行\列 坐标, 向上\左操作
                        for (pl_r = cd_k; pl_r > (cd_k-ck_row); pl_r--)
                        {
                            for (pl_c = cd_h; pl_c > (cd_h-ck_col); pl_c--)
                            {
                                if ((conved_Tsr_Tmp.data[cd_i][cd_j][pl_r][pl_c]) > pl_result_tmp)
                                {
                                    
                                    pl_result_tmp = (conved_Tsr_Tmp.data[cd_i][cd_j][pl_r][pl_c]);
                                }
                            }
                        }
                        pool_result->data[cd_i][cd_j][pl_k][pl_h] = pl_result_tmp;
                        pl_h++;
                    }
                    pl_k++;
                }
            }
        }
        TensorFree(&conved_Tsr_Tmp);
    }
}
