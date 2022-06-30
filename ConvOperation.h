#ifndef CONVOPERATION_H
#define CONVOPERATION_H

#include "GINFO.h"
#include "Tensor.h"

    // void Conv2Matrix(Matrix conved_mat, Matrix conv_kel);
    // void Conv2Matrix(Matrix *conved_Matrix, Matrix *conv_kel, Matrix *conv_result);
    // void Conv2Tensor3D(Tensor3D *conved_Tsr3D, Matrix *conv_kel, Tensor3D *conv_result);
    // void Conv2Tensor4D(Tensor4D *conved_Tsr4D, Matrix *conv_kel, Tensor4D *conv_result);
    void Conv2(Tensor *conved_Tsr, Tensor *conv_kel, CONVSTRIDE *conv_stride, Tensor *conv_bias, char *conv_mode, Tensor *conv_result);

#endif