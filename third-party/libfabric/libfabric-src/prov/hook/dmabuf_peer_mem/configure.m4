dnl Configury specific to the libfabrics dmabuf_peer_mem hooking provider

dnl Called to configure this provider
dnl
dnl Arguments:
dnl
dnl $1: action if configured successfully
dnl $2: action if not configured successfully
dnl

AC_DEFUN([FI_DMABUF_PEER_MEM_CONFIGURE],[
    # Determine if we can support the dmabuf_peer_mem hooking provider
    dmabuf_peer_mem_happy=0
    AS_IF([test x"$enable_dmabuf_peer_mem" != x"no"], [dmabuf_peer_mem_happy=1])
    AS_IF([test $dmabuf_peer_mem_happy -eq 1], [$1], [$2])
])
