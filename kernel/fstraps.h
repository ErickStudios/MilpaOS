#include "fs/fat16.h"

Fat16Dir* fs_get_file(char* fname, Fat16Info* spc) {
    uabssmall_t sec0[512];
    ata_read_sector(0, sec0);
    Fat16Boots* bpb = (Fat16Boots*)sec0;

    Fat16Info inf16;
    Fat16GetInfoByBPB(&inf16, bpb);

    abstract_t root_sectors = inf16.root_sectors;
    abstract_t bpb_root_entries = bpb->root_entries;

    abstract_t sec_per_cluster = bpb->sec_per_cluster;
    abstract_t root_entries_copy = bpb_root_entries;

    uabssmall_t rsec[512];
    Fat16Dir* copy_bpb = malloc(sizeof(Fat16Boots));
    memcpy(copy_bpb, bpb, sizeof(Fat16Boots));
    bpb = copy_bpb;

    memcpy(spc, &inf16, sizeof(Fat16Info));

    for (abstract_t i = 0; i < root_entries_copy; i++) {
        if (i % 16 == 0) {
            ata_read_sector(inf16.root_start + i/16, rsec);
        }
        Fat16Dir* fil = (Fat16Dir*)rsec + (i % 16);

        if (fil->name[0] == 0x00) break;
        if ((uabssmall_t)fil->name[0] == 0xE5) continue;
        if (fil->attr == 0x0F) continue;

        char fatname[12];
        memcpy(fatname, fil->name, 8);
        memcpy(fatname+8, fil->ext, 3);
        fatname[11]=0;

        if (strncmp(fname, fatname, 11) == 0) {
            Fat16Dir* Cpy = malloc(sizeof(Fat16Dir));
            memcpy(Cpy, fil, sizeof(Fat16Dir));
            free(copy_bpb);
            return Cpy;
        }
    }
    free(copy_bpb);
    return 0;
}

uabssmall_t* fs_read_file(Fat16Dir* dir, Fat16Info* inf) {
    // necesitas el bpb de nuevo para saber sec_per_cluster
    uabssmall_t sec0[512];
    ata_read_sector(0, sec0);
    Fat16Boots* bpb = (Fat16Boots*)sec0;
    Fat16Boots bpb_copy = *bpb; // guardalo porque vas a reusar sec0

    // carga FAT
    abslittl_t* fat = malloc(bpb_copy.sec_per_fat * 512);
    for(int i=0; i<bpb_copy.sec_per_fat; i++){
        ata_read_sector(inf->fat_start + i, (uabssmall_t*)fat + i*512);
    }

    abstract_t total_size = dir->size;
    abstract_t alloc = (total_size + 511) & ~511; // redondea a 512
    if(alloc==0) alloc=512;
    uabssmall_t* buf = malloc(alloc + 1);

    abstract_t offset = 0;
    abslittl_t cluster = dir->cluster_lo;
    uabssmall_t tmp[512];

    while(cluster < 0xFFF8 && offset < total_size) {
        abstract_t lba = Fat16ClusterToLBA(inf, cluster);

        for(int s=0; s < bpb_copy.sec_per_cluster; s++) {
            if(offset >= total_size) break;
            ata_read_sector(lba + s, tmp);

            abstract_t left = total_size - offset;
            abstract_t copy = left > 512? 512 : left;
            memcpy(buf + offset, tmp, copy);
            offset += copy;
        }
        cluster = fat[cluster];
    }

    buf[total_size] = 0;
    free(fat);
    return buf;
}