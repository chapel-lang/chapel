#ifndef QALLOC_H
#define QALLOC_H

#define _FILE_OFFSET_BITS 64

#include <sys/types.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct mapinfo_s mapinfo_t;
typedef struct dynmapinfo_s dynmapinfo_t;

/* there are two kinds of maps: dynamic maps and static maps. Static maps have
 * very low overhead, but have the restriction that all allocations in them
 * have the same size (also, they're slower to create, initially). Dynamic maps
 * allow you to decide how much memory to use for each allocation at runtime,
 * but have more overhead both in terms of space and time (also, they're very
 * fast to create, initially). */

/* The following functions are fairly straightforward. If the file specified
 * does not exist, it will be created. If the file specified does exist, it
 * will be loaded up. */
void *qalloc_makestatmap(const off_t filesize, void *addr,
			 const char *filename, size_t itemsize,
			 const size_t streams);
void *qalloc_makedynmap(const off_t filesize, void *addr,
			const char *filename, const size_t streams);

/* This function is a special version of the previous two. It reads the map's
 * headers to determine it's vital details, and then loads it up as if you'd
 * used the appropriate function (between the previous two). */
void *qalloc_loadmap(const char *filename);

/* This function sync's the mmap'd regions to disk. */
void qalloc_checkpoint(void);

/* This function performs a checkpoint, and then un-maps all of the currently
 * mapped regions */
void qalloc_cleanup(void);

/* These are the allocation functions.
 *
 * The statmalloc() function allocates one chunk of the size of memory associated with the
 * specified static-size map.
 * The dynmalloc() function allocates one chunk of at least "size" bytes from
 * the specified dynamic-size map.
 * The malloc() function merges the previous two; if the specified map is a
 * static-size map, the size argument is ignored.
 */
void *qalloc_statmalloc(mapinfo_t *map);
void *qalloc_dynmalloc(dynmapinfo_t *map, size_t size);
void *qalloc_malloc(void *map, size_t size);

/* These are the deallocation functions.
 *
 * The statfree() function returns a static-size chunk to its map.
 * The dynfree() function returns a dynamic-size chunk to its map.
 * The free() function is a merging of the above two.
 */
void qalloc_statfree(void *block, mapinfo_t *map);
void qalloc_dynfree(void *block, dynmapinfo_t *map);
void qalloc_free(void *block, void *map);

#ifdef __cplusplus
}
#endif

#endif
