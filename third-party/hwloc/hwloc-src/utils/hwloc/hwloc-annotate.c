/*
 * Copyright © 2012-2016 Inria.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <private/private.h>
#include <hwloc-calc.h>
#include <hwloc.h>

#include "misc.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void usage(const char *callname __hwloc_attribute_unused, FILE *where)
{
	fprintf(where, "Usage: hwloc-annotate [options] <input.xml> <output.xml> <location> <annotation>\n");
	fprintf(where, "  <location> may be:\n");
	fprintf(where, "    all, root, <type>:<logicalindex>, <type>:all\n");
	fprintf(where, "  <annotation> may be:\n");
	fprintf(where, "    info <name> <value>\n");
	fprintf(where, "    misc <name>\n");
	fprintf(where, "    none\n");
        fprintf(where, "Options:\n");
	fprintf(where, "  --ci\tClear existing infos\n");
	fprintf(where, "  --ri\tReplace or remove existing infos with same name (annotation must be info)\n");
	fprintf(where, "  --cu\tClear existing userdata\n");
}

static char *infoname = NULL, *infovalue = NULL;

static char *miscname = NULL;

static int clearinfos = 0;
static int replaceinfos = 0;
static int clearuserdata = 0;

static void apply(hwloc_topology_t topology, hwloc_obj_t obj)
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
	if (clearuserdata) {
		hwloc_utils_userdata_free(obj);
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
	if (miscname)
		hwloc_topology_insert_misc_object_by_parent(topology, obj, miscname);
}

static void apply_recursive(hwloc_topology_t topology, hwloc_obj_t obj)
{
	unsigned i;
	for(i=0; i<obj->arity; i++)
		apply_recursive(topology, obj->children[i]);
	apply(topology, obj);
}

static void
hwloc_calc_process_arg_info_cb(void *_data,
			       hwloc_obj_t obj,
			       int verbose __hwloc_attribute_unused)
{
	hwloc_topology_t topology = _data;
	apply(topology, obj);
}

int main(int argc, char *argv[])
{
	hwloc_topology_t topology;
	char *callname, *input, *output, *location;
	unsigned topodepth;
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
		else if (!strcmp(argv[0], "--cu"))
			clearuserdata = 1;
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

	} else if (!strcmp(argv[0], "misc")) {
		if (argc < 2) {
			usage(callname, stderr);
			exit(EXIT_FAILURE);
		}
		miscname = argv[1];

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
	hwloc_topology_set_flags(topology, HWLOC_TOPOLOGY_FLAG_WHOLE_SYSTEM | HWLOC_TOPOLOGY_FLAG_WHOLE_IO | HWLOC_TOPOLOGY_FLAG_ICACHES);
	err = hwloc_topology_set_xml(topology, input);
	if (err < 0)
		goto out;

	putenv("HWLOC_XML_USERDATA_NOT_DECODED=1");
	hwloc_topology_set_userdata_import_callback(topology, hwloc_utils_userdata_import_cb);
	hwloc_topology_set_userdata_export_callback(topology, hwloc_utils_userdata_export_cb);

	hwloc_topology_load(topology);

	topodepth = hwloc_topology_get_depth(topology);

	if (!strcmp(location, "all")) {
		apply_recursive(topology, hwloc_get_root_obj(topology));
	} else if (!strcmp(location, "root")) {
		apply(topology, hwloc_get_root_obj(topology));
	} else {
		size_t typelen;
		typelen = strspn(location, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
		if (typelen && (location[typelen] == ':' || location[typelen] == '=' || location[typelen] == '[')) {
			err = hwloc_calc_process_type_arg(topology, topodepth, location, typelen, 1,
							  hwloc_calc_process_arg_info_cb, topology,
							  0);
		}
	}

	err = hwloc_topology_export_xml(topology, output);
	if (err < 0)
		goto out;

	hwloc_utils_userdata_free_recursive(hwloc_get_root_obj(topology));
	hwloc_topology_destroy(topology);
	exit(EXIT_SUCCESS);

out:
	hwloc_utils_userdata_free_recursive(hwloc_get_root_obj(topology));
	hwloc_topology_destroy(topology);
	exit(EXIT_FAILURE);
}
