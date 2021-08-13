## get kernel code
git clone --depth 1 git://kernel.ubuntu.com/ubuntu/ubuntu-xenial.git

## setup kernel config
make menuconfig 提示找不到curse.h 需要解决编码问题

sudo apt-get install libncurses5-dev
```
chmod a+x debian/rules
chmod a+x debian/scripts/*
chmod a+x debian/scripts/misc/*
fakeroot debian/rules clean
```
提示找不到kernel-wedge
```
sudo apt install kernel-wedge
fakeroot debian/rules clean
fakeroot debian/rules editconfigs   /* always enter Y，always exit and save */
```

fakeroot debian/rules clean

make -j8

## compile samples/bpf
```
sudo apt-get install libelf-dev
cd /home/lbbxsxlz/kernel-src/ubuntu-xenial/tools/lib/bpf
make;make install
cp /usr/local/lib64/libbpf.* /lib/x86_64-linux-gnu/
```
修改 samples/bpf/Makefile下的LLC改成实际的llc

sudo make M=samples/bpf

