#include <math.h>
#include <string.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>

#include "Tensor.h"
#include "GINFO_VAR.h"


void MaxPooling2D(Tensor *conved_Tsr, POOLSIZE *pool_size, CONVSTRIDE *strides, char *pool_mode)
{
    // 对每个张量, 对每个通道单独卷积, 但应先定义结果张量和结果矩阵, 对每个通道赋值给矩阵, 然后矩阵卷积之后赋值给张量
    int i, j, k, h;
    // int ci, cj, ck, ch; //卷积操作专用索引, 避免符号混乱

    int ck_row = pool_size->row;
    int ck_col = pool_size->col;

    int pool_result_row; // 结果张量的行数
    int pool_result_col; // 结果张量的列数

    // 根据池化模式确定结果张量的尺寸, 以及补0的位宽
    if (strcmp(pool_mode, SAME) == 0)
    {
        pool_result_row = ceil((conved_Tsr->row) / (strides->row));
        pool_result_col = ceil((conved_Tsr->col) / (strides->col));

        // 补0个数, 高(宽)可以被步长整除
    }
    else if (strcmp(pool_mode, VALID) == 0)
    {
        pool_result_row = ceil(((conved_Tsr->row) - (pool_size->row) + 1) / (strides->row));
        pool_result_col = ceil(((conved_Tsr->col) - (pool_size->col) + 1) / (strides->col));
    }
    else
    {
        fprintf(stderr, "ERROR: MaxPooling2D: pool_mode=>[FULL, SAME, VALID]\n");
        exit(EXIT_FAILURE);
    }

    // 池化结果的行(列)数大于输入张量的行(列)数, 确定延拓半宽
    // if ((pool_result_row > (conved_Tsr->row)) || (pool_result_col > (conved_Tsr->col)))
    // {
    //     /* code */
    //     ext_half_len = 
    // }
    
    // printf("延拓半宽: %d\n", ext_half_len);

    // 复制 conved_Tsr 到 extend_tensor 用于实际运算
    Tensor extend_tensor;
    TENSOR4_SHAPE[0] = conved_Tsr->tsnum;
    TENSOR4_SHAPE[1] = conved_Tsr->chnum;
    TENSOR4_SHAPE[2] = (conved_Tsr->row) + (ext_half_len<<1);
    TENSOR4_SHAPE[3] = (conved_Tsr->col) + (ext_half_len<<1);
    // printf("延拓增宽量 %d\n", ext_half_len<<1);
    // printf("延拓张量\n");
    TensorInitial(&extend_tensor, TENSOR4_STR, TENSOR4_SHAPE);
    TensorPaddingZero(&extend_tensor);

    for (i = 0; i < (conved_Tsr->tsnum); i++)
    {
        for (j = 0; j < (conved_Tsr->chnum); j++)
        {
            for (k = 0; k < (conved_Tsr->row); k++)
            {
                for (h = 0; h < (conved_Tsr->col); h++)
                {
                    // printf("[%d][%d][%d][%d]\n", i, j, k + ext_half_len, h + ext_half_len);
                    extend_tensor.data[i][j][k + ext_half_len][h + ext_half_len] = conved_Tsr->data[i][j][k][h];
                }
            }
        }
    }

    // for (i=0;i<extend_tensor.tsnum;i++)
    // {
    //     printf("个数 %d ============================================\n", i);
    //     for (j=0;j<extend_tensor.chnum;j++)
    //     {
    //         printf("通道 %d ------------------------------------------\n", j);
    //         for (k = 0; k < extend_tensor.row; k++)
    //         {
    //             for (h = 0; h < extend_tensor.col; h++)
    //             {
    //                 printf("%f, ", extend_tensor.data[i][j][k][h]);
    //             }
    //             printf("\n");
    //         }
    //         printf("\n\n");
    //     }
    //     printf("\n\n\n");
    // }

    // 池化结果张量<根据池化核与步长决定>
    // 参考: https://zhuanlan.zhihu.com/p/46744988?ivk_sa=1024320u
    TENSOR4_SHAPE[0] = conved_Tsr->tsnum;
    TENSOR4_SHAPE[1] = conved_Tsr->chnum;  // 输出张量的通道与卷积核通道数一致
    TENSOR4_SHAPE[2] = floor(((conved_Tsr->row) + (ext_half_len<<1) - ck_row) / conv_stride->row) + 1;  // 输出张量的行数为 conved_Tsr->row + 延拓半宽*2
    TENSOR4_SHAPE[3] = floor(((conved_Tsr->col) + (ext_half_len<<1) - ck_row) / conv_stride->col) + 1;  // 输出张量的列数为 conved_Tsr->col + 延拓半宽*2
    // printf("结果卷积\n");
    TensorInitial(conv_result, TENSOR4_STR, TENSOR4_SHAPE);
    TensorPaddingZero(conv_result);

    int cd_i, cd_j, cd_k, cd_h;  // conved_Tsr
    int ck_i, ck_j, ck_k, ck_h;  // conv_kel
    int rs_i, rs_j, rs_k, rs_h;  // conv_result
    int et_i, et_j, et_k, et_h;  // extend_mat

    /*
                                个   通道   行    列
    被卷积张量索引 conved_Tsr    [cd_i][cd_j][cd_k][cd_h]
    卷积核索引     conv_kel      [0   ][ck_j][ck_k][ck_h]
    卷积结果索引   conv_result   [rs_i][rs_j][rs_k][rs_h]
    拓展卷积索引   extend_tensor [et_i][et_j][et_k][et_h]
    */    
    // 参考: https://www.cnblogs.com/chumingqian/articles/11495364.html
    // 每个卷积核通道对应1个输出
    // TODO 可并行处理
    // TODO 需要确定 conv_stride 放置位置是否正确
    // printf("%d\n", conv_stride->row);
    // #pragma omp parallel for num_threads(6)
    for (cd_i = 0; cd_i < conved_Tsr->tsnum; cd_i++)   // 遍历"被卷积"个数 => 对应实际输出的个数
    {
        // printf("level - 1\n");
        for (ck_j = 0; ck_j < conv_kel->chnum; ck_j++) // 遍历"卷积核"通道 => 对应实际输出通道数
        {
            // printf("level - 2\n");
            // 每通道\行\列,对应的核大小内容进行卷积, 然后循环外 + bias
            for (cd_k = 0; cd_k < conv_result->row; cd_k++)     // 遍历"卷积结果张量"行 => 对应实际输出行 (+步长)
            {
                // printf("level - 3\n");
                for (cd_h = 0; cd_h < conv_result->col; cd_h++) // 遍历"卷积结果张量"列 => 对应实际输出列 (+步长)
                {
                    // printf("level - 4\n");
                    for (cd_j = 0; cd_j < conved_Tsr->chnum; cd_j++)     // 遍历"被卷积张量"通道
                    {
                        // printf("level - 5\n");
                        // 当前点为核中心, 对应的核大小张量数据, 卷积
                        // += extend_tensor.data[cd_i][cd_j][核范围][核范围] .* conv_kel->data[0][ck_j][核范围][核范围]
                        for (ck_k = 0; ck_k < conv_kel->row; ck_k++)     // 遍历"卷积核"行
                        {
                            // #pragma omp parallel 
                            // printf("level - 6\n");
                            for (ck_h = 0; ck_h < conv_kel->col; ck_h++) // 遍历"卷积核"列
                            {
                                // printf("level - 7\n");
                                // printf("[%d][%d][%d][%d]\n", conved_Tsr->tsnum, conv_kel->chnum, conv_result->row, conv_result->col);
                                // printf("[%d][%d][%d][%d]\n", cd_i, ck_j, cd_k, cd_h);
                                // printf("[%d][%d][%d][%d] => %f\n", cd_i, cd_j, cd_k + ck_k, cd_h + ck_h, extend_tensor.data[cd_i][cd_j][cd_k + ck_k][cd_h + ck_h]);
                                conv_result->data[cd_i][ck_j][cd_k][cd_h] += (extend_tensor.data[cd_i][cd_j][cd_k*(conv_stride->row) + ck_k][cd_h*(conv_stride->col) + ck_h] * conv_kel->data[0][ck_j][ck_k][ck_h]);
                            }
                        }
                    }
                    // TODO 加偏置, 需确定是否对完整核卷积加一次偏置
                    // 参考: https://poloclub.github.io/cnn-explainer/
                    conv_result->data[cd_i][ck_j][cd_k][cd_h] += conv_bias->data[ck_j]; // 1个核输出通道对应一个卷积
                }
            }
        }
        
    }

    TensorFree(&extend_tensor);
    
}
