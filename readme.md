# 部署

> 该二进制使用 centos 环境。
> 二进制程序在./bin中。

1. 批量部署时将编译产物`zimcli`和`libZIM.so`库文档放到目标机器上即可。

```bash
$ ls
zimcli libZIM.so
```

2. 将libZIM.so所在目录导出
```bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`
```

3. 执行
```bash
./zimcli --help
```


# 如果遇到无法找到libZIM.so的问题，报错如下：

```bash
/zimcli: error while loading shared libraries: libZIM.so: canmnot open shared object file: No such file
or directory
```
就是没导出LD_LIBRARY_PATH， 参考上面的第二步

```bash
$ ls
zimcli libZIM.so
$ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`
$ ./zimcli --help
```


# 用法示例

```bash
Usage: ./zimcli [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  --sender TEXT               sender's userID
  --receiver TEXT             receiver's userID
  --qps INT [1]               qps, 1~10. Default is 1.
  --debug INT [0]             debug or not. Default is 0.
  --execution-time INT [300]  The execution time of this command line programs. 0~900s. Default is 300s.
```

注意事项：
1. 要使用正确的 appid 和 appsign
2. 执行前需要提前批量注册用户


完整参数示例:

```bash
./zimcli --sender test_sender_id --receiver test_receiver_id --qps 1 --execution-time 300 --debug 1
```