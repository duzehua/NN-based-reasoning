#ifndef POOLINGOPERATION_H
#define POOLINGOPERATION_H

    #include "Tensor.h"
    #include "GINFO_VAR.h"

    void MaxPooling2D(Tensor *conved_Tsr, int *pool_size, CONVSTRIDE *strides, char *conv_mode);



#endif