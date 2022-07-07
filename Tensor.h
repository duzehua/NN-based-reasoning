#ifndef TENSOR_H
#define TENSOR_H

#include "GINFO.h"


/**
* @�����ṹ�嶨��
* @details ������\3ά����\4ά����ȫ�������һ������, ͨ�������������
*/
typedef struct
{
	DATA**** data;     /*!<�������� [tsnum][chnum][row][col]*/
	char* ts_type;
	int* ts_shape;
	// ts_shape ts_shape_;
	int      tsnum;    /*!<��������>*/
	int      chnum;    /*!<����ͨ��>*/
	int      row;      /*!<��������>*/
	int      col;      /*!<��������>*/
	// int      i_type[3] = {1, 2, 3};                          /*!<����������������*/
	// char     s_type[3] = {'Matrix', 'Tensor3D', 'Tensor4D'}; /*!<���������ַ�������*/
}Tensor;

///**
//* @����ṹ�嶨��
//* @details ��
//*/
//typedef struct
//{
//    DATA** data; /*!<�������� [row][col]>*/
//    int    row;  /*!<��������>*/
//    int    col;  /*!<��������>*/
//}Matrix;
//
///**
//* @����ṹ�嶨��
//* @details ��
//*/
//typedef struct
//{
//    DATA* data;  /*!<�������� [idx]>*/
//    int    len;  /*!<���鳤��>*/
//}Array;

// ͨ����������
void TensorInitial(Tensor* tensor_data, char* ts_type, int* ts_shape);
void TensorFree(Tensor* tensor_data);
void TensorSet(Tensor* tensor_data, DATA* tensor_data_in, int ti_len);
void TensorPaddingZero(Tensor* tensor_data);
void ViewTensor(Tensor* tensor_data);
void TensorTranspose2D(Tensor* in_tensor, Tensor* out_tensor);

//// ���󲿷�
//void MatrixInitial(Matrix* mat_data, int row, int col);
//void MatrixFree(Matrix* mat_data);
//void MatrixSet(Matrix* mat_data, DATA* mat_data_in, int mdi_len);
//
//// ���鲿��
//void ArrayInitial(Array* arr_data, int len);
//void ArrayFree(Array* arr_data);
//void ArraySet(Array* arr_data, DATA* arr_data_in, int adi_len);



#endif
