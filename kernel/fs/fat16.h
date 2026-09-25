#include "../../lib/abstract.h"
#include "../../lib/stdlib.h"

void Fat16GetInfoByBPB(Fat16Info* inf, Fat16Boots* bpb) {
    inf->fat_start = bpb->reserved;
    inf->root_start = inf->fat_start + bpb->num_fats * bpb->sec_per_fat;
    inf->root_sectors = (bpb->root_entries * 32) / bpb->bytes_per_sector;
    inf->data_start = inf->root_start + inf->root_sectors;
    inf->sec_per_cluster = bpb->sec_per_cluster;
}

abstract_t Fat16ClusterToLBA(Fat16Info* inf, abslittl_t cluster) {
    return inf->data_start + (cluster - 2) * inf->sec_per_cluster;
}