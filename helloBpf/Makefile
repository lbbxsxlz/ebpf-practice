# test in linux-4.4.0
CLANG = clang

EXECABLE = hello

KERNELSRC = /home/lbbxsxlz/kernel-src/ubuntu-xenial

BPFTOOLS = $(KERNELSRC)/samples/bpf
BPFLOADER = $(BPFTOOLS)/bpf_load.c
LIBBPF = $(BPFTOOLS)/libbpf.c

CCINCLUDE += -I $(KERNELSRC)/tools/testing/selftests/bpf
CCINCLUDE += -I /usr/include/x86_64-linux-gnu/
LOADINCLUDE += -I $(KERNELSRC)/samples/bpf
LOADINCLUDE += -I $(KERNELSRC)/tools/lib
LOADINCLUDE += -I $(KERNELSRC)/tools/perf
LOADINCLUDE += -I $(KERNELSRC)/tools/include

.PHONY: clean bpfload build

clean:
	rm -f *.o *.so $(EXECABLE)

build: hello_kern.c
	$(CLANG) -O2 -target bpf -c hello_kern.c $(CCINCLUDE) -o hello_kern.o

bpfload: build
	clang -o $(EXECABLE) -lelf $(LOADINCLUDE) $(BPFLOADER) $(LIBBPF) hello_user.c

$(EXECABLE): bpfload

.DEFAULT_GOAL := $(EXECABLE)

