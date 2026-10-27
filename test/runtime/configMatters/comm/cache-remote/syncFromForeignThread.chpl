// Sync variable operations from a thread the runtime didn't create shouldn't
// crash
use CTypes;
require "pthread.h";

extern type pthread_t;
extern proc pthread_create(thread: c_ptr(pthread_t), attr: c_ptr(void),
                           start: c_fn_ptr, arg: c_ptr(void)): c_int;
extern proc pthread_join(thread: pthread_t,
                         retval: c_ptr(c_ptr(void))): c_int;

class channel {
  var request: sync int;
  var reply: sync int;
  proc init() { }
}

proc fromForeignThread(arg: c_ptr(void)): c_ptr(void) {
  var ch = (arg: unmanaged channel?)!;
  const x = ch.request.readFE();
  ch.reply.writeEF(x + 1);
  return nil;
}

coforall loc in Locales do on loc {
  var ch = new unmanaged channel();
  defer delete ch;
  var t: pthread_t;
  const rc = pthread_create(c_ptrTo(t), nil, c_ptrTo(fromForeignThread),
                            c_ptrTo(ch): c_ptr(void));
  assert(rc == 0);
  ch.request.writeEF(41);
  assert(ch.reply.readFE() == 42);
  pthread_join(t, nil);
}

writeln("done");
