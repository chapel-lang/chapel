/*
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 *
 * Copyright (c) 2021-2024 Hewlett Packard Enterprise Development LP
 */
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include <ofi.h>
#include "cxip.h"

#if ENABLE_CXI_CURL_DLOPEN
static void *cxip_curlhandle;
#endif
#if ENABLE_CXI_JSON_DLOPEN
static void *cxip_jsonhandle;
#endif
static CURLM *cxip_curlm;
static int cxip_curl_count;
static int cxip_json_dl_init(void);

#define	TRACE_CURL(fmt, ...)	CXIP_COLL_TRACE(CXIP_TRC_COLL_CURL, fmt, \
						##__VA_ARGS__)

#define CXIP_DBG(...) _CXIP_DBG(FI_LOG_FABRIC, __VA_ARGS__)
#define	CXIP_INFO(...) _CXIP_INFO(FI_LOG_FABRIC, __VA_ARGS__)
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
 * cxip_info.c), which is single-threaded. The (*dl_curl_global_init)() call can be
 * issued multiple times (non-concurrently) and has the same end result as
 * calling it once.
 */

/**
 * Initialize CURL globally for the application, enabling multi-curl
 * (concurrent calls).
 */

struct curldl_ops {
	CURLcode (*dl_curl_global_init)(long);
	void	 (*dl_curl_global_cleanup)(void);
	CURL *	 (*dl_curl_easy_init)(void);
	void	 (*dl_curl_easy_cleanup)(CURL *);
	CURLcode (*dl_curl_easy_getinfo)(CURL *, CURLINFO, ...);
	CURLcode (*dl_curl_easy_setopt)(CURL *, CURLoption, ...);
	const char *(*dl_curl_easy_strerror)(CURLcode);
	CURLcode (*dl_curl_easy_perform)(CURL *);
	CURLM *	 (*dl_curl_multi_init)(void);
	CURLMcode (*dl_curl_multi_cleanup)(CURLM *);
	CURLMcode (*dl_curl_multi_add_handle)(CURLM *multi_handle, CURL *);
	CURLMsg * (*dl_curl_multi_info_read)(CURLM *multi_handle, int *);
	CURLMcode (*dl_curl_multi_perform)(CURLM *multi_handle, int *);
	const char *(*dl_curl_multi_strerror)(CURLMcode);
	struct curl_slist *(*dl_curl_slist_append)(struct curl_slist *, const char *);
	void	  (*dl_curl_slist_free_all)(struct curl_slist *);
};
/* dynamic bind of symbols with dlopen()/dlsym() */
#if ENABLE_CXI_CURL_DLOPEN

#include <dlfcn.h>
static struct curldl_ops curldl_ops;

#else /* static bind of symbols */
static struct curldl_ops curldl_ops = {
	.dl_curl_global_init = curl_global_init,
	.dl_curl_global_cleanup = curl_global_cleanup,
	.dl_curl_easy_init = curl_easy_init,
	.dl_curl_easy_cleanup = curl_easy_cleanup,
	.dl_curl_easy_getinfo = curl_easy_getinfo,
	.dl_curl_easy_setopt = curl_easy_setopt,
	.dl_curl_easy_strerror = curl_easy_strerror,
	.dl_curl_easy_perform = curl_easy_perform,
	.dl_curl_multi_init = curl_multi_init,
	.dl_curl_multi_cleanup = curl_multi_cleanup,
	.dl_curl_multi_add_handle = curl_multi_add_handle,
	.dl_curl_multi_info_read = curl_multi_info_read,
	.dl_curl_multi_perform = curl_multi_perform,
	.dl_curl_multi_strerror = curl_multi_strerror,
	.dl_curl_slist_append = curl_slist_append,
	.dl_curl_slist_free_all = curl_slist_free_all,
};
#endif

static int cxip_curl_dl_init(void)
{
#if ENABLE_CXI_CURL_DLOPEN
	char *curl_libpath = NULL;
	char *lib_name = "libcurl.so";

	/* load successfully only once */
	if (cxip_curlhandle)
		return 0;

/*
 * Check for non standard search path
 */
#ifdef FI_CXI_CURL_LIB_PATH
        curl_libpath = strdup(FI_CXI_CURL_LIB_PATH);
        TRACE_CURL("FI_CXI_CURL_LIB_PATH set to %s\n", curl_libpath);
#else/* standard lookup of libcurl.so */
        curl_libpath = strdup(lib_name);
        TRACE_CURL("libcurl path set to %s\n", curl_libpath);
#endif

	cxip_curlhandle = dlopen(curl_libpath, RTLD_NOW);
	if (!cxip_curlhandle) {
		TRACE_CURL("Unable to dlopen libcurl - file path = %s\n", curl_libpath);
		free(curl_libpath);
		return -FI_ENOSYS;
	}
	curldl_ops.dl_curl_global_init = dlsym(cxip_curlhandle, "curl_global_init");
	if (!curldl_ops.dl_curl_global_init) {
		TRACE_CURL("Failed to find curl_global_init\n");
		goto err;
	}
	curldl_ops.dl_curl_global_cleanup = dlsym(cxip_curlhandle, "curl_global_cleanup");
	if (!curldl_ops.dl_curl_global_cleanup) {
		TRACE_CURL("Failed to find curl_global_cleanup\n");
		goto err;
	}
	curldl_ops.dl_curl_easy_init = dlsym(cxip_curlhandle, "curl_easy_init");
	if (!curldl_ops.dl_curl_easy_init) {
		TRACE_CURL("Failed to find curl_easy_init\n");
		goto err;
	}
	curldl_ops.dl_curl_easy_cleanup = dlsym(cxip_curlhandle, "curl_easy_cleanup");
	if (!curldl_ops.dl_curl_easy_cleanup) {
		TRACE_CURL("Failed to find curl_easy_cleanup\n");
		goto err;
	}
	curldl_ops.dl_curl_easy_getinfo = dlsym(cxip_curlhandle, "curl_easy_getinfo");
	if (!curldl_ops.dl_curl_easy_getinfo) {
		TRACE_CURL("Failed to find curl_easy_getinfo\n");
		goto err;
	}
	curldl_ops.dl_curl_easy_setopt = dlsym(cxip_curlhandle, "curl_easy_setopt");
	if (!curldl_ops.dl_curl_easy_setopt) {
		TRACE_CURL("Failed to find curl_easy_setopt\n");
		goto err;
	}
	curldl_ops.dl_curl_easy_strerror = dlsym(cxip_curlhandle, "curl_easy_strerror");
	if (!curldl_ops.dl_curl_easy_strerror) {
		TRACE_CURL("Failed to find curl_easy_strerror\n");
		goto err;
	}
	curldl_ops.dl_curl_easy_perform = dlsym(cxip_curlhandle, "curl_easy_perform");
	if (!curldl_ops.dl_curl_easy_init) {
		TRACE_CURL("Failed to find curl_easy_init\n");
		goto err;
	}
	curldl_ops.dl_curl_multi_init = dlsym(cxip_curlhandle, "curl_multi_init");
	if (!curldl_ops.dl_curl_multi_init) {
		TRACE_CURL("Failed to find curl_multi_init\n");
		goto err;
	}
	curldl_ops.dl_curl_multi_cleanup = dlsym(cxip_curlhandle, "curl_multi_cleanup");
	if (!curldl_ops.dl_curl_multi_cleanup) {
		TRACE_CURL("Failed to find curl_multi_cleanup\n");
		goto err;
	}
	curldl_ops.dl_curl_multi_add_handle = dlsym(cxip_curlhandle, "curl_multi_add_handle");
	if (!curldl_ops.dl_curl_multi_add_handle) {
		TRACE_CURL("Failed to find curl_multi_add_handle\n");
		goto err;
	}
	curldl_ops.dl_curl_multi_info_read = dlsym(cxip_curlhandle, "curl_multi_info_read");
	if (!curldl_ops.dl_curl_multi_info_read) {
		TRACE_CURL("Failed to find curl_multi_info_read\n");
		goto err;
	}
	curldl_ops.dl_curl_multi_perform = dlsym(cxip_curlhandle, "curl_multi_perform");
	if (!curldl_ops.dl_curl_multi_perform) {
		TRACE_CURL("Failed to find curl_multi_perform\n");
		goto err;
	}
	curldl_ops.dl_curl_multi_strerror = dlsym(cxip_curlhandle, "curl_multi_strerror");
	if (!curldl_ops.dl_curl_multi_strerror) {
		TRACE_CURL("Failed to find curl_multi_strerror\n");
		goto err;
	}
	curldl_ops.dl_curl_slist_append = dlsym(cxip_curlhandle, "curl_slist_append");
	if (!curldl_ops.dl_curl_slist_append) {
		TRACE_CURL("Failed to find curl_slist_append\n");
		goto err;
	}
	curldl_ops.dl_curl_slist_free_all= dlsym(cxip_curlhandle, "curl_slist_free_all");
	if (!curldl_ops.dl_curl_slist_free_all) {
		TRACE_CURL("Failed to find curl_slist_free_all\n");
		goto err;
	}
	free(curl_libpath);
	TRACE_CURL("DLOPEN SUCCESS\n");
	return FI_SUCCESS;
err:
	TRACE_CURL("DLOPEN FAILURE\n");
	dlclose(cxip_curlhandle);
	free(curl_libpath);
	return -FI_ENODATA;
#else
	TRACE_CURL("DLOPEN UNAVAILABLE\n");
	return FI_SUCCESS;
#endif
}

int cxip_curl_init(void)
{
	CURLcode res;
	int ret;

	ret = cxip_curl_dl_init();
	if (ret)
		return ret;

	ret = cxip_json_dl_init();
	if (ret)
		return ret;

	if(!cxip_curlm) {
		res = curldl_ops.dl_curl_global_init(CURL_GLOBAL_DEFAULT);
		if (res == CURLE_OK) {
			cxip_curlm = curldl_ops.dl_curl_multi_init();
			if (!cxip_curlm) {
				curldl_ops.dl_curl_global_cleanup();
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
		curldl_ops.dl_curl_multi_cleanup(cxip_curlm);
		curldl_ops.dl_curl_global_cleanup();
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
 * user information to identify which of multiple concurrent requests has
 * completed.
 *
 * An error return indicates that the dispatch was unsuccessful. All memory
 * cleanup is done here.
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
 * @return int          : 0 on success, -errno on failure
 * -FI_ENOMEM		: out-of-memory
 * -FI_ECONNREFUSED	: CURL easy/multi init failed
 */
int cxip_curl_perform(const char *endpoint, const char *request,
		      const char *sessionToken, size_t rsp_init_size,
		      enum curl_ops op, bool verbose,
		      curlcomplete_t usrfunc, void *usrptr)
{
	struct cxip_curl_handle *handle;
	struct curl_slist *headers;
	char *token;
	char *cert_env_var;
	bool verify = true;
	bool isdir = false;
	bool isfile = false;
	struct stat buf;
	CURLMcode mres;
	CURL *curl;
	int running;
	int ret;

	handle = calloc(1, sizeof(*handle));
	if (!handle) {
		ret = -FI_ENOMEM;
		goto fail;
	}

	/* libcurl is fussy about NULL requests */
	handle->endpoint = strdup(endpoint);
	if (!handle->endpoint) {
		ret = -FI_ENOMEM;
		goto fail;
	}
	handle->request = strdup(request ? request : "");
	if (!handle->request) {
		ret = -FI_ENOMEM;
		goto fail;
	}
	handle->response = NULL;
	handle->recv = (void *)init_curl_buffer(rsp_init_size);
	if (!handle->recv) {
		ret = -FI_ENOMEM;
		goto fail;
	}

	/* add user completion function and pointer */
	handle->usrfunc = usrfunc;
	handle->usrptr = usrptr;

	curl = curldl_ops.dl_curl_easy_init();
	if (!curl) {
		CXIP_WARN("curldl_ops.dl_curl_easy_init() failed\n");
		ret = -FI_ECONNREFUSED;
		goto fail;
	}
	/* HTTP 1.1 assumed */
	headers = NULL;
	headers = curldl_ops.dl_curl_slist_append(headers, "Expect:");
	headers = curldl_ops.dl_curl_slist_append(headers, "Accept: application/json");
	headers = curldl_ops.dl_curl_slist_append(headers, "Content-Type: application/json");
	headers = curldl_ops.dl_curl_slist_append(headers, "charset: utf-8");
	token = NULL;
	if (sessionToken) {
		ret = asprintf(&token, "Authorization: Bearer %s",
			       sessionToken);
		if (ret < 0) {
			CXIP_WARN("token string create failed\n");
			ret = -FI_ENOMEM;
			goto fail;
		}
		headers = curldl_ops.dl_curl_slist_append(headers, token);
	}
	handle->headers = (void *)headers;

	curldl_ops.dl_curl_easy_setopt(curl, CURLOPT_URL, handle->endpoint);
	if (op == CURL_GET) {
		curldl_ops.dl_curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
	} else if (op == CURL_DELETE) {
		curldl_ops.dl_curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
	} else {
		curldl_ops.dl_curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curldl_ops.dl_curl_easy_setopt(curl, CURLOPT_POSTFIELDS, handle->request);
		curldl_ops.dl_curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE,
				 strlen(handle->request));
	}
	curldl_ops.dl_curl_easy_setopt(curl, CURLOPT_STDERR, stderr);
	curldl_ops.dl_curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curldl_ops.dl_curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curldl_ops.dl_curl_easy_setopt(curl, CURLOPT_WRITEDATA, handle->recv);
	curldl_ops.dl_curl_easy_setopt(curl, CURLOPT_PRIVATE, (void *)handle);
	curldl_ops.dl_curl_easy_setopt(curl, CURLOPT_VERBOSE, (long)verbose);
	curldl_ops.dl_curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, cxip_curl_opname(op));
	/* allow 5s for connection + operation in case of slow DNS */
	curldl_ops.dl_curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);

	/* Value of fm_cacert variable in slurmctld configuration.
	 * If set to 'yes' or a path, the CACERT will be validated
	 * and used for the connection
	 */
	cert_env_var = getenv("FI_CXI_COLL_FABRIC_MGR_CACERT");

	if (!cert_env_var || !strcmp(cert_env_var, "no"))
		verify = false;
	else if (!strcmp(cert_env_var, "yes"))
		verify = true;
	else {
		if (stat(cert_env_var, &buf) == -1) {
			ret = FI_ENOENT;
			goto fail;
		}
		if (S_ISDIR(buf.st_mode))
			isdir = true;
		else if (S_ISREG(buf.st_mode))
			isfile = true;
		else {
			ret = FI_EINVAL;
			goto fail;
		}
	}

	if (!verify) {
		/* These are needed to work with self-signed certificates */
		curldl_ops.dl_curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curldl_ops.dl_curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	} else {
		/* FI_CXI_COLL_FABRIC_MGR_CACERT is "yes" or a pathname */
		curldl_ops.dl_curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
		curldl_ops.dl_curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
	}

	/* If certificate file/dir specified, use it */
	if (isdir)
		curldl_ops.dl_curl_easy_setopt(curl, CURLOPT_CAPATH, cert_env_var);
	else if (isfile)
		curldl_ops.dl_curl_easy_setopt(curl, CURLOPT_CAINFO, cert_env_var);

	curldl_ops.dl_curl_multi_add_handle(cxip_curlm, curl);
	mres = curldl_ops.dl_curl_multi_perform(cxip_curlm, &running);
	if (mres != CURLM_OK) {
		TRACE_CURL("curldl_ops.dl_curl_multi_perform)() failed: %s\n",
			   curldl_ops.dl_curl_multi_strerror(mres));
		CXIP_WARN("curldl_ops.dl_curl_multi_perform)() failed: %s\n",
			  curldl_ops.dl_curl_multi_strerror(mres));
		ret = -FI_ECONNREFUSED;
		goto fail;
	}
	cxip_curl_count += 1;
	return FI_SUCCESS;

fail:
	TRACE_CURL("%s failed %d (%s)\n", __func__, ret, fi_strerror(ret));
	CXIP_WARN("%s failed %d (%s)\n", __func__, ret, fi_strerror(ret));
	cxip_curl_free(handle);
	return ret;
}

/**
 * Progress the pending CURL requests.
 *
 * This progresses concurrent CURL requests, and returns the following:
 *
 * -  0			success
 * -  -FI_EAGAIN 	indicates operations are pending, none completed
 * -  -FI_ENODATA	indicates no operations are pending
 * -  -FI_ECONNREFUSED  fatal error, CURL is not functioning properly
 *
 * Note that -FI_ECONNREFUSED should be treated as a fatal CURL error. It
 * indicates that CURL is behaving in an abnormal fashion, and cannot be
 * relied upon. In normal use, it should not happen.
 *
 * All other error handling is performed by the usrfunc function (supplied
 * during cxip_curl_perform() call), see below.
 *
 * A CURL request will complete if the server is not reachable. It will return a
 * handle->status value of 0, which is an invalid HTTP status, and indicates
 * that it could not connect to a server.
 *
 * In normal use, handleptr is NULL. the caller has passed a a usrfunc callback
 * routine when dispatching the CURL request to process the returned errors and
 * data: see cxip_curl_perform(). This usrfunc callback is called after
 * completion of the request, before the handle is destroyed, and is expected to
 * know enough about CURL operations to interpret the results. This routine will
 * delete the handle after the callback has processed it.
 *
 * For unit testing, it can be useful for the test to be able to inspect the
 * handle and the error return, and it can be obtained by specifying a non-null
 * handleptr. If handleptr is supplied, the caller is responsible for
 * calling cxip_curl_free() on the returned handle.
 *
 * The callback routine has read-only access to the handle, and read-write
 * access to its own data area, available as handle->usrptr.
 *
 * The handle contains the following documented fields:
 *
 * - status   = HTTP status of the op, or 0 if the endpoint could not be reached
 * - endpoint = copy of the endpoint address (URL) supplied for the post
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

	/* running returns the number of curls running */
	mres = curldl_ops.dl_curl_multi_perform(cxip_curlm, &running);
	if (mres != CURLM_OK) {
		CXIP_WARN("curldl_ops.dl_curl_multi_perform() failed: %s\n",
			  curldl_ops.dl_curl_multi_strerror(mres));
		return -FI_ECONNREFUSED;
	}

	/* messages returns the number of additional curls finished */
	msg = curldl_ops.dl_curl_multi_info_read(cxip_curlm, &messages);
	if (!msg || msg->msg != CURLMSG_DONE) {
		return (running) ? -FI_EAGAIN : -FI_ENODATA;
	}

	/* These should not occur, but if (*dl_curl_easy_getinfo)() succeeds, we
	 * don't really care. Just post a warning.
	 */
	if (msg->data.result >= CURL_LAST) {
		CXIP_WARN("CURL unknown result %d\n", msg->data.result);
	} else if (msg->data.result > CURLE_OK) {
		CXIP_WARN("CURL error '%s'\n",
			  curldl_ops.dl_curl_easy_strerror(msg->data.result));
	}

	/* retrieve our handle from the private pointer */
	handle = NULL;
	res = curldl_ops.dl_curl_easy_getinfo(msg->easy_handle,
				CURLINFO_PRIVATE, (char **)&handle);
	if (res != CURLE_OK) {
		TRACE_CURL("curldl_ops.dl_curl_easy_getinfo(%s) failed: %s\n",
			   "CURLINFO_PRIVATE",curldl_ops.dl_curl_easy_strerror(res));
		CXIP_WARN("curldl_ops.dl_curl_easy_getinfo(%s) failed: %s\n",
			  "CURLINFO_PRIVATE",curldl_ops.dl_curl_easy_strerror(res));
		return -FI_ECONNREFUSED;
	}
	/* handle is now valid, must eventually be freed */
	/* retrieve the status code, should not fail */
	res = curldl_ops.dl_curl_easy_getinfo(msg->easy_handle,
				CURLINFO_RESPONSE_CODE, &status);
	if (res != CURLE_OK) {
		TRACE_CURL("curldl.dl_curl_easy_getinfo(%s) failed: %s\n",
			   "CURLINFO_RESPONSE_CODE",curldl_ops.dl_curl_easy_strerror(res));
		CXIP_WARN("curldl_ops.dl_curl_easy_getinfo(%s) failed: %s\n",
			  "CURLINFO_RESPONSE_CODE",curldl_ops.dl_curl_easy_strerror(res));
		/* continue, handle->status should show zero */
	}
	TRACE_CURL("curldl_ops.dl_curl_easy_getinfo)() success\n");

	/* we can recover resources now */
	curldl_ops.dl_curl_slist_free_all((struct curl_slist *)handle->headers);
	curldl_ops.dl_curl_easy_cleanup(msg->easy_handle);
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

struct jsondl_ops {
        enum json_type (*dl_json_object_get_type)(const struct json_object *);
        size_t (*dl_json_object_array_length)(const struct json_object *);
        struct json_object * (*dl_json_object_array_get_idx)(const struct json_object *, size_t);
        json_bool (*dl_json_object_object_get_ex)(const struct json_object *, const char *, struct json_object **);
        json_bool (*dl_json_object_get_boolean)(const struct json_object *);
        int32_t (*dl_json_object_get_int)(const struct json_object *);
        int64_t (*dl_json_object_get_int64)(const struct json_object *);
        double  (*dl_json_object_get_double)(const struct json_object *);
        const char * (*dl_json_object_get_string)(struct json_object *);
	struct json_object * (*dl_json_tokener_parse)(const char *);
	int (*dl_json_object_put)(struct json_object *);
};

/* dynamic bind of symbols with dlopen()/dlsym() */
#if ENABLE_CXI_JSON_DLOPEN

#include <dlfcn.h>
static struct jsondl_ops jsondl_ops;

#else /* static bind of symbols */
static struct jsondl_ops jsondl_ops = {
        .dl_json_object_get_type = json_object_get_type,
        .dl_json_object_array_length = json_object_array_length,
        .dl_json_object_array_get_idx = json_object_array_get_idx,
        .dl_json_object_object_get_ex = json_object_object_get_ex,
        .dl_json_object_get_boolean = json_object_get_boolean,
        .dl_json_object_get_int = json_object_get_int,
        .dl_json_object_get_int64 = json_object_get_int64,
        .dl_json_object_get_double = json_object_get_double,
        .dl_json_object_get_string = json_object_get_string,
	.dl_json_tokener_parse = json_tokener_parse,
	.dl_json_object_put = json_object_put,
};
#endif

static int cxip_json_dl_init(void)
{
#if ENABLE_CXI_JSON_DLOPEN
	char *json_libpath = NULL;
	char *lib_name = "libjson-c.so";

	/* load successfully only once */
	if (cxip_jsonhandle)
		return FI_SUCCESS;
/*
 * Check for non standard search path
 */
#ifdef FI_CXI_JSON_LIB_PATH
        json_libpath = strdup(FI_CXI_JSON_LIB_PATH);
        TRACE_CURL("FI_CXI_JSON_LIB_PATH set to %s\n", json_libpath);
#else/* standard lookup of libjson-c.so */
        json_libpath = strdup(lib_name);
        TRACE_CURL("libjson-c path set to %s\n", json_libpath);
#endif
	cxip_jsonhandle = dlopen(json_libpath, RTLD_NOW);
	if (!cxip_jsonhandle) {
		TRACE_CURL("Unable to dlopen libjson - file path = %s\n", json_libpath);
		free(json_libpath);
		return -FI_ENOSYS;
	}
	jsondl_ops.dl_json_object_get_type = dlsym(cxip_jsonhandle, "json_object_get_type");
	if (!jsondl_ops.dl_json_object_get_type) {
		TRACE_CURL("Failed to find json_object_get_type\n");
		goto err;
	}
	jsondl_ops.dl_json_object_array_length = dlsym(cxip_jsonhandle, "json_object_array_length");
	if (!jsondl_ops.dl_json_object_array_length) {
		TRACE_CURL("Failed to find json_object_array_length\n");
		goto err;
	}
	jsondl_ops.dl_json_object_array_get_idx = dlsym(cxip_jsonhandle, "json_object_array_get_idx");
	if (!jsondl_ops.dl_json_object_array_get_idx) {
		TRACE_CURL("Failed to find json_object_array_get_idx\n");
		goto err;
	}
	jsondl_ops.dl_json_object_object_get_ex = dlsym(cxip_jsonhandle, "json_object_object_get_ex");
	if (!jsondl_ops.dl_json_object_object_get_ex) {
		TRACE_CURL("Failed to find json_object_object_get_ex\n");
		goto err;
	}
	jsondl_ops.dl_json_object_get_boolean = dlsym(cxip_jsonhandle, "json_object_get_boolean");
	if (!jsondl_ops.dl_json_object_get_boolean) {
		TRACE_CURL("Failed to find json_object_get_boolean\n");
		goto err;
	}
	jsondl_ops.dl_json_object_get_int = dlsym(cxip_jsonhandle, "json_object_get_int");
	if (!jsondl_ops.dl_json_object_get_int) {
		TRACE_CURL("Failed to find json_object_get_int\n");
		goto err;
	}
	jsondl_ops.dl_json_object_get_int64 = dlsym(cxip_jsonhandle, "json_object_get_int64");
	if (!jsondl_ops.dl_json_object_get_int64) {
		TRACE_CURL("Failed to find json_object_get_int64\n");
		goto err;
	}
	jsondl_ops.dl_json_object_get_double = dlsym(cxip_jsonhandle, "json_object_get_double");
	if (!jsondl_ops.dl_json_object_get_double) {
		TRACE_CURL("Failed to find json_object_get_double\n");
		goto err;
	}
	jsondl_ops.dl_json_object_get_string = dlsym(cxip_jsonhandle, "json_object_get_string");
	if (!jsondl_ops.dl_json_object_get_string) {
		TRACE_CURL("Failed to find json_object_get_string\n");
		goto err;
	}
	jsondl_ops.dl_json_tokener_parse = dlsym(cxip_jsonhandle, "json_tokener_parse");
	if (!jsondl_ops.dl_json_tokener_parse) {
		TRACE_CURL("Failed to find json_tokener_parse\n");
		goto err;
	}
	jsondl_ops.dl_json_object_put = dlsym(cxip_jsonhandle, "json_object_put");
	if (!jsondl_ops.dl_json_object_put) {
		TRACE_CURL("Failed to find json_object_put\n");
		goto err;
	}

	TRACE_CURL("JSON DLOPEN SUCCESS\n");
	free(json_libpath);
	return FI_SUCCESS;
err:
	TRACE_CURL("JSON DLOPEN FAILURE\n");
	free(json_libpath);
	dlclose(cxip_jsonhandle);
	return -FI_ENODATA;
#else
	TRACE_CURL("JSON DLOPEN UNAVAILABLE\n");
	return FI_SUCCESS;
#endif
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
	jt = jsondl_ops.dl_json_object_get_type(jo);
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
			len = jsondl_ops.dl_json_object_array_length(jo);
			if (idx >= len)
				return json_type_null;
			/* get the indexed object and continue */
			jo = jsondl_ops.dl_json_object_array_get_idx(jo, idx);
			jt = jsondl_ops.dl_json_object_get_type(jo);
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
			if (!jsondl_ops.dl_json_object_object_get_ex(jo, key, &jo))
				return json_type_null;
			jt = jsondl_ops.dl_json_object_get_type(jo);
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
	*val = jsondl_ops.dl_json_object_get_boolean(jval);
	return 0;
}

int cxip_json_int(const char *desc, struct json_object *jobj, int *val)
{
	struct json_object *jval;
	if (json_type_int != cxip_json_extract(desc, jobj, &jval))
		return -EINVAL;
	*val = jsondl_ops.dl_json_object_get_int(jval);
	return 0;
}

int cxip_json_int64(const char *desc, struct json_object *jobj, int64_t *val)
{
	struct json_object *jval;
	if (json_type_int != cxip_json_extract(desc, jobj, &jval))
		return -EINVAL;
	*val = jsondl_ops.dl_json_object_get_int64(jval);
	return 0;
}

int cxip_json_double(const char *desc, struct json_object *jobj, double *val)
{
	struct json_object *jval;
	if (json_type_double != cxip_json_extract(desc, jobj, &jval))
		return -EINVAL;
	*val = jsondl_ops.dl_json_object_get_double(jval);
	return 0;
}

int cxip_json_string(const char *desc, struct json_object *jobj,
		     const char **val)
{
	struct json_object *jval;
	if (json_type_string != cxip_json_extract(desc, jobj, &jval))
		return -EINVAL;
	*val = jsondl_ops.dl_json_object_get_string(jval);
	return 0;
}

struct json_object *cxip_json_tokener_parse(const char *str)
{
	return jsondl_ops.dl_json_tokener_parse(str);
}

int cxip_json_object_put(struct json_object *obj)
{
	return jsondl_ops.dl_json_object_put(obj);
}
