#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "NormalizationFunction.h"

/**
 * @brief �����С��һ��
 *
 * @param input_tensor Ҫ��һ��������
 * @param inmin ������������Сֵ
 * @param inmax �������������ֵ
 * @param outmin ��һ������Сֵ
 * @param outmax ��һ�������ֵ
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