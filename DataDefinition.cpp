#include "GeneralDataType.h"

void QuantifiedDataType(double *data_for_quantified, Array *data_quatified, int data_num)
{
    // 先初始化数据
    ArrayInitial(data_quatified, data_num);
    for (int i = 0; i < data_num; i++)
    {
        data_quatified->data[i] = (float)data_for_quantified[i];
    }
}