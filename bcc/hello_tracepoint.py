#!/usr/bin/env python
# coding=utf-8

# sudo python3 ./hello_tracepoint.py
# trace random reading
from bcc import BPF
from bcc.utils import printb

#load eBPF program
b = BPF(text = '''
        /*  TRACEPOINT_PROBE(random, urandon_read)  \
             kernel tracepoint random:urandom_read. \
             formats ref: /sys/kernel/debug/tracing/events/random/urandom_read/format
        */
        TRACEPOINT_PROBE(random, urandom_read) {
                bpf_trace_printk("%d\\n", args->got_bits);

                return 0;
        }
        ''')

#header
print("%-18s %-16s %-6s %s"%("TIME(s)", "COMM", "PID", "GOTBITS"))

#format output
while 1:
    try:
        (task, pid, cpu, flags, ts, msg) = b.trace_fields()
    except ValueError:
        continue
    printb(b"%-18.9f %-16s %-6d %s"%(ts, task, pid, msg))

