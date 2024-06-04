/*
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 *
 * Copyright (c) 2021 Hewlett Packard Enterprise Development LP
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <getopt.h>
#include <complex.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <json-c/json.h>

#include <ofi.h>

#include "cxip.h"

/* Parsed arguments */
static bool autotest = false;
static bool verbose = false;
static char *cmd = "get";
static char *data = NULL;
static int parallel = 100;
static char *server = "http://127.0.0.1:5000";
static char *endpoint = "/test";
static char serverpath[1024];

/* Measure timings */
static inline void tmark(struct timespec *t0)
{
	clock_gettime(CLOCK_MONOTONIC, t0);
}

static inline void tmeas(struct timespec *t0)
{
	struct timespec t1;

	clock_gettime(CLOCK_MONOTONIC, &t1);
	if (t1.tv_nsec < t0->tv_nsec) {
		t1.tv_nsec += 1000000000;
		t1.tv_sec -= 1;
	}
	t0->tv_nsec = t1.tv_nsec - t0->tv_nsec;
	t0->tv_sec = t1.tv_sec - t0->tv_sec;
}

#define	failtest(action, fmt, ...) \
	do { fprintf(stderr, fmt, ##__VA_ARGS__); action; } while (0)

/**
 * @brief Exercise the json value parser.
 *
 * @return int : error code
 */
int auto_test_cxip_json(void)
{
	/* Two test objects to parse */
	char json1[] =
		"{"
			"'string': 'string',"
			"'double': 0.1234,"
			"'int64': 9000000000,"
			"'int': 2000000000,"
			"'bool': true,"
			"'object': {"
				"'one': 1,"
				"'two': 2,"
			"},"
			"'array': [0, 1, 2, 3],"
			"'nestedarr': ["
				"[0, 1, 2, 3],"
				"[4, 5, 6, 7]"
			"],"
			"'nestedobj': ["
				"{"
					"'one': 1,"
					"'two': 2"
				"},"
				"{"
					"'three': 3,"
					"'four': 4"
				"}"
			"]"
		"}";
	char json2[] = "[0, 1, 2, 3]";

	json_object *json_obj;
	const char *key;
	const char *string_val;
	double double_val;
	int64_t int64_val;
	int int_val;
	bool bool_val;
	int i;

	/* Change embedded single quotes to double quotes */
	single_to_double_quote(json1);
	single_to_double_quote(json2);

	/* Test parsing of json1 */
	if (!(json_obj = json_tokener_parse(json1)))
		failtest(return 1, "json1 could not be parsed\n");

	key = "string";
	if (cxip_json_string(key, json_obj, &string_val))
		failtest(return 1, "'%s' key not found\n", key);
	if (strcmp(string_val, "string"))
		failtest(return 1, "'%s' returned '%s'\n", key, string_val);

	key = "double";
	if (cxip_json_double(key, json_obj, &double_val))
		failtest(return 1, "'%s' key not found\n", key);
	if (double_val != 0.1234)
		failtest(return 1, "'%s' returned %f\n", key, double_val);

	key = "int64";
	if (cxip_json_int64(key, json_obj, &int64_val))
		failtest(return 1, "'%s' key not found\n", key);
	if (int64_val != 9000000000)
		failtest(return 1, "'%s' returned 0x%lx\n", key, int64_val);

	key = "int";
	if (cxip_json_int(key, json_obj, &int_val))
		failtest(return 1, "'%s' key not found\n", key);
	if (int_val != 2000000000)
		failtest(return 1, "'%s' returned 0x%x\n", key, int_val);

	key = "bool";
	if (cxip_json_bool(key, json_obj, &bool_val))
		failtest(return 1, "'%s' key not found\n", key);
	if (bool_val != true)
		failtest(return 1, "'%s' returned %d\n", key, bool_val);

	key = "object.one";
	if (cxip_json_int(key, json_obj, &int_val))
		failtest(return 1, "'%s' key not found\n", key);
	if (int_val != 1)
		failtest(return 1, "'%s' returned %d\n", key, int_val);

	key = "object.two";
	if (cxip_json_int(key, json_obj, &int_val))
		failtest(return 1, "'%s' key not found\n", key);
	if (int_val != 2)
		failtest(return 1, "'%s' returned %d\n", key, int_val);

	key = "nestedobj[0].one";
	if (cxip_json_int(key, json_obj, &int_val))
		failtest(return 1, "'%s' key not found\n", key);
	if (int_val != 1)
		failtest(return 1, "'%s' returned %d\n", key, int_val);

	key = "nestedobj[0].two";
	if (cxip_json_int(key, json_obj, &int_val))
		failtest(return 1, "'%s' key not found\n", key);
	if (int_val != 2)
		failtest(return 1, "'%s' returned %d\n", key, int_val);

	key = "nestedobj[1].three";
	if (cxip_json_int(key, json_obj, &int_val))
		failtest(return 1, "'%s' key not found\n", key);
	if (int_val != 3)
		failtest(return 1, "'%s' returned %d\n", key, int_val);

	key = "nestedobj[1].four";
	if (cxip_json_int(key, json_obj, &int_val))
		failtest(return 1, "'%s' key not found\n", key);
	if (int_val != 4)
		failtest(return 1, "'%s' returned %d\n", key, int_val);

	for (i = 0; i < 4; i++) {
		char key[256];
		snprintf(key, sizeof(key), "array[%d]", i);
		if (cxip_json_int(key, json_obj, &int_val))
			failtest(return 1, "'%s' key not found\n", key);
		if (int_val != i)
			failtest(return 1, "'%s' returned %d\n", key, int_val);
	}

	for (i = 0; i < 8; i++) {
		char key[256];
		snprintf(key, sizeof(key), "nestedarr[%d][%d]", i/4, i%4);
		if (cxip_json_int(key, json_obj, &int_val))
			failtest(return 1, "'%s' key not found\n", key);
		if (int_val != i)
			failtest(return 1, "'%s' returned %d\n", key, int_val);
	}
	json_object_put(json_obj);

	/* Test parsing of json2 */
	if (!(json_obj = json_tokener_parse(json2)))
		failtest(return 1, "json2 could not be parsed\n");
	for (i = 0; i < 4; i++) {
		char key[256];
		snprintf(key, sizeof(key), "[%d]", i);
		if (cxip_json_int(key, json_obj, &int_val))
			failtest(return 1, "'%s' key not found\n", key);
		if (int_val != i)
			failtest(return 1, "'%s' returned %d\n", key, int_val);
	}
	json_object_put(json_obj);

	if (verbose)
		printf("PASSED JSON tests\n");
	return 0;
}

/**
 * @brief Simple completion callback.
 *
 * This expects an (int) usrptr to be registered with the CURL initiation,
 * and simply increments it every time a CURL operation completes.
 *
 * @param handle : CURL operation handle
 */
static void complete(struct cxip_curl_handle *handle)
{
	int *counter = (int *)handle->usrptr;

	(*counter)++;
}

/**
 * @brief Exercise the CURL code.
 *
 * The flask_testsrv.py code must be running to perform this test. It will
 * pass with a warning message if the server is not found.
 *
 * @return int : error code
 */
int auto_test_cxip_curl(void)
{
	struct cxip_curl_handle *handle;
	struct timespec t0, t1;
	json_object *json_obj;
	int op, ret;
	char tag[256];
	int counter;

	/* confirm that the server is running : status is 0 if no server */
	ret = cxip_curl_perform(serverpath, NULL, NULL, 0, CURL_GET, false,
				complete, &counter);
	do {
		ret = cxip_curl_progress(&handle);
	} while (ret == -FI_EAGAIN);
	if (ret) {
		fprintf(stderr, "cxip_curl_perform() returned %s\n",
			fi_strerror(-ret));
		return ret;
	}
	if (!handle) {
		fprintf(stderr, "cxip_curl_perform() returned no handle\n");
		return -1;
	}
	if (handle->status == 0) {
		fprintf(stderr, "SERVER at %s is not running\n", serverpath);
		cxip_curl_free(handle);
		return 0;
	}
	cxip_curl_free(handle);

	/* Walk through all of the test-supported operations */
	for (op = CURL_GET; op < CURL_MAX; op++) {
		const char *opname = cxip_curl_opname(op);
		bool reordered = false;
		int nextseqid = 0;
		int seqid;
		int i, err = 0;

		/* reset the callback counter to zero on every opcode */
		counter = 0;

		if (verbose)
			printf("\nOperation %s\n", cxip_curl_opname(op));

		/* Run 'parallel' operations concurrently */
		tmark(&t0);
		for (i = 0; i < parallel; i++) {
			sprintf(tag, "{\"seqid\": %d}", i);
			ret = cxip_curl_perform(serverpath, tag, NULL, 0,
						op, false, complete, &counter);
			if (ret != 0)
				fprintf(stderr, "cxip_curl_perform(%d) = %s\n",
				 	i, fi_strerror(ret));
		}
		tmeas(&t0);

		/* Wait for all initiated operations to finish */
		tmark(&t1);
		while (i-- > 0) {
			do {
				sched_yield();
				ret = cxip_curl_progress(&handle);
			} while (ret == -FI_EAGAIN);
			if (ret) {
				/* should not happen, as we are counting */
				fprintf(stderr, "cxip_curl_progress() %s\n",
					fi_strerror(-ret));
				err++;
				continue;
			}
			if (!handle) {
				/* should NEVER happen with good return */
				fprintf(stderr,
					"cxip_curl_progress() no handle\n");
				err++;
				continue;
			}
			if (handle->status != 200) {
				/* our test server should generate 200 */
				fprintf(stderr, "status=%ld\n", handle->status);
				err++;
				goto free_handle;
			}
			if (!handle->response) {
				/* CURL should not return a NULL response */
				fprintf(stderr, "NULL response\n");
				err++;
				goto free_handle;
			}

			/* Test server should return:
			 * {
			 *    "operation": <GET, POST, ...>,
			 *    "data": {"seqid": <seqid>}
			 * }
			 */
			const char *str;
			json_obj = json_tokener_parse(handle->response);
			if (! json_obj) {
				fprintf(stderr, "%s: JSON unparseable\n",
					opname);
				err++;
				goto free_handle;
			}

			if (cxip_json_string("operation", json_obj, &str)) {
				fprintf(stderr, "no 'operation' field\n");
				err++;
				goto free_json;
			}

			if (strcmp(str, opname)) {
				fprintf(stderr, "op=%s exp %s\n", str, opname);
				err++;
				goto free_json;
			}

			/* For GET, seqid is is meaningless */
			if (op == CURL_GET)
				goto free_json;

			if (cxip_json_int("data.seqid", json_obj, &seqid)) {
				fprintf(stderr, "op=%s no seqid\n", opname);
				err++;
				goto free_json;
			}

			/* This confirms that CURL does not order responses */
			if (seqid != nextseqid)
				reordered = true;
free_json:
			json_object_put(json_obj);
free_handle:
			cxip_curl_free(handle);
			nextseqid++;
		}
		tmeas(&t1);

		/* Should be no strays */
		ret = cxip_curl_progress(&handle);
		if (ret != -FI_ENODATA) {
			fprintf(stderr, "op=%s stray handles\n", opname);
			err++;
		}

		/* Callback counter should match number of calls */
		if (counter != parallel) {
			fprintf(stderr, "op=%s count=%d, exp %d\n",
				opname, counter, parallel);
			err++;
		}

		if (verbose) {
			printf("  iterations(%d)\n", parallel);
			printf("  counter   (%d)\n", counter);
			printf("  reordered (%s)\n", reordered ? "true" : "false");
			printf("  errors    (%d)\n", err);
			printf("  issue     (%ld.%09lds)\n", t0.tv_sec, t0.tv_nsec);
			printf("  response  (%ld.%09lds)\n", t1.tv_sec, t1.tv_nsec);
		}

		if (err)
			failtest(return 1, "FAILED CURL tests\n");
	}
	if (verbose)
		printf("\n");

	printf("PASSED CURL tests\n");
	return 0;
}

/**
 * @brief Perform a manual (command-line arguments) test
 *
 * @return int : error code
 */
int do_test(void)
{
	struct cxip_curl_handle *handle;
	struct timespec t0;
	enum curl_ops op;
	int ret;

	if (!strcasecmp(cmd, "get"))
		op = CURL_GET;
	else if (!strcasecmp(cmd, "put"))
		op = CURL_PUT;
	else if (!strcasecmp(cmd, "post"))
		op = CURL_POST;
	else if (!strcasecmp(cmd, "patch"))
		op = CURL_PATCH;
	else if (!strcasecmp(cmd, "delete"))
		op = CURL_DELETE;
	else {
		fprintf(stderr, "Bad HTTP operation \"%s\"", cmd);
		return 1;
	}

	tmark(&t0);
	ret = cxip_curl_perform(serverpath, data, NULL, 0, op, verbose, 0, 0);
	if (ret) {
		fprintf(stderr, "cxip_curl_perform() returned %d\n", ret);
		return ret;
	}

	do {
		sched_yield();
		ret = cxip_curl_progress(&handle);
	} while (ret == -FI_EAGAIN);
	tmeas(&t0);

	if (ret)
		failtest(return 1, "cxip_curl_progress() ret %d\n", ret);
	if (!handle)
		failtest(return 1, "cxip_curl_progress() ret no handle\n");
	if (!handle->status) {
		fprintf(stderr, "SERVER at %s is not running\n", serverpath);
		return 0;
	}

	printf("\n");
	printf("endpoint = %s\n", handle->endpoint);
	printf("time     = %ld.%09lds\n", t0.tv_sec, t0.tv_nsec);
	printf("status   = %ld\n", handle->status);
	printf("request------------\n%s\n", handle->request);
	printf("response-----------\n%s\n", handle->response);

	return 0;
}

int main(int argc, char **argv) {
	static char *opts = "c:d:e:p:r:s:hv";
	static struct option lopts[] = {
		{"help", no_argument, NULL, 'h'},
		{"auto", no_argument, NULL, 'a'},
		{"verbose", no_argument, NULL, 'v'},
		{"command", required_argument, NULL, 'c'},
		{"data", required_argument, NULL, 'd'},
		{"parallel", required_argument, NULL, 'p'},
		{"server", required_argument, NULL, 's'},
		{"endpoint", required_argument, NULL, 'e'},
		{0, 0, 0, 0}
	};
	static const char *help =
		"\nExercise cxip_curl module:\n"
		"  --auto            Perform automated test suite\n"
		"  -c, --command     Define HTTP command\n"
		"  -d, --data        Define HTTP payload (json)\n"
		"  -p, --parallel    Define level of auto-test parallism\n"
		"  -s, --server      REST server address"
			" (default \"%s\")\n"
		"  -e, --endpoint    REST server endpoint\n"
		"  -v, --verbose     Verbose operation\n"
		"  -h, --help        Display help\n";

	int ret = 1;

	while (1) {
		int idx, c;

		c = getopt_long(argc, argv, opts, lopts, &idx);
		if (c == -1)
			break;
		switch (c) {
		case 0:	// long option, all map to single characters
			break;
		case 'a':
			autotest = 1;
			break;
		case 'c':
			cmd = strdup(optarg);
			break;
		case 'd':
			data = strdup(optarg);
			break;
		case 'p':
			parallel = atoi(optarg);
			break;
		case 's':
			server = strdup(optarg);
			break;
		case 'e':
			endpoint = strdup(optarg);
			break;
		case 'v':
			verbose = true;
			break;
		case 'h':
			ret = 0;
			// fall through
		default:
			printf(help, server);
			return ret;
		}
	}

	snprintf(serverpath, sizeof(serverpath), "%s%s", server, endpoint);

	if (cxip_curl_init()) {
		fprintf(stderr, "CURL could not be initialized\n");
		return ret;
	}

	if (autotest) {
		ret = auto_test_cxip_json() |
		      auto_test_cxip_curl();
	} else {
		ret = do_test();
	}

	cxip_curl_fini();
	return ret;
}
