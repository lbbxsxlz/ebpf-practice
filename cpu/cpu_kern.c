#include <linux/ptrace.h>
#include <linux/version.h>
#include <linux/cpufreq.h>
#include <uapi/linux/bpf.h>

#include "bpf_helpers.h"

struct bpf_map_def SEC("maps") pmu_cyl = {
        .type = BPF_MAP_TYPE_PERF_EVENT_ARRAY,
        .key_size = sizeof(int),
        .value_size = sizeof(u32),
        .max_entries = 64,
};


SEC("kprobe/cpufreq_freq_transition_end")

int bpf_prog1(struct pt_regs *ctx, struct cpufreq_policy *policy, struct cpufreq_freqs *freqs, int transition_failed)
{

    u32 freq_new = freqs->new;
    u32 freq_old = freqs->old;
    
    u64 ts = bpf_ktime_get_ns();
    u64 cpu = bpf_get_smp_processor_id();

    u64 cyl = bpf_perf_event_read(&pmu_cyl, cpu);
    char fmt[] = "CPU: %u CYL: %u \n";

    bpf_trace_printk(fmt, sizeof(fmt), cpu, cyl);

    return 0;
}

char _license[] SEC("license") = "GPL";
u32 _version SEC("version") = LINUX_VERSION_CODE;
