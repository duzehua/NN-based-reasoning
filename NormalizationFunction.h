#ifndef NORMALIZATION_H
#define NORMALIZATION_H

#include "Tensor.h"

void MinMaxNormalization(Tensor* input_tensor, DATA inmin, DATA inmax, DATA outmin, DATA outmax);


#endif // !NORMALIZATION_H

