#include <stdlib.h>
#include <error.h>
#include <stdio.h>

#include "Tensor.h"
#include "GINFO_VAR.h"

/**
* ����ƽ��
* ˵��: ���ھ��ģ��֮��, ȫ����ģ��֮ǰ, ����������ƽ��չ��.
* @param[in]  conved_Tsr   -> ��չ��������
* @param[in]  flatten_Data -> չ���������
* @param[out] Null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Apr. 21, 2022  
*/
void TensorFlatten(Tensor *conved_Tsr, Tensor *flatten_Data)
{
    int m_len = (conved_Tsr->chnum) * (conved_Tsr->row) * (conved_Tsr->col);
    // flatten_Data = (DATA*)malloc(m_len * sizeof(DATA));

    // �д������, �д���������ֵ
    TENSOR2_SHAPE[0] = conved_Tsr->tsnum;
    TENSOR2_SHAPE[1] = m_len;

    // MatrixInitial(flatten_Data, conved_Tsr->tsnum, m_len);
    TensorInitial(flatten_Data, TENSOR2_STR, TENSOR2_SHAPE);

    int i, j, k, h;  // ����, ͨ��, ��, ��
    int tsr_gain;    // ��������
    int chn_gain;    // ͨ������
    int row_gain;    // ������
    int idx_fla_d;  // չ����������
 	for (i = 0; i < (conved_Tsr->tsnum); i++)
    {
        idx_fla_d = 0;
		for (j = 0; j < (conved_Tsr->chnum); j++)
        {
            for (k = 0; k < (conved_Tsr->row); k++)
            {
                for (h = 0; h < (conved_Tsr->col); h++)
                {
                    // flatten_Data->data[i][idx_fla_d] = conved_Tsr->data[i][j][k][h];
                    flatten_Data->data[0][0][i][idx_fla_d] = conved_Tsr->data[i][j][k][h];
                    idx_fla_d++;
                }
            }
        }
	}
}


/**
* ȫ���Ӳ���
* ˵��: ���������ݽ���ȫ����.
* @param[in]  m_data_in    -> MxN ȫ���Ӳ�������
* @param[in]  weight       -> NxP   ȫ���Ӳ���Ȩ��
* @param[in]  bias         -> Px1   ȫ���Ӳ���ƫ��
* @param[in]  m_data_out   -> MxP ȫ���Ӳ������
* @param[out] Null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Apr. 21, 2022  
*/
void FullConnected(Tensor *m_data_in, Matrix *weight, Matrix *bias, Tensor *m_data_out)
{
    //
    // 1*N x N*P => 1*P
    // weight N�У�P��
    int data_o_num = (m_data_in->row);  
    int data_i_dim = (m_data_in->col);

    int data_w_dim = (weight->row);
    int data_o_dim = (weight->col);

    int data_b_dim = (bias->row);
    int data_b_len = (bias->col);

    if (data_i_dim != data_w_dim)
    {
        fprintf(stderr, "ERROR: FullConnected => (m_data_in->col[%d] != weight->row[%d])\n", data_i_dim, data_w_dim);
        exit(EXIT_FAILURE);
    }

    if (data_o_dim != data_b_dim)
    {
        fprintf(stderr, "ERROR: FullConnected => (weight->col[%d] != bias->row[%d])\n", data_o_dim, data_b_dim);
        exit(EXIT_FAILURE);
    }

    TENSOR2_SHAPE[0] = data_o_num;
    TENSOR2_SHAPE[1] = data_o_dim;
    // MatrixInitial(m_data_out, data_o_num, data_o_dim);
    TensorInitial(m_data_out, TENSOR2_STR, TENSOR2_SHAPE);

    int idx_M, idx_P, idx_N, h;

    // ����ÿ������
    //[idx_num][idx_P]
    for (idx_M = 0; idx_M < (m_data_in->row); idx_M++)
    {
        for (idx_P = 0; idx_P < data_o_dim; idx_P++)
        {
            m_data_out->data[0][0][idx_M][idx_P] = bias->data[idx_P][0];
            for (idx_N = 0; idx_N < data_i_dim; idx_N++)
            {
                m_data_out->data[0][0][idx_M][idx_P] = m_data_out->data[0][0][idx_M][idx_P] + (m_data_in->data[0][0][idx_M][idx_N])*(weight->data[idx_N][idx_P]);
            }
        }
    }
}
