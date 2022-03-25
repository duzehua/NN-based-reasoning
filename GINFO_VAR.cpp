#include "GINFO.h"

// 张量参数
TENSORTYPE  TENSOR2_STR[7] = "matrix";   // 字符串(矩阵), 表示张量类型
TENSORTYPE  TENSOR3_STR[9] = "tensor3d"; // 字符串(张量-3维), 表示张量类型
TENSORTYPE  TENSOR4_STR[9] = "tensor4d"; // 字符串(张量-4维), 表示张量类型

TENSORSHAPE TENSOR2_SHAPE[2] = {0, 1};
TENSORSHAPE TENSOR3_SHAPE[3] = {0, 0, 0};
TENSORSHAPE TENSOR4_SHAPE[4] = {0, 0, 0, 0};

// 卷积模式
CONVMODE FULL[5]  = "FULL";
CONVMODE SAME[5]  = "SAME";
CONVMODE VALID[6] = "VALID";





