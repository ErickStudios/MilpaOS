#include "../lib/abstract.h"
typedef abssmall_t      pipstat_t;
typedef abstract_t      pipptr_t;
typedef abstract_t      service_t;
typedef struct _pipeable {
    pipstat_t           stat;
    struct _pipeable*   parent;
    pipptr_t            data;
} pipeable_t;