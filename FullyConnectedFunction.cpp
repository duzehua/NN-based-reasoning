#include <stdlib.h>
#include <error.h>
#include <stdio.h>

#include "Tensor.h"
#include "GINFO_VAR.h"

/**
* 张量平铺
* 说明: 用于卷积模块之后, 全连接模块之前, 对张量进行平铺展开.
* @param[in]  conved_Tsr   -> 待展开的张量
* @param[in]  flatten_Data -> 展开后的张量
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

    // 行代表个数, 列代表具体的数值
    TENSOR2_SHAPE[0] = conved_Tsr->tsnum;
    TENSOR2_SHAPE[1] = m_len;

    // MatrixInitial(flatten_Data, conved_Tsr->tsnum, m_len);
    TensorInitial(flatten_Data, TENSOR2_STR, TENSOR2_SHAPE);

    int i, j, k, h;  // 个数, 通道, 行, 列
    int tsr_gain;    // 个数增益
    int chn_gain;    // 通道增益
    int row_gain;    // 行增益
    int idx_fla_d;  // 展开数组索引
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
* 全连接操作
* 说明: 对输入数据进行全连接.
* @param[in]  m_data_in    -> MxN 全连接操作输入
* @param[in]  weight       -> NxP   全连接操作权重
* @param[in]  bias         -> Px1   全连接操作偏置
* @param[in]  m_data_out   -> MxP 全连接操作输出
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
    // weight N行，P列
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

    // 遍历每个样本
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
