#include <linux/bpf.h>
//#include "bpf_helpers.h"
static int (*bpf_trace_printk)(const char *fmt, int fmt_size,
                               ...) = (void *)BPF_FUNC_trace_printk;
#define SEC(NAME) __attribute__((section(NAME), used))

SEC("tracepoint/syscalls/sys_enter_execve")
int bpf_prog(void *ctx)
{
 char msg[] = "Hello BPF!\n";
 bpf_trace_printk(msg, sizeof(msg));
 return 0;
}

char _license[] SEC("license") = "GPL";
