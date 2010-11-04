#ifndef _COMM_NB_MACROS_H_
#define _COMM_NB_MACROS_H_

#define CHPL_COMM_HAS_NB

typedef void *chpl_comm_get_nb_token_t;

#define CHPL_COMM_GET_NB(local, wide, type, ln, fn, token)   \
  do {                                                       \
    chpl_comm_get_nb(&(local), (wide).locale, (wide).addr,   \
                     SPECIFY_SIZE(type), token, ln, fn);    \
  } while (0)

#define CHPL_COMM_TEST_GET_NB(token, ln, fn)                  \
  chpl_comm_test_get_nb(token, ln, fn)

#define CHPL_COMM_WAIT_GET_NB(token, ln, fn)                  \
  do {                                                        \
    chpl_comm_wait_get_nb(token, ln, fn);                     \
  } while (0)

//
// get 'size' bytes of remote data at 'raddr' on locale 'locale' to
// local data at 'addr'
// notes:
//   address is arbitrary
//   size and locale are part of p
//   token is the handle returned by GASNet
//
void chpl_comm_get_nb(void *addr, int32_t locale, void* raddr, int32_t size,
                      chpl_comm_get_nb_token_t *token, int ln, chpl_string fn);

int chpl_comm_test_get_nb(chpl_comm_get_nb_token_t *token,
                          int ln, chpl_string fn);

void chpl_comm_wait_get_nb(chpl_comm_get_nb_token_t *token,
                           int ln, chpl_string fn);

#endif
