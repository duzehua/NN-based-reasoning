#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "NormalizationFunction.h"

/**
 * @brief 最大最小归一化
 *
 * @param input_tensor 要归一化的张量
 * @param inmin 输入张量的最小值
 * @param inmax 输入张量的最大值
 * @param outmin 归一化的最小值
 * @param outmax 归一化的最大值
*/
void MinMaxNormalization(Tensor* input_tensor, DATA inmin, DATA inmax, DATA outmin, DATA outmax)
{
	for (int i = 0; i < input_tensor->tsnum; i++)
	{
		for (int j = 0; j < input_tensor->chnum; j++)
		{
			for (int k = 0; k < input_tensor->row; k++)
			{
				for (int l = 0; l < input_tensor->col; l++)
				{
					//printf("NORMALIZATION input: %f\n", input_tensor->data[i][j][k][l]);
					input_tensor->data[i][j][k][l] = (input_tensor->data[i][j][k][l] - inmin) * (outmax - outmin) / (inmax - inmin) + outmin;
					//printf("NORMALIZATION output: %f\n", input_tensor->data[i][j][k][l]);
				}
			}
		}
	}
}