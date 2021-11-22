
## 修改Makefile
vi ~/kernel-src/linux-source-4.15.0/samples/bpf/Makefile

修改四处，分别添加
hostprogs-y += xdp_ip_tracker

xdp_ip_tracker-objs := bpf_load.o $(LIBBPF) xdp_ip_tracker_user.o

always += xdp_ip_tracker_kern.o

HOSTLOADLIBES_xdp_ip_tracker += -lelf

## 编译
sudo make M=samples/bpf

## 启动docker
通过ip a命令确定 容器的网卡编号是6，故可以在xdp_ip_tracker_user.c中指定ifindex = 6

目前的代码网卡索引通过命令行参数传入网卡名或索引号来获取。

## 启动测试命令
sudo samples/bpf/xdp_ip_tracker $ifname/$ifindex

e.p. sudo samples/bpf/xdp_ip_tracker veth4195f5d/6

在另一个窗口中访问容器
curl 127.0.0.1：49153

出现以下打印：
	start a new loop...
	start a new loop...
	start a new loop...
	The local ip of next key in the map is: '172.17.0.1'
	The remote ip of next key in the map is: '172.17.0.2'
	rx_cnt value read from the map: '1'
	rx_bytes value read from the map: '74'

## bpftool
编译 cd ~/kernel-src/linux-source-4.15.0;make -C tools/bpf/bpftool/

tools/bpf/bpftool/bpftool
Usage: tools/bpf/bpftool/bpftool [OPTIONS] OBJECT { COMMAND | help }
       tools/bpf/bpftool/bpftool batch file FILE
       tools/bpf/bpftool/bpftool version

       OBJECT := { prog | map }
       OPTIONS := { {-j|--json} [{-p|--pretty}] | {-f|--bpffs} }

## 编译libbpf
解决找不到bpd.h
sudo apt install binutils-dev  ##安装bunutils-dev需要先apt update

解决找不到readline.h
sudo apt install libreadline-dev
