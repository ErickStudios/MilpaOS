#ifndef abstractsConcepts_h
#define abstractsConcepts_h
#define true 1
#define false 0

#define cls_checksum 0x12345678

typedef char bool_t;
typedef unsigned long   abstract_t;
typedef long            iabstract_t;
typedef char            abssmall_t;
typedef void            dummy_t;
typedef unsigned short  abslittl_t;
typedef unsigned char   uabssmall_t;

#define __class_body    abstract_t  firm; \
                        abssmall_t* cls_nam;

typedef struct {
    __class_body
} class_hdr_t;

bool_t __constructor_cls(class_hdr_t* cls);

#define anonymus_fn(body) do { body } while (0);
#define grouped(body) body
#define classof(nam) _cls_##nam
#define classsize(nam) sizeof(classof(nam))

#define classdef(nam, attrs, ctr_i) \
    typedef struct nam { \
        __class_body \
        attrs \
    } _cls_##nam; \
    struct nam* nam(struct nam* cls) { \
        class_hdr_t* clsi = (class_hdr_t*)cls; \
        if (!__constructor_cls(clsi)) return 0; \
        ctr_i \
        return cls; \
    }

inline bool_t __valid_cls(class_hdr_t* cls) {
    if (cls) {
        return cls->firm == cls_checksum && cls->cls_nam;
    }
    return false;
}

bool_t __constructor_cls(class_hdr_t* cls) {
    if (cls) {
        cls->firm = cls_checksum;
        return true;
    }
    return false;
}

#endif