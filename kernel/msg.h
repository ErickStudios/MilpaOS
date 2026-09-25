#include "../lib/str.h"
#include "../lib/stdlib.h"

server_t* globKserv;

/**
// all about of messages
#define MSGNAME_MAX 10
typedef char msgname_t[MSGNAME_MAX];
typedef abstract_t msghandler_t;
typedef msghandler_t (*requestfn_t)(const msgname_t, abstract_t);
typedef abstract_t (*joinmsgfn_t)(msghandler_t);
typedef msghandler_t (*findfirst_t)(const msgname_t);
typedef void (*respondfn_t)(msghandler_t, abstract_t);
typedef abstract_t (*fetchrequestfn_t)(msghandler_t);

request("apple foo", 0);
 */

typedef struct {
    msgname_t ident;
    abstract_t param;
    abstract_t returns;
    bool_t inuse;
    bool_t commingval;
} msg_internal_t;

#define MAX_MSGS_INTERNAL 10
msg_internal_t msgs[MAX_MSGS_INTERNAL];

void init_msg_server() {
    for (abstract_t i = 0; i < MAX_MSGS_INTERNAL; i++)
    {
        msgs[i].inuse = false;
    }
}

msghandler_t request_msg(const msgname_t nam, abstract_t dat) {
    for (abstract_t i = 0; i < MAX_MSGS_INTERNAL; i++)
    {
        if (!msgs[i].inuse) {
            char* nm = nam;
            char* nm2 = msgs[i].ident;
            memcpy(nm2, nm, sizeof(msgname_t));

            msgs[i].param = dat;
            msgs[i].commingval = false;
            msgs[i].inuse = true;

            return i;
        }
    }

    return 0xFFFFFFFF;
}
abstract_t join_msg(msghandler_t h) {
    if (h == 0xFFFFFFFF) return 0;
    while (!msgs[h].commingval) {
        asm volatile("hlt");
    }
    abstract_t ret = msgs[h].returns;
    msgs[h].inuse = false;
    return ret;
}
msghandler_t findfirst_msg(const msgname_t nam) {
    for (abstract_t i = 0; i < MAX_MSGS_INTERNAL; i++)
    {
        if (!msgs[i].inuse) continue;
        if (msgs[i].commingval) continue;

        char* s1 = msgs[i].ident;
        char* s2 = nam;

        if (msgs[i].inuse == true && !strcmp(s1, s2)) {
            return i;
        }
    }
    return 0xFFFFFFFF;
}
abstract_t getparam_msg(msghandler_t handler) {
    if (handler != 0xFFFFFFFF) {
        return msgs[handler].param;
    }
    return 0xFFFFFFFF;
}
void respond_msg(msghandler_t handler, abstract_t retval) {
    if (handler != 0xFFFFFFFF) {
        msgs[handler].returns = retval;
        msgs[handler].commingval = true;
    }
}