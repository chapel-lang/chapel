dnl Copyright (C) Free Software Foundation, Inc.
dnl
dnl This program is free software; you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation; either version 2 of the License, or
dnl (at your option) any later version.
dnl 
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl 
dnl You should have received a copy of the GNU General Public License
dnl along with this program; if not, write to the Free Software
dnl Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
dnl
dnl This test is taken from libgfortran

dnl Check whether the target supports __sync_val_compare_and_swap.
AC_DEFUN([STARPU_CHECK_SYNC_VAL_COMPARE_AND_SWAP], [
  AC_CACHE_CHECK([whether the target supports __sync_val_compare_and_swap],
		 ac_cv_have_sync_val_compare_and_swap, [
  AC_LINK_IFELSE([AC_LANG_PROGRAM([int foo, bar;],
			[bar = __sync_val_compare_and_swap(&foo, 0, 1);])],
			[ac_cv_have_sync_val_compare_and_swap=yes],
			[ac_cv_have_sync_val_compare_and_swap=no])])
  if test $ac_cv_have_sync_val_compare_and_swap = yes; then
    AC_DEFINE(STARPU_HAVE_SYNC_VAL_COMPARE_AND_SWAP, 1,
	      [Define to 1 if the target supports __sync_val_compare_and_swap])
  fi])

dnl Check whether the target supports __sync_bool_compare_and_swap.
AC_DEFUN([STARPU_CHECK_SYNC_BOOL_COMPARE_AND_SWAP], [
  AC_CACHE_CHECK([whether the target supports __sync_bool_compare_and_swap],
		 ac_cv_have_sync_bool_compare_and_swap, [
  AC_LINK_IFELSE([AC_LANG_PROGRAM([int foo, bar;],
			[bar = __sync_bool_compare_and_swap(&foo, 0, 1);])],
			[ac_cv_have_sync_bool_compare_and_swap=yes],
			[ac_cv_have_sync_bool_compare_and_swap=no])])
  if test $ac_cv_have_sync_bool_compare_and_swap = yes; then
    AC_DEFINE(STARPU_HAVE_SYNC_BOOL_COMPARE_AND_SWAP, 1,
	      [Define to 1 if the target supports __sync_bool_compare_and_swap])
  fi])

dnl Check whether the target supports __sync_fetch_and_add.
AC_DEFUN([STARPU_CHECK_SYNC_FETCH_AND_ADD], [
  AC_CACHE_CHECK([whether the target supports __sync_fetch_and_add],
		 ac_cv_have_sync_fetch_and_add, [
  AC_LINK_IFELSE([AC_LANG_PROGRAM([int foo, bar;],
			[bar = __sync_fetch_and_add(&foo, 1);])],
			[ac_cv_have_sync_fetch_and_add=yes],
			[ac_cv_have_sync_fetch_and_add=no])])
  if test $ac_cv_have_sync_fetch_and_add = yes; then
    AC_DEFINE(STARPU_HAVE_SYNC_FETCH_AND_ADD, 1,
	      [Define to 1 if the target supports __sync_fetch_and_add])
  fi])

dnl Check whether the target supports __sync_fetch_and_or.
AC_DEFUN([STARPU_CHECK_SYNC_FETCH_AND_OR], [
  AC_CACHE_CHECK([whether the target supports __sync_fetch_and_or],
		 ac_cv_have_sync_fetch_and_or, [
  AC_LINK_IFELSE([AC_LANG_PROGRAM([int foo, bar;],
			[bar = __sync_fetch_and_or(&foo, 1);])],
			[ac_cv_have_sync_fetch_and_or=yes],
			[ac_cv_have_sync_fetch_and_or=no])])
  if test $ac_cv_have_sync_fetch_and_or = yes; then
    AC_DEFINE(STARPU_HAVE_SYNC_FETCH_AND_OR, 1,
	      [Define to 1 if the target supports __sync_fetch_and_or])
  fi])

dnl Check whether the target supports __sync_lock_test_and_set.
AC_DEFUN([STARPU_CHECK_SYNC_LOCK_TEST_AND_SET], [
  AC_CACHE_CHECK([whether the target supports __sync_lock_test_and_set],
		 ac_cv_have_sync_lock_test_and_set, [
  AC_LINK_IFELSE([AC_LANG_PROGRAM([int foo, bar;],
			[bar = __sync_lock_test_and_set(&foo, 1);])],
			[ac_cv_have_sync_lock_test_and_set=yes],
			[ac_cv_have_sync_lock_test_and_set=no])])
  if test $ac_cv_have_sync_lock_test_and_set = yes; then
    AC_DEFINE(STARPU_HAVE_SYNC_LOCK_TEST_AND_SET, 1,
	      [Define to 1 if the target supports __sync_lock_test_and_set])
  fi])

dnl Check whether the target supports __sync_synchronize.
AC_DEFUN([STARPU_CHECK_SYNC_SYNCHRONIZE], [
  AC_CACHE_CHECK([whether the target supports __sync_synchronize],
		 ac_cv_have_sync_synchronize, [
  AC_LINK_IFELSE([AC_LANG_PROGRAM(,
			[__sync_synchronize();])],
			[ac_cv_have_sync_synchronize=yes],
			[ac_cv_have_sync_synchronize=no])])
  if test $ac_cv_have_sync_synchronize = yes; then
    AC_DEFINE(STARPU_HAVE_SYNC_SYNCHRONIZE, 1,
	      [Define to 1 if the target supports __sync_synchronize])
  fi])
