#include <uapi/linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/in.h>
//#include <linux/pkt_cls.h>
#include <uapi/linux/pkt_cls.h>

typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

#define htons(x) __cpu_to_be16(x)

#define SEC(NAME) __attribute__((section(NAME), used))


static int parse_ipv4(void *data, u64 nh_off, void *data_end)
{
    struct iphdr *iph = data + nh_off;

    if ((void *)(iph + 1) > data_end)
        return 0;

    return iph->protocol;
}

SEC("tc-tcp")

int tc_drop_tcp(struct __sk_buff *skb) {
    void *data_end = (void *)(long)skb->data_end;
    void *data = (void *)(long)skb->data;
    struct ethhdr *eth = data;

    u16 h_proto;
    u64 nh_off;
    u32 ipproto;

    nh_off = sizeof(*eth);
    if (data + nh_off > data_end)
        return TC_ACT_SHOT;

    h_proto = eth->h_proto;

    if (h_proto == htons(ETH_P_IP)) {
        ipproto = parse_ipv4(data, nh_off, data_end);
        
        if (IPPROTO_TCP == ipproto) 
            return TC_ACT_SHOT;

    }

    return TC_ACT_OK;
}

char _license[] SEC("license") = "GPL";
