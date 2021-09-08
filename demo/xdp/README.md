# 验证
编译命令：
clang -O2 -Wall -target bpf -I /usr/include/x86_64-linux-gnu/ -c xdp-drop.c -o xdp-drop.o

## 编译错误
"error: use of undeclared identifier 'XDP_DROP'" 
 
修改代码头文件引用，
#include <linux/bpf.h>  改成 #include <uapi/linux/bpf.h>

拷贝内核头文件
```bash
sudo cp -rf /usr/src/linux-headers-4.15.18-041518/include/uapi/* /usr/include/uapi/
```

-I /usr/include/x86_64-linux-gnu/  确定引用asm下的头文件

## 测试
ip link set dev eth0 xdp obj xdp-drop.o sec xdp

报错：
Error: either "dev" is duplicate, or "xdp" is a garbage.

怀疑跟iproute2的版本有关，默认安装的iproute2版本过低，不支持xdp。

下载最新的iproute2-5.11.0，编译后，用以下命令验证，确认断网。
sudo /home/lbbxsxlz/workspace/iproute2-5.11.0/ip/ip link set dev eth0 xdp obj xdp-drop.o sec xdp verbose

Continuing without mounted eBPF fs. Too old kernel?
“
	Prog section 'xdp' loaded (5)!
	 - Type:         6
	 - Instructions: 2 (0 over limit)
	 - License:      GPL

	Verifier analysis:

	0: (b7) r0 = 1
	1: (95) exit
	processed 2 insns, stack depth 0
”

恢复网络：
sudo /home/lbbxsxlz/workspace/iproute2-5.11.0/ip/ip link set dev eth0 xdp off

## 使用docker验证
docker ps -a
	CONTAINER ID        IMAGE                        COMMAND             CREATED             STATUS              PORTS               NAMES
	44cce8b245a7        lbbxsxlz/ubuntu_20.04_ext4   "/bin/bash"         3 weeks ago         Up 21 hours                             test-show

启动docker
docker start test-show

确定docker容器的网卡
ip link show
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN mode DEFAULT group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
2: eth0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc mq state UP mode DEFAULT group default qlen 1000
    link/ether e4:b9:7a:f7:78:2b brd ff:ff:ff:ff:ff:ff
3: wlan0: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc mq state DOWN mode DORMANT group default qlen 1000
    link/ether 9c:b6:d0:c7:90:5d brd ff:ff:ff:ff:ff:ff
4: docker0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP mode DEFAULT group default 
    link/ether 02:42:c5:99:cb:63 brd ff:ff:ff:ff:ff:ff
6: veth8935654@if5: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue master docker0 state UP mode DEFAULT group default 
    link/ether 4a:0f:73:3c:52:6c brd ff:ff:ff:ff:ff:ff link-netnsid 0

确认docker容器的ip
docker inspect test-show --format='{{.NetworkSettings.Networks.bridge}}'
{<nil> [] [] 3c0f4a2efb769323551e2e7a8f0b611e59ff310876a1850536bfa4a0b7226546 25e07e86937b07245d4ff5f94f6e0aa2da53d226b3d096cc1db21271169361b8 172.17.0.1 172.17.0.2 16   0 02:42:ac:11:00:02 map[]}

对网卡veth8935654操作：
首先ping 172.17.0.2

	64 bytes from 172.17.0.2: icmp_seq=1 ttl=64 time=0.069 ms
	64 bytes from 172.17.0.2: icmp_seq=2 ttl=64 time=0.068 ms
	64 bytes from 172.17.0.2: icmp_seq=3 ttl=64 time=0.064 ms
	64 bytes from 172.17.0.2: icmp_seq=4 ttl=64 time=0.069 ms

加载xdp
sudo /home/lbbxsxlz/workspace/iproute2-5.11.0/ip/ip link set dev veth8935654 xdp obj xdp-drop.o sec xdp verbose
	From 172.17.0.1 icmp_seq=80 Destination Host Unreachable
	From 172.17.0.1 icmp_seq=81 Destination Host Unreachable
	From 172.17.0.1 icmp_seq=82 Destination Host Unreachable
	From 172.17.0.1 icmp_seq=83 Destination Host Unreachable

卸载xdp
sudo /home/lbbxsxlz/workspace/iproute2-5.11.0/ip/ip link set dev veth8935654 xdp obj off

	64 bytes from 172.17.0.2: icmp_seq=98 ttl=64 time=0.043 ms
	64 bytes from 172.17.0.2: icmp_seq=99 ttl=64 time=0.018 ms
	64 bytes from 172.17.0.2: icmp_seq=100 ttl=64 time=0.017 ms
	64 bytes from 172.17.0.2: icmp_seq=101 ttl=64 time=0.018 ms

