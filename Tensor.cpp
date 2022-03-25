#include <error.h>
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
                             TENSOR2_STR=>"matrix"
                             TENSOR2_STR=>"tensor3d"
                             TENSOR2_STR=>"tensor4d"
* @param[in]  ts_shape    -> �����ߴ�
                             TENSOR2_SHAPE[2]={row, col}
                             TENSOR2_SHAPE[3]={chnum, row, col}
                             TENSOR2_SHAPE[4]={tsnum, chnum, row, col}
* @param[out] Null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Mar. 23, 2022  
*/
void TensorInitial(Tensor *tensor_data, char *ts_type, int *ts_shape)
// void TensorInitial(Tensor *tensor_data, int tsnum, int chnum, int row, int col)
{
    int i, j, k;
    
    if (strcmp(ts_type, TENSOR2_STR) == 0)
    {
        // ���Ͷ���
        tensor_data->ts_type = (char*)malloc(6 * sizeof(char));
        strcpy(tensor_data->ts_type, TENSOR2_STR);

        // �ߴ綨��
        tensor_data->tsnum = 1;
        tensor_data->chnum = 1;
        tensor_data->row   = ts_shape[0];
        tensor_data->col   = ts_shape[1];
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
        tensor_data->row   = ts_shape[1];
        tensor_data->col   = ts_shape[2];
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
        tensor_data->row   = ts_shape[2];
        tensor_data->col   = ts_shape[3];
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
void TensorFree(Tensor *tensor_data)
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
                    free((DATA ****)(tensor_data->data[i][j][k]));
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
void TensorSet(Tensor *tensor_data, DATA *tensor_data_in, int ti_len)
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
void TensorPaddingZero(Tensor *tensor_data)
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
                    tensor_data->data[i][j][k][h] = 0;
                }
            }
        }
    }
}


/**
* �����ʼ��
* ˵��: ÿ����һ������, ��ִ�г�ʼ�������Ի�ȡָ����С���ڴ�ռ�洢����.
* @param[in]  mat_data -> ����ṹ��
* @param[in]  row      -> ��������
* @param[in]  col      -> ��������
* @param[out] Null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Mar. 18, 2022  
*/
void MatrixInitial(Matrix *mat_data, int row, int col)
{
    // ��ʼ��������
	mat_data->data = (DATA**)malloc(row * sizeof(DATA*));

    // ��ʼ��������
    int i;
	for (i = 0; i < row; i++)
    {
        mat_data->data[i] = (DATA*)malloc(col*sizeof(DATA));
    }
		
	mat_data->row = row;
	mat_data->col = col;
}


/**
* �ͷŶ�ά����
* ˵��: ÿ����һ������, ��ִ���ͷŲ������ͷ��ڴ�ռ�.
* @param[in]  mat_data -> ����ṹ��
* @param[out] Null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Mar. 18, 2022  
*/
void MatrixFree(Matrix *mat_data)
{
    // �ͷž�����
	int i;
	for (i = 0; i < (mat_data->row); i++)
    {
        if (mat_data->data[i] != NULL) 
        {
            free(mat_data->data[i]);
            mat_data->data[i] = NULL;
        }
    }


    // �ͷž���
    if (mat_data->data != NULL) 
    {
        free(mat_data->data);
        mat_data->data = NULL;
    }
}


/**
* ����ֵ
* ˵��: ���ھ����ʼ��֮��, �����ݸ�ֵ������.
* @param[in]  mat_data    -> ����ֵ����ṹ��
* @param[in]  mat_data_in -> ��ֵ����(������λ��ַ)
* @param[in]  mdi_len     -> ��ֵ���ݳ���
* @param[out] Null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Mar. 18, 2022  
*/
void MatrixSet(Matrix *mat_data, DATA *mat_data_in, int mdi_len)
{
    // ��ֵ�豣֤��ֵ����mat_data_in�뱻��ֵ����mat_data����һ��
    int md_len = (mat_data->row) * (mat_data->col);
    if(md_len != mdi_len){
        fprintf(stderr, "ERROR: MatrixSet => (mat_data[%d] != mat_data_in[%d])\n", md_len, mdi_len);
        exit(EXIT_FAILURE);
    }

	int i, j; // ��, ��
	for (i = 0; i < (mat_data->row); i++)
    {
		for (j = 0; j < (mat_data->col); j++)
        {
            mat_data->data[i][j] = *(mat_data_in + i*(mat_data->col) + j); //mat_data_in[i][j];
        }
			
	}
}


/**
* 1ά����
* ˵��: ÿ����1ά����, ��ִ�г�ʼ�������Ի�ȡָ����С���ڴ�ռ�洢����.
* @param[in]  arr_data -> ����ṹ��
* @param[in]  len      -> ���鳤��
* @param[out] Null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Mar. 23, 2022  
*/
void ArrayInitial(Array *arr_data, int len)
{
    // ��ʼ��
	arr_data->data = (DATA*)malloc(len * sizeof(DATA));
    // ��ʼ��Ϊ0
    for (int i = 0; i < len; i++)
    {
        arr_data->data[i] = 0;
    }
    
	arr_data->len = len;
}


/**
* �ͷ�1ά����
* ˵��: ÿ����һ������, ��ִ���ͷŲ������ͷ��ڴ�ռ�.
* @param[in]  arr_data -> ����ṹ��
* @param[out] Null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Mar. 23, 2022  
*/
void ArrayFree(Array *arr_data)
{
    // �ͷ�����
    if (arr_data->data != NULL) 
    {
        free(arr_data->data);
        arr_data->data = NULL;
    }
}


/**
* ���鸳ֵ
* ˵��: ���������ʼ��֮��, �����ݸ�ֵ������.
* @param[in]  arr_data    -> ����ֵ����ṹ��
* @param[in]  arr_data_in -> ��ֵ����(������λ��ַ)
* @param[in]  adi_len     -> ��ֵ���ݳ���
* @param[out] Null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Mar. 23, 2022  
*/
void ArraySet(Array *arr_data, DATA *arr_data_in, int adi_len)
{
    // ��ֵ�豣֤��ֵ����arr_data_in�뱻��ֵ����arr_data����һ��
    if(arr_data->len != adi_len){
        fprintf(stderr, "ERROR: ArraySet => (arr_data[%d] != arr_data_in[%d])\n", arr_data->len, adi_len);
        exit(EXIT_FAILURE);
    }

	for (int i = 0; i < (arr_data->len); i++)
    {
        arr_data->data[i] = *(arr_data_in + i); //mat_data_in[i][j];
	}
}


// -------------------------------------------------------------------------


// /**
// * ��ά������ʼ��
// * ˵��: ÿ����һ����ά����, ��ִ�г�ʼ�������Ի�ȡָ����С���ڴ�ռ�洢����.
// * @param[in]  tensor3d_data -> 3ά�����ṹ��
// * @param[in]  chnum         -> 3ά����ͨ����
// * @param[in]  row           -> 3ά�����е�ͨ����������
// * @param[in]  col           -> 3ά�����е�ͨ����������
// * @param[out] Null
// * @retval Null
// * @par Null
// * Built-By: Zehua Du
// * Date: Mar. 18, 2022  
// */
// void Tensor3DInitial(Tensor3D *tensor3d_data, int chnum, int row, int col)
// {
//     int i, j;

//     // ��ʼ��ͨ��
// 	tensor3d_data->data = (DATA***)malloc(chnum * sizeof(DATA**));

//     // ��ʼ��ͨ����
//     for (i = 0; i < row; i++)
//     {
//         tensor3d_data->data[i] = (DATA**)malloc(row * sizeof(DATA*));
//     }

//     // ��ʼ��ͨ����
//     for (i = 0; i < chnum; i++)
//     {
//         for (j = 0; j < row; j++)
//         {
//             tensor3d_data->data[i][j] = (DATA*)malloc(col * sizeof(DATA));
//         }
//     }

//     tensor3d_data->chnum = chnum;
// 	tensor3d_data->row   = row;
// 	tensor3d_data->col   = col;
// }


// /**
// * �ͷ���ά����
// * ˵��: ÿ����һ����ά����, ��ִ���ͷŲ������ͷ��ڴ�ռ�.
// * @param[in]  tensor3d_data -> ��ά�����ṹ��
// * @param[out] Null
// * @retval Null
// * @par Null
// * Built-By: Zehua Du
// * Date: Mar. 18, 2022  
// */
// void Tensor3DFree(Tensor3D *tensor3d_data)
// {
//     // // �ͷ�������
// 	int i, j;
// 	for (i = 0; i < (tensor3d_data->chnum); i++)
//     {
//         for (j = 0; j < (tensor3d_data->row); j++)
//         {
//             // printf("�ͷ�һ��");
//             if (tensor3d_data->data[i][j] != NULL) 
//             {
//                 free((DATA ***)(tensor3d_data->data[i][j]));
//                 tensor3d_data->data[i][j] = NULL;
// 			}
//         }
//     }

//     // �ͷ�������
//     for (i = 0; i < (tensor3d_data->chnum); i++)
//     {
//         if (tensor3d_data->data[i] != NULL) 
//         {
//             free(*tensor3d_data->data[i]);
//             tensor3d_data->data[i] = NULL;
//         }
//     }
    
//     // �ͷ�����
//     if (tensor3d_data->data != NULL) 
//     {
//         printf("�ǿ�");
//         free(*tensor3d_data->data);
//         tensor3d_data->data = NULL;
//     }
// }


// /**
// * ��ά������ֵ
// * ˵��: ������ά��ʼ��֮��, �����ݸ�ֵ����ά����.
// * @param[in]  tensor3d_data    -> ����ֵ�����ṹ��
// * @param[in]  tensor3d_data_in -> ��ֵ����(������λ��ַ)
// * @param[in]  t3di_len         -> ��ֵ���ݳ���
// * @param[out] Null
// * @retval Null
// * @par Null
// * Built-By: Zehua Du
// * Date: Mar. 18, 2022  
// */
// void Tensor3DSet(Tensor3D *tensor3d_data, DATA *tensor3d_data_in, int t3di_len)
// {
//     // ��ֵ�豣֤��ֵ����tensor3d_data_in�뱻��ֵ����tensor3d_data����һ��
//     int m3d_len = (tensor3d_data->chnum) * (tensor3d_data->row) * (tensor3d_data->col);
//     if (m3d_len != t3di_len)
//     {
//         fprintf(stderr, "ERROR: Tensor3DSet => (tensor3d_data[%d] != tensor3d_data_in[%d])\n", m3d_len, t3di_len);
//         exit(EXIT_FAILURE);
//     }

//     // printf("3ά������ʼ��ֵ1\n");
// 	int i, j, k;  // ͨ��, ��, ��
//     int chn_gain; // ͨ������
//     int row_gain; // ������
// 	for (i = 0; i < (tensor3d_data->chnum); i++)
//     {
// 		for (j = 0; j < (tensor3d_data->row); j++)
//         {
//             for (k = 0; k < (tensor3d_data->col); k++)
//             {
//                 chn_gain = i * (tensor3d_data->row) * (tensor3d_data->col);
//                 row_gain = j * (tensor3d_data->col);
//                 // printf("3ά������ʼ��ֵ2\n");
//                 tensor3d_data->data[i][j][k] = *(tensor3d_data_in + chn_gain + row_gain + k); //tensor3d_data_in[ͨ��][��][��];
//             }
//         }
// 	}
// }


// /**
// * ��ά������ʼ��
// * ˵��: ÿ����һ����ά����, ��ִ�г�ʼ�������Ի�ȡָ����С���ڴ�ռ�洢����.
// * @param[in] tensor4d_data -> 4ά�����ṹ��
// * @param[in] tsnum         -> 4ά������3ά��������
// * @param[in] chnum         -> 4ά������3ά����ͨ����
// * @param[in] row           -> 4ά������, 3ά������ͨ����������
// * @param[in] col           -> 4ά������, 3ά������ͨ����������
// * @param[out] Null
// * @retval Null
// * @par Null
// * Built-By: Zehua Du
// * Date: Mar. 18, 2022  
// */
// void Tensor4DInitial(Tensor4D *tensor4d_data, int tsnum, int chnum, int row, int col)
// {
//     int i, j, k;

//     // ��ʼ������
// 	tensor4d_data->data = (DATA****)malloc(tsnum * sizeof(DATA***));

//     // ��ʼ��ͨ��
//     for (i = 0; i < tsnum; i++)
//     {
//         tensor4d_data->data[i] = (DATA***)malloc(chnum * sizeof(DATA**));
//     }

//     // ��ʼ����ͨ����
//     for (i = 0; i < tsnum; i++)
//     {
//         for (j = 0; j < chnum; j++)
//         {
//             tensor4d_data->data[i][j] = (DATA**)malloc(row * sizeof(DATA*));
//         }
//     }

//     // ��ʼ����ͨ����
//     for (i = 0; i < tsnum; i++)
//     {
//         for (j = 0; j < chnum; j++)
//         {
//             for (k = 0; k < row; k++)
//             {
//                 tensor4d_data->data[i][j][k] = (DATA*)malloc(col * sizeof(DATA));
//             }
//         }
//     }

//     tensor4d_data->tsnum = tsnum;
//     tensor4d_data->chnum = chnum;
// 	tensor4d_data->row   = row;
// 	tensor4d_data->col   = col;
// }


// /**
// * �ͷ���ά����
// * ˵��: ÿ����һ����ά����, ��ִ���ͷŲ������ͷ��ڴ�ռ�.
// * @param[in]  tensor4d_data -> ��ά�����ṹ��
// * @param[out] Null
// * @retval Null
// * @par Null
// * Built-By: Zehua Du
// * Date: Mar. 18, 2022  
// */
// void Tensor4DFree(Tensor4D *tensor4d_data)
// {
//  	int i, j, k;   

//     // �ͷ�������
//     for (i = 0; i < (tensor4d_data->tsnum); i++)
//     {
//         for (j = 0; j < (tensor4d_data->chnum); j++)
//         {
//             for (k = 0; k < (tensor4d_data->row); k++)
//             {
//                 if (tensor4d_data->data[i][j][k] != NULL) 
//                 {
//                     free((DATA ****)(tensor4d_data->data[i][j][k]));
//                     tensor4d_data->data[i][j][k] = NULL;
//                 }
//             }
//         }
//     }

//     // �ͷ�������
// 	for (i = 0; i < (tensor4d_data->tsnum); i++)
//     {
//         for (j = 0; j < (tensor4d_data->chnum); j++)
//         {
//             if (tensor4d_data->data[i][j] != NULL) 
//             {
//                 free(*tensor4d_data->data[i][j]);
//                 tensor4d_data->data[i][j] = NULL;                
//             }
//         }
//     }

//     // �ͷ�����ͨ��
//     for (i = 0; i < (tensor4d_data->tsnum); i++)
//     {
//         if (tensor4d_data->data[i] != NULL) 
//         {
//             free(*tensor4d_data->data[i]);
//             tensor4d_data->data[i] = NULL;            
//         }
//     }
    
//     // �ͷ�����
//     if (tensor4d_data->data[i] != NULL) 
//     {
//         free(*tensor4d_data->data);
//         tensor4d_data->data = NULL;        
//     }
// }


// /**
// * ��ά������ֵ
// * ˵��: ������ά��ʼ��֮��, �����ݸ�ֵ����ά����.
// * @param[in]  tensor4d_data    -> ����ֵ�����ṹ��
// * @param[in]  tensor4d_data_in -> ��ֵ����(������λ��ַ)
// * @param[in]  t4di_len         -> ��ֵ���ݳ���
// * @param[out] Null
// * @retval Null
// * @par Null
// * Built-By: Zehua Du
// * Date: Mar. 18, 2022  
// */
// void Tensor4DSet(Tensor4D *tensor4d_data, DATA *tensor4d_data_in, int t4di_len)
// {
//     // ��ֵ�豣֤��ֵ����tensor3d_data_in�뱻��ֵ����tensor3d_data����һ��
//     int m4d_len = (tensor4d_data->tsnum) * (tensor4d_data->chnum) * (tensor4d_data->row) * (tensor4d_data->col);
//     if (m4d_len != t4di_len)
//     {
//         fprintf(stderr, "ERROR: Tensor4DSet => (tensor4d_data[%d] != tensor3d_data_in[%d])\n", m4d_len, t4di_len);
//         exit(EXIT_FAILURE);
//     }

// 	int i, j, k, h;  // ����, ͨ��, ��, ��
//     int tsr_gain;    // ��������
//     int chn_gain;    // ͨ������
//     int row_gain;    // ������
// 	for (i = 0; i < (tensor4d_data->tsnum); i++)
//     {
// 		for (j = 0; j < (tensor4d_data->chnum); j++)
//         {
//             for (k = 0; k < (tensor4d_data->row); k++)
//             {
//                 for (h = 0; h < (tensor4d_data->col); h++)
//                 {
//                     tsr_gain = i * (tensor4d_data->chnum) * (tensor4d_data->row) * (tensor4d_data->col);
//                     chn_gain = j * (tensor4d_data->row) * (tensor4d_data->col);
//                     row_gain = k * (tensor4d_data->col);
//                     tensor4d_data->data[i][j][k][h] = *(tensor4d_data_in + tsr_gain + chn_gain + row_gain + h); //tensor4d_data_in[����][ͨ��][��][��];
//                 }
//             }
//         }
// 	}
// }