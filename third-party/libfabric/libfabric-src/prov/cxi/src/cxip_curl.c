/*
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 *
 * Copyright (c) 2021 Hewlett Packard Enterprise Development LP
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <curl/curl.h>

#include <ofi.h>

#include "cxip.h"

#define	TRACE_CURL(fmt, ...)	CXIP_COLL_TRACE(CXIP_TRC_COLL_CURL, fmt, \
						##__VA_ARGS__)

#define CXIP_DBG(...) _CXIP_DBG(FI_LOG_FABRIC, __VA_ARGS__)
#define CXIP_WARN(...) _CXIP_WARN(FI_LOG_FABRIC, __VA_ARGS__)

#define	CHUNK_SIZE	4096
#define	CHUNK_MASK	(CHUNK_SIZE-1)

/**
 * Expandable buffer that can receive data in arbitrary-sized chunks.
 */
struct curl_buffer {
	char *buffer;
	size_t size;
	size_t offset;
};

/**
 * Allocate an expandable CURL buffer.
 *
 * This expands as necessary to accommodate the data, which may be delivered in
 * chunks over the network. If you know in advance the approximate size of the
 * return data on a large transfer, you can avoid repeated calls to realloc().
 *
 * @param rsp_init_size : initial size of buffer area (> 0), default 4k
 *
 * @return struct curl_buffer* : returned CURL buffer
 */
static inline struct curl_buffer *init_curl_buffer(size_t rsp_init_size)
{
	struct curl_buffer *buf;

	if (rsp_init_size == 0)
		rsp_init_size = 4096;
	buf = calloc(1, sizeof(*buf));
	if (!buf)
		return NULL;

	buf->buffer = malloc(rsp_init_size);
	if (!buf->buffer) {
		free(buf);
		buf = NULL;
	}

	return buf;
}

/**
 * Free a curl_buffer and all its data.
 *
 * @param buf : curl buffer to delete
 */
static inline void free_curl_buffer(struct curl_buffer *buf)
{
	if (buf)
		free(buf->buffer);
	free(buf);
}

/**
 * Curl is curl-centric, facing the application (not the server). For Curl, a
 * "write" is a write to application memory. This is the function that fills
 * the user's curl buffer with data returned by the server.
 *
 * Buffer expands as needed to accommodate data. Note that this means the buffer
 * itself must be treated as uninitialized memory beyond buf->offset (see
 * realloc()).
 *
 * If the return value does not match the number of bytes requested, it will
 * abort the transfer and the curl function will return CURLE_WRITE_ERROR.
 *
 * @param curl_rcvd : poiter to data received from server
 * @param size : size of member
 * @param nmemb : number of members
 * @param userp : (void *)-cast curl_buffer
 *
 * @return ssize_t : number of bytes added
 */
static size_t write_callback(void *curl_rcvd, size_t size, size_t nmemb,
			     void *userp)
{
	struct curl_buffer *curl_buf = (struct curl_buffer *)userp;
	size_t sz = size * nmemb;
	size_t need = curl_buf->offset + sz;

	if (need >= curl_buf->size) {
		curl_buf->size = (need + CHUNK_MASK) & ~CHUNK_MASK;
		curl_buf->buffer = realloc(curl_buf->buffer, curl_buf->size);
		if (!curl_buf->buffer)
			return 0;
	}
	memcpy(&curl_buf->buffer[curl_buf->offset], curl_rcvd, sz);

	curl_buf->offset += sz;
	return sz;
}

/*
 * The CURL library must be explicitly initialized. It is application-global,
 * and the initialization is not thread-safe, according to the documentation. We
 * do not protect this call, because it is running under CXI_INIT (see
 * cxip_info.c), which is single-threaded. The curl_global_init() call can be
 * issued multiple times (non-concurrently) and has the same end result as
 * calling it once.
 */
static CURLM *cxip_curlm;
static int cxip_curl_count;

/**
 * Initialize CURL globally for the application, enabling multi-curl
 * (concurrent calls).
 */
int cxip_curl_init(void)
{
	int ret = FI_SUCCESS;
	CURLcode res;

	if (!cxip_curlm) {
		res = curl_global_init(CURL_GLOBAL_DEFAULT);
		if (res == CURLE_OK) {
			cxip_curlm = curl_multi_init();
			if (!cxip_curlm) {
				curl_global_cleanup();
				ret = -FI_EINVAL;
			}
		} else
			ret = -FI_EINVAL;
	}
	return ret;
}

/**
 * Globally terminate this module.
 */
void cxip_curl_fini(void)
{
	cxip_curl_count = 0;
	if (cxip_curlm) {
		curl_multi_cleanup(cxip_curlm);
		curl_global_cleanup();
		cxip_curlm = NULL;
	}
}

/**
 * Return a name for an opcode.
 *
 * @param op            : curl operation
 * @return const char*  : printable name for curl operation
 */
const char *cxip_curl_opname(enum curl_ops op)
{
	static char * const curl_opnames[] = {
		"GET",
		"PUT",
		"POST",
		"PATCH",
		"DELETE",
	};
	return (op >= 0 && op < CURL_MAX) ? curl_opnames[op] : NULL;
}

/**
 * Free a handle created by cxip_curl_perform().
 *
 * @param handle : handle created by cxip_curl_perform()
 */
void cxip_curl_free(struct cxip_curl_handle *handle)
{
	if (!handle)
		return;

	free((void *)handle->endpoint);
	free((void *)handle->request);
	/* do not directly free handle->response (== handle->recv->buffer) */
	free_curl_buffer((struct curl_buffer *)handle->recv);
	free(handle);
	cxip_curl_count -= 1;
}

/**
 * Dispatch a CURL request.
 *
 * This is a general-purpose CURL multi (async) JSON format curl request.
 *
 * Note that this function only dispatches the request. cxip_curl_progress()
 * must be called to progress the dispatched operations and retrieve data.
 *
 * The usrfunc is called in cxip_curl_progress() when the request completes,
 * and receives the handle as its sole argument. The handle also contains an
 * arbitrary usrptr supplied by the caller. This usrptr can contain specific
 * information to identify which of multiple concurrent requests has completed.
 *
 * There are no "normal" REST errors from this call. REST errors are instead
 * returned on attempts to progress the dispatched operation.
 *
 * @param endpoint      : HTTP server endpoint address
 * @param request       : JSON-formatted request
 * @param rsp_init_size : initial size of response buffer (can be 0)
 * @param op            : curl operation
 * @param verbose       : use to display sent HTTP headers
 * @param userfunc      : user-defined completion function
 * @param usrptr	: user-defined data pointer
 *
 * @return int          : 0 on success, -1 on failure
 */
int cxip_curl_perform(const char *endpoint, const char *request,
		      const char *sessionToken, size_t rsp_init_size,
		      enum curl_ops op, bool verbose,
		      curlcomplete_t usrfunc, void *usrptr)
{
	struct cxip_curl_handle *handle;
	struct curl_slist *headers;
	char *token;
	char *verify_peer_str;
	int verify_peer;
	CURLMcode mres;
	CURL *curl;
	int running;
	int ret;

	ret = -FI_ENOMEM;
	handle = calloc(1, sizeof(*handle));
	if (!handle)
		goto fail;

	/* libcurl is fussy about NULL requests */
	handle->endpoint = strdup(endpoint);
	if (!handle->endpoint)
		goto fail;
	handle->request = strdup(request ? request : "");
	if (!handle->request)
		goto fail;
	handle->response = NULL;
	handle->recv = (void *)init_curl_buffer(rsp_init_size);
	if (!handle->recv)
		goto fail;
	/* add user completion function and pointer */
	handle->usrfunc = usrfunc;
	handle->usrptr = usrptr;

	ret = -FI_EACCES;
	curl = curl_easy_init();
	if (!curl) {
		CXIP_WARN("curl_easy_init() failed\n");
		goto fail;
	}

	/* HTTP 1.1 assumed */
	headers = NULL;
	headers = curl_slist_append(headers, "Expect:");
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");
	headers = curl_slist_append(headers, "charset: utf-8");
	token = NULL;
	if (sessionToken) {
		ret = asprintf(&token, "Authorization: Bearer %s",
			       sessionToken);
		if (ret < 0) {
			CXIP_WARN("token string create failed\n");
			goto fail;
		}
		headers = curl_slist_append(headers, token);
	}
	handle->headers = (void *)headers;

	curl_easy_setopt(curl, CURLOPT_URL, handle->endpoint);
	if (op == CURL_GET) {
		curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
	} else {
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, handle->request);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE,
				 strlen(handle->request));
	}
	curl_easy_setopt(curl, CURLOPT_STDERR, stderr);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, handle->recv);
	curl_easy_setopt(curl, CURLOPT_PRIVATE, (void *)handle);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, (long)verbose);
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, cxip_curl_opname(op));

	verify_peer_str = getenv("CURLOPT_SSL_VERIFYPEER");
	if (verify_peer_str)
		verify_peer = atoi(verify_peer_str);
	else
		verify_peer = 0;
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, verify_peer);

	curl_multi_add_handle(cxip_curlm, curl);
	mres = curl_multi_perform(cxip_curlm, &running);
	if (mres != CURLM_OK) {
		CXIP_WARN("curl_multi_perform() failed: %s\n",
			  curl_multi_strerror(mres));
		goto fail;
	}
	cxip_curl_count += 1;
	return FI_SUCCESS;

fail:
	CXIP_WARN("%s failed %d\n", __func__, ret);
	cxip_curl_free(handle);
	return ret;
}

/**
 * Progress the CURL requests.
 *
 * This progresses concurrent CURL requests, and returns the following:
 *
 * -  0 indicates an operation completed
 * -  -FI_EAGAIN  indicates operations are pending, none completed
 * -  -FI_ENODATA indicates no operations are pending
 * -  -errorcode  a fatal error
 *
 * Repeated calls will return additional completions, until there are no more
 * pending and -FI_ENODATA is returned.
 *
 * Note that a CURL request will succeed if the server is not reachable. It will
 * return a handle->status value of 0, which is an invalid HTTP status, and
 * indicates that it could not connect to a server.
 *
 * For unit testing, it is useful for the test to be able to inspect the handle
 * directly, and it can be obtained by specifying a non-null handleptr value. If
 * handleptr is supplied, the caller is responsible for calling cxip_curl_free()
 * on the returned handle. In normal usage, handleptr is NULL, and this routine
 * will clean up the handle after the operation completes.
 *
 * The user should provide a callback routine to examine the final state of the
 * CURL request, as well as any data it returns: see cxip_curl_perform(). This
 * user callback is called after completion of the request, before the handle is
 * destroyed.
 *
 * The callback routine has read-only access to the handle, and read-write
 * access to its own data area, available as handle->usrptr.
 *
 * The handle contains the following documented fields:
 *
 * - status   = HTTP status of the op, or 0 if the endpoint could not be reached
 * - endpoint = copy of the endpoint address supplied for the post
 * - request  = copy of the JSON request data supplied for the post
 * - response = pointer to the JSON response returned by the endpoint
 * - usrptr  = arbitrary user pointer supplied during CURL request
 *
 * @param handleptr : if not NULL, returns the request handle
 * @return int      : return code, see above
 */
int cxip_curl_progress(struct cxip_curl_handle **handleptr)
{
	struct cxip_curl_handle *handle;
	struct CURLMsg *msg;
	CURLMcode mres;
	CURLcode res;
	int running;
	int messages;
	long status;
	struct curl_buffer *recv;

	/* This needs to be quick if nothing is pending */
	if (!cxip_curl_count)
		return -FI_ENODATA;

	handle = NULL;

	/* running returns the number of curls running */
	mres = curl_multi_perform(cxip_curlm, &running);
	if (mres != CURLM_OK) {
		CXIP_WARN("curl_multi_perform() failed: %s\n",
			  curl_multi_strerror(mres));
		return -FI_EOTHER;
	}

	/* messages returns the number of additional curls finished */
	msg = curl_multi_info_read(cxip_curlm, &messages);
	if (!msg || msg->msg != CURLMSG_DONE) {
		return (running) ? -FI_EAGAIN : -FI_ENODATA;
	}

	if (msg->data.result >= CURL_LAST) {
		CXIP_WARN("CURL unknown result %d\n", msg->data.result);
	}
	else if (msg->data.result > CURLE_OK) {
		CXIP_WARN("CURL error '%s'\n",
			  curl_easy_strerror(msg->data.result));
	}
	/* retrieve our handle from the private pointer */
	res = curl_easy_getinfo(msg->easy_handle,
				CURLINFO_PRIVATE, (char **)&handle);
	if (res != CURLE_OK) {
		TRACE_CURL("curl_easy_getinfo(%s) failed: %s\n",
			   "CURLINFO_PRIVATE", curl_easy_strerror(res));
		CXIP_WARN("curl_easy_getinfo(%s) failed: %s\n",
			  "CURLINFO_PRIVATE", curl_easy_strerror(res));
		return -FI_EOTHER;
	}
	/* handle is now valid, must eventually be freed */
	/* retrieve the status code, should not fail */
	res = curl_easy_getinfo(msg->easy_handle,
				CURLINFO_RESPONSE_CODE, &status);
	if (res != CURLE_OK) {
		TRACE_CURL("curl_easy_getinfo(%s) failed: %s\n",
			   "CURLINFO_RESPONSE_CODE", curl_easy_strerror(res));
		CXIP_WARN("curl_easy_getinfo(%s) failed: %s\n",
			  "CURLINFO_RESPONSE_CODE", curl_easy_strerror(res));
		/* continue, handle->status should show zero */
	}
	TRACE_CURL("curl_easy_getinfo() success\n");

	/* we can recover resources now */
	curl_slist_free_all((struct curl_slist *)handle->headers);
	curl_easy_cleanup(msg->easy_handle);
	handle->headers = NULL;

	/* make sure response string is terminated */
	recv = (struct curl_buffer *)handle->recv;
	recv->buffer[recv->offset] = 0;
	handle->response = recv->buffer;
	handle->status = status;

	/* call the user function */
	if (handle->usrfunc)
		handle->usrfunc(handle);

	/* return the handle, or free it */
	if (handleptr) {
		*handleptr = handle;
	} else {
		cxip_curl_free(handle);
	}
	return FI_SUCCESS;
}

/**
 * @brief Simplified search for JSON objects.
 *
 * Simplified object search using a descriptor like the following:
 * Example: "firstkey.secondkey.arraykey[3].thirdkey"
 *
 * The first character is '.' or '['. If omitted, it is assumed to be '.'.
 *
 * The appearance of '.' indicates that the current object is expected to be
 * a json_type_object, and the text that follows is a key within the object.
 *
 * The appearance of '[' must be part of a '[<size_t>]' construction, and
 * indicates that the current object is expected to be a json_type_array, and
 * the specified integer value is an index into the array.
 *
 * The descriptor allows you to dive into the structure and return the endpoint
 * of the dive in the returned jval pointer, and returns the type of this
 * endpoint object.
 *
 * Note that this is a convenience method, primarily for testing. Results are
 * undefined if the '.' or '[' or ']' characters appear in a key.
 *
 * Note that the returned jval is a json_object. You can use the following
 * libjson functions to directly extract values:
 *
 * - json_object_get_boolean()
 * - json_object_get_int()
 * - json_object_get_int64()
 * - json_object_get_uint64()
 * - json_object_get_double()
 * - json_object_get_string()
 *
 * Note also that these functions are used in the variants below.
 *
 * All memory is managed by json, so on 'put' of the head object, all memory is
 * recovered.
 *
 * This returns json_type_null on any error.
 *
 * @param desc - string descriptor of endpoint argument
 * @param jobj - starting object
 * @param jval - final endpoint object, or NULL
 * @return enum json_type - type of the endpoint object
 */
enum json_type cxip_json_extract(const char *desc, struct json_object *jobj,
				 struct json_object **jval)
{
	const char *beg;
	struct json_object *jo;
	enum json_type jt;

	*jval = NULL;

	beg = desc;
	jo = jobj;
	jt = json_object_get_type(jo);
	while (*beg) {
		if (*beg == '[') {
			/* expect "[<integer>]" */
			size_t idx = 0;
			size_t len;

			if (jt != json_type_array)
				return json_type_null;
			/* skip '[' and ensure index is not empty */
			if (*(++beg) == ']')
				return json_type_null;
			idx = strtoul(beg, (char **)&beg, 10);
			/* ensure strtol consumed index */
			if (*(beg++) != ']')
				return json_type_null;
			/* check index validity */
			len = json_object_array_length(jo);
			if (idx >= len)
				return json_type_null;
			/* get the indexed object and continue */
			jo = json_object_array_get_idx(jo, idx);
			jt = json_object_get_type(jo);
			continue;
		}
		if (beg == desc || *beg == '.') {
			/* expect ".key" */
			char key[256], *p = key;
			size_t len = sizeof(key);

			if (jt != json_type_object)
				return json_type_null;
			/* skip leading '.' */
			if (*beg == '.')
				beg++;
			/* copy key from descriptor to local storage */
			while (*beg && *beg != '.' && *beg != '[' && --len > 0)
				*p++ = *beg++;
			*p = 0;
			/* extract the associated value */
			if (!json_object_object_get_ex(jo, key, &jo))
				return json_type_null;
			jt = json_object_get_type(jo);
			continue;
		}
	}

	/* return the final object */
	*jval = jo;
	return jt;
}

/**
 * @brief Simplified search for JSON terminal type values.
 *
 * @param desc : search descriptor for cxip_json_extract()
 * @param jobj : starting object
 * @param val  : return value
 * @return int : 0 on success, -EINVAL on error
 */
int cxip_json_bool(const char *desc, struct json_object *jobj, bool *val)
{
	struct json_object *jval;
	if (json_type_boolean != cxip_json_extract(desc, jobj, &jval))
		return -EINVAL;
	*val = json_object_get_boolean(jval);
	return 0;
}

int cxip_json_int(const char *desc, struct json_object *jobj, int *val)
{
	struct json_object *jval;
	if (json_type_int != cxip_json_extract(desc, jobj, &jval))
		return -EINVAL;
	*val = json_object_get_int(jval);
	return 0;
}

int cxip_json_int64(const char *desc, struct json_object *jobj, int64_t *val)
{
	struct json_object *jval;
	if (json_type_int != cxip_json_extract(desc, jobj, &jval))
		return -EINVAL;
	*val = json_object_get_int64(jval);
	return 0;
}

int cxip_json_double(const char *desc, struct json_object *jobj, double *val)
{
	struct json_object *jval;
	if (json_type_double != cxip_json_extract(desc, jobj, &jval))
		return -EINVAL;
	*val = json_object_get_double(jval);
	return 0;
}

int cxip_json_string(const char *desc, struct json_object *jobj,
		     const char **val)
{
	struct json_object *jval;
	if (json_type_string != cxip_json_extract(desc, jobj, &jval))
		return -EINVAL;
	*val = json_object_get_string(jval);
	return 0;
}
