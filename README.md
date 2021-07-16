# ebpf-practice
ebpf learning and practice

# bcc
## how to install bcc in ubuntu
make practice in ubuntu Xenial Xerus

sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 4052245BD4284CDD
echo "deb https://repo.iovisor.org/apt/$(lsb_release -cs) $(lsb_release -cs) main" | sudo tee /etc/apt/sources.list.d/iovisor.list
sudo apt-get update
sudo apt-get install bcc-tools libbcc-examples linux-headers-$(uname -r)

## practice
sudo /usr/share/bcc/tools/tcplife
sudo /usr/share/bcc/tools/tcptracer 

# how to make bpf bytecode in C

## bcc

## libbpf-bootstrap
[sourcecode](https://github.com/libbpf/libbpf-bootstrap)


## kernel code  sample/bpf
### install denpend libs

make M=sample/bpf/

### hello bpf
在Makefile中添加
hostprogs-y += hello
hello-objs := bpf_load.o hello_user.o
always += hello_kern.o

make M=sample/bpf/

# reference
## video
[高效入门eBPF](https://www.bilibili.com/video/BV1LX4y157Gp/)<br>
[BPF C编程入门](https://www.bilibili.com/video/BV1f54y1h74r/)<br>
[透过ebpf观察进程行为](https://www.bilibili.com/video/BV1Bt411S7tg?from=search&seid=13293646945432916857)
## blog
[ebpf_website](https://www.ebpf.top)
[ebpf_blog](https://davidlovezoe.club/wordpress/archives/tag/bpf)

[ebpf-and-k8s-zh](http://arthurchiao.art/blog/ebpf-and-k8s-zh/)<br>
[understanding-ebpf-datapath-in-cilium-zh](http://arthurchiao.art/blog/understanding-ebpf-datapath-in-cilium-zh/)<br>
[how-to-make-linux-microservice-aware-with-cilium-ebpf](https://github.com/DavadDi/bpf_study/blob/master/how-to-make-linux-microservice-aware-with-cilium-ebpf/index.md)

## website
[official-site](https://ebpf.io)<br>
[ebpf_tracing_tools](http://www.brendangregg.com/ebpf.html)

