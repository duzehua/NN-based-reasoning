
[toc]

# NN-based-reasoning

## Part 1 Lenet5 C++实现推理
*本次测试环境为 Windows + VS + Matlab + Opencv*
### 相关源码文件列表
- show_mnist.m decodefile.m 使用matlab解析MNIST数据集并生成图片
- lenet5_pytorch.ipynb 主要用于训练和生成lenet5模型的权重参数
- ActivationFunction.cpp/h  激活函数
- Convoperation.cpp/h 卷积函数
- DataDedinition.cpp/h 数据类型定义
- FullyConnectedFunction.cpp/h  全连接函数
- GINFO.h 全局定义
- GINFO_VAR.cpp/h 全集定义
- GeneralDataType.cpp/h 数据类型定义
- GetFileName.cpp/h 获取文件名
- LayerParamLoad.cpp/h 加载权重参数
- NormalizationFunction.cpp/h 归一化函数
- PoolingOperation.cpp/h 池化函数
- StringOperation.cpp/h 字符操作
- decodepng.cpp/h (不依赖其他库进png图片解析)
- decodeImage_CV.cpp/h (依赖OpenCV)
- lenet5_test1/2/3.cpp lenet5推理主函数（版本不同对应不同时期的成果）
- lenet5_test1_cmex.cpp lenent5_test1_mex.m Matlab进行编译
- 其余文件（不重要可废弃）
### 1.如何运行
#### 1.1打开并运行lenet5_pytorch.ipynb文件
为了训练模型和生成权重参数(分别存放在生成的checkpoint、layer_para、layeroutput_data文件夹下）
#### 1.2打开并运行show_mnist.m
相关依赖文件有decodefile.m和MNIST数据集（上一步可得到），运行此文件可以生成多个数字图片存放与test_image文件夹下
#### 1.3编译与运行lenet5_test1/2/3.cpp
使用VS将相关的CPP/H文件放在一个项目中，注意加入权重参数和图片文件夹，运行可以得到推理结果。（暂未尝试Makefile编译方式）
#### 1.4使用Matlab进行混编
相关依赖文件为lenet5_test1_cmex.cpp lenent5_test1_mex.m，先配置好matalb的mex编译器（mex -setup），编译命令为“mex ActivationFunction.cpp ConvOperation.cpp DataDefinition.cpp FullyConnectedFunction.cpp PoolingOperation.cpp NormalizationFunction.cpp GeneralDataType.cpp GetFileNames.cpp GINFO_VAR.cpp StringOperation.cpp LayerParamLoad.cpp Tensor.cpp lenet5_test1_cmex.cpp”即编译所有相关文件，会生成“ActivationFunction.mexw64”文件，然后再运行lenent5_test1_mex.m

