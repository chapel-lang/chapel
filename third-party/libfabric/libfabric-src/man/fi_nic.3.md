---
layout: page
title: fi_nic(3)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}

# NAME

fi_nic \- Fabric network interface card attributes

# NETWORK INTERFACE CARD ATTRIBUTES

The fid_nic structure defines attributes for a struct fi_info that
is directly associated with underlying networking hardware and may
be returned directly from calling [`fi_getinfo`(3)](fi_getinfo.3.html).
The format of fid_nic and the related substructures are defined below.

Note that not all fields of all structures may be available.  Unavailable
or fields that are not applicable to the indicated device will be set to
NULL or 0.

```c
struct fid_nic {
	struct fid             fid;
	struct fi_device_attr *device_attr;
	struct fi_bus_attr    *bus_attr;
	struct fi_link_attr   *link_attr;
	void                  *prov_attr;
};

struct fi_device_attr {
	char *name;
	char *device_id;
	char *device_version;
	char *vendor_id;
	char *driver;
	char *firmware;
};

struct fi_pci_attr {
	uint16_t domain_id;
	uint8_t  bus_id;
	uint8_t  device_id;
	uint8_t  function_id;
};

struct fi_bus_attr {
	enum fi_bus_type       bus_type;
	union {
		struct fi_pci_attr pci;
	} attr;
};

struct fi_link_attr {
	char               *address;
	size_t             mtu;
	size_t             speed;
	enum fi_link_state state;
	char               *network_type;
};

```

## Device Attributes

Device attributes are used to identify the specific virtual or hardware
NIC associated with an fi_info structure.

*name*
: The operating system name associated with the device.  This may be a
  logical network interface name (e.g. eth0 or eno1) or an absolute
  filename.

*device_id*
: This is a vendor specific identifier for the device or product.

*device_version*
: Indicates the version of the device.

*vendor_id*
: Indicates the name of the vendor that distributes the NIC.

*driver*
: The name of the driver associated with the device

*firmware*
: The device's firmware version.

## Bus Attributes

The bus attributes are used to identify the physical location of the NIC in
the system.

*bus_type*
: Indicates the type of system bus where the NIC is located.  Valid values
  are FI_BUS_PCI or FI_BUS_UNKNOWN.

*attr.pci.domain_id*
: The domain where the PCI bus is located.  Valid only if bus_type is
  FI_BUS_PCI.

*attr.pci.bus_id*
: The PCI bus identifier where the device is located.  Valid only if
  bus_type is FI_BUS_PCI.

*attr.pci.device_id*
: The identifier on the PCI bus where the device is located.  Valid only
  if bus_type is FI_BUS_PCI.

*attr.pci.function_id*
: The function on the device being referenced.  Valid only if bus_type is
  FI_BUS_PCI.

## Link Attributes

Link attributes describe low-level details about the network connection
into the fabric.

*address*
: The primary link-level address associated with the NIC, such as a MAC
  address.  If multiple addresses are available, only one will be reported.

*mtu*
: The maximum transfer unit of link level frames or packets, in bytes.

*speed*
: The active link data rate, given in bits per second.

*state*
: The current physical port state.  Possible values are FI_LINK_UNKNOWN,
  FI_LINK_DOWN, and FI_LINK_UP, to indicate if the port state is unknown
  or not applicable (unknown), inactive (down), or active (up).

*network_type*
: Specifies the type of network interface currently active, such as
  Ethernet or InfiniBand.

## Provider Attributes

Provider attributes reference provider specific details of the device.
These attributes are both provider and device specific.  The attributes
can be interpreted by [`fi_tostr`(3)](fi_tostr.3.html).  Applications
may also use the other attribute fields, such as related fi_fabric_attr:
prov_name field, to determine an appropriate structure to cast the
attributes.  The format and definition of this field is outside the
scope of the libfabric core framework, but may be available as part
of a provider specific header file included with libfabric package.

# NOTES

The fid_nic structure is returned as part of a call to
[`fi_getinfo`(3)](fi_getinfo.3.html).  It is automatically freed as part
of calling [`fi_freeinfo`(3)](fi_freeinfo.3.html)

# SEE ALSO

[`fi_getinfo`(3)](fi_getinfo.3.html)
