#ifndef LOAD_DATA_H
#define LOAD_DATA_H

#include "Tensor.h"

void loadweight_csv2tensor(char const *filename, Tensor *tensor_data, int *ts_shape, char *ts_type);

void loadbias_csv2array(char const *filename, Array *arr_data, int len);


#endif
