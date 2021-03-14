// Generated with c2chapel version 0.1.0

// Header given to c2chapel:
require "sysCTypes.h";

// Note: Generated with fake std headers

use CPtr;
extern proc test_int(a : c_int, ref b : c_int) : c_int;

extern proc test_int(a : c_int, b : c_ptr(c_int)) : c_int;

extern proc test_uint(a : c_uint, b : c_int, ref c : c_uint) : c_uint;

extern proc test_uint(a : c_uint, b : c_int, c : c_ptr(c_uint)) : c_uint;

extern proc test_longs(a : c_long, b : c_ulong, c : c_longlong, d : c_ulonglong, ref e : c_longlong) : c_long;

extern proc test_longs(a : c_long, b : c_ulong, c : c_longlong, d : c_ulonglong, e : c_ptr(c_longlong)) : c_long;

extern proc test_chars(a : c_char, b : c_string, c : c_schar, d : c_uchar) : c_char;

extern proc test_shorts(a : c_short, b : c_ushort) : c_short;

extern proc test_ptrs(a : c_intptr, b : c_uintptr, c : c_ptrdiff, d : ssize_t, e : size_t) : void;

extern proc signedWidths(a : int(8), b : int(16), c : int(32), d : int(64)) : void;

extern proc unsignedWidths(a : uint(8), b : uint(16), c : uint(32), d : uint(64)) : void;

// c2chapel thinks these typedefs are from the fake headers:
/*
extern type FILE = c_int;

extern type _LOCK_RECURSIVE_T = c_int;

extern type _LOCK_T = c_int;

extern type __FILE = c_int;

extern type __ULong = c_int;

extern type __builtin_va_list = c_int;

extern type __dev_t = c_int;

extern type __gid_t = c_int;

extern type __gnuc_va_list = c_int;

extern type __int16_t = c_int;

extern type __int32_t = c_int;

extern type __int64_t = c_int;

extern type __int8_t = c_int;

extern type __int_least16_t = c_int;

extern type __int_least32_t = c_int;

extern type __loff_t = c_int;

extern type __off_t = c_int;

extern type __pid_t = c_int;

extern type __s16 = c_int;

extern type __s32 = c_int;

extern type __s64 = c_int;

extern type __s8 = c_int;

extern type __sigset_t = c_int;

extern type __tzinfo_type = c_int;

extern type __tzrule_type = c_int;

extern type __u16 = c_int;

extern type __u32 = c_int;

extern type __u64 = c_int;

extern type __u8 = c_int;

extern type __uid_t = c_int;

extern type __uint16_t = c_int;

extern type __uint32_t = c_int;

extern type __uint64_t = c_int;

extern type __uint8_t = c_int;

extern type __uint_least16_t = c_int;

extern type __uint_least32_t = c_int;

extern type _flock_t = c_int;

extern type _fpos_t = c_int;

extern type _iconv_t = c_int;

extern type _mbstate_t = c_int;

extern type _off64_t = c_int;

extern type _off_t = c_int;

extern type _sig_func_ptr = c_int;

extern type _ssize_t = c_int;

extern type _types_fd_set = c_int;

extern type bool = _Bool;

extern type caddr_t = c_int;

extern type clock_t = c_int;

extern type clockid_t = c_int;

extern type cookie_close_function_t = c_int;

extern type cookie_io_functions_t = c_int;

extern type cookie_read_function_t = c_int;

extern type cookie_seek_function_t = c_int;

extern type cookie_write_function_t = c_int;

extern type daddr_t = c_int;

extern type dev_t = c_int;

extern type div_t = c_int;

extern type fd_mask = c_int;

extern type fpos_t = c_int;

extern type gid_t = c_int;

extern type ino_t = c_int;

extern type int16_t = c_int;

extern type int32_t = c_int;

extern type int64_t = c_int;

extern type int8_t = c_int;

extern type int_fast16_t = c_int;

extern type int_fast32_t = c_int;

extern type int_fast64_t = c_int;

extern type int_fast8_t = c_int;

extern type int_least16_t = c_int;

extern type int_least32_t = c_int;

extern type int_least64_t = c_int;

extern type int_least8_t = c_int;

extern type intmax_t = c_int;

extern type intptr_t = c_int;

extern type jmp_buf = c_int;

extern type key_t = c_int;

extern type ldiv_t = c_int;

extern type lldiv_t = c_int;

extern type mbstate_t = c_int;

extern type mode_t = c_int;

extern type nlink_t = c_int;

extern type off_t = c_int;

extern type pid_t = c_int;

extern type pthread_attr_t = c_int;

extern type pthread_barrier_t = c_int;

extern type pthread_barrierattr_t = c_int;

extern type pthread_cond_t = c_int;

extern type pthread_condattr_t = c_int;

extern type pthread_key_t = c_int;

extern type pthread_mutex_t = c_int;

extern type pthread_mutexattr_t = c_int;

extern type pthread_once_t = c_int;

extern type pthread_rwlock_t = c_int;

extern type pthread_rwlockattr_t = c_int;

extern type pthread_spinlock_t = c_int;

extern type pthread_t = c_int;

extern type ptrdiff_t = c_int;

extern type rlim_t = c_int;

extern type sa_family_t = c_int;

extern type sem_t = c_int;

extern type sig_atomic_t = c_int;

extern type siginfo_t = c_int;

extern type sigjmp_buf = c_int;

extern type sigset_t = c_int;

extern type size_t = c_int;

extern type ssize_t = c_int;

extern type stack_t = c_int;

extern type suseconds_t = c_int;

extern type time_t = c_int;

extern type timer_t = c_int;

extern type u_char = c_int;

extern type u_int = c_int;

extern type u_long = c_int;

extern type u_short = c_int;

extern type uid_t = c_int;

extern type uint = c_int;

extern type uint16_t = c_int;

extern type uint32_t = c_int;

extern type uint64_t = c_int;

extern type uint8_t = c_int;

extern type uint_fast16_t = c_int;

extern type uint_fast32_t = c_int;

extern type uint_fast64_t = c_int;

extern type uint_fast8_t = c_int;

extern type uint_least16_t = c_int;

extern type uint_least32_t = c_int;

extern type uint_least64_t = c_int;

extern type uint_least8_t = c_int;

extern type uintmax_t = c_int;

extern type uintptr_t = c_int;

extern type useconds_t = c_int;

extern type ushort = c_int;

extern type va_list = c_int;

extern type wchar_t = c_int;

extern type wint_t = c_int;

extern type z_stream = c_int;

*/
