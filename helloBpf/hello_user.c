#include <stdio.h>
#include "bpf_load.h"
#include "libbpf.h"

int main(int argc, char **argv)
{
    char filename[256];
    snprintf(filename, sizeof(filename), "%s_kern.o", argv[0]);

    if(load_bpf_file(filename) != 0)
     {
        printf("The kernel didn't load BPF program\n");
        return -1;
     }

    read_trace_pipe();
    return 0;
}
