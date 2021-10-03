
## 修改Makefile
~/kernel-src/linux-source-4.15.0/samples/bpf/Makefile

hostprogs-y += xdp_ip_tracker

xdp_ip_tracker-objs := bpf_load.o $(LIBBPF) xdp_ip_tracker_user.o

always += xdp_ip_tracker_kern.o

HOSTLOADLIBES_xdp_ip_tracker += -lelf

## 编译
sudo make M=samples/bpf


## 启动docker
通过ip a命令确定 容器的网卡编号是6
故 xdp_ip_tracker_user.c中的ifindex = 6

## 启动测试命令
sudo samples/bpf/xdp_ip_tracker 

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



