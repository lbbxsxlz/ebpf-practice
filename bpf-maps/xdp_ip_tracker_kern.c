#define KBUILD_MODNAME "foo"
#include <uapi/linux/bpf.h>
#include <uapi/linux/if_ether.h>
#include <uapi/linux/if_packet.h>
#include <uapi/linux/if_vlan.h>
#include <uapi/linux/ip.h>
#include <uapi/linux/in.h>
#include <uapi/linux/tcp.h>
#include <uapi/linux/udp.h>
#include "bpf_helpers.h"
#include "xdp_ip_tracker_common.h"

struct bpf_map_def SEC("maps") tracker_map = {
    .type = BPF_MAP_TYPE_HASH,
    .key_size = sizeof(struct pair),
    .value_size = sizeof(struct stats),
    .max_entries = 2048,
};

static __always_inline bool parse_and_track(void *data_begin, void *data_end, struct pair *pair)
{
    struct ethhdr *eth = data_begin;

    if ((void *)(eth + 1) > data_end)
        return false;

    if (eth->h_proto == htons(ETH_P_IP))
    {
        struct iphdr *iph = (struct iphdr *)(eth + 1);
        if ((void *)(iph + 1) > data_end)
            return false;

        pair->src_ip = iph->daddr;
        pair->dest_ip = iph->saddr;

        // update the map for track
        struct stats *stats, newstats = {0, 0, 0, 0};
        long long bytes = data_end - data_begin;

        stats = bpf_map_lookup_elem(&tracker_map, pair);
        if (stats)
        {
            stats->rx_cnt++;
            stats->rx_bytes += bytes;
        }
        else
        {
            newstats.rx_cnt = 1;
            newstats.rx_bytes = bytes;
            bpf_map_update_elem(&tracker_map, pair, &newstats, BPF_NOEXIST);
        }
        return true;
    }
    return false;
}

SEC("xdp_ip_tracker")
int xdp_ip_tracker(struct xdp_md *ctx)
{
    // the struct to store the ip address as the keys of bpf map
    struct pair pair;

    void *data_end = (void *)(long)ctx->data_end;
    void *data = (void *)(long)ctx->data;
    // pass if the network packet is not ipv4
    if (!parse_and_track(data, data_end, &pair))
        return XDP_PASS;

    return XDP_DROP;
}

char _license[] SEC("license") = "GPL";
