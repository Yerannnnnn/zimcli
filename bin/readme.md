# 部署

编译后将这`libZIM.so`,`zimcli`两个文件部署到目标机器上即可

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
./zimcli --sender test_sender_id --receiver test_receiver_id --qps 1 --execution-time 300
```

# 如果遇到无法找到libZIM.so的问题

报错如下：
```bash
/zimcli: error while loading shared libraries: libZIM.so: canmnot open shared object file: No such file
or directory
```

则需要导出链接地址

```bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`
./zimcli --help
```
