#ifndef GINF_VAR_H
#define GINF_VAR_H

#include "GINFO.h"

    // 张量参数
    extern TENSORTYPE  TENSOR1_STR[6];
    extern TENSORTYPE  TENSOR2_STR[7];
    extern TENSORTYPE  TENSOR3_STR[9];
    extern TENSORTYPE  TENSOR4_STR[9];

    extern TENSORSHAPE TENSOR1_SHAPE[1];
    extern TENSORSHAPE TENSOR2_SHAPE[2];
    extern TENSORSHAPE TENSOR3_SHAPE[3];
    extern TENSORSHAPE TENSOR4_SHAPE[4];

    // 卷积模式
    extern CONVMODE FULL[5];
    extern CONVMODE SAME[5];
    extern CONVMODE VALID[6];

    /**
    * @卷积步长
    * @details 需在main函数中起始位置定义, 并赋初始值. 在卷积操作前通过该结构体定义卷积步长并作为实参传入卷积运算函数.
    */
    typedef struct
    {
        // int batch;   /*!<样本滑动步长>*/
        // int channel; /*!<通道滑动步长>*/
        int row;     /*!<行滑动步长>*/
        int col;     /*!<列滑动步长>*/
    }CONVSTRIDE;

    /**
    * @池化尺寸
    * @details 需在main函数中起始位置定义, 并赋初始值. 在池化操作前通过该结构体定义池化尺寸并作为实参传入池化运算函数.
    */
    typedef struct
    {
        int row;     /*!<池化核行数>*/
        int col;     /*!<池化核列数>*/
    }POOLSIZE;

    /**
    * @池化步长
    * @details 需在main函数中起始位置定义, 并赋初始值. 在池化操作前通过该结构体定义池化步长并作为实参传入池化运算函数.
    */
    typedef struct
    {
        // int batch;   /*!<样本滑动步长>*/
        // int channel; /*!<通道滑动步长>*/
        int row;     /*!<行滑动步长>*/
        int col;     /*!<列滑动步长>*/
    }POOLSTRIDE;

#endif