#include <uapi/linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/in.h>

typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

#define htons(x) __cpu_to_be16(x)

#define SEC(NAME) __attribute__((section(NAME), used))


static int handle_ipv4(void *data, u64 nh_off, void *data_end)
{
    struct iphdr *iph = data + nh_off;

    if ((void *)(iph + 1) > data_end)
         return XDP_DROP;

    if (IPPROTO_TCP == iph->protocol)
		return XDP_DROP;
	
	return XDP_PASS;
}

SEC("xdp-tcp")

int xdp_drop_tcp(struct xdp_md *ctx) {
    void *data_end = (void *)(long)ctx->data_end;
    void *data = (void *)(long)ctx->data;
    struct ethhdr *eth = data;

    u16 h_proto;
    u64 nh_off;

    nh_off = sizeof(*eth);
    if (data + nh_off > data_end)
        return XDP_DROP;

    h_proto = eth->h_proto;

    if (h_proto == htons(ETH_P_IP)) 
        return handle_ipv4(data, nh_off, data_end);

    return XDP_PASS;
}

char _license[] SEC("license") = "GPL";
