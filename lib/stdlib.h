#ifndef _Stdl__MILPA_OS__H
#define _Stdl__MILPA_OS__H
#include "abstract.h"

 #ifndef __GNUC__
 #define __attribute__(x)
 #define packed
 #endif

typedef struct  __attribute__((packed)) {
    uabssmall_t jmp[3];
    abssmall_t oem[8];
    abslittl_t bytes_per_sector;
    uabssmall_t sec_per_cluster;
    abslittl_t reserved;
    uabssmall_t num_fats;
    abslittl_t root_entries;
    abslittl_t total_sec16;
    uabssmall_t media;
    abslittl_t sec_per_fat;
    abslittl_t sec_per_track;
    abslittl_t heads;
    abstract_t hidden;
    abstract_t total_sec32;
} Fat16Boots;

typedef struct  __attribute__((packed)) {
    abssmall_t name[8];
    abssmall_t ext[3];
    uabssmall_t attr;
    uabssmall_t _res;
    uabssmall_t ctime_ms;
    abslittl_t ctime_hms;
    abslittl_t cdate;
    abslittl_t adate;
    abslittl_t cluster_hi;
    abslittl_t mtime_hms;
    abslittl_t mdate;
    abslittl_t cluster_lo;
    abstract_t size;
} Fat16Dir;

typedef struct {
    abstract_t fat_start;
    abstract_t root_start;
    abstract_t root_sectors;
    abstract_t data_start;
    uabssmall_t sec_per_cluster;
} Fat16Info;

typedef struct {
    abstract_t* row;
    abstract_t* column;
    uabssmall_t* color;
} VirtusTermController;

#define INVALID_MSG_ID 0xFFFFFFFF

typedef abstract_t fnptr_t;
typedef abstract_t thread_t;

// all about of Task and Threadings
typedef thread_t (*forkfn_t)(fnptr_t);
typedef void (*joinfn_t)(thread_t);

// all about of messages
#define MSGNAME_MAX 10
typedef char msgname_t[MSGNAME_MAX];
typedef abstract_t msghandler_t;
typedef msghandler_t (*requestfn_t)(const msgname_t, abstract_t);
typedef abstract_t (*joinmsgfn_t)(msghandler_t);
typedef msghandler_t (*findfirst_t)(const msgname_t);
typedef void (*respondfn_t)(msghandler_t, abstract_t);
typedef abstract_t (*fetchrequestfn_t)(msghandler_t);

// all about of my console
typedef void (*putcfn_t)(char c);
typedef void (*putstrfn_t)(const char* s);
typedef char (*getcfn_t)();

// all about of files
typedef Fat16Dir* (*openfn_t)(char* fname, Fat16Info* spc);
typedef uabssmall_t* (*readfn_t)(Fat16Dir* dir, Fat16Info* inf);

// all about of memory
typedef void* (*mallocfn_t)(abstract_t size);
typedef void (*freefn_t)(void* ptr);

typedef struct {
    // tasks and threads
    forkfn_t createThread;
    joinfn_t joinThread;

    // messages
    requestfn_t createRequest;
    joinmsgfn_t joinRequest;
    findfirst_t findFirstRequest;
    respondfn_t respondRequest;
    fetchrequestfn_t fetchRequest;

    // console
    putcfn_t teletypeCon;
    putstrfn_t printCon;
    getcfn_t inputCon;

    // files
    readfn_t readFile;
    openfn_t openFile;

    // memory
    mallocfn_t malloc;
    freefn_t free;

    VirtusTermController structCon;
} server_t;

#endif // !_Stdl__MILPA_OS__H