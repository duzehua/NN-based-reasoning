//#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "Tensor.h"
#include "GINFO_VAR.h"

/**
* ������ʼ��
* ˵��: ÿ����һ������, ��ִ�г�ʼ�������Ի�ȡָ����С���ڴ�ռ�洢����.
* @param[in]  tensor_data -> �����ṹ��
* @param[in]  ts_type     -> ��������
							 TENSOR1_STR=>"array"
							 TENSOR2_STR=>"matrix"
							 TENSOR3_STR=>"tensor3d"
							 TENSOR4_STR=>"tensor4d"
* @param[in]  ts_shape    -> �����ߴ�
							 TENSOR1_SHAPE[1]={col}
							 TENSOR2_SHAPE[2]={row, col}
							 TENSOR3_SHAPE[3]={chnum, row, col}
							 TENSOR4_SHAPE[4]={tsnum, chnum, row, col}
* @param[out] Null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Mar. 23, 2022
* Data: Jun. 28, 2022; newly add: ����һά����; By: Zehua Du
*/
void TensorInitial(Tensor* tensor_data, char* ts_type, int* ts_shape)
// void TensorInitial(Tensor *tensor_data, int tsnum, int chnum, int row, int col)
{
	int i, j, k;
	if (strcmp(ts_type, TENSOR1_STR) == 0)
	{
		// ���Ͷ���
		tensor_data->ts_type = (char*)malloc(5 * sizeof(char));
		strcpy(tensor_data->ts_type, TENSOR1_STR);

		// �ߴ綨��
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
		// ���Ͷ���
		tensor_data->ts_type = (char*)malloc(6 * sizeof(char));
		strcpy(tensor_data->ts_type, TENSOR2_STR);

		// �ߴ綨��
		tensor_data->tsnum = 1;
		tensor_data->chnum = 1;
		tensor_data->row = ts_shape[0];
		tensor_data->col = ts_shape[1];
		tensor_data->ts_shape = (int*)malloc(2 * sizeof(int));
		tensor_data->ts_shape[0] = ts_shape[0];
		tensor_data->ts_shape[1] = ts_shape[1];

		// debug
		// printf("���� %s, ", tensor_data->ts_type);
		// printf("�ߴ� [%d, %d]\n", tensor_data->ts_shape[0], tensor_data->ts_shape[1]);
	}
	// 3ά����
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
		// printf("���� %s\n", tensor_data->ts_type);
		// printf("�ߴ� [%d, %d, %d]\n", tensor_data->ts_shape[0], tensor_data->ts_shape[1], tensor_data->ts_shape[2]);
	}
	// 4ά����
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
		// printf("���� %s\n", tensor_data->ts_type);
		// printf("�ߴ� [%d, %d, %d, %d]\n", tensor_data->ts_shape[0], tensor_data->ts_shape[1], tensor_data->ts_shape[2], tensor_data->ts_shape[3]);
	}
	else
	{
		fprintf(stderr, "ERROR: TensorType => (%s not in [matrix, tensor3d, tensor4d])\n", ts_type);
		exit(EXIT_FAILURE);
	}

	// ��ʼ������
	tensor_data->data = (DATA****)malloc(tensor_data->tsnum * sizeof(DATA***));
	// ��ʼ��ͨ��
	for (i = 0; i < tensor_data->tsnum; i++)
	{
		tensor_data->data[i] = (DATA***)malloc(tensor_data->chnum * sizeof(DATA**));
	}
	// ��ʼ����ͨ����
	for (i = 0; i < tensor_data->tsnum; i++)
	{
		for (j = 0; j < tensor_data->chnum; j++)
		{
			tensor_data->data[i][j] = (DATA**)malloc(tensor_data->row * sizeof(DATA*));
		}
	}
	// ��ʼ����ͨ����
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
* �ͷ�����
* ˵��: ÿ����һ������, ��ִ���ͷŲ������ͷ��ڴ�ռ�.
* @param[in]  tensor_data -> �����ṹ��
* @param[out] Null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Mar. 23, 2022
*/
void TensorFree(Tensor* tensor_data)
{
	int i, j, k;

	// �ͷ�������
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

	// �ͷ�������
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

	// �ͷ�����ͨ��
	for (i = 0; i < (tensor_data->tsnum); i++)
	{
		if (tensor_data->data[i] != NULL)
		{
			free(*tensor_data->data[i]);
			tensor_data->data[i] = NULL;
		}
	}

	// �ͷ�����
	if (tensor_data->data[i] != NULL)
	{
		free(*tensor_data->data);
		tensor_data->data = NULL;
	}
}


/**
* ������ֵ
* ˵��: ����������ʼ��֮��, �����ݸ�ֵ������.
* @param[in]  Tensor         -> ����ֵ�����ṹ��
* @param[in]  tensor_data_in -> ��ֵ����(������λ��ַ)
* @param[in]  ti_len         -> ��ֵ���ݳ���
* @param[out] Null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Mar. 23, 2022
*/
void TensorSet(Tensor* tensor_data, DATA* tensor_data_in, int ti_len)
{
	// ��ֵ�豣֤��ֵ����tensor3d_data_in�뱻��ֵ����tensor3d_data����һ��
	int m_len = (tensor_data->tsnum) * (tensor_data->chnum) * (tensor_data->row) * (tensor_data->col);
	if (m_len != ti_len)
	{
		fprintf(stderr, "ERROR: TensorSet => (tensor_data[%d] != tensor3d_data_in[%d])\n", m_len, ti_len);
		exit(EXIT_FAILURE);
	}

	int i, j, k, h;  // ����, ͨ��, ��, ��
	int tsr_gain;    // ��������
	int chn_gain;    // ͨ������
	int row_gain;    // ������
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
					tensor_data->data[i][j][k][h] = *(tensor_data_in + tsr_gain + chn_gain + row_gain + h); //tensor_data_in[����][ͨ��][��][��];
					// printf("%f\n", tensor_data->data[i][j][k][h]);
				}
			}
		}
	}
}


/**
* ����ֵ����
* ˵��: ����������ʼ��֮��, ��������ֵΪ0.
* @param[in]  tensor_data -> ����ֵ�����ṹ��
* @param[out] Null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Mar. 23, 2022
*/
void TensorPaddingZero(Tensor* tensor_data)
{
	int i, j, k, h;  // ����, ͨ��, ��, ��
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
* �鿴����
* ˵��: �鿴���������ͺ���״.
* @param[in]  tensor_data -> ���鿴������
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

//��Tensor����ά����ת��
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
	TENSORSHAPE out_tensor_shape[4] = { out_tsnum, out_chnum, out_row, out_col };
	TensorInitial(out_tensor, TENSOR4_STR, out_tensor_shape);

	for (int i = 0; i < in_tsnum; i++)
	{
		for (int j = 0; j < in_chnum; j++)
		{
			for (int k = 0; k < in_row; k++)
			{
				for (int l = 0; l < in_col; l++)
					out_tensor->data[i][j][l][k] = in_tensor->data[i][j][k][l];
			}
		}
	}

}