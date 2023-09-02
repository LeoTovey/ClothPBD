# 编译胶水代码

首先编辑idl文件，固定接口代码
然后使用 
```bash
tools/webidl_binder my_classes.idl glue
```

然后产生胶水代码文件是glue.js和glue.cpp
然后把这个