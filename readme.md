# install cmake

```
yum install openssl openssl-devel
wget -c https://github.com/Kitware/CMake/releases/download/v3.17.0-rc3/cmake-3.17.0-rc3.tar.gz
tar zxvf cmake-3.17.0-rc3.tar.gz
cd cmake-3.17.0-rc3
./bootstrap
gmake
gmake install
```

# build zimcli

```dart
mkdir build
cd build
cmake ..
make
./zimcli --appid ** --appsign ** --sender user1 --receiver user2 --qps 1
```


