---
layout: page
title: fi_usnic(7)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}

# NAME

fi_usnic \- The usNIC Fabric Provider

# OVERVIEW

The *usnic* provider is designed to run over the Cisco VIC
(virtualized NIC) hardware on Cisco UCS servers.  It utilizes the
Cisco usNIC (userspace NIC) capabilities of the VIC to enable ultra
low latency and other offload capabilities on Ethernet networks.

# RELEASE NOTES

* The *usnic* libfabric provider requires the use of the "libnl"
  library.
    - There are two versions of libnl generally available: v1 and v3;
      the usnic provider can use either version.
    - If you are building libfabric/the usnic provider from source, you
      will need to have the libnl header files available (e.g., if you
      are installing libnl from RPM or other packaging system, install
      the "-devel" versions of the package).
    - If you have libnl (either v1 or v3) installed in a non-standard
      location (e.g., not in /usr/lib or /usr/lib64), you may need to
      tell libfabric's configure where to find libnl via the
      `--with-libnl=DIR` command line option (where DIR is the
      installation prefix of the libnl package).
* The most common way to use the libfabric usnic provider is via an
  MPI implementation that uses libfabric (and the usnic provider) as a
  lower layer transport.  MPI applications do not need to know
  anything about libfabric or usnic in this use case -- the MPI
  implementation hides all these details from the application.
* If you are writing applications directly to the libfabric API:
    - *FI_EP_DGRAM* endpoints are the best supported method of utilizing
      the usNIC interface.  Specifically, the *FI_EP_DGRAM* endpoint
      type has been extensively tested as the underlying layer for Open
      MPI's *usnic* BTL.
    - *FI_EP_MSG* and *FI_EP_RDM* endpoints are implemented, but are
      only lightly tested.  It is likely that there are still some bugs
      in these endpoint types. In particular, there are known bugs in RDM
      support in the presence of congestion or packet loss (issue 1621).
      RMA is not yet supported.
    - [`fi_provider`(7)](fi_provider.7.html) lists requirements for all
      providers.  The following limitations exist in the *usnic*
      provider:
        * multicast operations are not supported on *FI_EP_DGRAM* and
          *FI_EP_RDM* endpoints.
        * *FI_EP_MSG* endpoints only support connect, accept, and getname
          CM operations.
        * Passive endpoints only support listen, setname, and getname CM
          operations.
        * *FI_EP_DGRAM* endpoints support `fi_sendmsg()` and
          `fi_recvmsg()`, but some flags are ignored.  `fi_sendmsg()`
          supports `FI_INJECT` and `FI_COMPLETION`.  `fi_recvmsg()`
          supports `FI_MORE`.
        * Address vectors only support `FI_AV_MAP`.
        * No counters are supported.
        * The tag matching interface is not supported.
        * *FI_MSG_PREFIX* is only supported on *FI_EP_DGRAM* and usage
          is limited to releases 1.1 and beyond.
        * fi_control with FI_GETWAIT may only be used on CQs that have been
          bound to an endpoint. If fi_control is used on an unbound CQ, it will
          return -FI_EOPBADSTATE.
        * There is limited support for data returned as part of an erroneous
          asynchronous operation. EQs will return error data for CM operations,
          CQs do not support returning error data.
        * As of 1.5, usNIC supports fi_mr_regv, and fi_mr_regattr. Support is
          limited to a single iov.
        * Atomic operations are not supported.
    - Resource management is not supported. The application is responsible for
      resource protection.
    - The usnic libfabric provider supports extensions that provide
      information and functionality beyond the standard libfabric
      interface.  See the "USNIC EXTENSIONS" section, below.

# USNIC EXTENSIONS

The usnic libfabric provider exports extensions for additional VIC,
usNIC, and Ethernet capabilities not provided by the standard
libfabric interface.

These extensions are available via the "fi_ext_usnic.h" header file.

## Fabric Extension: getinfo

Version 2 of the "fabric getinfo" extension was introduced in Libfabric release
v1.3.0 and can be used to retrieve IP and SR-IOV information about a usNIC
device obtained from the [`fi_getinfo`(3)](fi_getinfo.3.html) function.

The "fabric getinfo" extension is obtained by calling `fi_open_ops` and
requesting `FI_USNIC_FABRIC_OPS_1` to get the usNIC fabric extension
operations.  The `getinfo` function accepts a version parameter that can be
used to select different versions of the extension. The information returned by
the "fabric getinfo" extension is accessible through a `fi_usnic_info` struct
that uses a version tagged union. The accessed union member must correspond
with the requested version. It is recommended that applications explicitly
request a version rather than using the header provided
`FI_EXT_USNIC_INFO_VERSION`. Although there is a version 1 of the extension,
its use is discouraged, and it may not be available in future releases.

### Compatibility issues

The addition of version 2 of the extension caused an alignment issue that
could lead to invalid data in the v1 portion of the structure. This means that
the alignment difference manifests when an application using v1 of the
extension is compiled with Libfabric v1.1.x or v1.2.x, but then runs with
Libfabric.so that is v1.3.x or higher (and vice versa).

The v1.4.0 release of Libfabric introduced a padding field to explicitly
maintain compatibility with the v1.3.0 release. If the issue is encountered,
then it is recommended that you upgrade to a release containing version 2 of
the extension, or recompile with a patched version of an older release.


```c
#include <rdma/fi_ext_usnic.h>

struct fi_usnic_info {
    uint32_t ui_version;
    uint8_t ui_pad0[4];
    union {
        struct fi_usnic_info_v1 v1;
        struct fi_usnic_info_v2 v2;
    } ui;
} __attribute__((packed));

int getinfo(uint32_t version, struct fid_fabric *fabric,
        struct fi_usnic_info *info);
```

*version*
: Version of getinfo to be used

*fabric*
: Fabric descriptor

*info*
: Upon successful return, this parameter will contain information about the
fabric.

- Version 2

```c
struct fi_usnic_cap {
    const char *uc_capability;
    int uc_present;
} __attribute__((packed));

struct fi_usnic_info_v2 {
    uint32_t        ui_link_speed;
    uint32_t        ui_netmask_be;
    char            ui_ifname[IFNAMSIZ];
    unsigned        ui_num_vf;
    unsigned        ui_qp_per_vf;
    unsigned        ui_cq_per_vf;

    char            ui_devname[FI_EXT_USNIC_MAX_DEVNAME];
    uint8_t         ui_mac_addr[6];

    uint8_t         ui_pad0[2];

    uint32_t        ui_ipaddr_be;
    uint32_t        ui_prefixlen;
    uint32_t        ui_mtu;
    uint8_t         ui_link_up;

    uint8_t         ui_pad1[3];

    uint32_t        ui_vendor_id;
    uint32_t        ui_vendor_part_id;
    uint32_t        ui_device_id;
    char            ui_firmware[64];

    unsigned        ui_intr_per_vf;
    unsigned        ui_max_cq;
    unsigned        ui_max_qp;

    unsigned        ui_max_cqe;
    unsigned        ui_max_send_credits;
    unsigned        ui_max_recv_credits;

    const char      *ui_nicname;
    const char      *ui_pid;

    struct fi_usnic_cap **ui_caps;
} __attribute__((packed));
```

- Version 1

```c
struct fi_usnic_info_v1 {
    uint32_t ui_link_speed;
    uint32_t ui_netmask_be;
    char ui_ifname[IFNAMSIZ];

    uint32_t ui_num_vf;
    uint32_t ui_qp_per_vf;
    uint32_t ui_cq_per_vf;
} __attribute__((packed));
```

Version 1 of the "fabric getinfo" extension can be used by explicitly
requesting it in the call to `getinfo` and accessing the `v1` portion of the
`fi_usnic_info.ui` union. Use of version 1 is not recommended and it may be
removed from future releases.


The following is an example of how to utilize version 2 of the usnic "fabric
getinfo" extension.

```c
#include <stdio.h>
#include <rdma/fabric.h>

/* The usNIC extensions are all in the
   rdma/fi_ext_usnic.h header */
#include <rdma/fi_ext_usnic.h>

int main(int argc, char *argv[]) {
    struct fi_info *info;
    struct fi_info *info_list;
    struct fi_info hints = {0};
    struct fi_ep_attr ep_attr = {0};
    struct fi_fabric_attr fabric_attr = {0};

    fabric_attr.prov_name = "usnic";
    ep_attr.type = FI_EP_DGRAM;

    hints.caps = FI_MSG;
    hints.mode = FI_LOCAL_MR | FI_MSG_PREFIX;
    hints.addr_format = FI_SOCKADDR;
    hints.ep_attr = &ep_attr;
    hints.fabric_attr = &fabric_attr;

    /* Find all usnic providers */
    fi_getinfo(FI_VERSION(1, 0), NULL, 0, 0, &hints, &info_list);

    for (info = info_list; NULL != info; info = info->next) {
        /* Open the fabric on the interface */
        struct fid_fabric *fabric;
        fi_fabric(info->fabric_attr, &fabric, NULL);

        /* Pass FI_USNIC_FABRIC_OPS_1 to get usnic ops
           on the fabric */
        struct fi_usnic_ops_fabric *usnic_fabric_ops;
        fi_open_ops(&fabric->fid, FI_USNIC_FABRIC_OPS_1, 0,
                (void **) &usnic_fabric_ops, NULL);

        /* Now use the returned usnic ops structure to call
           usnic extensions.  The following extension queries
           some IP and SR-IOV characteristics about the
           usNIC device. */
        struct fi_usnic_info usnic_info;

        /* Explicitly request version 2. */
        usnic_fabric_ops->getinfo(2, fabric, &usnic_info);

        printf("Fabric interface %s is %s:\n"
               "\tNetmask:  0x%08x\n\tLink speed: %d\n"
               "\tSR-IOV VFs: %d\n\tQPs per SR-IOV VF: %d\n"
               "\tCQs per SR-IOV VF: %d\n",
               info->fabric_attr->name,
               usnic_info.ui.v2.ui_ifname,
               usnic_info.ui.v2.ui_netmask_be,
               usnic_info.ui.v2.ui_link_speed,
               usnic_info.ui.v2.ui_num_vf,
               usnic_info.ui.v2.ui_qp_per_vf,
               usnic_info.ui.v2.ui_cq_per_vf);

        fi_close(&fabric->fid);
    }

    fi_freeinfo(info_list);
    return 0;
}
```

## Adress Vector Extension: get_distance

The "address vector get_distance" extension was introduced in Libfabric release
v1.0.0 and can be used to retrieve the network distance of an address.

The "get_distance" extension is obtained by calling `fi_open_ops` and
requesting `FI_USNIC_AV_OPS_1` to get the usNIC address vector extension
operations.

```c
int get_distance(struct fid_av *av, void *addr, int *metric);
```

*av*
: Address vector

*addr*
: Destination address

*metric*
: On output this will contain `-1` if the destination host is unreachable, `0`
is the destination host is locally connected, and `1` otherwise.

See fi_ext_usnic.h for more details.

# VERSION DIFFERENCES

## New naming convention for fabric/domain starting with libfabric v1.4

The release of libfabric v1.4 introduced a new naming convention for fabric and domain. However the usNIC provider
remains backward compatible with applications supporting the old scheme and decides which one to use based on
the version passed to `fi_getinfo`:

* When `FI_VERSION(1,4)` or higher is used:
     - fabric name is the network address with the CIDR notation (i.e., `a.b.c.d/e`)
     - domain name is the usNIC Linux interface name (i.e., `usnic_X`)

* When a lower version number is used, like `FI_VERSION(1, 3)`, it follows the same behavior the usNIC provider exhibited in libfabric <= v1.3:
     - fabric name is the usNIC Linux interface name (i.e., `usnic_X`)
     - domain name is `NULL`

# SEE ALSO

[`fabric`(7)](fabric.7.html),
[`fi_open_ops`(3)](fi_open_ops.3.html),
[`fi_provider`(7)](fi_provider.7.html),
