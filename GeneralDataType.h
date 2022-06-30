#ifndef GENERALDATATYPE_H
#define GENERALDATATYPE_H

    #include "GINFO.h"

    /**
    * @矩阵结构体定义
    * @details 无
    */
    typedef struct
    {
        DATA **data; /*!<矩阵数据 [row][col]>*/
        int    row;  /*!<矩阵行数>*/
        int    col;  /*!<矩阵列数>*/
    }Matrix;

    /**
    * @数组结构体定义
    * @details 无
    */
    typedef struct
    {
        DATA *data;  /*!<数组数据 [idx]>*/
        int    len;  /*!<数组长度>*/
    }Array;

    // 矩阵部分
    void MatrixInitial(Matrix *mat_data, int row, int col);
    void MatrixFree(Matrix *mat_data);
    void MatrixSet(Matrix *mat_data, DATA *mat_data_in, int mdi_len);

    // 数组部分
    void ArrayInitial(Array *arr_data, int len);
    void ArrayFree(Array *arr_data);
    void ArraySet(Array *arr_data, DATA *arr_data_in, int adi_len);

#endif