# fla-turingMachine
这个是形式语言与自动机大作业：使用c++实现一个多纸带图灵机。

#### 使用方法

编译：`/turing-project/cmake .` && `/turing-project/make`

编译后的可执行文件在`/bin`中。

运行：

-  获取帮助：`/bin/turing -h`
- 执行程序：`/bin/turing {tmFile} {tape}`，`tmFile`为.tm后缀的图灵机程序文件(格式见项目要求.pdf)，`tape`为图灵机的第一条纸带的初始化值。
  - 详细的使用方法可以参考项目要求.pdf。

#### 项目目录组成

```
- /bin: 输出的可执行文件
- /examples: 大作业要求给出的参考图灵机程序及执行效果
- /programs: 实现的两个大作业要求的图灵机程序
- /turing-project: 源码仓库
- CMakeLists.txt
- README.txt
- 项目要求.pdf
```



