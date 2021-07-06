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

# ref
## video
[高效入门eBPF](https://www.bilibili.com/video/BV1LX4y157Gp/)
[BPF C编程入门](https://www.bilibili.com/video/BV1f54y1h74r/)
[透过ebpf观察进程行为](https://www.bilibili.com/video/BV1Bt411S7tg?from=search&seid=13293646945432916857)
## blog
[ebpf_website](https://www.ebpf.top)
[ebpf_blog](https://davidlovezoe.club/wordpress/archives/tag/bpf)

[ebpf-and-k8s-zh](http://arthurchiao.art/blog/ebpf-and-k8s-zh/)