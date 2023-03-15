#ifndef HWLOC_PORT_DARWIN_IOKIT_IOKITLIB_H
#define HWLOC_PORT_DARWIN_IOKIT_IOKITLIB_H

#include "CoreFoundation/CoreFoundation.h"

typedef int io_registry_entry_t;
typedef int io_iterator_t;
typedef int io_object_t;
typedef const char io_name_t[10];
typedef const char * io_string_t;

typedef int kern_return_t;
#define KERN_SUCCESS 0

typedef int mach_port_t;
#define kIOMainPortDefault 0
extern io_registry_entry_t IORegistryEntryFromPath(mach_port_t, const io_string_t);
extern kern_return_t IOObjectRelease(io_object_t);

extern kern_return_t IORegistryEntryGetChildIterator(io_registry_entry_t, const io_name_t, io_iterator_t *);
extern io_object_t IOIteratorNext(io_iterator_t);

extern kern_return_t IORegistryEntryGetNameInPlane(io_registry_entry_t, const io_name_t, io_name_t);

typedef unsigned IOOptionBits;
#define kNilOptions 0
extern CFTypeRef IORegistryEntrySearchCFProperty(io_registry_entry_t, const io_name_t, CFStringRef, CFAllocatorRef, IOOptionBits);

#endif /* HWLOC_PORT_DARWIN_IOKIT_IOKITLIB_H */

