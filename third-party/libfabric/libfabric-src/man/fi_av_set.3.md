---
layout: page
title: fi_av_set(3)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}

# NAME

fi_av_set \- Address vector set operations

fi_av_set / fi_close
: Open or close an address vector set

fi_av_set_union
: Perform a set union operation on two AV sets

fi_av_set_intersect
: Perform a set intersect operation on two AV sets

fi_av_set_diff
: Perform a set difference operation on two AV sets

fi_av_set_insert
: Add an address to an AV set

fi_av_set_remove
: Remove an address from an AV set

fi_av_set_addr
: Obtain a collective address for current addresses in an AV set


# SYNOPSIS

```c
#include <rdma/fi_collective.h>

int fi_av_set(struct fid_av *av, struct fi_av_set_attr *attr,
	  struct fid_av_set **set, void * context);

int fi_av_set_union(struct fid_av_set *dst,	const struct fid_av_set *src);

int fi_av_set_intersect(struct fid_av_set *dst, const struct fid_av_set *src);

int fi_av_set_diff(struct fid_av_set *dst, const struct fid_av_set *src);

int fi_av_set_insert(struct fid_av_set *set, fi_addr_t addr);

int fi_av_set_remove(struct fid_av_set *set, fi_addr_t addr);

int fi_av_set_addr(struct fid_av_set *set, fi_addr_t *coll_addr);

int fi_close(struct fid *av_set);
```

# ARGUMENTS

*av*
: Address vector

*set*
: Address vector set

*dst*
: Address vector set updated by set operation

*src*
: Address vector set providing input to a set operation

*attr*
: Address vector set attributes

*context*
: User specified context associated with the address vector set

*flags*
: Additional flags to apply to the operation.

*addr*
: Destination address to insert to remove from AV set.

*coll_addr*
: Address identifying collective group.

# DESCRIPTION

An address vector set (AV set) represents an ordered subset of addresses of an
address vector.  AV sets are used to identify the participants in a collective
operation.  Endpoints use the fi_join_collective() operation to associate
itself with an AV set.  The join collective operation provides an fi_addr that
is used when communicating with a collective group.

The creation and manipulation of an AV set is a local operation.  No fabric
traffic is exchanged between peers.  As a result, each peer is responsible
for creating matching AV sets as part of their collective membership definition.
See [`fi_collective`(3)](fi_collective.3.html) for a discussion of membership
models.

## fi_av_set

The fi_av_set call creates a new AV set.  The initial properties of the AV
set are specified through the struct fi_av_set_attr parameter.  This
structure is defined below, and allows including a subset of addresses in the
AV set as part of AV set creation.  Addresses may be added or removed from an
AV set using the AV set interfaces defined below.

## fi_av_set_attr

{% highlight c %}
struct fi_av_set_attr {
	size_t count;
	fi_addr_t start_addr;
	fi_addr_t end_addr;
	uint64_t stride;
	size_t comm_key_size;
	uint8_t *comm_key;
	uint64_t flags;
};
{% endhighlight %}

*count*
: Indicates the expected the number of members that will be a part of
  the AV set.  The provider uses this to optimize resource allocations.
  If count is 0, the provider will select a size based on
  available system configuration data or underlying limitations.

*start_addr / end_addr*
: The starting and ending addresses, inclusive, to
  include as part of the AV set.  The use of start and end address require
  that the associated AV have been created as type FI_AV_TABLE.  Valid
  addresses in the AV which fall within the specified range and which meet other
  requirements (such as stride) will be added as initial members to the AV set.
  The start_addr and end_addr must be set to FI_ADDR_NOTAVAIL if creating an
  empty AV set, a communication key is being provided, or the AV is of
  type FI_AV_MAP.

  The number of addresses between start_addr and end_addr must be less than
  or equal to the specified count value.

*stride*
: The number of entries between successive addresses included in the
  AV set.  The AV set will include all addresses from start_addr + stride x i,
  for increasing, non-negative, integer values of i, up to end_addr.  A stride
  of 1 indicates that all addresses between start_addr and end_addr should be
  added to the AV set.  Stride should be set to 0 unless the start_addr and
  end_addr fields are valid.

*comm_key_size*
: The length of the communication key in bytes.  This
  field should be 0 if a communication key is not available.

*comm_key*
: If supported by the fabric, this represents a key
  associated with the AV set.  The communication key is used by applications
  that directly manage collective membership through a fabric management agent
  or resource manager.  The key is used to convey that results of the
  membership setup to the underlying provider.  The use and format of a
  communication key is fabric provider specific.

*flags*
: If the flag FI_UNIVERSE is set, then the AV set will be created
  containing all addresses stored in the AV.

## fi_av_set_union

The AV set union call adds all addresses in the source AV set that are not
in the destination AV set to the destination AV set.  Where ordering matters,
the newly inserted addresses are placed at the end of the AV set.

## fi_av_set_intersect

The AV set intersect call remove all addresses from the destination AV set that
are not also members of the source AV set.  The order of the addresses in the
destination AV set is unchanged.

## fi_av_set_diff

The AV set difference call removes all address from the destination AV set
that are also members of the source AV set.  The order of the addresses in the
destination AV set is unchanged.

## fi_av_set_insert

The AV set insert call appends the specified address to the end of the AV set.

## fi_av_set_remove

The AV set remove call removes the specified address from the given AV set.
The order of the remaining addresses in the AV set is unchanged.

## fi_av_set_addr

Returns an address that may be used to communicate with all current members
of an AV set.  This is a local operation only that does not involve network
communication.  The returned address may be used as input into
fi_join_collective.  Note that attempting to use the address returned from
fi_av_set_addr (e.g. passing it to fi_join_collective) while simultaneously
modifying the addresses stored in an AV set results in undefined behavior.

## fi_close

Closes an AV set and releases all resources associated with it.  Any
operations active at the time an AV set is closed will be aborted, with
the result of the collective undefined.

# NOTES

Developers who are familiar with MPI will find that AV sets are similar to
MPI groups, and may act as a direct mapping in some, but not all, situations.

# RETURN VALUES

Returns 0 on success. On error, a negative value corresponding to fabric
errno is returned. Fabric errno values are defined in
`rdma/fi_errno.h`.

# SEE ALSO

[`fi_av`(3)](fi_av.3.html),
[`fi_collective`(3)](fi_collective.3.html)
