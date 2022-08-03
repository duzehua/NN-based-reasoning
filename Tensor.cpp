//#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "Tensor.h"
#include "GINFO_VAR.h"

/**
* 张量初始化
* 说明: 每定义一个张量, 须执行初始化操作以获取指定大小的内存空间存储数据.
* @param[in]  tensor_data -> 张量结构体
* @param[in]  ts_type     -> 张量类型
							 TENSOR1_STR=>"array"
							 TENSOR2_STR=>"matrix"
							 TENSOR3_STR=>"tensor3d"
							 TENSOR4_STR=>"tensor4d"
* @param[in]  ts_shape    -> 张量尺寸
							 TENSOR1_SHAPE[1]={col}
							 TENSOR2_SHAPE[2]={row, col}
							 TENSOR3_SHAPE[3]={chnum, row, col}
							 TENSOR4_SHAPE[4]={tsnum, chnum, row, col}
* @param[out] Null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Mar. 23, 2022
* Data: Jun. 28, 2022; newly add: 插入一维张量; By: Zehua Du
*/
void TensorInitial(Tensor* tensor_data, char* ts_type, int* ts_shape)
// void TensorInitial(Tensor *tensor_data, int tsnum, int chnum, int row, int col)
{
	int i, j, k;
	if (strcmp(ts_type, TENSOR1_STR) == 0)
	{
		// 类型定义
		tensor_data->ts_type = (char*)malloc(5 * sizeof(char));
		strcpy(tensor_data->ts_type, TENSOR1_STR);

		// 尺寸定义
		tensor_data->tsnum = 1;
		tensor_data->chnum = 1;
		tensor_data->row = 1;
		//printf("%d, %d\n", ts_shape[0], ts_shape[1]);
		tensor_data->col = ts_shape[0];
		tensor_data->ts_shape = (int*)malloc(1 * sizeof(int));
		tensor_data->ts_shape[0] = ts_shape[0];
	}
	else if (strcmp(ts_type, TENSOR2_STR) == 0)
	{
		// 类型定义
		tensor_data->ts_type = (char*)malloc(6 * sizeof(char));
		strcpy(tensor_data->ts_type, TENSOR2_STR);

		// 尺寸定义
		tensor_data->tsnum = 1;
		tensor_data->chnum = 1;
		tensor_data->row = ts_shape[0];
		tensor_data->col = ts_shape[1];
		tensor_data->ts_shape = (int*)malloc(2 * sizeof(int));
		tensor_data->ts_shape[0] = ts_shape[0];
		tensor_data->ts_shape[1] = ts_shape[1];

		// debug
		// printf("类型 %s, ", tensor_data->ts_type);
		// printf("尺寸 [%d, %d]\n", tensor_data->ts_shape[0], tensor_data->ts_shape[1]);
	}
	// 3维张量
	else if (strcmp(ts_type, TENSOR3_STR) == 0)
	{
		tensor_data->ts_type = (char*)malloc(8 * sizeof(char));
		strcpy(tensor_data->ts_type, TENSOR3_STR);
		tensor_data->tsnum = 1;
		tensor_data->chnum = ts_shape[0];
		tensor_data->row = ts_shape[1];
		tensor_data->col = ts_shape[2];
		tensor_data->ts_shape = (int*)malloc(3 * sizeof(int));
		tensor_data->ts_shape[0] = ts_shape[0];
		tensor_data->ts_shape[1] = ts_shape[1];
		tensor_data->ts_shape[2] = ts_shape[2];

		// debug
		// printf("类型 %s\n", tensor_data->ts_type);
		// printf("尺寸 [%d, %d, %d]\n", tensor_data->ts_shape[0], tensor_data->ts_shape[1], tensor_data->ts_shape[2]);
	}
	// 4维张量
	else if (strcmp(ts_type, TENSOR4_STR) == 0)
	{
		tensor_data->ts_type = (char*)malloc(8 * sizeof(char));
		strcpy(tensor_data->ts_type, TENSOR3_STR);
		tensor_data->tsnum = ts_shape[0];
		tensor_data->chnum = ts_shape[1];
		tensor_data->row = ts_shape[2];
		tensor_data->col = ts_shape[3];
		tensor_data->ts_shape = (int*)malloc(4 * sizeof(int));
		tensor_data->ts_shape[0] = ts_shape[0];
		tensor_data->ts_shape[1] = ts_shape[1];
		tensor_data->ts_shape[2] = ts_shape[2];
		tensor_data->ts_shape[3] = ts_shape[3];

		// debug
		// printf("类型 %s\n", tensor_data->ts_type);
		// printf("尺寸 [%d, %d, %d, %d]\n", tensor_data->ts_shape[0], tensor_data->ts_shape[1], tensor_data->ts_shape[2], tensor_data->ts_shape[3]);
	}
	else
	{
		fprintf(stderr, "ERROR: TensorType => (%s not in [matrix, tensor3d, tensor4d])\n", ts_type);
		exit(EXIT_FAILURE);
	}

	// 初始化个数
	tensor_data->data = (DATA****)malloc(tensor_data->tsnum * sizeof(DATA***));
	// 初始化通道
	for (i = 0; i < tensor_data->tsnum; i++)
	{
		tensor_data->data[i] = (DATA***)malloc(tensor_data->chnum * sizeof(DATA**));
	}
	// 初始化单通道行
	for (i = 0; i < tensor_data->tsnum; i++)
	{
		for (j = 0; j < tensor_data->chnum; j++)
		{
			tensor_data->data[i][j] = (DATA**)malloc(tensor_data->row * sizeof(DATA*));
		}
	}
	// 初始化单通道列
	for (i = 0; i < tensor_data->tsnum; i++)
	{
		for (j = 0; j < tensor_data->chnum; j++)
		{
			for (k = 0; k < tensor_data->row; k++)
			{
				tensor_data->data[i][j][k] = (DATA*)malloc(tensor_data->col * sizeof(DATA));
			}
		}
	}
}


/**
* 释放张量
* 说明: 每用完一个张量, 须执行释放操作来释放内存空间.
* @param[in]  tensor_data -> 张量结构体
* @param[out] Null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Mar. 23, 2022
*/
void TensorFree(Tensor* tensor_data)
{
	int i, j, k;

	// 释放张量列
	for (i = 0; i < (tensor_data->tsnum); i++)
	{
		for (j = 0; j < (tensor_data->chnum); j++)
		{
			for (k = 0; k < (tensor_data->row); k++)
			{
				if (tensor_data->data[i][j][k] != NULL)
				{
					free((DATA****)(tensor_data->data[i][j][k]));
					tensor_data->data[i][j][k] = NULL;
				}
			}
		}
	}

	// 释放张量行
	for (i = 0; i < (tensor_data->tsnum); i++)
	{
		for (j = 0; j < (tensor_data->chnum); j++)
		{
			if (tensor_data->data[i][j] != NULL)
			{
				free(*tensor_data->data[i][j]);
				tensor_data->data[i][j] = NULL;
			}
		}
	}

	// 释放张量通道
	for (i = 0; i < (tensor_data->tsnum); i++)
	{
		if (tensor_data->data[i] != NULL)
		{
			free(*tensor_data->data[i]);
			tensor_data->data[i] = NULL;
		}
	}

	// 释放张量
	if (tensor_data->data[i] != NULL)
	{
		free(*tensor_data->data);
		tensor_data->data = NULL;
	}
}


/**
* 张量赋值
* 说明: 用于张量初始化之后, 将数据赋值给张量.
* @param[in]  Tensor         -> 被赋值张量结构体
* @param[in]  tensor_data_in -> 赋值数据(数组首位地址)
* @param[in]  ti_len         -> 赋值数据长度
* @param[out] Null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Mar. 23, 2022
*/
void TensorSet(Tensor* tensor_data, DATA* tensor_data_in, int ti_len)
{
	// 赋值需保证赋值变量tensor3d_data_in与被赋值变量tensor3d_data长度一致
	int m_len = (tensor_data->tsnum) * (tensor_data->chnum) * (tensor_data->row) * (tensor_data->col);
	if (m_len != ti_len)
	{
		fprintf(stderr, "ERROR: TensorSet => (tensor_data[%d] != tensor3d_data_in[%d])\n", m_len, ti_len);
		exit(EXIT_FAILURE);
	}

	int i, j, k, h;  // 个数, 通道, 行, 列
	int tsr_gain;    // 个数增益
	int chn_gain;    // 通道增益
	int row_gain;    // 行增益
	for (i = 0; i < (tensor_data->tsnum); i++)
	{
		for (j = 0; j < (tensor_data->chnum); j++)
		{
			for (k = 0; k < (tensor_data->row); k++)
			{
				for (h = 0; h < (tensor_data->col); h++)
				{
					tsr_gain = i * (tensor_data->chnum) * (tensor_data->row) * (tensor_data->col);
					chn_gain = j * (tensor_data->row) * (tensor_data->col);
					row_gain = k * (tensor_data->col);
					tensor_data->data[i][j][k][h] = *(tensor_data_in + tsr_gain + chn_gain + row_gain + h); //tensor_data_in[个数][通道][行][列];
					// printf("%f\n", tensor_data->data[i][j][k][h]);
				}
			}
		}
	}
}


/**
* 张量值置零
* 说明: 用于张量初始化之后, 将张量赋值为0.
* @param[in]  tensor_data -> 被赋值张量结构体
* @param[out] Null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Mar. 23, 2022
*/
void TensorPaddingZero(Tensor* tensor_data)
{
	int i, j, k, h;  // 个数, 通道, 行, 列
	for (i = 0; i < (tensor_data->tsnum); i++)
	{
		for (j = 0; j < (tensor_data->chnum); j++)
		{
			for (k = 0; k < (tensor_data->row); k++)
			{
				for (h = 0; h < (tensor_data->col); h++)
				{
					tensor_data->data[i][j][k][h] = DATA_0;
				}
			}
		}
	}
}


/**
* 查看张量
* 说明: 查看张量的类型和形状.
* @param[in]  tensor_data -> 被查看的张量
* @param[out] Null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Jun. 29, 2022
*/
void ViewTensor(Tensor* tensor_data)
{
	//
	printf("-------------------------------\n");
	printf("tensor type : %s\n", tensor_data->ts_type);
	printf("       shape: [%d, %d, %d, %d]\n", tensor_data->tsnum, tensor_data->chnum, tensor_data->row, tensor_data->col);
	printf("-------------------------------\n");
}

/**
* 对Tensor后两维（row，col）转置->（col， row）
* 说明: 全连接之前为了进行矩阵乘法，先对权重矩阵后两位进行转置（这里无法进行“原地转置”）
* @param[in]  in_tensor  -> 转置前Tensor
* @param[out] out_tensor -> 转置后Tensor
*/
void TensorTranspose2D(Tensor* in_tensor, Tensor* out_tensor)
{
	int in_tsnum, in_chnum, in_row, in_col;
	int out_tsnum, out_chnum, out_row, out_col;
	in_tsnum = in_tensor->tsnum;
	in_chnum = in_tensor->chnum;
	in_row = in_tensor->row;
	in_col = in_tensor->col;

	// Transpose
	out_tsnum = in_tensor->tsnum;
	out_chnum = in_tensor->chnum;
	out_row = in_tensor->col;
	out_col = in_tensor->row;
	TENSORSHAPE out_tensor_shape[4] = { out_tsnum, out_chnum, out_row, out_col };  //先定义转置后Tensor并初始化
	TensorInitial(out_tensor, TENSOR4_STR, out_tensor_shape);

	for (int i = 0; i < in_tsnum; i++)
	{
		for (int j = 0; j < in_chnum; j++)
		{
			for (int k = 0; k < in_row; k++)
			{
				for (int l = 0; l < in_col; l++)
					out_tensor->data[i][j][l][k] = in_tensor->data[i][j][k][l];   //(row, col) -> (col, row)
			}
		}
	}

}
