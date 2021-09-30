
## 修改Makefile
~/kernel-src/linux-source-4.15.0/samples/bpf/Makefile

hostprogs-y += xdp_ip_tracker

xdp_ip_tracker-objs := bpf_load.o $(LIBBPF) xdp_ip_tracker_user.o

always += xdp_ip_tracker_kern.o

HOSTLOADLIBES_xdp_ip_tracker += -lelf


