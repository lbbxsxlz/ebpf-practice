# bpftrace practice

## first demo
```
sudo bpftrace -e 'BEGIN {print("hello world!\n")}'
```

## 
```
sudo bpftrace -e 'kprobe:vfs_read {@PID=pid;}'
sudo bpftrace -e 'kprobe:vfs_read {@PID = pid;} interval:s:2 {printf("process id:%d \n", @PID);}'
``` 
