/*
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


// Expects the following to be defined
//  BUFFERED_OP_NAME
//  BUFFERED_OP_REMOTE_TERM
//  BUFFERED_OP_{GET,AMO}
//  BUFFERED_OP_BUFFER_SIZE
//  BUFFERED_VARS_EPHEMERAL
//  BUFFERED_VARS_TYPED_EPHEMERAL


/*********************************************************/

#define CONCAT2(a, b) a ## b
#define CONCAT(a, b) CONCAT2(a, b)

#define BUFFERED_THREAD_STRUCT_TYPE CONCAT(BUFFERED_OP_NAME, _buff_thread_info_t)
#define BUFFERED_THREAD_GLOBAL_TYPE CONCAT(BUFFERED_OP_NAME, _buff_global_info_t)

#define BUFFERED_THREAD_STRUCT_NAME CONCAT(BUFFERED_OP_NAME, _buff_thread_info)
#define BUFFERED_THREAD_GLOBAL_NAME CONCAT(BUFFERED_OP_NAME, _buff_global_info)


// Expose the names:
// - <op>_buff_thread_info_init
// - <op>_buff_thread_info_flush
// - <op>_buff_thread_info_destroy
// - <remote><op>_buff_init
// - <remote><op>_buff_flush
// - <remote><op>_buff_task_flush
// - do_<remote><op>_buff_i
#define BUFFERED_THREAD_INFO_INIT CONCAT(BUFFERED_OP_NAME, _buff_thread_info_init)
#define BUFFERED_THREAD_INFO_FLUSH CONCAT(BUFFERED_OP_NAME, _buff_thread_info_flush)
#define BUFFERED_THREAD_INFO_DESTROY CONCAT(BUFFERED_OP_NAME, _buff_thread_info_destroy)
#define BUFFERED_INIT CONCAT(CONCAT(BUFFERED_OP_REMOTE_TERM, BUFFERED_OP_NAME), _buff_init)
#define BUFFERED_FLUSH CONCAT(CONCAT(BUFFERED_OP_REMOTE_TERM, BUFFERED_OP_NAME), _buff_flush)
#define BUFFERED_TASK_FLUSH CONCAT(CONCAT(BUFFERED_OP_REMOTE_TERM, BUFFERED_OP_NAME), _buff_task_flush)
#define BUFFERED_OP_INTERNAL CONCAT(CONCAT(CONCAT(do_, BUFFERED_OP_REMOTE_TERM), BUFFERED_OP_NAME), _buff_i)


typedef struct BUFFERED_THREAD_STRUCT_TYPE {
  int vi;
  spinlock lock;
  chpl_bool inited;
  struct BUFFERED_THREAD_STRUCT_TYPE* next;

#define _DECL(var) var##_v[BUFFERED_OP_BUFFER_SIZE];
  BUFFERED_VARS_TYPED_EPHEMERAL(_DECL, _DECL)
#undef _DECL
} BUFFERED_THREAD_STRUCT_TYPE;

typedef struct {
  BUFFERED_THREAD_STRUCT_TYPE* list;
  rwlock lock;
  pthread_key_t destructor_key;
} BUFFERED_THREAD_GLOBAL_TYPE;

static __thread BUFFERED_THREAD_STRUCT_TYPE BUFFERED_THREAD_STRUCT_NAME;

static BUFFERED_THREAD_GLOBAL_TYPE BUFFERED_THREAD_GLOBAL_NAME;

static
inline
void BUFFERED_THREAD_INFO_FLUSH(BUFFERED_THREAD_STRUCT_TYPE* info) {
  if (info->vi > 0) {
    // TODO shouldn't be hardcoded here
#if BUFFERED_OP_GET == 1
    do_remote_get_V(info->vi, info->tgt_addr_v, info->locale_v,
                    info->remote_mr_v, info->src_addr_v, info->size_v,
                    info->local_mr_v, may_proxy_true);
#endif
    info->vi = 0;
  }
}

static
void BUFFERED_THREAD_INFO_INIT(BUFFERED_THREAD_STRUCT_TYPE* info) {
  rwlock_writer_lock(&BUFFERED_THREAD_GLOBAL_NAME.lock);
  // need to recheck now that we have to lock
  if (!info->inited) {
    spinlock_init(&info->lock);
    info->vi = 0;

    // add thread to linked list
    info->next = BUFFERED_THREAD_GLOBAL_NAME.list;
    BUFFERED_THREAD_GLOBAL_NAME.list = info;

    // dummy key binding needed so key destructor is called
    pthread_setspecific(BUFFERED_THREAD_GLOBAL_NAME.destructor_key, info);

    info->inited = true;
  }
  rwlock_unlock(&BUFFERED_THREAD_GLOBAL_NAME.lock);
}


static
void BUFFERED_THREAD_INFO_DESTROY(void* p) {
  BUFFERED_THREAD_STRUCT_TYPE* info = &BUFFERED_THREAD_STRUCT_NAME;

  // remove the thread from the linked list
  rwlock_writer_lock(&BUFFERED_THREAD_GLOBAL_NAME.lock);
  BUFFERED_THREAD_STRUCT_TYPE* global_info = BUFFERED_THREAD_GLOBAL_NAME.list;
  if (info == global_info) {
    BUFFERED_THREAD_GLOBAL_NAME.list = info->next;
  } else {
    while (global_info != NULL) {
      if (info == global_info->next) {
        global_info->next = info->next;
        break;
      }
      global_info = global_info->next;
    }
  }
  rwlock_unlock(&BUFFERED_THREAD_GLOBAL_NAME.lock);

  // flush any pending ops
  spinlock_lock(&info->lock);
  BUFFERED_THREAD_INFO_FLUSH(info);
  spinlock_unlock(&info->lock);
  spinlock_destroy(&info->lock);
}

static
void BUFFERED_INIT(void) {
  BUFFERED_THREAD_GLOBAL_NAME.list = NULL;
  rwlock_init(&BUFFERED_THREAD_GLOBAL_NAME.lock);
  pthread_key_create(&BUFFERED_THREAD_GLOBAL_NAME.destructor_key, BUFFERED_THREAD_INFO_DESTROY);
}


// Flush buffered OPS operations for all threads
static
inline
void BUFFERED_FLUSH(void) {
  BUFFERED_THREAD_STRUCT_TYPE* info;

  rwlock_reader_lock(&BUFFERED_THREAD_GLOBAL_NAME.lock);
  info = BUFFERED_THREAD_GLOBAL_NAME.list;
  while (info != NULL) {
    spinlock_lock(&info->lock);
    BUFFERED_THREAD_INFO_FLUSH(info);
    spinlock_unlock(&info->lock);
    info = info->next;
  }
  rwlock_unlock(&BUFFERED_THREAD_GLOBAL_NAME.lock);
}

static
inline
void BUFFERED_TASK_FLUSH(void) {
  if (chpl_task_canMigrateThreads()) {
    BUFFERED_FLUSH();
  } else {
    BUFFERED_THREAD_STRUCT_TYPE* info = &BUFFERED_THREAD_STRUCT_NAME;
    // Safe to check inited/vi outside the lock because no other thread can be
    // modifying them, but concurrent flushing from other threads is possible.
    if (info->inited && info->vi > 0) {
      spinlock_lock(&info->lock);
      BUFFERED_THREAD_INFO_FLUSH(info);
      spinlock_unlock(&info->lock);
    }
  }
}


static
inline
#define _COMMA(var) var,
#define _NOCOMMA(var) var
void BUFFERED_OP_INTERNAL(BUFFERED_VARS_TYPED_EPHEMERAL(_COMMA, _NOCOMMA)) {
#undef _COMMA
#undef _NO_COMMA

  BUFFERED_THREAD_STRUCT_TYPE* info = &BUFFERED_THREAD_STRUCT_NAME;

  if (!info->inited) {
    BUFFERED_THREAD_INFO_INIT(info);
  }

  // grab lock for this thread
  spinlock_lock(&info->lock);

  int vi = info->vi;

#define _ASSIGN(var) info->var##_v[vi] = var;
  BUFFERED_VARS_EPHEMERAL(_ASSIGN)
#undef _ASSIGN

  info->vi++;

  // flush if buffers are full
  if (info->vi == BUFFERED_OP_BUFFER_SIZE) {
    BUFFERED_THREAD_INFO_FLUSH(info);
  }

  // release lock for this thread
  spinlock_unlock(&info->lock);
}

#undef CONCAT2
#undef CONCAT

#undef BUFFERED_THREAD_STRUCT_TYPE
#undef BUFFERED_THREAD_GLOBAL_TYPE
#undef BUFFERED_THREAD_STRUCT_NAME
#undef BUFFERED_THREAD_GLOBAL_NAME

#undef BUFFERED_THREAD_INFO_INIT
#undef BUFFERED_THREAD_INFO_FLUSH
#undef BUFFERED_THREAD_INFO_DESTROY
#undef BUFFERED_INIT
#undef BUFFERED_FLUSH
#undef BUFFERED_TASK_FLUSH
#undef BUFFERED_OP_INTERNAL
