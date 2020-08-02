# Thread Pool

参考《linux高性能服务器编程》- 游双实现的简单linux下的线程池。

#### 文件结构

locker.h：linux下同步机制的封装，方便调用，包括信号量、互斥锁、条件变量。

threadpool.h：线程池类实现

test.cpp：测试代码

makefile：makefile文件

#### 编译与运行

编译：make test

运行：./test

