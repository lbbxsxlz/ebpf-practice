#!/usr/bin/python

# run in project examples directory with:
# sudo python hello_bpfcc.py"
from bcc import BPF

# This may not work for 4.17 on x64, you need replace kprobe__sys_clone with kprobe____x64_sys_clone
Pro = '''
int kprobe__sys_clone(void *ctx)
{
    bpf_trace_printk("Hello, World!\\n");
    return 0;
}
'''
b = BPF(text=Pro)
b.trace_print()
