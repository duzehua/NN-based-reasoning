#ifndef LOAD_DATA_H
#define LOAD_DATA_H

#include "Tensor.h"

void loadweight_csv2tensor(const char  *filename, Tensor *tensor_data, int *ts_shape, char *ts_type);

void loadFCweight_csv2Matrix(const char *filename, Matrix *mat_data);

void loadbias_csv2array(const char  *filename, Array *arr_data, int len);


#endif