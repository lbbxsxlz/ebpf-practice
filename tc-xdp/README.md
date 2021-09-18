# ebpf in tc and xdp

clang -O2 -Wall -target bpf -I /usr/include/x86_64-linux-gnu/ -c tc-xdp-drop-tcp.c -o tc-xdp-drop-tcp.o

curl 127.0.0.1:49153     有输出

sudo mkdir /var/run/netns
docker inspect firstWeb -f "{{.NetworkSettings.SandboxKey}}"
sudo ln -s /var/run/docker/netns/5e9605a3282f /var/run/netns/firstWeb
sudo ip netns exec firstWeb curl www.baidu.com   有输出

sudo ip link set dev vethb9f28cb xdp obj tc-xdp-drop-tcp.o sec xdp-tcp verbose



sudo ~/tools/iproute2-5.11.0/tc/tc qdisc add dev vethb9f28cb clsact
sudo ~/tools/iproute2-5.11.0/tc/tc filter add dev vethb9f28cb egress bpf da obj tc-xdp-drop-tcp.o sec tc-tcp verbose

sudo ip netns exec firstWeb curl www.baidu.com   无输出
curl 127.0.0.1:49153     无输出


sudo ~/tools/iproute2-5.11.0/tc/tc filter del dev vethb9f28cb egress
sudo ~/tools/iproute2-5.11.0/tc/tc qdisc del dev vethb9f28cb clsact

sudo ip link set dev vethb9f28cb xdp off


sudo ip netns exec firstWeb curl www.baidu.com   有输出
curl 127.0.0.1:49153     有输出



