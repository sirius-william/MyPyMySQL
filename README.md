# 项目概况

这是一个Python连接MySQL的库，采用c++编写。利用Boost.Python C++库，将C++的函数或类进行包装，暴露给Python进行import调用。

# 最新

+ 完善了execute函数的功能：

    1）增加了SQL语句异常处理，并显示在日志中；

    2）对于非查询语句，返回受到影响的行数值的字典，字典仅含字段'affect_rows'；

    3）提升了代码稳定性

+ 完善了日志系统：

    1）定义了日志级别：STATUS、INFO、FATAL、WARNING；

    2）固定了日志显示格式；

    3）SQL语句出现错误时，会显示MySQL返回的SQL错误信息；

    4）用户可设置是否显示日志；

    5）当发生错误时，返回带有err字段的字典；

+ 完善了自动提交：

    1）用户可自定义是否自动提交更新；

    2）用户可在python内通过commit()函数来手动提交更新；

+ 内部优化：

    1）用户可以访问对象的公共属性（其实只能访问option）

    2）用户可以访问Option对象的属性

    3）新增了新的构造函数，可以传入Option对象，来方便配置

# 环境

编程环境：Boost 1.74.0，Python3.7.7，Windows10

编译环境：

对于编译环境，我要开始吐槽了。

用vs2019，可完美编译。

用Cmake + Jetbrains Clion方式，可能是我水平不行，玩转不来Cmake，导致Boost动态链接库一直都无法连接。

# 编译

无论是用Visual Studio还是CMake，在这里我只阐述工程所需要的头文件与依赖库。

1、以Visual Studio为例：

C/C++头文件目录：

​			Python安装目录下的include目录；

​			MySQL安装目录下的include目录；

​			boost解压文件夹（下载的压缩包解压，这个自身就是个头文件目录）。

C/C++库目录：

​			Python安装文件夹下的libs目录；

​			MySQL安装文件夹下的lib目录，并手动把目录内的libmysql.dll动态链接库复制到生成文件夹内，这个文件与生成的动态链接库永远在一起；

​			Boost编译生成的动态链接库目录，默认为解压目录内的stage/lib目录。

链接器需要链接的动态链接库：

​			Python3.x.lib（版本按着自己的来，Python2没试过不知道能不能用）

​			libmysql.lib

生成后将生成的dll文件改扩展名为pyd，连同libmysql.dll一同复制到Python安装文件夹内的Dlls文件夹，便可以在Python中使用。

2、对于CMake

​		需要find_package的包有：Python3（REQUIRED），Boost COMPONENT python37 REQUIRED

​		需要检查的文件：MySQL安装目录/include/mysql.h、MySQL安装目录/lib/libmysql.lib

​		需要复制的文件：MySQL安装目录/lib/libmysql.dll，TARGET为生成目录

​		add_library的方式为SHARED，

如果谁成功配置出了CMakeLists.txt文件并成功编译，麻烦踹我一脚谢谢~~~本人写的CMakeLists也放进来了，求指导。

# 目前代码存在的不足

仅实现了执行正确的SQL语句，尚未添加SQL语句执行出错的代码。

# 联系

如有问题，请联系我。（废话）

本人新人，代码极为青涩，有任何建议欢迎提出。

嘻嘻嘻。