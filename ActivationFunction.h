#ifndef ACTIVATIONFUNCATION_H
#define ACTIVATIONFUNCATION_H

    #include "GINFO.h"
    #include "Tensor.h"

    void RELU(Tensor *conv_Tsr);
    void TANH(Tensor *conv_Tsr);
    void SOFTMAX(Tensor *conv_Tsr);
#endif