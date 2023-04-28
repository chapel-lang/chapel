/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2017 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  Contact Information:
  Intel Corporation, www.intel.com

  BSD LICENSE

  Copyright(c) 2017 Intel Corporation.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef PSM2_H
#define PSM2_H

#include <stdint.h>
#include <stddef.h>
#include <uuid/uuid.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @file psm2.h
 * @page psm2_main PSM2 API
 *
 * @brief PSM2 OPA Messaging Library
 *
 * The PSM2 OPA Messaging API, or PSM2 API, is Intel's low-level
 * user-level communications interface for the OPA family of products.
 * PSM2 users are enabled with mechanisms necessary to implement higher level
 * communications interfaces in parallel environments.
 *
 * Since PSM2 targets clusters of multicore processors, it internally implements
 * two levels of communication: intra-node shared memory communication and
 * inter-node OPA communication.  Both of these levels are encapsulated
 * below the interface and the user is free to assume that intra-node and
 * inter-node communication is transparently handled within PSM.
 *
 * @section compat Compatibility
 *
 * PSM2 can coexist with other QLogic/Pathscale software distributions, such as
 * OpenIB/OpenFabrics, which allows applications to simultaneously target
 * PSM-based and non PSM-based applications on a single node without changing
 * any system-level configuration.  However, PSM2 does not support running
 * PSM-based and non PSM-based communication within the same user process.
 *
 * Except where noted, PSM2 does not assume an SPMD (single program, multiple
 * data) parallel model and extends to MPMD (multiple program, multiple data)
 * environments in specific areas. However, PSM2 assumes the runtime environment
 * to be homogeneous on all nodes in bit width (32-bit or 64-bit) and endianness
 * (little or big) and will fail at startup if any of these assumptions do not
 * hold.  For homogeneous systems PSM2 can run either in 32-bit or 64-bit
 * environments.  Even though both environments should expect similar
 * performance from the API, PSM2 has chosen to favor 64-bit environments in
 * some minor areas.
 *
 * @section ep_model Endpoint Communication Model
 *
 * PSM2 follows an endpoint communication model where an endpoint is defined as
 * an object (or handle) instantiated to support sending and receiving messages
 * to other endpoints.  In order to prevent PSM2 from being tied to a particular
 * parallel model (such as SPMD), control over the parallel layout of endpoints
 * is retained by the user.  Opening endpoints (@ref psm3_ep_open) and
 * connecting endpoints to enable communication (@ref psm3_ep_connect) are two
 * decoupled mechanisms.  Users that do not dynamically change the number of
 * endpoints beyond parallel startup will probably lump both mechanisms
 * together at startup.  Users that wish to manipulate the location and number
 * of endpoints at runtime can do so by explicitly connecting sets or subsets
 * of endpoints.
 *
 * As a side effect, this greater flexibility forces the user to cope with a
 * two-stage initialization process.  In the first stage of opening an endpoint
 * (@ref psm3_ep_open), a user obtains an opaque handle to the endpoint and a
 * globally distributable endpoint identifier (@ref psm2_epid_t).  Prior to the
 * second stage of connecting endpoints (@ref psm3_ep_connect), a user must
 * distribute all relevent endpoint identifiers through an out-of-band
 * mechanism.  Once the endpoint identifiers are successfully distributed to
 * all processes that wish to communicate, the user
 * connects all endpoint identifiers to the locally opened endpoint
 * (@ref psm3_ep_connect).  In connecting the endpoints, the user obtains an
 * opaque endpoint address (@ref psm2_epaddr_t), which is required for all PSM
 * communication primitives.
 *
 *
 * @section components PSM2 Components
 *
 * PSM2 exposes a single endpoint initialization model, but enables various
 * levels of communication functionality and semantics through @e components.
 * The first major component available in PSM2 is PSM2 Matched Queues
 * (@ref psm2_mq), and the second is PSM2 Active Message (@ref psm2_am).
 *
 * Matched Queues (MQ) present a queue-based communication model with the
 * distinction that queue consumers use a 3-tuple of metadata to match incoming
 * messages against a list of preposted receive buffers.  The MQ semantics are
 * sufficiently akin to MPI to cover the entire MPI-1.2 standard.
 *
 * The Active Message (AM) component presents a request/reply model where
 * the arrival of a message triggers the execution of consumer-provided
 * handler code. This can be used to implement many one-sided and two-sided
 * communications paradigms.
 *
 * With future releases of the PSM2 interface, more components will
 * be exposed to accommodate users that implement parallel communication
 * models that deviate from the Matched Queue semantics.  For example, PSM
 * plans to expose a connection management component to make it easier to
 * handle endpoint management for clients without their own connection
 * managers.
 *
 *
 * @section progress PSM2 Communication Progress Guarantees
 *
 * PSM2 internally ensures progress of both intra-node and inter-node messages,
 * but not autonomously.  This means that while performance does not depend
 * greatly on how the user decides to schedule communication progress,
 * explicit progress calls are required for correctness.  The @ref psm2_poll
 * function is available to make progress over all PSM2 components in a generic
 * manner.  For more information on making progress over many communication
 * operations in the MQ component, see the @ref mq_progress documentation.
 *
 *
 * @section completion PSM2 Completion semantics
 *
 * PSM2 implements the MQ component, which documents its own
 * message completion semantics (@ref mq_completion).
 *
 *
 * @section error_handling PSM2 Error handling
 *
 * PSM2 exposes a list of user and runtime errors enumerated in @ref psm2_error.
 * While most errors are fatal in that the user is not expected to be able to
 * recover from them, PSM2 still allows some level of control.  By
 * default, PSM2 returns all errors to the user but as a convenience, allows
 * users to either defer errors internally to PSM2 or to have PSM2 return all
 * errors to the user (callers to PSM2 functions).  PSM2 attempts to deallocate
 * its resources as a best effort, but exits are always non-collective with
 * respect to endpoints opened in other processes.  The user is expected to be
 * able to handle non-collective exits from any endpoint and in turn cleanly
 * and independently terminate the parallel environment.  Local error handling
 * can be handled in three modes:
 *
 * Errors and error handling can be individually registered either globally or
 * per-endpoint:
 * @li @b Per-endpoint error handling captures errors for functions where the
 * error scoping is determined to be over an endpoint.  This includes all
 * communication functions that include an EP or MQ handle as the first
 * parameter.
 *
 * @li @b Global error handling captures errors for functions where a
 * particular endpoint cannot be identified or for @ref psm3_ep_open, where
 * errors (if any) occur before the endpoint is opened.
 *
 * Error handling is controlled by registering error handlers (@ref
 * psm3_error_register_handler).  The global error handler can
 * be set at any time (even before @ref psm3_init), whereas a per-endpoint error
 * handler can be set as soon as a new endpoint is successfully created.  If a
 * per-endpoint handle is not registered, the per-endpoint handler inherits
 * from the global error handler at time of open.
 *
 * PSM2 predefines two different mechanisms for handling errors:
 *
 * @li PSM-internal error handler (@ref PSM2_ERRHANDLER_PSM_HANDLER)
 * @li No-op PSM2 error handler where errors are returned
 *     (@ref PSM2_ERRHANDLER_NO_HANDLER)
 *
 * The default PSM-internal error handler effectively frees the user from
 * explicitly handling the return values of ever PSM2 function but may not
 * return to the user in a function determined to have caused a fatal error.
 *
 * The No-op PSM2 error handler bypasses all error handling functionality and
 * always returns the error to the user.  The user can then use @ref
 * psm3_error_get_string to obtain a generic string from an error code (compared
 * to a more detailed error message available through registering of error
 * handlers).
 *
 * For even more control, users can register their own error handlers to have
 * access to more precise error strings and selectively control when an when
 * not to return to callers of PSM2 functions.  All error handlers shown defer
 * error handling to PSM2 for errors that are not recognized using @ref
 * psm2_error_defer.  Deferring an error from a custom error handler is
 * equivalent to relying on the default error handler.
 *
 * @section env_var Environment variables
 *
 * Some PSM2 behaviour can be controlled via environment variables.
 *
 * @li @b PSM3_DEVICES. PSM2 implements three devices for communication which
 * are, in order,  @c self, @c shm and @c hfi.  For PSM2 jobs that do not
 * require shared-memory communications, @b PSM3_DEVICES can be specified as @c
 * self, @c hfi.  Similarly, for shared-memory only jobs, the @c hfi device
 * can be disabled.  It is up to the user to ensure that the endpoint ids
 * passed in @ref psm3_ep_connect do not require a device that has been
 * explicitly disabled by the user.  In some instances, enabling only the
 * devices that are required may improve performance.
 *
 * @li @b PSM2_TRACEMASK. Depending on the value of the tracemask, various parts
 * of PSM2 will output debugging information.  With a default value of @c 0x1,
 * informative messages will be printed (this value should be considered a
 * minimum).  At @c 0x101, startup and finalization messages are added to the
 * output.  At @c 0x1c3, every communication event is logged and should hence
 * be used for extreme debugging only.
 *
 * @li @b PSM3_MULTI_EP. By default, only one PSM2 endpoint may be opened in
 * a process. With the correct setting of this environment variable, a process
 * may open more than one PSM2 endpoint. In order to enable multiple endpoint
 * per process support, the value of this environment variable should be set
 * to "1" or "yes".
 *
 * @section thr_sfty Thread safety and reentrancy
 * Unless specifically noted otherwise, all PSM2 functions should not be considered
 * to be thread safe or reentrant.
 */

/** @brief Local endpoint handle (opaque)
 *  @ingroup ep
 *
 * Handle returned to the user when a new local endpoint is created.  The
 * handle is a local handle to be used in all communication functions and is
 * not intended to globally identify the opened endpoint in any way.
 *
 * All open endpoint handles can be globally identified using the endpoint id
 * integral type (@ref psm2_epid_t) and all communication must use an endpoint
 * address (@ref psm2_epaddr_t) that can be obtained by connecting a local
 * endpoint to one or more endpoint identifiers.
 *
 * @remark The local endpoint handle is opaque to the user.  */
typedef struct psm2_ep *psm2_ep_t;

/** @brief MQ handle (opaque)
 * @ingroup mq
 *
 * Handle returned to the user when a new Matched queue is created (@ref
 * psm3_mq_init).  */
typedef struct psm2_mq *psm2_mq_t;

/*! @defgroup init PSM2 Initialization and Maintenance
 * @{
 */
#define PSM2_VERNO       0x0300	/*!< Header-defined Version number */
#define PSM2_VERNO_MAJOR 0x03	/*!< Header-defined Major Version Number */
#define PSM2_VERNO_MINOR 0x00	/*!< Header-defined Minor Version Number */
#define PSM2_VERNO_COMPAT_MAJOR 0x02    /*!<Minimum Major Version Number for Compatibility */

/*! @brief PSM2 Error type
 */
enum psm2_error {
	/*! Interface-wide "ok", guaranteed to be 0. */
	PSM2_OK = 0,
	/*! No events progressed on @ref psm3_poll (not fatal) */
	PSM2_OK_NO_PROGRESS = 1,
	/*! Error in a function parameter */
	PSM2_PARAM_ERR = 3,
	/*! PSM2 ran out of memory */
	PSM2_NO_MEMORY = 4,
	/*! PSM2 has not been initialized by @ref psm3_init */
	PSM2_INIT_NOT_INIT = 5,
	/*! API version passed in @ref psm3_init is incompatible */
	PSM2_INIT_BAD_API_VERSION = 6,
	/*! PSM2 Could not set affinity */
	PSM2_NO_AFFINITY = 7,
	/*! PSM2 Unresolved internal error */
	PSM2_INTERNAL_ERR = 8,
	/*! PSM2 could not set up shared memory segment */
	PSM2_SHMEM_SEGMENT_ERR = 9,
	/*! PSM2 option is a read-only option */
	PSM2_OPT_READONLY = 10,
	/*! PSM2 operation timed out */
	PSM2_TIMEOUT = 11,
	/*! Too many endpoints */
	PSM2_TOO_MANY_ENDPOINTS = 12,

	/*! PSM2 is finalized */
	PSM2_IS_FINALIZED = 13,

	/*! TCP data send is successful */
	PSM2_TCP_DATA_SENT = 14,

	/*! Endpoint was closed */
	PSM2_EP_WAS_CLOSED = 20,
	/*! PSM2 Could not find an OPA Unit */
	PSM2_EP_NO_DEVICE = 21,
	/*! User passed a bad unit or port number or address index */
	PSM2_EP_UNIT_NOT_FOUND = 22,
	/*! Failure in initializing endpoint */
	PSM2_EP_DEVICE_FAILURE = 23,
	/*! Error closing the endpoing error */
	PSM2_EP_CLOSE_TIMEOUT = 24,
	/*! No free ports could be obtained */
	PSM2_EP_NO_PORTS_AVAIL = 25,
	/*! Could not detect network connectivity */
	PSM2_EP_NO_NETWORK = 26,
	/*! Invalid Unique job-wide UUID Key */
	PSM2_EP_INVALID_UUID_KEY = 27,
	/*! Internal out of resources */
	PSM2_EP_NO_RESOURCES = 28,

	/*! Endpoint connect status unknown (because of other failures or if
	 * connect attempt timed out) */
	PSM2_EPID_UNKNOWN = 40,
	/*! Endpoint could not be reached by any PSM2 component */
	PSM2_EPID_UNREACHABLE = 41,
	/*! At least one of the connecting nodes was incompatible in endianess */
	PSM2_EPID_INVALID_NODE = 43,
	/*! At least one of the connecting nodes provided an invalid MTU */
	PSM2_EPID_INVALID_MTU = 44,
	/*! At least one of the connecting nodes provided a bad key */
	PSM2_EPID_INVALID_UUID_KEY = 45,
	/*! At least one of the connecting nodes is running an incompatible
	 * PSM2 protocol version */
	PSM2_EPID_INVALID_VERSION = 46,
	/*! At least one node provided garbled information */
	PSM2_EPID_INVALID_CONNECT = 47,
	/*! EPID was already connected */
	PSM2_EPID_ALREADY_CONNECTED = 48,
	/*! EPID is duplicated, network connectivity problem */
	PSM2_EPID_NETWORK_ERROR = 49,
	/*! EPID incompatible partition keys */
	PSM2_EPID_INVALID_PKEY = 50,
	/*! Unable to resolve path for endpoint */
	PSM2_EPID_PATH_RESOLUTION = 51,
	/*! Unable to connect rv QP */
	PSM2_EPID_RV_CONNECT_ERROR = 52,
	/*! Recovering rv QP conection */
	PSM2_EPID_RV_CONNECT_RECOVERING = 53,

	/*! MQ Non-blocking request is incomplete */
	PSM2_MQ_NO_COMPLETIONS = 60,
	/*! MQ Message has been truncated at the receiver */
	PSM2_MQ_TRUNCATION = 61,

	/*! AM reply error */
	PSM2_AM_INVALID_REPLY = 70,

	/*! Info query invalid query error */
	PSM2_IQ_INVALID_QUERY = 71,

    /*! Reserved Value to indicate highest ENUM value */
    PSM2_ERROR_LAST = 80
};

/*! Backwards header compatibility for a confusing error return name */
#define PSM2_MQ_INCOMPLETE PSM2_MQ_NO_COMPLETIONS

/*! @see psm2_error */
typedef enum psm2_error psm2_error_t;

/*! @brief PSM2 Error type
 */
enum psm2_component {
	/*! PSM2 core library */
	PSM2_COMPONENT_CORE = 0,
	/*! MQ component */
	PSM2_COMPONENT_MQ = 1,
	/*! AM component */
	PSM2_COMPONENT_AM = 2,
	/*! IB component */
	PSM2_COMPONENT_IB = 3
};

/*! @see psm2_component */
typedef enum psm2_component psm2_component_t;

/*! @brief PSM2 Path resolution mechanism
 */
enum psm2_path_res {
	/*! PSM2 no path resolution */
	PSM2_PATH_RES_NONE = 0,
	/*! Use OFED Plus for path resolution */
	PSM2_PATH_RES_OPP = 1,
	/*! Use OFED UMAD for path resolution */
	PSM2_PATH_RES_UMAD = 2
};

/*! @see psm2_path_resolution */
typedef enum psm2_path_res psm2_path_res_t;

/** @brief Initialize PSM2 interface
 *
 * Call to initialize the PSM2 library for a desired API revision number.
 *
 * @param[in,out] api_verno_major As input a pointer to an integer that holds
 *                                @ref PSM2_VERNO_MAJOR. As output, the pointer
 *                                is updated with the major revision number of
 *                                the loaded library.
 * @param[in,out] api_verno_minor As input, a pointer to an integer that holds
 *                                @ref PSM2_VERNO_MINOR.  As output, the pointer
 *                                is updated with the minor revision number of
 *                                the loaded library.
 *
 * @pre The user has not called any other PSM2 library call except @ref
 *      psm3_error_register_handler to register a global error handler.
 *
 * @post Depending on the environment variable @ref PSM3_MULTI_EP being set and
 * 	 its contents, support for opening multiple endpoints is either enabled
 * 	 or disabled.
 *
 * @warning PSM2 initialization is a precondition for all functions used in the
 *          PSM2 library.
 *
 * @returns PSM2_OK The PSM2 interface could be opened and the desired API
 *                 revision can be provided.
 * @returns PSM2_INIT_BAD_API_VERSION The PSM2 library cannot compatibility for
 *                                   the desired API version.
 *
 * @code{.c}
   	// In this example, we want to handle our own errors before doing init,
   	// since we don't want a fatal error if OPA is not found.
   	// Note that @ref psm2_error_register_handler
   	// (and @ref psm3_get_capability_mask)
   	// are the only function that can be called before @ref psm2_init

   	int try_to_initialize_psm() {
   	    int verno_major = PSM2_VERNO_MAJOR;
   	    int verno_minor = PSM2_VERNO_MINOR;

   	    int err = psm3_error_register_handler(NULL,  // Global handler
   	                                 PSM2_ERRHANDLER_NO_HANDLER); // return errors
   	    if (err) {
   	       fprintf(stderr, "Couldn't register global handler: %s\n",
   	   	          psm3_error_get_string(err));
   	       return -1;
   	    }

   	    err = psm3_init(&verno_major, &verno_minor);
   	    if (err || verno_major > PSM2_VERNO_MAJOR) {
   	       if (err)
   	         fprintf(stderr, "PSM3 initialization failure: %s\n",
   	                 psm3_error_get_string(err));
   	     else
   	         fprintf(stderr, "PSM3 loaded an unexpected/unsupported "
   	                         "version (%d.%d)\n", verno_major, verno_minor);
   	     return -1;
   	    }

   	    // We were able to initialize PSM2 but will defer all further error
   	    // handling since most of the errors beyond this point will be fatal.
   	    int err = psm3_error_register_handler(NULL,  // Global handler
   	                                          PSM2_ERRHANDLER_PSM_HANDLER);
   	    if (err) {
   	       fprintf(stderr, "Couldn't register global errhandler: %s\n",
   	   	          psm3_error_get_string(err));
   	       return -1;
   	    }
   	    return 1;
   	}
   @endcode
 */
psm2_error_t psm3_init(int *api_verno_major, int *api_verno_minor);

/*! @brief PSM2 capabilities definitions
 *
 * Each capability is defined as a separate bit,
 * i.e. next capabilities must be defined as
 * consecutive bits : 0x2, 0x4 ... and so on.
 */
#define PSM2_MULTI_EP_CAP 0x1	/* Multiple Endpoints capability */
#define PSM2_LIB_REFCOUNT_CAP 0x2	/* Library finalization is managed with reference count */

/** @brief PSM2 capabilities provider
 *
 * @param[in] req_cap_mask Requested capabilities are given as bit field.
 *
 * @returns internal capabilities bit field ANDed with a requested bit mask */
uint64_t psm3_get_capability_mask(uint64_t req_cap_mask);

/** @brief Finalize PSM2 interface
 *
 * Single call to finalize PSM2 and close all unclosed endpoints
 *
 * @post The user guarantees not to make any further PSM2 calls, including @ref
 * psm2_init.
 *
 * @returns PSM2_OK Always returns @c PSM2_OK */
psm2_error_t psm3_finalize(void);

/** @brief Error handling opaque token
 *
 * A token is required for users that register their own handlers and wish to
 * defer further error handling to PSM. */
typedef struct psm2_error_token *psm2_error_token_t;

/** @brief Error handling function
 *
 * Users can handle errors explicitly instead of relying on PSM's own error
 * handler.  There is one global error handler and error handlers that can be
 * individually set for each opened endpoint.  By default, endpoints will
 * inherit the global handler registered at the time of open.
 *
 * @param[in] ep Handle associated to the endpoint over which the error occurred
 *               or @c NULL if the error is being handled by the global error
 *               handler.
 * @param[in] error PSM2 error identifier
 * @param[in] error_string A descriptive error string of maximum length @ref
 *                         PSM2_ERRSTRING_MAXLEN.
 * @param[in] token Opaque PSM2 token associated with the particular event that
 *		    generated the error.  The token can be used to extract the
 *		    error string and can be passed to @ref psm3_error_defer to
 *		    defer any remaining or unhandled error handling to PSM.
 *
 * @post If the error handler returns, the error returned is propagated to the
 *       caller.  */
typedef psm2_error_t(*psm2_ep_errhandler_t) (psm2_ep_t ep,
					   const psm2_error_t error,
					   const char *error_string,
					   psm2_error_token_t token);

#define PSM2_ERRHANDLER_DEFAULT	((psm2_ep_errhandler_t)-1)
/**< Obsolete names, only here for backwards compatibility */
#define PSM2_ERRHANDLER_NOP	((psm2_ep_errhandler_t)-2)
/**< Obsolete names, only here for backwards compatibility */

#define PSM2_ERRHANDLER_PSM_HANDLER  ((psm2_ep_errhandler_t)-1)
/**< PSM2 error handler as explained in @ref error_handling */

#define PSM2_ERRHANDLER_NO_HANDLER   ((psm2_ep_errhandler_t)-2)
/**< Bypasses the default PSM2 error handler and returns all errors to the user
 * (this is the default) */

#define PSM2_ERRSTRING_MAXLEN	512 /**< Maximum error string length. */

/** @brief PSM2 error handler registration
 *
 * Function to register error handlers on a global basis and on a per-endpoint
 * basis.  PSM2_ERRHANDLER_PSM_HANDLER and PSM2_ERRHANDLER_NO_HANDLER are special
 * pre-defined handlers to respectively enable use of the default PSM-internal
 * handler or the no-handler that disables registered error handling and
 * returns all errors to the caller (both are documented in @ref
 * error_handling).
 *
 * @param[in] ep Handle of the endpoint over which the error handler should be
 *               registered.  With ep set to @c NULL, the behavior of the
 *               global error handler can be controlled.
 * @param[in] errhandler Handler to register.  Can be a user-specific error
 *                       handling function or PSM2_ERRHANDLER_PSM_HANDLER or
 *                       PSM2_ERRHANDLER_NO_HANDLER.
 *
 * @remark When ep is set to @c NULL, this is the only function that can be
 * called before @ref psm2_init
 */
psm2_error_t
psm3_error_register_handler(psm2_ep_t ep, const psm2_ep_errhandler_t errhandler);

/** @brief PSM2 deferred error handler
 *
 * Function to handle fatal PSM2 errors if no error handler is installed or if
 * the user wishes to defer further error handling to PSM.  Depending on the
 * type of error, PSM2 may or may not return from the function call.
 *
 * @param[in] err_token Error token initially passed to error handler
 *
 * @pre The user is calling into the function because it has decided that PSM
 *      should handle an error case.
 *
 * @post The function may or may not return depending on the error
 */
psm2_error_t psm3_error_defer(psm2_error_token_t err_token);

/** @brief Get generic error string from error
 *
 * Function to return the default error string associated to a PSM2 error.
 *
 * While a more detailed and precise error string is usually available within
 * error handlers, this function is available to obtain an error string out of
 * an error handler context or when a no-op error handler is registered.
 *
 * @param[in] error PSM2 error
 */
const char *psm3_error_get_string(psm2_error_t error);

/** @brief Option key/pair structure
 *
 * Currently only used in MQ.
 */
struct psm2_optkey {
	uint32_t key;	/**< Option key */
	void *value;	/**< Option value */
};

/*! @} */

/*! @defgroup ep PSM2 Device Endpoint Management
 * @{
 */

/** @brief Endpoint ID
 *
 * Integral type of size 24 bytes that can be used by the user to globally
 * identify a successfully opened endpoint.  Although the contents of the
 * endpoint id integral type remains opaque to the user, unique network id and
 * context within node (aka port) can be extracted using @ref psm3_epid_nid
 * and @ref psm3_epid_context.
 */
typedef struct {
	uint64_t w[3];
} psm2_epid_t;

/** @brief Endpoint Address (opaque)
 *
 * Remote endpoint addresses are created when the user binds an endpoint ID
 * to a particular endpoint handle using @ref psm3_ep_connect.  A given endpoint
 * address is only guaranteed to be valid over a single endpoint.
 */
typedef struct psm2_epaddr *psm2_epaddr_t;

/** @brief PSM2 Unique UID
 *
 * PSM2 type equivalent to the DCE-1 uuid_t, used to uniquely identify an
 * endpoint within a particular job.  Since PSM2 does not participate in job
 * allocation and management, users are expected to generate a unique ID to
 * associate endpoints to a particular parallel or collective job.
 */
typedef uuid_t psm2_uuid_t;

/** @brief Network ID
 *
 * A type of epid that can be used by the user to globally
 * identify a NIC within a given node.  This may be extracted from an
 * Endpoint ID by using @ref psm3_epid_nid
 */
typedef psm2_epid_t psm2_nid_t;

#if 0
/** @brief Get Endpoint identifier's Unique Network ID
 */
psm2_nid_t psm3_epid_nid(psm2_epid_t epid);

/** @brief Get Endpoint identifier's OPA context number */
uint64_t psm3_epid_context(psm2_epid_t epid);
#endif // 0

/** @brief Get Endpoint identifier's OPA port (deprecated, use
 * @ref psm3_epid_context instead) */
uint64_t psm3_epid_port(psm2_epid_t epid);

/** @brief Compare Endpoint identifiers */
int psm3_epid_cmp(psm2_epid_t a, psm2_epid_t b);

/** @brief Simple hex format of Endpoint identifier, bufno = 0 or 1 */
const char *psm3_epid_fmt(psm2_epid_t epid, int bufno);

/** @brief Compare Endpoint identifier to zero/empty */
int psm3_epid_zero(psm2_epid_t a);

/** @brief Zero out Endpoint identifier */
psm2_epid_t psm3_epid_zeroed(void);

/** @brief Compare Network identifiers */
int psm3_nid_cmp(psm2_nid_t a, psm2_nid_t b);

/** @brief Format of Network identifier, bufno = 0 or 1 */
const char *psm2_nid_fmt(psm2_nid_t nid, int bufno);

/** @brief Compare Network identifier to zero/empty */
int psm3_nid_zero(psm2_nid_t a);

/** @brief Zero out Network identifier */
psm2_nid_t psm3_nid_zeroed(void);

/** @brief List the number of available units (NIC devices)
 *
 * Function used to determine the number of locally available units (NICs).
 * For @c N units, valid unit numbers in @ref psm3_ep_open are @c 0 to @c N-1.
 *
 * @returns PSM2_OK unless the user has not called @ref psm2_init
 */
psm2_error_t psm3_ep_num_devunits(uint32_t *num_units);

/* Affinity modes for the affinity member of struct psm3_ep_open_opts */
#define PSM2_EP_OPEN_AFFINITY_SKIP     0	/**< Disable setting affinity */
#define PSM2_EP_OPEN_AFFINITY_SET      1	/**< Enable setting affinity unless
					  already set */
#define PSM2_EP_OPEN_AFFINITY_FORCE    2	/**< Enable setting affinity regardless
					  of current affinity setting */

/* Default values for some constants */
#define PSM2_EP_OPEN_PKEY_DEFAULT    0xffffffffffffffffULL
				    /**< Default protection key */

/** @brief Endpoint Open Options
 *
 * These options are available for opening a PSM2 endpoint.  Each is
 * individually documented and setting each option to -1 or passing NULL as the
 * options parameter in @ref psm3_ep_open instructs PSM2 to use
 * implementation-defined defaults.
 *
 * Each option is documented in @ref psm3_ep_open
 */
struct psm3_ep_open_opts {
	int64_t timeout;	/**< timeout in nanoseconds to open device */
	int unit;		/**< OPA Unit ID to open on */
	int affinity;		/**< How PSM2 should set affinity */
	int shm_mbytes;	/**< Megabytes used for intra-node, deprecated */
	int sendbufs_num;	/**< Preallocated send buffers */
	uint64_t network_pkey;	/**< Network Protection Key (v1.01) */
	int port;		/**< IB port to use (1 to N) */
	int addr_index;		/**< address index within port to use (0 to N) */
	int outsl;		/**< IB SL to use when sending pkts */
	uint64_t service_id;	/* IB Service ID to use for endpoint */
	psm2_path_res_t path_res_type;	/* Path resolution type */
	int senddesc_num;	/* Preallocated send descriptors */
	int imm_size;		/* Immediate data size for endpoint */
};

/** @brief OPA endpoint creation
 *
 * Function used to create a new local communication endpoint on an OPA
 * adapter.  The returned endpoint handle is required in all PSM2 communication
 * operations, as PSM2 can manage communication over multiple endpoints.  An
 * opened endpoint has no global context until the user connects the endpoint
 * to other global endpoints by way of @ref psm3_ep_connect.  All local endpoint
 * handles are globally identified by endpoint IDs (@ref psm2_epid_t) which are
 * also returned when an endpoint is opened.  It is assumed that the user can
 * provide an out-of-band mechanism to distribute the endpoint IDs in order to
 * establish connections between endpoints (@ref psm3_ep_connect for more
 * information).
 *
 * @param[in] unique_job_key Endpoint key, to uniquely identify the endpoint in
 *                           a parallel job.  It is up to the user to ensure
 *                           that the key is globally unique over a period long
 *                           enough to prevent duplicate keys over the same set
 *                           of endpoints (see comments below).
 *
 * @param[in] opts Open options of type @ref psm3_ep_open_opts
 *                 (see @ref psm3_ep_open_opts_get_defaults).
 *
 * @param[out] ep User-supplied storage to return a pointer to the newly
 *                created endpoint.  The returned pointer of type @ref psm2_ep_t
 *                is a local handle and cannot be used to globally identify the
 *                endpoint.
 * @param[out] epid User-supplied storage to return the endpoint ID associated
 *                  to the newly created local endpoint returned in the @c ep
 *                  handle.  The endpoint ID is an integral type suitable for
 *                  uniquely identifying the local endpoint.
 *
 * PSM2 does not internally verify the consistency of the uuid, it is up to the
 * user to ensure that the uid is unique enough not to collide with other
 * currently-running jobs.  Users can employ two mechanisms to obtain a uuid.
 *
 * 1. Use an OS or library-specific uuid generation utility, that complies with
 *    OSF DCE 1.1, such as @c uuid_generate on Linux or @c uuid_create on
 *    FreeBSD.
 *    (see http://www.opengroup.org/onlinepubs/009629399/uuid_create.htm)
 *
 * 2. Manually pack a 16-byte string using a utility such as /dev/random or
 *    other source with enough entropy and proper seeding to prevent two nodes
 *    from generating the same uuid_t.
 *
 * The following options are relevent when opening an endpoint:
 *   @li @c timeout establishes the number of nanoseconds to wait before
 *                  failing to open a port (with -1, defaults to 15 secs).
 *   @li @c unit sets the OPA unit number to use to open a port (with
 *               -1, PSM2 determines the best unit to open the port).  If @c
 *               PSM3_NIC is set in the environment, this setting is ignored.
 *   @li @c affinity enables or disables PSM2 setting processor affinity.  The
 *                   option can be controlled to either disable (@ref
 *                   PSM2_EP_OPEN_AFFINITY_SKIP) or enable the affinity setting
 *                   only if it is already unset (@ref
 *                   PSM2_EP_OPEN_AFFINITY_SET) or regardless of affinity being
 *                   set or not (@ref PSM2_EP_OPEN_AFFINITY_FORCE).
 *                   If @c PSM3_NO_CPUAFFINITY is set in the environment, this
 *                   setting is ignored.
 *   @li @c shm_mbytes sets a maximum number of megabytes that can be allocated
 *		       to each local endpoint ID connected through this
 *		       endpoint (with -1, defaults to 10 MB).
 *   @li @c sendbufs_num sets the number of send buffers that can be
 *                       pre-allocated for communication (with -1, defaults to
 *                       512 buffers of MTU size).
 *   @li @c network_pkey sets the protection key to employ for point-to-point
 *                       PSM2 communication.  Unless a specific value is used,
 *                       this parameter should be set to
 *                       PSM2_EP_OPEN_PKEY_DEFAULT.
 *
 * @warning By default, PSM2 limits the user to calling @ref psm3_ep_open only
 * once per process and subsequent calls will fail. In order to enable creation
 * of multiple endoints per process, one must properly set the environment variable
 * @ref PSM3_MULTI_EP before calling @ref psm2_init.
 *
 * @code{.c}
    	// In order to open an endpoint and participate in a job, each endpoint has
    	// to be distributed a unique 16-byte UUID key from an out-of-band source.
    	// Presumably this can come from the parallel spawning utility either
    	// indirectly through an implementors own spawning interface or as in this
    	// example, the UUID is set as a string in an environment variable
    	// propagated to all endpoints in the job.

    	int try_to_open_psm2_endpoint(psm2_ep_t *ep, // output endpoint handle
    	                             psm2_epid_t *epid, // output endpoint identifier
    	                             int unit)  // unit of our choice
    	{
    	   struct psm3_ep_open_opts epopts;
    	   psm2_uuid_t job_uuid;
    	   char *c;

    	   // Let PSM2 assign its default values to the endpoint options.
    	   psm3_ep_open_opts_get_defaults(&epopts);

    	   // We want a stricter timeout and a specific unit
    	   epopts.timeout = 15*1e9;  // 15 second timeout
    	   epopts.unit = unit;	// We want a specific unit, -1 would let PSM
    	                             // choose the unit for us.
    	   epopts.port = port;	// We want a specific unit, <= 0 would let PSM
    	                             // choose the port for us.
    	   epopts.addr_index = index;	// We want a specific address, <= 0 would let PSM
    	                             // choose the address for us.
    	   // We've already set affinity, don't let PSM2 do so if it wants to.
    	   if (epopts.affinity == PSM2_EP_OPEN_AFFINITY_SET)
    	      epopts.affinity = PSM2_EP_OPEN_AFFINITY_SKIP;

    	   // ENDPOINT_UUID is set to the same value in the environment of all the
    	   // processes that wish to communicate over PSM2 and was generated by
    	   // the process spawning utility
    	   c = getenv("ENDPOINT_UUID");
    	   if (c && *c)
    	      implementor_string_to_16byte_packing(c, job_uuid);
    	   else {
    	      fprintf(stderr, "Can't find UUID for endpoint\n);
    	      return -1;
    	   }

    	   // Assume we don't want to handle errors here.
    	   psm3_ep_open(job_uuid, &epopts, ep, epid);
    	   return 1;
    	}
   @endcode
 */
psm2_error_t
psm3_ep_open(const psm2_uuid_t unique_job_key,
	    const struct psm3_ep_open_opts *opts, psm2_ep_t *ep,
	    psm2_epid_t *epid);

/** @brief Endpoint open default options.
 *
 * Function used to initialize the set of endpoint options to their default
 * values for use in @ref psm3_ep_open.
 *
 * @param[out] opts Endpoint Open options.
 *
 * @warning For portable operation, users should always call this function
 * prior to calling @ref psm3_ep_open.
 *
 * @return PSM2_OK If result could be updated
 * @return PSM2_INIT_NOT_INIT If psm has not been initialized.
 */
psm2_error_t
psm3_ep_open_opts_get_defaults(struct psm3_ep_open_opts *opts);

/** @brief Endpoint shared memory query
 *
 * Function used to determine if a remote endpoint shares memory with a
 * currently opened local endpiont.
 *
 * @param[in] ep Endpoint handle
 * @param[in] epid Endpoint ID
 *
 * @param[out] result Result is non-zero if the remote endpoint shares memory with the local
 * endpoint @c ep, or zero otherwise.
 *
 * @return PSM2_OK If result could be updated
 * @return PSM2_EPID_UNKNOWN If the epid is not recognized
 */
psm2_error_t
psm3_ep_epid_share_memory(psm2_ep_t ep, psm2_epid_t epid, int *result);

/** @brief Close endpoint
 * @param[in] ep PSM2 endpoint handle
 * @param[in] mode One of @ref PSM2_EP_CLOSE_GRACEFUL or @ref PSM2_EP_CLOSE_FORCE
 * @param[in] timeout How long to wait in nanoseconds if mode is
 *			PSM2_EP_CLOSE_GRACEFUL, 0 waits forever.  If @c mode is
 *			@ref PSM2_EP_CLOSE_FORCE, this parameter is ignored.
 *
 * The following errors are returned, others are handled by the per-endpoint
 * error handler:
 *
 * @return PSM2_OK  Endpoint was successfully closed without force or
 *                 successfully closed with force within the supplied timeout.
 * @return PSM2_EP_CLOSE_TIMEOUT Endpoint could not be successfully closed
 *                              within timeout.
 */
psm2_error_t psm3_ep_close(psm2_ep_t ep, int mode, int64_t timeout);

#define PSM2_EP_CLOSE_GRACEFUL	0	/**< Graceful mode in @ref psm3_ep_close */
#define PSM2_EP_CLOSE_FORCE	1	/**< Forceful mode in @ref psm3_ep_close */

/** @brief Provide mappings for network id to hostname
 *
 * Since PSM2 does not assume or rely on the availability of an external
 * networkid-to-hostname mapping service, users can provide one or more of
 * these mappings.  The @ref psm3_map_nid_hostname function allows a list of
 * network ids to be associated to hostnames.
 *
 * This function is not mandatory for correct operation but may allow PSM2 to
 * provide better diagnostics when remote endpoints are unavailable and can
 * otherwise only be identified by their network id.
 *
 * @param[in] num Number elements in @c nid and @c hostnames arrays
 * @param[in] nids User-provided array of network ids
 *                 should be obtained by calling @ref psm3_epid_nid on each
 *                 epid.
 * @param[in] hostnames User-provided array of hostnames (array of
 *                      NUL-terimated strings) where each hostname index
 *                      maps to the provided nid hostname.
 *
 * @warning Duplicate nids may be provided in the input @c nids array, only
 *          the last corresponding hostname will be remembered.
 *
 * @pre The user may or may not have already provided a hostname mappings.
 * @post The user may free any dynamically allocated memory passed to the
 *       function.
 *
 */
psm2_error_t
psm3_map_nid_hostname(int num, const psm2_nid_t *nids, const char **hostnames);

/** @brief Connect one or more remote endpoints to a local endpoint
 *
 * Function to non-collectively establish a connection to a set of endpoint IDs
 * and translate endpoint IDs into endpoint addresses.  Establishing a remote
 * connection with a set of remote endpoint IDs does not imply a collective
 * operation and the user is free to connect unequal sets on each process.
 * Similarly, a given endpoint address does not imply that a pairwise
 * communication context exists between the local endpoint and remote endpoint.
 *
 * @param[in] ep PSM2 endpoint handle
 *
 * @param[in] num_of_epid The number of endpoints to connect to, which
 *                        also establishes the number of elements contained in
 *                        all of the function's array-based parameters.
 *
 * @param[in] array_of_epid User-allocated array that contains @c num_of_epid
 *                          valid endpoint identifiers.  Each endpoint id (or
 *                          epid) has been obtained through an out-of-band
 *                          mechanism and each endpoint must have been opened
 *                          with the same uuid key.
 *
 * @param[in] array_of_epid_mask User-allocated array that contains
 *                          @c num_of_epid integers.  This array of masks
 *                          allows users to select which of the epids in @c
 *                          array_of_epid should be connected.  If the integer
 *                          at index i is zero, psm does not attempt to connect
 *                          to the epid at index i in @c array_of_epid.  If
 *                          this parameter is NULL, psm will try to connect to
 *                          each epid.
 *
 * @param[out] array_of_errors User-allocated array of at least @c num_of_epid
 *                             elements. If the function does not return
 *                             PSM2_OK, this array can be consulted for each
 *                             endpoint not masked off by @c array_of_epid_mask
 *                             to know why the endpoint could not be connected.
 *                             Endpoints that could not be connected because of
 *                             an unrelated failure will be marked as @ref
 *                             PSM2_EPID_UNKNOWN.  If the function returns
 *                             PSM2_OK, the errors for all endpoints will also
 *                             contain PSM2_OK.
 *
 * @param[out] array_of_epaddr User-allocated array of at least @c num_of_epid
 *                             elements of type psm2_epaddr_t.  Each
 *                             successfully connected endpoint is updated with
 *                             an endpoint address handle that corresponds to
 *                             the endpoint id at the same index in @c
 *                             array_of_epid.  Handles are only updated if the
 *                             endpoint could be connected and if its error in
 *                             array_of_errors is PSM2_OK.
 *
 * @param[in] timeout Timeout in nanoseconds after which connection attempts
 *                    will be abandoned.  Setting this value to 0 disables
 *                    timeout and waits until all endpoints have been
 *                    successfully connected or until an error is detected.
 *
 * @pre The user has opened a local endpoint and obtained a list of endpoint
 *      IDs to connect to a given endpoint handle using an out-of-band
 *      mechanism not provided by PSM.
 *
 * @post If the connect is successful, @c array_of_epaddr is updated with valid
 *       endpoint addresses.
 *
 * @post If unsuccessful, the user can query the return status of each
 *       individual remote endpoint in @c array_of_errors.
 *
 * @post The user can call into @ref psm3_ep_connect many times with the same
 *       endpoint ID and the function is guaranteed to return the same output
 *       parameters.
 *
 * @post PSM2 does not keep any reference to the arrays passed into the
 *       function and the caller is free to deallocate them.
 *
 * The error value with the highest importance is returned by
 * the function if some portion of the communication failed.  Users should
 * always refer to individual errors in @c array_of_errors whenever the
 * function cannot return PSM2_OK.
 *
 * @returns PSM2_OK  The entire set of endpoint IDs were successfully connected
 *                  and endpoint addresses are available for all endpoint IDs.
 *
 * @code{.c}
   	int connect_endpoints(psm2_ep_t ep, int numep,
   	                      const psm2_epid_t *array_of_epid,
   	                      psm2_epaddr_t **array_of_epaddr_out)
   	{
   	    psm2_error_t *errors = (psm2_error_t *) calloc(numep, sizeof(psm2_error_t));
   	    if (errors == NULL)
   	        return -1;

   	    psm2_epaddr_t *all_epaddrs =
   	             (psm2_epaddr_t *) calloc(numep, sizeof(psm2_epaddr_t));

   	    if (all_epaddrs == NULL)
   	        return -1;

   	    psm3_ep_connect(ep, numep, array_of_epid,
   	                   NULL, // We want to connect all epids, no mask needed
   	                   errors,
   	                   all_epaddrs,
   	                   30*e9); // 30 second timeout, <1 ns is forever
   	    *array_of_epaddr_out = all_epaddrs;
   	    free(errors);
   	    return 1;
   	}
   @endcode
 */
psm2_error_t
psm3_ep_connect(psm2_ep_t ep, int num_of_epid, const psm2_epid_t *array_of_epid,
		   const int *array_of_epid_mask, psm2_error_t *array_of_errors,
		   psm2_epaddr_t *array_of_epaddr, int64_t timeout);

/* @brief Disconnect one or more remote endpoints from a local endpoint.
*
* Function to non-collectively disconnect a connection to a set of endpoint
* addresses and free the endpoint addresses. After disconnecting, the
* application cannot send messages to the remote processes and PSM2 is
* restored back to the state before calling psm3_ep_connect. The application
* must call psm3_ep_connect to establish the connections again.
*
* This function is equivalent to calling psm3_ep_disconnect2() with mode ==
* PSM2_EP_DISCONNECT_GRACEFUL.
*
* @param[in] ep PSM2 endpoint handle
*
* @param[in] num_of_epaddr The number of endpoint addresses to disconnect from,
*                          which also indicates the number of elements contained
*                          in all of the function’s array-based parameters.
*
* @param[in] array_of_epaddr User-allocated array that contains num_of_epaddr
*                            valid endpoint addresses. Each endpoint address (or
*                            epaddr) has been obtained through a previous
*                            psm3_ep_connect call.
*
* @param[in] array_of_epaddr_mask User-allocated array that contains
*                                 num_of_epaddr integers. This array of masks
*                                 allows users to select which of the
*                                 epaddresses in array_of_epaddr should be
*                                 disconnected. If the integer at index i is
*                                 zero, PSM2 does not attempt to disconnect to
*                                 the epaddr at index i in array_of_epaddr. If
*                                 this parameter is NULL, PSM2 tries to
*                                 disconnect all epaddr in array_of_epaddr.
*
* @param[out] array_of_errors User-allocated array of at least num_of_epaddr
*                             elements. If the function does not return PSM2_OK,
*                             this array can be consulted for each endpoint
*                             address not masked off by array_of_epaddr_mask to
*                             know why the endpoint could not be disconnected.
*                             Any endpoint address that could not be
*                             disconnected because of an unrelated failure is
*                             marked as PSM2_EPID_UNKNOWN. If the function
*                             returns PSM2_OK, the errors for all endpoint
*                             addresses also contain PSM2_OK.
*
* @param[in] timeout Timeout in nanoseconds after which disconnection attempts
*                    are abandoned. Setting this value to 0 disables timeout and
*                    waits until all endpoints have been successfully
*                    disconnected or until an error is detected.
*
* @pre You have established the connections with previous psm3_ep_connect calls.
*
* @post If the disconnect is successful, the corresponding epaddr in
*       array_of_epaddr is reset to NULL pointer.
*
* @post If unsuccessful, you can query the return status of each individual
*       remote endpoint in array_of_errors.
*
* @post PSM2 does not keep any reference to the arrays passed into the function
*       and the caller is free to deallocate them.
*
* @post The error value with the highest importance is returned by the function
*       if some portion of the communication failed. Refer to individual errors
*       in array_of_errors whenever the function cannot return PSM2_OK.
*
* @returns PSM2_OK The entire set of endpoint IDs were successfully disconnected
*          and endpoint addresses are freed by PSM2.
*
* @code{.c}
int disconnect_endpoints(psm2_ep_t ep, int num_epaddr,
             const psm2_epaddr_t *array_of_epaddr)
{
    psm2_error_t *errors =
        (psm2_error_t *)calloc(num_epaddr, sizeof(psm2_error_t));
    if (errors == NULL)
        return -1;
    psm3_ep_disconnect(
        ep, num_epaddr, array_of_epaddr,
        NULL, // We want to disconnect all epaddrs, no mask needed,
        errors,
        30 * e9); // 30 second timeout, <1 ns is forever
    free(errors);
    return 1;
}
@endcode
*/
psm2_error_t psm3_ep_disconnect(psm2_ep_t ep, int num_of_epaddr,
				psm2_epaddr_t *array_of_epaddr,
				const int *array_of_epaddr_mask,
				psm2_error_t *array_of_errors, int64_t timeout);

/* @brief Disconnect one or more remote endpoints from a local endpoint.
*
* Function to non-collectively disconnect a connection to a set of endpoint
* addresses and free the endpoint addresses. After disconnecting, the
* application cannot send messages to the remote processes and PSM2 is
* restored back to the state before calling psm3_ep_connect. The application
* must call psm3_ep_connect to establish the connections again.
*
* @param[in] ep PSM2 endpoint handle
*
* @param[in] num_of_epaddr The number of endpoint addresses to disconnect from,
*                          which also indicates the number of elements contained
*                          in all of the function’s array-based parameters.
*
* @param[in] array_of_epaddr User-allocated array that contains num_of_epaddr
*                            valid endpoint addresses. Each endpoint address (or
*                            epaddr) has been obtained through a previous
*                            psm3_ep_connect call.
*
* @param[in] array_of_epaddr_mask User-allocated array that contains
*                                 num_of_epaddr integers. This array of masks
*                                 allows users to select which of the
*                                 epaddresses in array_of_epaddr should be
*                                 disconnected. If the integer at index i is
*                                 zero, PSM2 does not attempt to disconnect to
*                                 the epaddr at index i in array_of_epaddr. If
*                                 this parameter is NULL, PSM2 tries to
*                                 disconnect all epaddr in array_of_epaddr.
*
* @param[out] array_of_errors User-allocated array of at least num_of_epaddr
*                             elements. If the function does not return PSM2_OK,
*                             this array can be consulted for each endpoint
*                             address not masked off by array_of_epaddr_mask to
*                             know why the endpoint could not be disconnected.
*                             Any endpoint address that could not be
*                             disconnected because of an unrelated failure is
*                             marked as PSM2_EPID_UNKNOWN. If the function
*                             returns PSM2_OK, the errors for all endpoint
*                             addresses also contain PSM2_OK.
*
* @param[in] mode One of @ref PSM2_EP_DISCONECT_GRACEFUL or @ref PSM2_EP_DISCONECT_FORCE
*
* @param[in] timeout Timeout in nanoseconds after which disconnection attempts
*                    are abandoned. Setting this value to 0 disables timeout and
*                    waits until all endpoints have been successfully
*                    disconnected or until an error is detected. Supplying a
*                    negative value here sets the disconnection mode to "force".
*
* @pre You have established the connections with previous psm3_ep_connect calls.
*
* @post If the disconnect is successful, the corresponding epaddr in
*       array_of_epaddr is reset to NULL pointer.
*
* @post If unsuccessful, you can query the return status of each individual
*       remote endpoint in array_of_errors.
*
* @post PSM2 does not keep any reference to the arrays passed into the function
*       and the caller is free to deallocate them.
*
* @post The error value with the highest importance is returned by the function
*       if some portion of the communication failed. Refer to individual errors
*       in array_of_errors whenever the function cannot return PSM2_OK.
*
* @returns PSM2_OK The entire set of endpoint IDs were successfully disconnected
*          and endpoint addresses are freed by PSM2.
*
* @code{.c}
int disconnect_endpoints(psm2_ep_t ep, int num_epaddr,
             const psm2_epaddr_t *array_of_epaddr)
{
    psm2_error_t *errors =
        (psm2_error_t *)calloc(num_epaddr, sizeof(psm2_error_t));
    if (errors == NULL)
        return -1;
    psm3_ep_disconnect2(
        ep, num_epaddr, array_of_epaddr,
        NULL, // We want to disconnect all epaddrs, no mask needed,
        errors,
	PSM2_EP_DISCONECT_GRACEFUL,
        30 * e9); // 30 second timeout, 0 ns is forever
    free(errors);
    return 1;
}
@endcode
*/
psm2_error_t psm3_ep_disconnect2(psm2_ep_t ep, int num_of_epaddr,
				psm2_epaddr_t *array_of_epaddr,
				const int *array_of_epaddr_mask,
				psm2_error_t *array_of_errors,
				int mode, int64_t timeout);

#define PSM2_EP_DISCONNECT_GRACEFUL	PSM2_EP_CLOSE_GRACEFUL   /**< Graceful mode in @ref psm3_ep_disconnect2 */
#define PSM2_EP_DISCONNECT_FORCE	PSM2_EP_CLOSE_FORCE   /**< Forceful mode in @ref psm3_ep_disconnect2 */

/** @brief Ensure endpoint communication progress
 *
 * Function to ensure progress for all PSM2 components instantiated on an
 * endpoint (currently, this only includes the MQ component).  The function
 * never blocks and is typically required in two cases:
 *
 * @li Allowing all PSM2 components instantiated over a given endpoint to make
 *     communication progress. Refer to @ref mq_progress for a detailed
 *     discussion on MQ-level progress issues.
 *
 * @li Cases where users write their own synchronization primitives that
 *     depend on remote communication (such as spinning on a memory location
 *     which's new value depends on ongoing communication).
 *
 * The poll function doesn't block, but the user can rely on the @ref
 * PSM2_OK_NO_PROGRESS return value to control polling behaviour in terms of
 * frequency (poll until an event happens) or execution environment (poll for a
 * while but yield to other threads of CPUs are oversubscribed).
 *
 * @returns PSM2_OK             Some communication events were progressed
 * @returns PSM2_OK_NO_PROGRESS Polling did not yield any communication progress
 *
 */
psm2_error_t psm3_poll(psm2_ep_t ep);

/** @brief Set a user-determined ep address label.
 *
 * @param[in] epaddr Endpoint address, obtained from @ref psm3_ep_connect
 * @param[in] epaddr_label_string User-allocated string to print when
 *                   identifying endpoint in error handling or other verbose
 *                   printing.  The NULL-terminated string must be allocated by
 *                   the user since PSM2 only keeps a pointer to the label.  If
 *                   users do not explicitly set a label for each endpoint,
 *                   endpoints will identify themselves as hostname:port.
 */
void psm3_epaddr_setlabel(psm2_epaddr_t epaddr,
			 const char *epaddr_label_string);

/** @brief Set a user-determined ep address context.
 *
 * @param[in] epaddr Endpoint address, obtained from @ref psm3_ep_connect
 * @param[in] ctxt   Opaque user defined state to associate with an endpoint
 *                   address. This state can be retrieved via
 *                   @ref psm3_epaddr_getctxt.
 */
void
psm3_epaddr_setctxt(psm2_epaddr_t epaddr, void *ctxt);

/** @brief Get the user-determined ep address context. Users can associate an
 *  opaque context with each endpoint via @ref psm3_epaddr_setctxt.
 *
 * @param[in] epaddr Endpoint address, obtained from @ref psm3_ep_connect.
 */
void *psm3_epaddr_getctxt(psm2_epaddr_t epaddr);

/* Below are all component specific options. The component object for each of
 * the options is also specified.
 */

/* PSM2_COMPONENT_CORE options */
/* PSM2 debug level */
#define PSM2_CORE_OPT_DEBUG     0x101
  /**< [@b uint32_t ] Set/Get the PSM2 debug level. This option can be set
   * before initializing the PSM2 library.
   *
   * component object: (null)
   * option value: PSM2 Debug mask to set or currently active debug level.
   */

/* PSM2 endpoint address context */
#define PSM2_CORE_OPT_EP_CTXT   0x102
  /**< [@b uint32_t ] Set/Get the context associated with a PSM2 endpoint
   * address (psm2_epaddr_t).
   *
   * component object: PSM2 endpoint (@ref psm2_epaddr_t) address.
   * option value: Context associated with PSM2 endpoint address.
   */

/* PSM2_COMPONENT_IB options */
/* Default service level to use to communicate with remote endpoints */
#define PSM2_IB_OPT_DF_SL 0x201
  /**< [@b uint32_t ] Default OPA SL to use for all remote communication.
   * If unset defaults to Service Level 0.
   *
   * component object: Opened PSM2 endpoint id (@ref psm2_ep_t).
   * option value: Default IB SL to use for endpoint. (0 <= SL < 15)
   */

/* Set IB service level to use for communication to an endpoint */
#define PSM2_IB_OPT_EP_SL 0x202
  /**< [@b uint32_t ] OPA SL to use for communication to specified
   * remote endpoint.
   *
   * component object: PSM2 endpoint (@ ref psm2_epaddr_t) address.
   * option value: SL used to communicate with remote endpoint. (0 <= SL < 15)
   */

/* PSM2_COMPONENT_MQ options (deprecates psm2_mq_set|getopt) */
/* MQ options that can be set in psm3_mq_init and psm2_{set,get}_opt */
#define PSM2_MQ_OPT_RNDV_IB_SZ       0x301
  /**< [@b uint32_t ] Size at which to start enabling rendezvous
   * messaging for OPA messages (if unset, defaults to values
   * between 56000 and 72000 depending on the system configuration)
   *
   * component object: PSM2 Matched Queue (@ref psm2_mq_t).
   * option value: Size at which to switch to rendezvous protocol.
   */
#define PSM2_MQ_RNDV_HFI_SZ          PSM2_MQ_OPT_RNDV_IB_SZ
#define PSM2_MQ_RNDV_IPATH_SZ        PSM2_MQ_OPT_RNDV_IB_SZ

#define PSM2_MQ_OPT_RNDV_SHM_SZ      0x302
#define PSM2_MQ_RNDV_SHM_SZ          PSM2_MQ_OPT_RNDV_SHM_SZ
  /**< [@b uint32_t ] Size at which to start enabling
   * rendezvous messaging for shared memory (intra-node) messages (If
   * unset, defaults to 64000 bytes).
   *
   * component object: PSM2 Matched Queue (@ref psm2_mq_t).
   * option value: Size at which to switch to rendezvous protocol.
   */

#define PSM2_MQ_OPT_SYSBUF_MYBYTES   0x303
#define PSM2_MQ_MAX_SYSBUF_MBYTES    PSM2_MQ_OPT_SYSBUF_MYBYTES
  /**< [@b uint32_t ] Maximum number of bytes to allocate for unexpected
   * messages.
   *
   * component object: PSM2 Matched Queue (@ref psm2_mq_t).
   * option value: Deprecated; this option has no effect.
   */

/* PSM2_COMPONENT_AM options */
#define PSM2_AM_OPT_FRAG_SZ          0x401
#define PSM2_AM_MAX_FRAG_SZ          PSM2_AM_OPT_FRAG_SZ
/*!< [@b uint32_t ] Maximum active message fragment size that can be sent
 * for a given endpoint or across all endpoints. This value can only be
 * queried.
 *
 * component object: PSM2 endpoint (@ref psm2_epaddr_t) address. If NULL then
 *                   option value is the smalles fragment size across all
 *                   active endpoints.
 * option value: Maximum active message fragment size in bytes.
 */

#define PSM2_AM_OPT_NARGS 0x402
#define PSM2_AM_MAX_NARGS PSM2_AM_OPT_NARGS

/*!< [@b uint32_t ] Maximum number of message arguments that can be sent
 * for a given endpoint or across all endpoints. This value can only be
 * queried.
 *
 * component object: PSM2 endpoint (@ref psm2_epaddr_t) address. If NULL then
 *                   option value is the smalles fragment size across all
 *                   active endpoints.
 * option value: Maximum number of active message arguments.
 */

#define PSM2_AM_OPT_HANDLERS 0x403
#define PSM2_AM_MAX_HANDLERS PSM2_AM_OPT_HANDLERS
/*!< [@b uint32_t ] Maximum number of message handlers that can be registered
 * for a given endpoint or across all endpoints. This value can only be
 * queried.
 *
 * component object: PSM2 endpoint (@ref psm2_epaddr_t) address. If NULL then
 *                   option value is the smalles fragment size across all
 *                   active endpoints.
 * option value: Maximum number of active message handlers.
 */

/** @brief Set an option for a PSM2 component
 *
 * Function to set the value of a PSM2 component option
 *
 * @param[in] component Type of PSM2 component for which to set the option
 * @param[in] component_obj Opaque component specify object to apply the set
 *                          operation on. These are passed uninterpreted to the
 *                          appropriate component for interpretation.
 * @param[in] optname Name of component option to set. These are component
 *                    specific and passed uninterpreted to the appropriate
 *                    component for interpretation.
 * @param[in] optval Pointer to storage that contains the value to be updated
 *                   for the supplied option.  It is up to the user to
 *                   ensure that the pointer points to a memory location with a
 *                   correct size and format.
 * @param[in] optlen Size of the memory region pointed to by optval.
 *
 * @returns PSM2_OK if option could be set.
 * @returns PSM2_PARAM_ERR if the component or optname are not valid.
 * @returns PSM2_OPT_READONLY if the option to be set is a read-only option.
 *
 */
psm2_error_t
psm3_setopt(psm2_component_t component, const void *component_obj,
	   int optname, const void *optval, uint64_t optlen);

/** @brief Get an option for a PSM2 component
 *
 * Function to get the value of a PSM2 component option
 *
 * @param[in] component Type of PSM2 component for which to get the option
 * @param[in] component_obj Opaque component specify object to apply the get
 *                          operation on. These are passed uninterpreted to the
 *                          appropriate component for interpretation.
 * @param[in] optname Name of component option to get. These are component
 *                    specific and passed uninterpreted to the appropriate
 *                    component for interpretation.
 * @param[out] optval Pointer to storage that contains the value to be updated
 *                    for the supplied option.  It is up to the user to
 *                    ensure that the pointer points to a valid memory region.
 * @param[in,out] optlen This is a value result parameter initially containing
 *                      the size of the memory region pointed to by optval and
 *                      modified to return the actual size of optval.
 *
 * @returns PSM2_OK if option value could be retrieved successfully.
 * @returns PSM2_PARAM_ERR if the component or optname are not valid.
 * @returns PSM2_NO_MEMORY if the memory region optval is of insufficient size.
 *                         optlen contains the required memory region size for
 *                         optname value.
 *
 */
psm2_error_t
psm3_getopt(psm2_component_t component, const void *component_obj,
	   int optname, void *optval, uint64_t *optlen);

/** @brief Datatype for end-point information */
typedef struct psm2_epinfo {
	psm2_ep_t ep;		/**< The ep for this end-point*/
	psm2_epid_t epid;	/**< The epid for this end-point */
	psm2_uuid_t uuid;	/**< The UUID for this end-point */
	uint16_t jkey;		/**< The job key for this end-point */
	char uuid_str[64];	/**< String representation of the UUID for this end-point */
} psm2_epinfo_t;

/** @brief Datatype for end-point connection */
typedef struct psm2_epconn {
	psm2_epaddr_t addr;	/**< The epaddr for this connection */
	psm2_ep_t ep;		/**< The ep for this connection */
	psm2_mq_t mq;		/**< The mq for this connection */
} psm2_epconn_t;

/** @brief Query PSM2 for end-point information.
 *
 * Function to query PSM2 for end-point information. This allows retrieval of
 * end-point information in cases where the caller does not have access to the
 * results of psm3_ep_open().  In the default single-rail mode PSM2 will use
 * a single endpoint. If either multi-rail mode or multi-endpoint mode is
 * enabled, PSM2 will use multiple endpoints.
 *
 * @param[in,out] num_of_epinfo On input, sizes the available number of entries
 *                              in array_of_epinfo.  On output, specifies the
 *                              returned number of entries in array_of_epinfo.
 * @param[out] array_of_epinfo Returns end-point information structures.
 *
 * @pre PSM2 is initialized and the end-point has been opened.
 *
 * @returns PSM2_OK indicates success.
 * @returns PSM2_PARAM_ERR if input num_if_epinfo is less than or equal to zero.
 * @returns PSM2_EP_WAS_CLOSED if PSM2 end-point is closed or does not exist.
 */
psm2_error_t psm3_ep_query(int *num_of_epinfo, psm2_epinfo_t *array_of_epinfo);

/** @brief Query PSM2 for end-point connections.
 *
 * Function to query PSM2 for end-point connections. This allows retrieval of
 * end-point connections in cases where the caller does not have access to the
 * results of psm3_ep_connect().  The epid values can be found using
 * psm3_ep_query() so that each PSM2 process can determine its own epid. These
 * values can then be distributed across the PSM2 process so that each PSM
 * process knows the epid for all other PSM2 processes.
 *
 * @param[in] epid The epid of a PSM2 process.
 * @param[out] epconn The connection information for that PSM2 process.
 *
 * @pre PSM2 is initialized and the end-point has been connected to this epid.
 *
 * @returns PSM2_OK indicates success.
 * @returns PSM2_EP_WAS_CLOSED if PSM2 end-point is closed or does not exist.
 * @returns PSM2_EPID_UNKNOWN if the epid value is not known to PSM.
 */
psm2_error_t psm3_ep_epid_lookup(psm2_epid_t epid, psm2_epconn_t *epconn);

/** @brief Query given PSM2 end-point for its connections.
 *
 * The need for this function comes with 'multi-ep' feature.
 * Function is similar to (@ref psm3_ep_epid_lookup).
 * It differs in that an extra parameter which identifies
 * the end-point [ep] must be provided which limits the lookup to that single ep.
 *
 * @returns PSM2_OK indicates success.
 * @returns PSM2_EP_WAS_CLOSED if PSM2 end-point [ep] is closed or does not exist.
 * @returns PSM2_EPID_UNKNOWN if the [epid] value is not known to PSM.
 * @returns PSM2_PARAM_ERR if output [epconn] is NULL.
 */
psm2_error_t psm3_ep_epid_lookup2(psm2_ep_t ep, psm2_epid_t epid, psm2_epconn_t *epconn);

/** @brief Get PSM2 epid for given epaddr.
 *
 * @param[in] epaddr The endpoint address.
 * @param[out] epid The epid of a PSM2 process.
 *
 * @returns PSM2_OK indicates success.
 * @returns PSM2_PARAM_ERR if input [epaddr] or output [epid] is NULL.
 */
psm2_error_t psm3_epaddr_to_epid(psm2_epaddr_t epaddr, psm2_epid_t *epid);

/*! @} */

/*! @addtogroup init PSM2 Information Query
 * @{
 */

/** @brief Enumeration for info query APIs
 *
 * Note that calling the function:
 *
 @code{.c}
 psm2_error_t psm3_info_query(psm2_info_query_t, void *out,
                              size_t nargs, psm2_info_query_arg_t []);
 @endcode
 *
 * Takes a variable number of input arguments, per the initial psm2_info_query_t
 *
 * Below, there is an explanation of the number, type and order of the
 * required input arguments, as well as a definition of the type of the output.
 */
typedef enum psm2_info_query_et
{
/*! Required input arguments 0
   Output parameter: uint32_t*, description: the number of units */
	PSM2_INFO_QUERY_NUM_UNITS,

/*! Required input arguments: 0
   Output parameter: uint32_t*, description: the number of ports */
	PSM2_INFO_QUERY_NUM_PORTS,

/*! Required input arguments: 1
   1.  type: uint32_t, description: the unit for which status is
       desired (use: psm2_info_query_arg_t.unit).
   Output parameter: uint32_t, description: zero, when the unit
                     is not active, non-zero when the unit is
                     active.  */
	PSM2_INFO_QUERY_UNIT_STATUS,

/*! Required input arguments: 2
   1.  type: uint32_t, description: the unit for which status is
       desired (use: psm2_info_query_arg_t.unit).
   2.  type: uint32_t, description: the port for which status is
       desired (use: psm2_info_query_arg_t.port).
   Output parameter: uint32_t, description: zero, when the unit
                     is not active, non-zero when the unit is
                     active.  */
	PSM2_INFO_QUERY_UNIT_PORT_STATUS,

/*! Required input arguments: 1
   1.  type: uint32_t, description: the unit for which the number of
       free contexts is desired (use: psm2_info_query_arg_t.unit).
   Output parameter: uint32_t, description: the number of free
                     contexts..  */
	PSM2_INFO_QUERY_NUM_FREE_CONTEXTS,

/*! Required input arguments: 1
   1.  type: uint32_t, description: the unit for which the number of
       contexts is desired (use: psm2_info_query_arg_t.unit).
   Output parameter: uint32_t, description: the number of
                     contexts..  */
	PSM2_INFO_QUERY_NUM_CONTEXTS,

/*! removed QUERY_CONFIG, but kept placeholder to retain values in enum */
	PSM2_WAS_INFO_QUERY_CONFIG,

/*! removed QUERY_THRESH, but kept placeholder to retain values in enum */
	PSM2_WAS_INFO_QUERY_THRESH,

/*! removed QUERY_DEVICE_NAME, but kept placeholder to retain values in enum */
	PSM2_WAS_INFO_QUERY_DEVICE_NAME,

/*! removed QUERY_MTU, but kept placeholder to retain values in enum */
	PSM2_WAS_INFO_QUERY_MTU,

/*! removed QUERY_LINK_SPEED, but kept placeholder to retain values in enum */
	PSM2_WAS_INFO_QUERY_LINK_SPEED,

/*! removed QUERY_NETWORK_TYPE, but kept placeholder to retain values in enum */
	PSM2_WAS_INFO_QUERY_NETWORK_TYPE,

/*! Required input arguments 0
    Output parameter: uint32_t*, description: a bit mask of the features in libpsm2.
    See psm2_info_query_feature_mask below for bit mask definition. */
	PSM2_INFO_QUERY_FEATURE_MASK,

/*! Required input arguments 2
   1.  type: uint32_t, description: the unit # of the device you want to
       identify.
   2.  type: size_t, description: the length of the output buffer that will
       receive the device name.
       Output parameter: char*, description: name of the device. */
	PSM2_INFO_QUERY_UNIT_NAME,

/*! removed QUERY_DEVICE_UNIT_SYS_PATH, but kept placeholder to retain values in enum */
	PSM2_WAS_INFO_QUERY_UNIT_SYS_PATH,

/*! Required input arguments: 2
   1.  type: uint32_t, description: unit number for which the pci bus is wanted.
	   (use: psm2_info_query_arg_t.unit).
   2.  type: size_t, description: the length of the output buffer that will
       recieve the pci bus array which must be the size of 4x uint32_t.
	   (use: psm2_info_query_arg_t.length).
       Output parameter: uint32_t[4], description: the pci bus in BDF Notation
       with PCI Domain: { Domain, Bus, Device, Function } */
	PSM2_INFO_QUERY_UNIT_PCI_BUS,

/*! Required input arguments 4
   1.  type: uint32_t, description: the unit # of the device you want to
       identify.
   2.  type: uint32_t, description: the port for which name is
       desired (use: psm2_info_query_arg_t.port).
   3.  type: uint32_t, description: the address index for which name is
       desired (use: psm2_info_query_arg_t.addr_index).
   4.  type: size_t, description: the length of the output buffer that will
       receive the subnet name.
       Output parameter: char*, description: name of the subnet for the device. */
	PSM2_INFO_QUERY_UNIT_SUBNET_NAME,

/*! Required input arguments: 2
   1.  type: uint32_t, description: unit number for which the pci bus is wanted.
	   (use: psm2_info_query_arg_t.unit).
   2.  type: size_t, description: the length of the output buffer that will
       recieve the device ID, Device Version, Vendor ID, or Driver.
	   (use: psm2_info_query_arg_t.length).
       Output parameter: char *, description:
	   the device id: "0x1592"    cat /sys/class/infiniband/irdma0/device/device
	   the device version: "0x02" cat /sys/class/infiniband/irdma0/device/revision
	   the vendor id: "0x8086"    cat /sys/class/infiniband/irdma0/device/vendor
	   the driver: "ice"          basename `realpath /sys/class/infiniband/irdma0/device/driver` */
	PSM2_INFO_QUERY_UNIT_DEVICE_ID,
	PSM2_INFO_QUERY_UNIT_DEVICE_VERSION,
	PSM2_INFO_QUERY_UNIT_VENDOR_ID,
	PSM2_INFO_QUERY_UNIT_DRIVER,

/*! Required input arguments: 2
   1.  type: uint32_t, description: the unit for which speed is
       desired (use: psm2_info_query_arg_t.unit).
   2.  type: uint32_t, description: the port for which speed is
       desired (use: psm2_info_query_arg_t.port).
   Output parameter: uint64_t, description: port speed in bits per sec (bps) */
	PSM2_INFO_QUERY_PORT_SPEED,

/*! Required input arguments 0
   Output parameter: uint32_t*, description: the number of addresses per port */
	PSM2_INFO_QUERY_NUM_ADDR_PER_UNIT,

/*! Required input arguments 4
   1.  type: uint32_t, description: the unit # of the device you want to
       identify (use: psm2_info_query_arg_t.unit).
   2.  type: uint32_t, description: the port for which name is
       desired (use: psm2_info_query_arg_t.port).
   3.  type: uint32_t, description: the address index for which name is
       desired (use: psm2_info_query_arg_t.addr_index).
   4.  type: size_t, description: the length of the output buffer that will
       receive the device name.
       Output parameter: char*, description: name of the device's address. */
	PSM2_INFO_QUERY_UNIT_ADDR_NAME,


	PSM2_INFO_QUERY_LAST, /* must appear last, and the info query
				 constants are used as an index. */
} psm2_info_query_t;

enum psm2_info_query_feature_mask
{
	/*! The following bit means that the libpsm2 _can_ support cuda.
	    If the PSM2_INFO_QUERY_FEATURE_MASK request is made and
	    the PSM2_INFO_QUERY_FEATURE_CUDA bit is not present, then cuda
            is not supported. */
	PSM2_INFO_QUERY_FEATURE_CUDA      = (1 << 0),
	/* Intel ONEAPI Level-zero support for Intel GPU */
	PSM2_INFO_QUERY_FEATURE_ONEAPI    = (1 << 1),
};

/** @brief Union for info query arg type
 */
typedef union psm2_info_query_arg
{
	uint32_t                       unit;
	uint32_t                       port;
	uint32_t                       addr_index;
	size_t                         length;
	psm2_mq_t                      mq;
	psm2_epaddr_t                  epaddr;
} psm2_info_query_arg_t;

/** @brief PSM2 info query
 *
 * Function that allows a client to interrogate PSM2 for various information.
 *
 * @param[in] psm2_info_query_t  What information is requested.
 * @param[out] void * out, where the information will be delivered on a
 *                         PSM2_OK return.
 * @param[in] size_t nargs, the number of following arguments.
 * @param[in] psm2_info_query_arg_t [], The arguments that are required for
 *                                      certain queries.  See documentation
 *                                      at @ref psm2_info_query_t for what
 *                                      arguments are required for what
 *                                      queries as well as what the type
 *                                      the output is expected to be.
 *
 * @retval PSM2_OK The out buffer has successfully been written with the
 * result of the query.
 */
psm2_error_t psm3_info_query(psm2_info_query_t, void *out,
			     size_t nargs, psm2_info_query_arg_t []);

/*! @} */

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif
