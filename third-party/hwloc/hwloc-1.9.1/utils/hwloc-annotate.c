/*
 * Copyright © 2012 Inria.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <private/private.h>
#include <hwloc-calc.h>
#include <hwloc.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void usage(const char *callname __hwloc_attribute_unused, FILE *where)
{
	fprintf(where, "Usage: hwloc-annotate [options] <input.xml> <output.xml> <location> <annotation>\n");
	fprintf(where, "  <location> may be:\n");
	fprintf(where, "    all, root, <type>:<logicalindex>, <type>:all\n");
	fprintf(where, "  <annotation> may be:\n");
	fprintf(where, "    info <name> <value>\n");
	fprintf(where, "    none\n");
        fprintf(where, "Options:\n");
	fprintf(where, "  --ci\tClear existing infos\n");
	fprintf(where, "  --ri\tReplace or remove existing infos with same name (annotation must be info)\n");
}

static char *infoname = NULL, *infovalue = NULL;

static int clearinfos = 0;
static int replaceinfos = 0;

static void apply(hwloc_obj_t obj)
{
	unsigned i,j;
	if (clearinfos) {
		/* this may be considered dangerous, applications should not modify objects directly */
		for(i=0; i<obj->infos_count; i++) {
			free(obj->infos[i].name);
			free(obj->infos[i].value);
		}
		free(obj->infos);
		obj->infos = NULL;
		obj->infos_count = 0;
	}
	if (infoname) {
		if (replaceinfos) {
			/* this may be considered dangerous, applications should not modify objects directly */
			for(i=0, j=0; i<obj->infos_count; i++) {
				if (!strcmp(infoname, obj->infos[i].name)) {
					/* remove info */
					free(obj->infos[i].name);
					free(obj->infos[i].value);
					obj->infos[i].name = NULL;
				} else {
					if (i != j) {
						/* shift info to where it belongs */
						obj->infos[j].name = obj->infos[i].name;
						obj->infos[j].value = obj->infos[i].value;
					}
					j++;
				}
			}
			obj->infos_count = j;
			if (!j) {
				free(obj->infos);
				obj->infos = NULL;
			}
		}
		if (infovalue)
			hwloc_obj_add_info(obj, infoname, infovalue);
	}
}

static void apply_recursive(hwloc_obj_t obj)
{
	unsigned i;
	for(i=0; i<obj->arity; i++)
		apply_recursive(obj->children[i]);
	apply(obj);
}

int main(int argc, char *argv[])
{
	hwloc_topology_t topology;
	char *callname, *input, *output, *location;
	int err;

	putenv("HWLOC_XML_VERBOSE=1");

	callname = argv[0];
	/* skip argv[0], handle options */
	argc--;
	argv++;

	while (argc && *argv[0] == '-') {
		if (!strcmp(argv[0], "--ci"))
			clearinfos = 1;
		else if (!strcmp(argv[0], "--ri"))
			replaceinfos = 1;
		else {
			fprintf(stderr, "Unrecognized options: %s\n", argv[0]);
			usage(callname, stderr);
			exit(EXIT_FAILURE);
		}
		argc--;
		argv++;
	}

	if (argc < 3) {
		usage(callname, stderr);
		exit(EXIT_FAILURE);
	}
	input = argv[0];
	output = argv[1];
	location = argv[2];
	argc -= 3;
	argv += 3;

	if (argc < 1) {
		usage(callname, stderr);
		exit(EXIT_FAILURE);
	}
	if (!strcmp(argv[0], "info")) {
		if (argc < 2 || (!replaceinfos && argc < 3)) {
			usage(callname, stderr);
			exit(EXIT_FAILURE);
		}
		infoname = argv[1];
		infovalue = argc >= 3 ? argv[2] : NULL;

	} else if (!strcmp(argv[0], "none")) {
		/* do nothing (maybe clear) */
	} else {
		fprintf(stderr, "Unrecognized annotation type: %s\n", argv[0]);
		usage(callname, stderr);
		exit(EXIT_FAILURE);
	}

	if (replaceinfos && !infoname) {
		fprintf(stderr, "--ri missing a info name\n");
		usage(callname, stderr);
		exit(EXIT_FAILURE);
	}

	hwloc_topology_init(&topology);
	err = hwloc_topology_set_xml(topology, input);
	if (err < 0)
		goto out;
	hwloc_topology_load(topology);

	if (!strcmp(location, "all")) {
		apply_recursive(hwloc_get_root_obj(topology));
	} else if (!strcmp(location, "root")) {
		apply(hwloc_get_root_obj(topology));
	} else {
		unsigned i;
		hwloc_obj_t obj;
		hwloc_obj_type_t type;
		size_t typelen;
		int depth;
		char *sep;
		typelen = strspn(location, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
		if (!typelen || location[typelen] != ':') {
			/* FIXME: warn */
			goto out;
		}
		sep = &location[typelen];
		depth = hwloc_calc_parse_depth_prefix(topology, hwloc_topology_get_depth(topology),
						      location, typelen, &type, 0);
		if (depth < 0) {
			/* FIXME: warn */
			goto out;
		}
		if (!strcmp(sep+1, "all")) {
			for(i=0; i<hwloc_get_nbobjs_by_depth(topology, depth); i++) {
				obj = hwloc_get_obj_by_depth(topology, depth, i);
				assert(obj);
				apply(obj);
			}
		} else {
			i = atoi(sep+1);
			obj = hwloc_get_obj_by_depth(topology, depth, i);
			if (!obj) {
				/* FIXME: warn */
				goto out;
			}
			apply(obj);
		}
	}

	err = hwloc_topology_export_xml(topology, output);
	if (err < 0)
		goto out;

	hwloc_topology_destroy(topology);
	exit(EXIT_SUCCESS);

out:
	hwloc_topology_destroy(topology);
	exit(EXIT_FAILURE);
}
