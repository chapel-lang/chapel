dnl Configury specific to the libfabric udp provider

dnl Called to configure this provider
dnl
dnl Arguments:
dnl
dnl $1: action if configured successfully
dnl $2: action if not configured successfully
dnl
AC_DEFUN([FI_UDP_CONFIGURE],[
	# Determine if we can support the udp provider
	udp_h_happy=0
	udp_shm_happy=0
	AS_IF([test x"$enable_udp" != x"no"],
	      [AC_CHECK_HEADER([sys/socket.h], [udp_h_happy=1],
	                       [udp_h_happy=0])


	       # check if shm_open is already present
	       AC_CHECK_FUNC([shm_open],
			     [udp_shm_happy=1],
			     [udp_shm_happy=0])

	       # look for shm_open in librt if not already present
	       AS_IF([test $udp_shm_happy -eq 0],
		     [FI_CHECK_PACKAGE([udp_shm],
				[sys/mman.h],
				[rt],
				[shm_open],
				[],
				[],
				[],
				[udp_shm_happy=1],
				[udp_shm_happy=0])])
	      ])

	AS_IF([test $udp_h_happy -eq 1 && \
	       test $udp_shm_happy -eq 1], [$1], [$2])
])
