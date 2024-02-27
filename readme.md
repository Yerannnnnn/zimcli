# 安装 Cmake

## cmake版本检查
1. 在 linux 环境下执行`cmake --version`，如果没有 cmake，或者cmake版本低于 3.15，则需要先安装下 cmake
2. 执行yum info cmake，看下 yum 的 cmake 版本，如果版本大于3.15，可以直接`yum install cmake`
3. yum 上的 cmake 版本太旧的话， 按下面的步骤安装 cmake

```bash
yum install openssl openssl-devel
wget -c https://github.com/Kitware/CMake/releases/download/v3.17.0-rc3/cmake-3.17.0-rc3.tar.gz
tar zxvf cmake-3.17.0-rc3.tar.gz
cd cmake-3.17.0-rc3
./bootstrap
gmake
gmake install
```
# gcc环境检查
执行`gcc -v`，如果命令不存在，则需要执行`yum install gcc`

# 构建命令行工具

下载本仓库后，进入根目录，按以下步骤执行编译

```bash
cd zimcli
mkdir build
cd build
cmake ..
make
./zimcli --help
```

# 用法示例
```bash
Usage: ./zimcli [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  --appid INT                 appid
  --appsign TEXT              appsign
  --sender TEXT               sender's userID
  --receiver TEXT             receiver's userID
  --qps INT [1]               qps, 1~10. Default is 1.
  --execution-time INT [300]  The execution time of this command line programs. 0~900s. Default is 300s.
```

注意事项：
1. 要使用正确的 appid 和 appsign
2. 执行前需要提前批量注册用户


完整参数示例:

```bash
./zimcli --appid 111111111 --appsign ****** --sender test_sender_id --receiver test_receiver_id --qps 1 --execution-time 300
```
