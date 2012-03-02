#ifndef _COMM_TASK_DECLS_H_
#define _COMM_TASK_DECLS_H_

#define CHPL_COMM_HAS_NB

typedef void* chpl_comm_get_nb_token_t;
#define CHPL_COMM_NB_TOKEN_NULL ((chpl_comm_get_nb_token_t) 0)

#define CHPL_COMM_GET_NB(localvar, locale, addr, type, tid, len, ln, fn)  \
  do {                                                             \
    chpl_comm_get_nb(&(local), locale, addr,                       \
                     sizeof(type), tid, len, token, ln, fn);       \
  } while (0)

#define CHPL_COMM_TEST_GET_NB(token, ln, fn)                       \
  chpl_comm_test_get_nb(token, ln, fn)

#define CHPL_COMM_WAIT_GET_NB(token, ln, fn)                       \
  do {                                                             \
    chpl_comm_wait_get_nb(token, ln, fn);                          \
  } while (0)

//
// get 'size' bytes of remote data at 'raddr' on locale 'locale' to
// local data at 'addr'
// notes:
//   address is arbitrary
//   size and locale are part of p
//   token is the unique handle for the reference
//
void chpl_comm_get_nb(void *addr, int32_t locale, void* raddr,
                      int32_t elemSize, int32_t typeIndex, int32_t len,
                      chpl_comm_get_nb_token_t *token, int ln, chpl_string fn);

int chpl_comm_test_get_nb(chpl_comm_get_nb_token_t token,
                          int ln, chpl_string fn);

void chpl_comm_wait_get_nb(chpl_comm_get_nb_token_t token,
                           int ln, chpl_string fn);

#endif
