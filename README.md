# Ray Tracing

姓名：陈天垚
学号：5140309566

# 编译环境

Ubuntu 16.04 或 Mac OS X 10.12.3
c++14

在`bin`文件夹下，使用`make`命令生成可执行文件`main`。

运行`./main`生成图片，图片保存在`output.ppm`文件中。

# 效果

## 软阴影

运行`git checkout soft-shadow`并编译运行。

![Soft Shadow](images/soft-shadow.png)

可以看出茶壶的阴影是软阴影。

茶壶由`2644`个点和`6320`个三角面片组成，模型文件为`models/teapot.obj`。

## 透射

运行`git checkout refraction`并编译运行

![Soft Shadow](images/refraction.png)

场景中有两个绿色的球，远的是绿色实心球，近的是折射率为`1.1`的透明球。

## Color Bleeding

运行`git checkout color-bleeding`并编译运行。

![Soft Shadow](images/color-bleeding.png)

地板的左右两侧的颜色略有不同，左侧偏绿，右侧偏蓝。


