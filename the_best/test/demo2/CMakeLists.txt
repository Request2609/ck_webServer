#当源文件和多的时候，使用aux_source_directory
cmake_minimum_required(VERSOIN 2.8)
#项目信息
project(demo2)
#查找当前目录中的所有源文件
#将名称保存在DIR_SRCS变量
aux_source_directory(. DIR_SRCS)
#指定生成目标
add_executable(demo ${DIR_SRCS}) 
