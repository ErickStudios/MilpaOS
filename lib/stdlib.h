#ifndef _Stdl__MILPA_OS__H
#define _Stdl__MILPA_OS__H
#include "abstract.h"

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

// all about of my console
typedef void (*putcfn_t)(char c);
typedef void (*putstrfn_t)(char* s);
typedef char (*getcfn_t)();

typedef struct {
    // tasks and threads
    forkfn_t createThread;
    joinfn_t joinThread;

    // messages
    requestfn_t createRequest;
    joinmsgfn_t joinRequest;
    findfirst_t findFirstRequest;
    respondfn_t respondRequest;

    // console
    putcfn_t teletypeCon;
    putstrfn_t printCon;
    getcfn_t inputCon;
} server_t;

#endif // !_Stdl__MILPA_OS__H