#ifndef POOLINGOPERATION_H
#define POOLINGOPERATION_H

    #include "Tensor.h"
    #include "GINFO_VAR.h"

    void MaxPooling2D(Tensor *conved_Tsr, POOLSIZE *pool_size, CONVSTRIDE *strides, char *pool_mode, Tensor *pool_result);



#endif