#多文件编译的CMakeLists.txt
#最低版本号
cmake_minimum_required(VERSION 2.8)
#项目信息
project(Demo1)
#指定目标文件，编译多个文件就将多个文件加到add_executable中，这样当然是有问题的
#不过先从浅入深
add_executable(main main.cpp test.cpp)
