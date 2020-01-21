#pragma once

#define AR_ATTR_ALIGN(alignVal)

#ifdef AR_CLANG
#define AR_VEC_CALL
#else
#define AR_VEC_CALL
#endif // ifdef AR_CLANG

#ifdef AR_CLANG
#define AR_ATTR_UNUSED __attribute__((unused))
#else
#define AR_ATTR_UNUSED
#endif // ifdef AR_CLANG

#define AR_FORCE_INLINE inline

#define AR_SYM_EXPORT __attribute__((visibility("default")))
