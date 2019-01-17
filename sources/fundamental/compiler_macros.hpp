#pragma once

#define AR_ATTR_ALIGN(alignVal)

#ifdef AR_UNIX
#define AR_VEC_CALL
#elif AR_WINDOWS
#define AR_VEC_CALL
#else
#define AR_VEC_CALL
#endif // ifdef AR_UNIX

#define AR_FORCE_INLINE inline
