/**
* @file ConvOperation
* @ 卷积操作,对矩阵\3维张量\4维张量的卷积
* @details 目前卷积支持2种形式的卷积,
* @Zehua Du
* @date Mar. 22, 2022
* @version v1.0
* @par Copyright(c):
* @par History:
*version: author, date, desc\n
*/

#include <math.h>
//#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
// #include <omp.h>

#include "Tensor.h"
#include "GINFO_VAR.h"


/**
* 张量卷积
* 说明: 矩阵与矩阵的卷积运算.
* @param[in]  conved_Tsr  -> 被卷积张量结构体
* @param[in]  conv_kel    -> 卷积核张量结构体
* @param[in]  conv_stride -> 卷积运算步长结构体[conv_stride.row, conv_stride.col]
* @param[in]  conv_bias   -> 卷积操作偏置(默认0)
* @param[in]  conv_mode   -> 卷积模式("FULL", "VALID", "SAME") https://zhuanlan.zhihu.com/p/62760780
* @param[in]  conv_result -> 卷积结果(传入前仅需定义变量)
* @param[out] Null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Mar. 23, 2022
*/
void Conv2(Tensor* conved_Tsr, Tensor* conv_kel, CONVSTRIDE* conv_stride, Tensor* conv_bias, char* conv_mode, Tensor* conv_result)
{
	// 对每个张量, 对每个通道单独卷积, 但应先定义结果张量和结果矩阵, 对每个通道赋值给矩阵, 然后矩阵卷积之后赋值给张量
	int i, j, k, h;
	// int ci, cj, ck, ch; //卷积操作专用索引, 避免符号混乱

	int ck_row = conv_kel->row;
	int ck_col = conv_kel->col;

	// 限定卷积核尺寸为奇数且行列一致, 不接受偶数输入
	if (((ck_row % 2) == 0) && (ck_row != ck_col))
	{
		fprintf(stderr, "ERROR: Conv2: conv_kel => (shape[%d x %d] dimensions shall be odd and row == col)\n", ck_row, ck_col);
		exit(EXIT_FAILURE);
	}

	// 限定卷积核通道与偏置长度一致
	if ((conv_result->chnum) != (conv_bias->col))
	{
		fprintf(stderr, "ERROR: Conv2: conv_kel->chnum len[%d] != conv_bias->col len[%d])\n", conv_result->chnum, conv_bias->col);
		exit(EXIT_FAILURE);
	}

	// 确定conved_Tsr延拓半宽
	int cm_ext = (ck_row - 1) >> 1;
	// printf("拓展半宽%d\n", cm_ext);

	int ext_half_len; // 延拓半宽
	// 根据卷积模式确定extend_tensor尺寸
	// 三种模式区别, 参考: https://zhuanlan.zhihu.com/p/62760780
	if (strcmp(conv_mode, FULL) == 0)
	{
		// 计算延拓张量的长宽
		ext_half_len = (conv_kel->row - 1);
	}
	else if (strcmp(conv_mode, SAME) == 0)
	{
		ext_half_len = (conv_kel->row - 1) >> 1;
	}
	else if (strcmp(conv_mode, VALID) == 0)
	{
		ext_half_len = 0;
	}
	else
	{
		fprintf(stderr, "ERROR: Conv2: conv_mode=>[FULL, SAME, VALID]\n");
		exit(EXIT_FAILURE);
	}

	// printf("延拓半宽: %d\n", ext_half_len);

	// 复制 conved_Tsr 到 extend_tensor 用于实际运算
	Tensor extend_tensor;
	TENSOR4_SHAPE[0] = conved_Tsr->tsnum;
	TENSOR4_SHAPE[1] = conved_Tsr->chnum;
	TENSOR4_SHAPE[2] = (conved_Tsr->row) + (ext_half_len << 1);
	TENSOR4_SHAPE[3] = (conved_Tsr->col) + (ext_half_len << 1);
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

	//for (i=0;i<extend_tensor.tsnum;i++)
	//{
	//    printf("extend_tensor个数 %d ============================================\n", i);
	//    for (j=0;j<extend_tensor.chnum;j++)
	//    {
	//        printf("通道 %d ------------------------------------------\n", j);
	//        for (k = 0; k < extend_tensor.row; k++)
	//        {
	//            for (h = 0; h < extend_tensor.col; h++)
	//            {
	//                printf("%f, ", extend_tensor.data[i][j][k][h]);
	//            }
	//            printf("\n");
	//        }
	//        printf("\n\n");
	//    }
	//    printf("\n\n\n");
	//}

   // 卷积结果张量
   // 参考: https://oldtang.com/6775.html
	TENSOR4_SHAPE[0] = conved_Tsr->tsnum;
	TENSOR4_SHAPE[1] = conv_kel->tsnum;   // 输出张量的通道与卷积核个数一致
	TENSOR4_SHAPE[2] = floor(((conved_Tsr->row) + (ext_half_len << 1) - ck_row) / conv_stride->row) + 1;  // 输出张量的行数为 conved_Tsr->row + 延拓半宽*2
	TENSOR4_SHAPE[3] = floor(((conved_Tsr->col) + (ext_half_len << 1) - ck_row) / conv_stride->col) + 1;  // 输出张量的列数为 conved_Tsr->col + 延拓半宽*2
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
	卷积核索引     conv_kel      [ck_i ][ck_j][ck_k][ck_h]
	卷积结果索引   conv_result   [rs_i][rs_j][rs_k][rs_h]
	拓展卷积索引   extend_tensor [et_i][et_j][et_k][et_h]
	*/
	// 参考: https://www.cnblogs.com/chumingqian/articles/11495364.html
	// 每个卷积核通道对应1个输出
	// TODO 可并行处理
	// TODO 需要确定 conv_stride 放置位置是否正确
	// printf("%d\n", conv_stride->row);
	// #pragma omp parallel for num_threads(6)
	//DATA tmp = 0.0, tmp1 = 0.0;

	//printf("CONV2: \n");
	for (cd_i = 0; cd_i < conved_Tsr->tsnum; cd_i++)   // 遍历"被卷积"个数 => 对应实际输出的个数
	{
		//printf("level - 1\n");
		for (ck_j = 0; ck_j < conv_kel->tsnum; ck_j++) // 遍历"卷积核"个数 => 对应实际输出通道数 m
		//for (ck_j = 0; ck_j < conv_kel->chnum; ck_j++) // 遍历"卷积核"通道 => 对应实际输出通道数
		{
			//printf("level - 2\n");
		   // 每通道\行\列,对应的核大小内容进行卷积, 然后循环外 + bias
			for (cd_k = 0; cd_k < conv_result->row; cd_k++)     // 遍历"卷积结果张量"行 => 对应实际输出行 (+步长) k
			{
				//printf("level - 3\n");
				for (cd_h = 0; cd_h < conv_result->col; cd_h++) // 遍历"卷积结果张量"列 => 对应实际输出列 (+步长) j
				{
					//printf("level - 4\n");
					for (cd_j = 0; cd_j < conved_Tsr->chnum; cd_j++)     // 遍历"被卷积张量"通道 n
					{
						//printf("level - 5\n");
					   // 当前点为核中心, 对应的核大小张量数据, 卷积
					   // += extend_tensor.data[cd_i][cd_j][核范围][核范围] .* conv_kel->data[0][ck_j][核范围][核范围]
						for (ck_k = 0; ck_k < conv_kel->row; ck_k++)     // 遍历"卷积核"行 i
						{
							// #pragma omp parallel 
							// printf("level - 6\n");
							for (ck_h = 0; ck_h < conv_kel->col; ck_h++) // 遍历"卷积核"列 j
							{
								// printf("level - 7\n");
								// printf("[%d][%d][%d][%d]\n", conved_Tsr->tsnum, conv_kel->chnum, conv_result->row, conv_result->col);
								// printf("[%d][%d][%d][%d]\n", cd_i, ck_j, cd_k, cd_h);
								// printf("[%d][%d][%d][%d] => %f\n", cd_i, cd_j, cd_k + ck_k, cd_h + ck_h, extend_tensor.data[cd_i][cd_j][cd_k + ck_k][cd_h + ck_h]);
								conv_result->data[cd_i][ck_j][cd_k][cd_h] += (extend_tensor.data[cd_i][cd_j][cd_k * (conv_stride->row) + ck_k][cd_h * (conv_stride->col) + ck_h] * conv_kel->data[ck_j][cd_j][ck_k][ck_h]);
								//printf("卷积结果：%f\n", conv_result->data[cd_i][ck_j][cd_k][cd_h]);
							}
						}
					}
					// TODO 加偏置, 需确定是否对完整核卷积加一次偏置
					// 参考: https://poloclub.github.io/cnn-explainer/
					conv_result->data[cd_i][ck_j][cd_k][cd_h] += conv_bias->data[0][0][0][ck_j]; // 1个核输出通道对应一个卷积
				}
			}
		}

	}

	TensorFree(&extend_tensor);
}

//----------------------------------------------------------------------------