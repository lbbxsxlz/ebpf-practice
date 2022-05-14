# bpftrace practice

## hello world
```
sudo bpftrace -e 'BEGIN {print("hello world!\n")}'
```

## tracepoints
sudo bpftrace -l | grep tracepoint

## trace linux kernel kprobe
```
sudo bpftrace -e 'kprobe:vfs_read {@PID=pid;}'
sudo bpftrace -e 'kprobe:vfs_read {@PID = pid;} interval:s:2 {printf("process id:%d \n", @PID);}'
sudo bpftrace -e 'kprobe:vfs_read {@start[pid] = nsecs;} kretprobe:vfs_read /@start[pid]/ {@ns[comm] = nsecs - @start[pid]; delete(@start[pid]);}'
``` 

sudo bpftrace --include linux/mm_types.h -e 'kprobe:alloc_pages_vma {@vma[comm] = ((struct vm_area_struct*)arg2)->vm_start;}'
sudo bpftrace --include linux/mm_types.h -e 'kprobe:alloc_pages_vma {@vma[comm] = hist(((struct vm_area_struct*)arg2)->vm_start);}'

### bpftrace files
sudo bpftrace kprobe_alloc_pages_vma

## trace user space
sudo bpftrace -e 'uprobe:/home/$elf:$func {printf("arg:%d", arg0)}'
sudo pftrace -e 'uretprobe:/bin/bash:readline { printf("readline: \"%s\"\n", str(retval)); }'
sudo bpftrace -e 'uprobe:/home/lbbxsxlz/gitworkspace/ebpf-practice/bpftrace/test:myadd {printf("arg0:%d \n", arg0);}'


