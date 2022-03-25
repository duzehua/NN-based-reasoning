main:Tensor.o ConvOperation.o ActivationFunction.o PoolingOperation.o GINFO_VAR.o main.o 
# g++ -fopenmp Tensor.o ConvOperation.o ActivationFunction.o GINFO_VAR.o main.o -o main
	gcc Tensor.o ConvOperation.o ActivationFunction.o PoolingOperation.o GINFO_VAR.o main.o -o main

# 功能函数----------------------

Tensor.o:Tensor.cpp
# g++ -fopenmp -c Tensor.cpp
	gcc -c Tensor.cpp

ConvOperation.o:ConvOperation.cpp
# g++ -fopenmp -c ConvOperation.cpp
	gcc -c ConvOperation.cpp

ActivationFunction.o:ActivationFunction.cpp
# g++ -fopenmp -c ActivationFunction.cpp
	gcc -c ActivationFunction.cpp

PoolingOperation.o:PoolingOperation.cpp
# g++ -fopenmp -c ActivationFunction.cpp
	gcc -c PoolingOperation.cpp

# 全局参数----------------------

GINFO_VAR.o:GINFO_VAR.cpp
# g++ -fopenmp -c GINFO_VAR.cpp
	gcc -c GINFO_VAR.cpp	

main.o:main.cpp
# g++ -fopenmp -c main.cpp
	gcc -c main.cpp

.PHONY:clean

#linux 下用 rm -rf *.o main
clean:
	@echo "=======clean project========="
	del  *.o 
	@echo "=======clean completed========="