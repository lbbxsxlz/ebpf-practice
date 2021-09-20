#include <uapi/linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/in.h>
//#include <linux/pkt_cls.h>
#include <uapi/linux/pkt_cls.h>

typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

#define bpfprint(fmt, ...)                        \
    ({                                             \
        char ____fmt[] = fmt;                      \
        bpf_trace_printk(____fmt, sizeof(____fmt), \
                         ##__VA_ARGS__);           \
    })

#define htons(x) __cpu_to_be16(x)

#define SEC(NAME) __attribute__((section(NAME), used))


static int isTCP(void *data, u64 nh_off, void *data_end)
{
    struct iphdr *iph = data + nh_off;

    if ((void *)(iph + 1) > data_end)
        return 0;

    u32 ip_src = iph->saddr;
    u32 ip_dst = iph->daddr;
    
    bpfprint("src ip addr part1: %d.%d.%d\n",(ip_src) & 0xFF,(ip_src >> 8) & 0xFF,(ip_src >> 16) & 0xFF);
    bpfprint("src ip addr part2:.%d\n",(ip_src >> 24) & 0xFF);
    bpfprint("dst ip addr part1: %d.%d.%d\n",(ip_dst) & 0xFF,(ip_dst >> 8) & 0xFF,(ip_dst >> 16) & 0xFF);
    bpfprint("dst ip addr part2: .%d\n",(ip_dst >> 24) & 0xFF);

    if (IPPROTO_TCP == iph->protocol) 
        return 1;

    return 0;
}

SEC("tc-tcp")

int tc_drop_tcp(struct __sk_buff *skb) {
    void *data_end = (void *)(long)skb->data_end;
    void *data = (void *)(long)skb->data;
    struct ethhdr *eth = data;

    u16 h_proto;
    u64 nh_off;

    nh_off = sizeof(*eth);
    if (data + nh_off > data_end)
        return TC_ACT_SHOT;

    h_proto = eth->h_proto;

    if (h_proto == htons(ETH_P_IP)) {
       if (isTCP(data, nh_off, data_end))
            return TC_ACT_SHOT;
    }

    return TC_ACT_OK;
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
        return XDP_PASS;

    h_proto = eth->h_proto;

    if (h_proto == htons(ETH_P_IP)) { 
        if (isTCP(data, nh_off, data_end))
            return XDP_DROP;
    }

    return XDP_PASS;
}

char _license[] SEC("license") = "GPL";
