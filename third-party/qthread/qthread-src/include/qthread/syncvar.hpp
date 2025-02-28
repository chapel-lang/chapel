#ifndef QTHREAD_SYNCVAR_HPP
#define QTHREAD_SYNCVAR_HPP

#include <assert.h>

#include "qthread.h"

class syncvar;

class syncvar {
public:
  inline syncvar(void) noexcept {
    // Doing it this way because extended initializers (e.g.
    // SYNCVAR_STATIC_INITIALIZER) are not (yet) supported by C++
    the_syncvar_t.u.w = 0;
  }

  inline syncvar(uint64_t const &val) noexcept {
    assert(!(val & 0xf000000000000000ull));
    the_syncvar_t.u.s.data = val;
  }

  inline syncvar(syncvar const &val) noexcept {
    the_syncvar_t.u.w = val.the_syncvar_t.u.w;
  }

  inline syncvar(syncvar_t const &val) { the_syncvar_t.u.w = val.u.w; }

  int empty(void) { return qthread_syncvar_empty(&the_syncvar_t); }

  int fill(void) { return qthread_syncvar_fill(&the_syncvar_t); }

  uint64_t readFF(void) {
    uint64_t ret = 0;
#ifndef NDEBUG
    int status = readFF(&ret);
    assert(status == QTHREAD_SUCCESS);
#else
    readFF(&ret);
#endif
    return ret;
  }

  int readFF(uint64_t *const dest) {
    return qthread_syncvar_readFF(dest, &the_syncvar_t);
  }

  uint64_t readFE(void) {
    uint64_t ret = 0;
#ifndef NDEBUG
    int status = readFE(&ret);
    assert(status == QTHREAD_SUCCESS);
#else
    readFE(&ret);
#endif
    return ret;
  }

  int readFE(uint64_t *const dest) {
    return qthread_syncvar_readFE(dest, &the_syncvar_t);
  }

  /*********/
  int writeF(uint64_t src) {
    assert(!(src & 0xf000000000000000ull));
    return qthread_syncvar_writeF_const(&the_syncvar_t, src);
  }

  int writeEF(uint64_t const src) {
    assert(!(src & 0xf000000000000000ull));
    return qthread_syncvar_writeEF_const(&the_syncvar_t, src);
  }

  uint64_t incr(int64_t inc = 1) {
    return qthread_syncvar_incrF(&the_syncvar_t, inc);
  }

  int status() { return qthread_syncvar_status(&the_syncvar_t); }

  uint64_t read() const { return the_syncvar_t.u.s.data; }

  void write(uint64_t const src) {
    assert(!(src & 0xf000000000000000ull));
    the_syncvar_t.u.s.data = src;
  }
protected:
  syncvar_t the_syncvar_t;
};

#endif
