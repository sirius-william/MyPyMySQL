# 项目概况

这是一个Python连接MySQL的库，采用c++编写。分为纯Python和使用boost两种编写方式。

纯Python仅使用了Python提供的原生API，仅使用Python.h头文件，优点是占用空间小，缺点是编写难度大，代码冗长。

boost方式利用了boost动态链接库，使用命名空间boost::python，利用boost封装好的接口来实现，缺点是依赖文件加载很慢，优点是编写难度低。

# 环境

1、目前由于精力有限，CMakeLists.txt只编写了windows部分，故目前只能在windows环境下编译，有能力的小伙伴可以自行修改CMakeLists文件来在其他平台下编译。

如要在其他平台下编译，CMakeLists需要修改gcc，g++，cmake目录，Python3安装目录内include文件夹，libs文件夹，检查Python3安装目录内libs文件夹内的python3的静态库名称，检查mysql安装目录文件夹内lib文件夹下的libmysql动态链接库与静态链接库，修改cmakelists内的file命令，该命令用于把libmysql动态链接库复制到输出目录，这个文件是必须有的，以上种种，来重写编辑最下方的target_link_libraries。

对于文本消息，可自行编辑（其实文本信息我只是为了装x，显得输出多）。

2、编程环境

c++20标准，Clion编辑器，Python3.7动/静态链接库，MySQL Server 8.0，boost 1.74.0

3、编译环境

gcc，g++（或visual studio），Python3.x，MySQL Server 8.0（对于5.7是否支持尚不明确，还没倒出功夫去查阅MySQL8.0和5.7之间mysql.h和libmysql是否有区别，小伙伴们可以尝试一下），Boost

# 编译

1、修改CMakeLists.txt文件，也可在cmake-gui打开（我在cmake-gui打开后，使用目录浏览框的时候会闪退不知为何）。

修改变量如图：

![](.\image-20201102233633923.png)

根据自己电脑的环境配置好编译器，python，mysql，boost目录。

2、cd到项目根目录，命令行下输入：

```powershell
cmake . -G "xx"
```

其中xx为编译驱动。

命令行输入：

```powershell
cmake --help
```

查看所支持的驱动。本人使用驱动MinGW Makefiles，Clion编辑器使用的是CodeBlocks - MinGW Makefiles驱动，问题不大

3、项目根目录下，命令行：

```powershell
make
```

开始编译

4、生成的dll，so文件在bin目录下，检查bin目录下是否有libmysql动态链接库，如果没有，请务去mysql安装目录下的lib文件夹把libmysql复制过来，否则库无法运行。

5、将生成的文件扩展名改为pyd，将pyd与libmysql一同复制到python安装目录下的DLLS文件夹内，然后在python内使用。

# 联系

如有问题，请联系我。（废话）

本人新人，代码极为青涩，有任何建议欢迎提出。嘻嘻嘻。