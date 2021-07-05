# ebpf-practice
ebpf learning and practice

# install in ubuntu
make practice in ubuntu Xenial Xerus

sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 4052245BD4284CDD
echo "deb https://repo.iovisor.org/apt/$(lsb_release -cs) $(lsb_release -cs) main" | sudo tee /etc/apt/sources.list.d/iovisor.list
sudo apt-get update
sudo apt-get install bcc-tools libbcc-examples linux-headers-$(uname -r)

# ref
[ebpf-and-k8s-zh](http://arthurchiao.art/blog/ebpf-and-k8s-zh/)
[ebpf_website](https://www.ebpf.top)

[高效入门eBPF](https://www.bilibili.com/video/BV1LX4y157Gp/)
[BPF C编程入门](https://www.bilibili.com/video/BV1f54y1h74r/)
