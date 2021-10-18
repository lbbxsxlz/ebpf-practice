# ebpf in tc and xdp

## tc and xdp drop tcp
编译
```
clang -O2 -Wall -target bpf -I /usr/include/x86_64-linux-gnu/ -c tc-xdp-drop-tcp.c -o tc-xdp-drop-tcp.o
```

启动容器，访问容器，有输出
```
docker start firstWeb
curl 127.0.0.1:49153
```

创建netns，容器访问外部网络（需要修改域名服务），有输出
```
sudo mkdir /var/run/netns
docker inspect firstWeb -f "{{.NetworkSettings.SandboxKey}}"
sudo ln -s /var/run/docker/netns/5e9605a3282f /var/run/netns/firstWeb

sudo ip netns exec firstWeb curl www.baidu.com
```

设置xdp
```
sudo ip link set dev vethb9f28cb xdp obj tc-xdp-drop-tcp.o sec xdp-tcp verbose
```

设置tc规则
```
sudo ~/tools/iproute2-5.11.0/tc/tc qdisc add dev vethb9f28cb clsact
sudo ~/tools/iproute2-5.11.0/tc/tc filter add dev vethb9f28cb egress bpf da obj tc-xdp-drop-tcp.o sec tc-tcp verbose
```

实验验证
容器内访问外部网络
sudo ip netns exec firstWeb curl www.baidu.com   无输出
访问容器
curl 127.0.0.1:49153     无输出

恢复环境
```
sudo ~/tools/iproute2-5.11.0/tc/tc filter del dev vethb9f28cb egress
sudo ~/tools/iproute2-5.11.0/tc/tc qdisc del dev vethb9f28cb clsact

sudo ip link set dev vethb9f28cb xdp off
```

sudo ip netns exec firstWeb curl www.baidu.com   有输出
curl 127.0.0.1:49153     有输出


## tc and xdp drop tcp with print ipaddr
bpf_helper   from   ~/kernel-src/linux-source-4.15.0/tools/testing/selftests/bpf/

编译
clang -O2 -Wall -target bpf -I /usr/include/x86_64-linux-gnu/ -c tc-xdp-drop-tcp-print.c -o tc-xdp-drop-tcp-print.o

添加tc规则
```
sudo ~/tools/iproute2-5.11.0/tc/tc qdisc add dev vethb9f28cb clsact
sudo ~/tools/iproute2-5.11.0/tc/tc filter add dev vethb9f28cb egress bpf da obj tc-xdp-drop-tcp.o sec tc-tcp verbose
```

查看内核中bpf打印
sudo cat /sys/kernel/debug/tracing/trace_pipe

sudo ip netns exec firstWeb curl www.baidu.com
          	
		  <idle>-0     [005] .Ns.  1472.645742: 0x00000001: src ip addr part1: 223.5.5
	          <idle>-0     [005] .Ns.  1472.645742: 0x00000001: src ip addr part2:.5
	          <idle>-0     [005] .Ns.  1472.645744: 0x00000001: dst ip addr part1: 172.17.0
	          <idle>-0     [005] .Ns.  1472.645744: 0x00000001: dst ip addr part2: .2
	          <idle>-0     [011] ..s.  1472.805959: 0x00000001: src ip addr part1: 182.61.200
	          <idle>-0     [011] .Ns.  1472.805979: 0x00000001: src ip addr part2:.6
	          <idle>-0     [011] .Ns.  1472.805981: 0x00000001: dst ip addr part1: 172.17.0
	          <idle>-0     [011] .Ns.  1472.805982: 0x00000001: dst ip addr part2: .2
	 irq/128-iwlwifi-570   [011] ....  1473.709837: 0x00000001: src ip addr part1: 182.61.200
	 irq/128-iwlwifi-570   [011] ....  1473.709854: 0x00000001: src ip addr part2:.6
	 irq/128-iwlwifi-570   [011] ....  1473.709855: 0x00000001: dst ip addr part1: 172.17.0
	 irq/128-iwlwifi-570   [011] ....  1473.709856: 0x00000001: dst ip addr part2: .2
	 irq/128-iwlwifi-570   [011] ....  1475.727897: 0x00000001: src ip addr part1: 182.61.200
	 irq/128-iwlwifi-570   [011] ....  1475.727914: 0x00000001: src ip addr part2:.6
	 irq/128-iwlwifi-570   [011] ....  1475.727916: 0x00000001: dst ip addr part1: 172.17.0
	 irq/128-iwlwifi-570   [011] ....  1475.727917: 0x00000001: dst ip addr part2: .2


curl 127.0.0.1:49153

	    docker-proxy-3814  [009] ....  1565.159081: 0x00000001: src ip addr part1: 172.17.0
	    docker-proxy-3814  [009] ....  1565.159084: 0x00000001: src ip addr part2:.1
	    docker-proxy-3814  [009] ....  1565.159085: 0x00000001: dst ip addr part1: 172.17.0
	    docker-proxy-3814  [009] ....  1565.159085: 0x00000001: dst ip addr part2: .2
	          <idle>-0     [009] ..s.  1566.162399: 0x00000001: src ip addr part1: 172.17.0
	          <idle>-0     [009] .Ns.  1566.162417: 0x00000001: src ip addr part2:.1
	          <idle>-0     [009] .Ns.  1566.162419: 0x00000001: dst ip addr part1: 172.17.0
	          <idle>-0     [009] .Ns.  1566.162420: 0x00000001: dst ip addr part2: .2

