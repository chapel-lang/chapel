# -*- Autoconf -*-
#
# Copyright (c)      2008  Sandia Corporation
#

# QTHREAD_CHECK_ALTIX_TIMER([action-if-found], [action-if-not-found])
# ------------------------------------------------------------------------------
AC_DEFUN([QTHREAD_CHECK_ALTIX_TIMER], [
AC_CHECK_HEADERS([sn/mmtimer.h linux/mmtimer.h],
  [timer_altix_happy="yes"
  break],
  [timer_altix_happy="no"])

AS_IF([test "x$timer_altix_happy" = "xyes"],
      [AC_CHECK_HEADERS([sys/ioctl.h sys/mman.h])])

AS_IF([test "x$timer_altix_happy" = "xyes"],
      [AC_CACHE_CHECK([if MM timer can be opened],
         [qthread_cv_mm_timer_mmap],
         [AC_RUN_IFELSE([AC_LANG_PROGRAM([
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#if HAVE_SN_MMTIMER_H
# include <sn/mmtimer.h>
#elif HAVE_LINUX_MMTIMER_H
# include <linux/mmtimer.h>
#endif
#ifndef MMTIMER_FULLNAME
# define MMTIMER_FULLNAME "/dev/mmtimer"
#endif
], [
    int fd, ret;
	unsigned long val;
	long offset;
	unsigned long *mmdev_map = NULL;
	volatile unsigned long *timer_address;
    fd = open(MMTIMER_FULLNAME, O_RDONLY);
    if (fd < 0) return -1;
	ret = ioctl(fd, MMTIMER_GETFREQ, &val);
	if (ret == -ENOSYS) return -1;
	ret = ioctl(fd, MMTIMER_GETOFFSET, 0);
	if (ret == -ENOSYS) return -1;
	offset = ret;
	mmdev_map = mmap(0, getpagesize(), PROT_READ, MAP_SHARED, fd, 0);
	if (NULL == mmdev_map) return -1;
	timer_address = mmdev_map + offset;
	val = *timer_address;
	if (val == 0) return -1;
	return 0;])],
            [qthread_cv_mm_timer_mmap="yes"],
            [qthread_cv_mm_timer_mmap="no"],
			[qthread_cv_mm_timer_mmap="no"])])
AS_IF([test "x$qthread_cv_mm_timer_mmap" = "xyes"],
      [timer_altix_happy="yes"],
      [timer_altix_happy="no"])])

AS_IF([test "x$timer_altix_happy" = "xyes"], [$1], [$2])
])
