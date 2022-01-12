---
layout: page
title: fi_provider(3)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}

# NAME

fi_prov_ini \- External provider entry point

fi_param_define / fi_param_get
: Register and retrieve environment variables with the libfabric core

fi_log_enabled / fi_log
: Control and output debug logging information.

fi_open / fi_close
: Open a named library object

fi_export_fid / fi_import_fid
: Share a fabric object between different providers or resources

# SYNOPSIS

```c
#include <rdma/fabric.h>
#include <rdma/prov/fi_prov.h>

struct fi_provider* fi_prov_ini(void);

int fi_param_define(const struct fi_provider *provider, const char *param_name,
	enum fi_param_type type, const char *help_string_fmt, ...);

int fi_param_get_str(struct fi_provider *provider, const char *param_name,
	char **value);

int fi_param_get_int(struct fi_provider *provider, const char *param_name,
	int *value);

int fi_param_get_bool(struct fi_provider *provider, const char *param_name,
	int *value);

int fi_param_get_size_t(struct fi_provider *provider, const char *param_name,
	size_t *value);
```

```c
#include <rdma/fabric.h>
#include <rdma/prov/fi_prov.h>
#include <rdma/prov/fi_log.h>

int fi_log_enabled(const struct fi_provider *prov, enum fi_log_level level,
	enum fi_log_subsys subsys);

void fi_log(const struct fi_provider *prov, enum fi_log_level level,
	enum fi_log_subsys subsys, const char *func, int line,
	const char *fmt, ...);
```

```c
#include <rdma/fabric.h>

int fi_open(uint32_t version, const char *name, void *attr,
	size_t attr_len, uint64_t flags, struct fid **fid, void *context);

int fi_close(struct fid *fid);
```

```c
#include <rdma/fabric.h>
#include <rdma/fi_ext.h>

int fi_export_fid(struct fid *fid, uint64_t flags,
	struct fid **expfid, void *context);

int fi_import_fid(struct fid *fid, struct fid *expfid, uint64_t flags);
```

# ARGUMENTS

*provider*
: Reference to the provider.

*version*
: API version requested by application.

*name*
: Well-known name of the library object to open.

*attr*
: Optional attributes of object to open.

*attr_len*
: Size of any attribute structure passed to fi_open.  Should be 0
  if no attributes are give.

*fid*
: Returned fabric identifier for opened object.

# DESCRIPTION

A fabric provider implements the application facing software
interfaces needed to access network specific protocols,
drivers, and hardware.  The interfaces and structures defined by
this man page are exported by the libfabric library, but are
targeted for provider implementations, rather than for direct
use by most applications.

Integrated providers are those built directly into the libfabric
library itself.  External providers are loaded dynamically by
libfabric at initialization time.  External providers must be in
a standard library path or in the libfabric library search path
as specified by environment variable.  Additionally, external
providers must be named with the suffix "-fi.so" at the end of
the name.

Named objects are special purpose resources which are accessible directly
to applications.  They may be used to enhance or modify the behavior of
library core.  For details, see the fi_open call below.

## fi_prov_ini

This entry point must be defined by external providers.  On loading,
libfabric will invoke fi_prov_ini() to retrieve the provider's
fi_provider structure.  Additional interactions between the libfabric
core and the provider will be through the interfaces defined by that
struct.

## fi_param_define

Defines a configuration parameter for use by a specified provider. The
help_string and param_name arguments must be non-NULL, help_string
must additionally be non-empty. They are copied internally and may be
freed after calling fi_param_define.

## fi_param_get

Gets the value of a configuration parameter previously defined using
fi_param_define(). The value comes from the environment variable name of
the form FI_<provider_name>_<param_name>, all converted to upper case.

If the parameter was previously defined and the user set a value,
FI_SUCCESS is returned and (*value) points to the retrieved
value.

If the parameter name was previously defined, but the user did
not set a value, -FI_ENODATA is returned and the value of (*value)
is unchanged.

If the parameter name was not previously defined via
fi_param_define(), -FI_ENOENT will be returned and the value of
(*value) is unchanged.

If the value in the environment is not valid for the parameter type,
-FI_EINVAL will be returned and the value of (*value) is unchanged.

## fi_log_enabled / fi_log

TODO

## fi_open

Open a library resource using a well-known name.  This feature allows
applications and providers a mechanism which can be used to modify or
enhance core library services and behavior.  The details are specific
based on the requested object name.  Most applications will not need
this level of control.

The library API version known to the application should be provided
through the version parameter.  The use of attributes is object dependent.
If required, attributes should be provided through the attr parameter,
with attr_len set to the size of the referenced attribute structure.
The following is a list of published names, along with descriptions
of the service or resource to which they correspond.

*mr_cache*
: The mr_cache object references the internal memory registration cache
  used by the different providers.  Additional information on the cache
  is available in the `fi_mr(3)` man page.

## fi_export_fid / fi_import_fid

Generally, fabric objects are allocated and managed entirely by a single
provider.  Typically only the application facing software interfaces of
a fabric object are defined, for example, the message or tagged operations
of an endpoint.  The fi_export_fid and fi_import_fid calls provide a
a mechanism by which provider facing APIs may be accessed.  This allows
the creation of fid objects that are shareable between providers, or
for library plug-in services.  The ability to export a shareable object
is object and provider implementation dependent.

Shareable fids typically contain at least 3 main components: a
base fid, a set of exporter defined ops, and a set of importer defined
ops.

# NOTES

TODO

# PROVIDER INTERFACE

The fi_provider structure defines entry points for the libfabric core
to use to access the provider.  All other calls into a provider are
through function pointers associated with allocated objects.

```c
struct fi_provider {
	uint32_t version;
	uint32_t fi_version;
	struct fi_context context;
	const char *name;
	int	(*getinfo)(uint32_t version, const char *node, const char *service,
			uint64_t flags, const struct fi_info *hints,
			struct fi_info **info);
	int	(*fabric)(struct fi_fabric_attr *attr, struct fid_fabric **fabric,
			void *context);
	void	(*cleanup)(void);
};
```

## version

The provider version.  For providers integrated with the library, this is
often the same as the library version.

## fi_version

The library interface version that the provider was implemented against.
The provider's fi_version must be greater than or equal to an application's
requested api version for the application to use the provider.  It is a
provider's responsibility to support older versions of the api if it
wishes to supports legacy applications.  For integrated providers

## TODO

# RETURN VALUE

Returns FI_SUCCESS on success. On error, a negative value corresponding to
fabric errno is returned. Fabric errno values are defined in
`rdma/fi_errno.h`.

# ERRORS


# SEE ALSO

[`fabric`(7)](fabric.7.html),
[`fi_getinfo`(3)](fi_getinfo.3.html)
[`fi_mr`(3)](fi_mr.3.html),
