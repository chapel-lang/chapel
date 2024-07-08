/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2015 Intel Corporation.

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

  Copyright(c) 2015 Intel Corporation.

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

/* Copyright (c) 2003-2014 Intel Corporation. All rights reserved. */

#ifndef PSM2_AM_H
#define PSM2_AM_H

#include <stddef.h>
#include <stdint.h>
#include <psm2.h>

#ifndef PACK_SUFFIX
/* XXX gcc only */
#define PACK_SUFFIX __attribute__((packed))
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @file psm2_am.h
 * @brief PSM2 Active Message.
 *
 * @page psm2_am Active Message Interface
 *
 * PSM2 implements an Active Message (AM) component that lives alongside the
 * Matched Queues (MQ) component. The active message interface essentially
 * provides a remote procedure call mechanism. A PSM2 process can generate a
 * request to run an active message handler on a remote PSM2 process
 * identified by its end-point address (epaddr). End-point address values
 * are returned by PSM2 when connecting end-points using the psm3_ep_connect()
 * function.
 *
 * An AM handler may make local state updates, and may generate at most
 * one reply to be returned to the original requestor. This reply will cause
 * a handler to be run on that requestor. The requestor handler may make
 * local state updates but is not allowed to reply nor request in that handler
 * context. A request or reply can convey a small number of in-line arguments
 * plus a short amount of data. A tight bound is placed on the number of
 * in-line arguments to allow them to be packed into a header. A bound is
 * placed on the size of the data payload so that the request or reply can
 * be sent as a single packet within the MTU of the underlying communication
 * transport. Longer payloads must be synthesized on top of the provided
 * short request/reply mechanism by fragmentation and reassembly, or
 * transported by some other means.
 *
 * Handlers are run in the process context of the targeted PSM2 process,
 * either in its main thread of execution or in a progress thread. A handler
 * may therefore be executed concurrently with the main thread of execution
 * of the PSM2 process. PSM2 ensures that its own state is protected against this
 * concurrent execution. However, a handler must make its own arrangements to
 * protect its own state. Alternatively, the PSM2 progress thread can be
 * disabled using the PSM3_RCVTHREAD environment variable if this is too
 * onerous for the handler.
 *
 * PSM2 has an active progress model and requires that the PSM2 library is
 * called in order to make progress. This can be achieved using the psm3_poll()
 * function. A PSM2 implementatation may provide passive progress through some
 * other mechanism (e.g. a receive thread), but a PSM2 consumer must not assume
 * this and must arrange to make active progress through calls into the PSM
 * library. Note that the PSM2 AM interface is not MTsafe, same as the other PSM
 * interfaces, and that MTsafety must be provided by the consumer if required.
 *
 * The order in which AM requests are issued by an initiator to a particular
 * target defines the order in which those AM requests will be executed on
 * that target. Therefore the AM implementation will maintain the order
 * of handler executions on a flow, and this also applies when progress
 * threads are used. For multiple initiators issuing requests to a particular
 * target, the handler executions will be interleaved in some sequentially
 * consistent ordering.
 */

/*! @defgroup am PSM2 Active Message
 *
 * @{
 */

/** @brief Datatype for an index representing an active message handler */
typedef uint32_t psm2_handler_t;

/** @brief Datatype for a token for an active message handler.*/
typedef void *psm2_am_token_t;

/* PSM2 AM flags
 * These flags may be combined using bitwise-or.
 */
#define PSM2_AM_FLAG_NONE    0 /**< No other PSM2 AM flags are needed. */
#define PSM2_AM_FLAG_ASYNC   1 /**< No need to copy source data. */
#define PSM2_AM_FLAG_NOREPLY 2 /**< The handler for this AM request is
				   guaranteed not to generate a reply. */

/** @brief The psm2_amarg type represents the type of an AM argument. This is
 *  a 64-bit type and is broken down into four 16-bit fields, two 32-bit
 *  fields or one 64-bit field for the convenience of code using the PSM2 AM
 *  interface.
 */
typedef
struct psm2_amarg {
	union {
		struct {
			uint16_t u16w3;
			uint16_t u16w2;
			uint16_t u16w1;
			uint16_t u16w0;
		} PACK_SUFFIX;
		struct {
			uint32_t u32w1;
			uint32_t u32w0;
		} PACK_SUFFIX;
		uint64_t u64w0;
		uint64_t u64;
	};
} PACK_SUFFIX psm2_amarg_t;

/** @brief The AM handler function type
 *
 * psm2_am_handler_fn_t is the datatype for an AM handler. PSM2 AM will call-back
 * into an AM handler using this function prototype. The parameters and result
 * of these handler functions are described here.
 *
 * @param[in] token This is an opaque token value passed into a handler.
 *                  A request handler may send at most one reply back to the
 *                  original requestor, and must pass this value as the token
 *                  parameter to the psm3_am_reply_short() function. A reply
 *                  handler is also passed a token value, but must not attempt
 *                  to reply.
 * @param[in] args A pointer to the arguments provided to this handler.
 * @param[in] nargs The number of arguments.
 * @param[in] src A pointer to the data payload provided to this handler.
 * @param[in] len The length of the data payload in bytes.
 *
 * @returns 0 The handler should always return a result of 0.
 */
typedef
int (*psm2_am_handler_fn_t) (psm2_am_token_t token,
			    psm2_amarg_t *args, int nargs,
			    void *src, uint32_t len);

/** @brief The AM handler function type with caller context
 *
 * psm2_am_handler_2_fn_t is the datatype for an AM handler that
 * includes a user context. PSM2 AM will call-back into an AM handler using
 * this function prototype. The parameters and result
 * of these handler functions are described here.
 *
 * @param[in] token This is an opaque token value passed into a handler.
 *                  A request handler may send at most one reply back to the
 *                  original requestor, and must pass this value as the token
 *                  parameter to the psm3_am_reply_short() function. A reply
 *                  handler is also passed a token value, but must not attempt
 *                  to reply.
 * @param[in] args A pointer to the arguments provided to this handler.
 * @param[in] nargs The number of arguments.
 * @param[in] src A pointer to the data payload provided to this handler.
 * @param[in] len The length of the data payload in bytes.
 * @param[in] hctx The user context pointer provided at handler registration.
 *
 * @returns 0 The handler should always return a result of 0.
 */
typedef
int (*psm2_am_handler_2_fn_t) (psm2_am_token_t token,
			    psm2_amarg_t *args, int nargs,
			    void *src, uint32_t len, void *hctx);

/** @brief Type for a completion call-back handler.
 *
 * A completion handler can be specified to give a call-back on the initiation
 * side that an AM request or reply has completed on the target side. The
 * call-back has a context pointer which is provided along with the call-back
 * function pointer when the initiator generates the request or reply. This
 * approach will typically give higher performance than using an AM request or
 * reply to achieve the same effect, though note that no additional information
 * can be passed from the target side back to the initiator side with the
 * completion handler approach.
 *
 * @param[in] context A context pointer.
 * @returns void This handler has no return result.
 */
typedef
void (*psm2_am_completion_fn_t) (void *context);

/** @brief Register AM call-back handlers at the specified end-point.
 *
 * This function is used to register an array of handlers, and may be called
 * multiple times to register additonal handlers. The maximum number of
 * handlers that can be registered is limited to the max_handlers value
 * returned by psm3_am_get_parameters(). Handlers are associated with a PSM
 * end-point. The handlers are allocated index numbers in the the handler table
 * for that end-point.  The allocated index for the handler function in
 * handlers[i] is returned in handlers_idx[i] for i in (0, num_handlers]. These
 * handler index values are used in the psm3_am_request_short() and
 * psm3_am_reply_short() functions.
 *
 * @param[in] ep End-point value
 * @param[in] handlers Array of handler functions
 * @param[in] num_handlers Number of handlers (sizes the handlers and
 *                         handlers_idx arrays)
 * @param[out] handlers_idx Used to return handler index mapping table
 *
 * @returns PSM2_OK Indicates success
 * @returns PSM2_EP_NO_RESOURCES Insufficient slots in the AM handler table
 */
psm2_error_t psm3_am_register_handlers(psm2_ep_t ep,
				     const psm2_am_handler_fn_t *
				     handlers, int num_handlers,
				     int *handlers_idx);

/** @brief Register AM call-back handlers at the specified end-point.
 *
 * This function is used to register an array of handlers, and may be called
 * multiple times to register additonal handlers. The maximum number of
 * handlers that can be registered is limited to the max_handlers value
 * returned by psm3_am_get_parameters(). Handlers are associated with a PSM
 * end-point. The handlers are allocated index numbers in the the handler table
 * for that end-point.  The allocated index for the handler function in
 * handlers[i] is returned in handlers_idx[i] for i in (0, num_handlers]. These
 * handler index values are used in the psm3_am_request_short() and
 * psm3_am_reply_short() functions.
 *
 * @param[in] ep End-point value
 * @param[in] handlers Array of handler functions
 * @param[in] num_handlers Number of handlers (sizes the handlers and
 *                         handlers_idx arrays)
 * @param[in] hctx Array of void* pointers to a user contexts for identifying the
 *                         target ep that registered these handlers.
 * @param[out] handlers_idx Used to return handler index mapping table
 *
 * @returns PSM2_OK Indicates success
 * @returns PSM2_EP_NO_RESOURCES Insufficient slots in the AM handler table
 */
psm2_error_t psm3_am_register_handlers_2(psm2_ep_t ep,
				     const psm2_am_handler_2_fn_t *
				     handlers, int num_handlers,
				     void **hctx,
				     int *handlers_idx);

/** @brief Unregister all AM call-back handlers for the specific end-point.
 *
 * This function is used to unregister all AM handlers registered to the
 * specified end-point.
 *
 * @param[in] ep End-point value
 *
 */
void psm3_am_unregister_handlers(psm2_ep_t ep);

/** @brief Generate an AM request.
 *
 * This function generates an AM request causing an AM handler function to be
 * called in the PSM2 process associated with the specified end-point address.
 * The number of arguments is limited to max_nargs and the payload length in
 * bytes to max_request_short returned by the psm3_am_get_parameters() function.
 * If arguments are not required, set the number of arguments to 0 and the
 * argument pointer will not be dereferenced. If payload is not required, set
 * the payload size to 0 and the payload pointer will not be dereferenced.
 *
 * Optionally a completion function and completion context pointer can be
 * provided, and a local call-back will be made to that function passing in
 * that context pointer once remote execution of the handler has completed. If
 * the completion call-back is not required, the handler should be specified as
 * NULL and the pointer value will not be used.
 *
 * The allowed flags are any combination of the following combined with
 * bitwise-or:
 *   PSM2_AM_FLAG_NONE    - No flags
 *   PSM2_AM_FLAG_ASYNC   - Indicates no need to copy source data
 *   PSM2_AM_FLAG_NOREPLY - The handler for this AM request is guaranteed not to
 *                         generate a reply
 *
 * The PSM2 AM implementation will not dereference the args pointer after return
 * from this function. If PSM2_AM_FLAG_ASYNC is not provided, the PSM2 AM
 * implementation will not dereference the src pointer after return from this
 * function. This may require the implementation to take a copy of the payload
 * if the request cannot be issued immediately.  However, if PSM2_AM_FLAG_ASYNC
 * is provided then a copy will not be taken and the PSM2 AM implementation
 * retains ownership of the payload src memory until the request is locally
 * complete. Local completion can be determined using the completion handler
 * call-back, or through an AM handler associated with an AM reply.
 *
 * The PSM2_AM_FLAG_NOREPLY flag indicates ahead of time to the AM handler that
 * a reply will not be generated. Use of this flag is optional, but it may
 * enable a performance optimization in this case by indicating that reply
 * state is not required.
 *
 * @param[in] epaddr End-point address to run handler on
 * @param[in] handler Index of handler to run
 * @param[in] args Array of arguments to be provided to the handler
 * @param[in] nargs Number of arguments to be provided to the handler
 * @param[in] src Pointer to the payload to be delivered to the handler
 * @param[in] len Length of the payload in bytes
 * @param[in] flags These are PSM2 AM flags and may be combined together with
 *                  bitwise-or
 * @param[in] completion_fn The completion function to called locally when
 *                          remote handler is complete
 * @param[in] completion_ctxt User-provided context pointer to be passed to the
 *                            completion handler
 *
 * @returns PSM2_OK indicates success.
 */
psm2_error_t
psm3_am_request_short(psm2_epaddr_t epaddr, psm2_handler_t handler,
		     psm2_amarg_t *args, int nargs, void *src,
		     size_t len, int flags,
		     psm2_am_completion_fn_t completion_fn,
		     void *completion_ctxt);

/** @brief Generate an AM reply.
 *
 * This function may only be called from an AM handler called due to an AM
 * request.  If the AM request uses the PSM2_AM_FLAG_NOREPLY flag, the AM
 * handler must not call this function. Otherwise, the AM request handler may
 * call psm3_am_reply_short() at most once, and must pass in the token value
 * that it received in its own handler call-back.
 *
 * This function generates an AM reply causing an AM handler function to be
 * called in the PSM2 process associated with the specified end-point address.
 * The number of arguments is limited to max_nargs and the payload length in
 * bytes to max_reply_short returned by the psm3_am_get_parameters() function.
 * If arguments are not required, set the number of arguments to 0 and the
 * argument pointer will not be dereferenced. If payload is not required, set
 * the payload size to 0 and the payload pointer will not be dereferenced.
 *
 * Optionally a completion function and completion context pointer can be
 * provided, and a local call-back will be made to that function passing in
 * that context pointer once remote execution of the handler has completed. If
 * the completion call-back is not required, the handler should be specified as
 * NULL and the pointer value will not be used.
 *
 * The allowed flags are any combination of the following combined with
 * bitwise-or:
 *   PSM2_AM_FLAG_NONE    - No flags
 *   PSM2_AM_FLAG_ASYNC   - Indicates no need to copy source data
 *
 * The PSM2 AM implementation will not dereference the args pointer after return
 * from this function. If PSM2_AM_FLAG_ASYNC is not provided, the PSM2 AM
 * implementation will not dereference the src pointer after return from this
 * function. This may require the implementation to take a copy of the payload
 * if the reply cannot be issued immediately.  However, if PSM2_AM_FLAG_ASYNC is
 * provided then a copy will not be taken and the PSM2 AM implementation retains
 * ownership of the payload src memory until the reply is locally complete.
 * Local completion can be determined using the completion handler call-back.
 *
 * @param[in] token Token value provided to the AM handler that is generating
 *                  the reply.
 * @param[in] handler Index of handler to run
 * @param[in] args Array of arguments to be provided to the handler
 * @param[in] nargs Number of arguments to be provided to the handler
 * @param[in] src Pointer to the payload to be delivered to the handler
 * @param[in] len Length of the payload in bytes
 * @param[in] flags These are PSM2 AM flags and may be combined together with
 *                  bitwise-or
 * @param[in] completion_fn The completion function to called locally when
 *                          remote handler is complete
 * @param[in] completion_ctxt User-provided context pointer to be passed to the
 *                            completion handler
 *
 * @returns PSM2_OK indicates success.
 */
psm2_error_t
psm3_am_reply_short(psm2_am_token_t token, psm2_handler_t handler,
		   psm2_amarg_t *args, int nargs, void *src,
		   size_t len, int flags,
		   psm2_am_completion_fn_t completion_fn,
		   void *completion_ctxt);

/** @brief Return the source end-point address for a token.
 *
 * This function is used to obtain the epaddr object representing the message
 * initiator from a token passed by PSM2 to a message handler.
 *
 * @param[in] token Token value provided to the AM handler that is generating
 *                  the reply.
 * @param[out] epaddr_out Pointer to the where the epaddr should be returned.
 *
 * @returns PSM2_OK indicates success.
 * @returns PSM2_PARAM_ERR token is invalid or epaddr_out is NULL.
 */
psm2_error_t psm3_am_get_source(psm2_am_token_t token,
			      psm2_epaddr_t *epaddr_out);

/** @brief AM parameters
 *
 * This structure is used to return PSM2 AM implementation-specific parameter
 * values back to the caller of the psm3_am_get_parameters() function. This
 * API also specifies the minimum values for these parameters that an
 * implementation must at least provide:
 *   max_handlers >= 64,
 *   max_nargs >= 2,
 *   max_request_short >= 256 and
 *   max_reply_short >= 256.
 */
struct psm2_am_parameters {
	/** Maximum number of handlers that can be registered. */
	uint32_t max_handlers;
	/** Maximum number of arguments to an AM handler. */
	uint32_t max_nargs;
	/** Maximum number of bytes in a request payload. */
	uint32_t max_request_short;
	/** Maximum number of bytes in a reply payload. */
	uint32_t max_reply_short;
};

/** @brief Get the AM parameter values
 *
 * This function retrieves the implementation-specific AM parameter values for
 * the specified end-point.
 *
 * @param[in] ep The end-point value returned by psm3_ep_open().
 * @param[out] parameters Pointer to the struct where the parameters will be
 *                        returned.
 * @param[in] sizeof_parameters_in The size in bytes of the struct provided by
 *                                 the caller.
 * @param[out] sizeof_parameters_out The size in bytes of the struct returned
 *                                   by PSM.
 *
 * @returns PSM2_OK indicates success.
 */
psm2_error_t
psm3_am_get_parameters(psm2_ep_t ep,
		      struct psm2_am_parameters *parameters,
		      size_t sizeof_parameters_in,
		      size_t *sizeof_parameters_out);

/*! @} */

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif
