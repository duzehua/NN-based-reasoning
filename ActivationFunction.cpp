/**
* @file ActivationFunction
* @ 激活函数, 对输入的张量进行激活处理.
* @details 无
* @Zehua Du
* @date Mar. 24, 2022  
* @version v1.0
* @par Copyright(c): 
* @par History:
*version: author, date, desc\n
*/ 

#include <math.h>
#include "Tensor.h"


/**
* RELU激活
* 说明: 对输入的张量进行激活处理, 将所有小于0的值置为0.
* @param[in]  conved_Tsr -> 待激活张量结构体
* @param[out] Null
* @retval Null
* @par Null
* Built-By: Zehua Du
* Date: Mar. 24, 2022  
*/
void RELU(Tensor *conv_Tsr)
{
    // 直接对输入的卷积进行激活操作
    int i, j, k, h;
    for (i = 0; i < conv_Tsr->tsnum; i++)
    {
        for (j = 0; j < conv_Tsr->chnum; j++)
        {
            for (k = 0; k < conv_Tsr->row; k++)
            {
                for (h = 0; h < conv_Tsr->col; h++)
                {
                    if ((conv_Tsr->data[i][j][k][h]) < 0)
                    {
                        conv_Tsr->data[i][j][k][h] = 0;
                    }
                }
            }
        }
    }
}