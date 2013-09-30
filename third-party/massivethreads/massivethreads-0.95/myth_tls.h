/*
 * myth_tls.h
 *
 *  Created on: 2011/05/17
 *      Author: jnakashima
 */

#ifndef MYTH_TLS_H_
#define MYTH_TLS_H_

#include "myth_desc.h"

typedef unsigned int myth_key_t;

typedef struct myth_tls_entry
{
	myth_thread_t th;
	myth_key_t key;
	void *value;
}myth_tls_entry,*myth_tls_entry_t;

#endif /* MYTH_TLS_H_ */
