## get kernel code
apt-cache search linux-source

查找对应的内核版并安装

```
sudo apt install linux-source-4.4.0
cp /usr/src/linux-source-4.4.0/linux-source-4.4.0.tar.bz2 /home/lbbxsxlz/kernel-src/
cd /home/lbbxsxlz/kernel-src/
tar jxvf linux-source-4.4.0.tar.bz2
```

## setup kernel config
```
cd /home/lbbxsxlz/kernel-src/linux-source-4.4.0/
make headers_install
```
make menuconfig 提示找不到curse.h 需要解决编码问题
```
sudo apt-get install libncurses5-dev
make menuconfig
make -j8
```

## compile samples/bpf
sudo apt-get install libelf-dev

修改 samples/bpf/Makefile下的LLC改成实际的llc

sudo make M=samples/bpf

