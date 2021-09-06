# ebpf-practice
ebpf learning and practice

# bcc
## how to install bcc in ubuntu

make practice in ubuntu Xenial Xerus
```bash
sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 4052245BD4284CDD<br>
echo "deb https://repo.iovisor.org/apt/$(lsb_release -cs) $(lsb_release -cs) main" | sudo tee /etc/apt/sources.list.d/iovisor.list<br>
sudo apt-get update<br>
sudo apt-get install bcc-tools libbcc-examples linux-headers-$(uname -r)
```
## practice
```bash
sudo /usr/share/bcc/tools/tcplife
sudo /usr/share/bcc/tools/tcptracer
sudo /usr/share/bcc/tools/execsnoop
sudo /usr/share/bcc/tools/opensnoop
sudo /usr/share/bcc/tools/biolatency
/usr/share/bcc/tools/trace 'pam:pam_start "%s: %s", arg1, arg2'
```
[ref](https://linux.cn/article-9139-1.html)

# how to make bpf bytecode in C

## bcc

## libbpf-bootstrap
[sourcecode](https://github.com/libbpf/libbpf-bootstrap)

## kernel code samples/bpf in linux-source-4.4.0
### install denpending libs
install libelf libbpf<br>
more in [samples_bpf_build_in_ubuntu-xenial](samples_bpf_build_in_ubuntu-xenial.md)

### hello bpf
在Makefile中修改如下：
```
hostprogs-y += hello
hello-objs := bpf_load.o libbpf.o hello_user.o
always += hello_kern.o
HOSTLOADLIBES_hello += -lelf
```
编译：
sudo make M=samples/bpf/

### another method
please check [Makefile](demo/helloBpf/Makefile)

然后make即可

## kernel code samples/bpf in linux-source-4.15.0
### install denpending libs
install libelf <br>
more in [samples_bpf_build_in_linux-4.15.0](samples_bpf_build_in_linux-4.15.0.md)

### hello bpf
在Makefile中修改如下：
```
hostprogs-y += hello
hello-objs := bpf_load.o $(LIBBPF) hello_user.o
always += hello_kern.o
HOSTLOADLIBES_hello += -lelf
```
编译：
sudo make M=samples/bpf/

运行：
sudo samples/bpf/hello

            bash-4981  [008] ....  1733.269269: 0x00000001: Hello BPF!

            bash-4982  [000] ....  1754.544873: 0x00000001: Hello BPF!

### use clang compile 
clang一次性编译
```bash
sudo clang -O2 -Wall -target bpf  -I /usr/include/x86_64-linux-gnu/ -c hello_kern.c -o hello_kern.o
```
clang，llvm前端生成.ll文件，然后llvm后端把.ll文件生成bpf字节码
```bash
sudo clang -O2 -Wall -emit-llvm -S hello_kern.c
sudo llc hello_kern.ll -march=bpf -filetype=obj -o hello_kern.o
```
查看字节码<br>
llvm-objdump -d hello_kern.o

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
[ebpf_tracing_tools](http://www.brendangregg.com/ebpf.html)<br>
[read list](https://linux.cn/article-9507-1.html)


