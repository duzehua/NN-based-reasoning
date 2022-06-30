#ifndef FULLYCONNECTEDFUNCTION_H
#define FULLYCONNECTEDFUNCTION_H

    #include "Tensor.h"
    void TensorFlatten(Tensor *conved_Tsr, Tensor *flatten_Data);
    void FullConnected(Tensor *m_data_in, Tensor *weight, Tensor *bias, Tensor *Tensor);

#endif