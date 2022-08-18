# User-Equilibrium

## General

Frank Wolfe 求解 UE 的代码，替换掉根目录同名代码即可

路网通过Node类和Network类实现，求解相关的函数单独写在了对应的.h和.cpp文件中，可相应进行拓展

具体细节见代码注释

## Parallelization

即根目录中的代码，基于 MPI 修改 General 中的代码实现并行计算

在本例中并行化了全有全无分配和目标函数的求解，当进程数为 4 时，便可达到 3.024 的加速比

目前仅在一台机器上测试，下一步计划完成在服务器上的测试