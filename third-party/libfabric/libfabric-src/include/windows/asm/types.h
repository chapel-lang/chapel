#ifndef _ASM_X86_TYPES_H
#define _ASM_X86_TYPES_H

typedef signed   char __s8;
typedef unsigned char __u8;

typedef signed   short __s16;
typedef unsigned short __u16;

typedef signed   int __s32;
typedef unsigned int __u32;

typedef signed   long long __s64;
typedef unsigned long long __u64;

typedef __u16 __le16;
typedef __u16 __be16;
typedef __u32 __le32;
typedef __u32 __be32;
typedef __u64 __le64;
typedef __u64 __be64;

#define __aligned_u64 __u64 __attribute__((aligned(8)))

#define __builtin_constant_p(exp) 0

#endif /* _ASM_X86_TYPES_H */
