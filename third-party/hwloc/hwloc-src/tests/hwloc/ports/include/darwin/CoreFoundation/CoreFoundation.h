#ifndef HWLOC_PORT_DARWIN_COREFOUNDATION_COREFOUNDATION_H
#define HWLOC_PORT_DARWIN_COREFOUNDATION_COREFOUNDATION_H

typedef unsigned char UInt8;
typedef long int CFIndex;
typedef const int * CFNumberRef;
typedef const void * CFDataRef;
typedef const char * CFStringRef;
typedef const void * CFTypeRef;

#define CFSTR(x) (x)

#define CFRangeMake(x,y) (x)
typedef unsigned CFRange;

typedef unsigned CFTypeID;
extern CFTypeID CFGetTypeID(CFTypeRef);
extern CFTypeID CFNumberGetTypeID(void);
extern CFTypeID CFDataGetTypeID(void);

typedef int CFNumberType;
#define kCFNumberLongLongType 0
extern int CFNumberGetValue(CFNumberRef, CFNumberType, void *);

extern CFIndex CFDataGetLength(CFDataRef);
extern void CFDataGetBytes(CFDataRef, CFRange, UInt8 *);

typedef unsigned CFStringEncoding;
#define kCFStringEncodingUTF8 0
extern const char * CFStringGetCStringPtr(CFStringRef, CFStringEncoding);
extern CFStringRef CFCopyTypeIDDescription(CFTypeID);

extern void CFRelease(CFTypeRef);

typedef const void * CFAllocatorRef;
#define kCFAllocatorDefault NULL

#endif /* HWLOC_PORT_DARWIN_COREFOUNDATION_COREFOUNDATION_H */
