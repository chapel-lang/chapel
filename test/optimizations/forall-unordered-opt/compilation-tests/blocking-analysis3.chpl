config const N = 100;
var globalAtomicInt: atomic int;
var globalInt: int;

// Explicit main included to reduce test maintenance
proc main() { }

iter myiter() {
  for i in 1..N {
    yield i;
  }
}

// non-blocking cases
proc recursive_not_blocking(arg:int):int {
  if arg <=0 then
    return 0;

  return 1+recursive_not_blocking(arg-1);
}
recursive_not_blocking(N);

proc while_not_blocking() {
  while true {
  }
}
while_not_blocking();

proc do_while_not_blocking() {
  do {
  } while true;
}
do_while_not_blocking();

proc for_not_blocking(arg:int) {
  for i in 1..arg {
  }
}
for_not_blocking(N);

proc for_iter_not_blocking() {
  for x in myiter() {
  }
}
for_iter_not_blocking();

proc one_atomic_op() {
  globalAtomicInt.add(1);
}
one_atomic_op();

// task constructs by themselves should not be considered blocking
proc begin_only() {
  begin with (ref globalInt) {
    globalInt = 1;
  }
}
begin_only();

proc begin_sync() {
  sync {
    begin with (ref globalInt) {
      globalInt = 1;
    }
  }
}
begin_sync();

proc do_cobegin() {
  cobegin with (ref globalInt) {
    // yes this is a ridiculous race condition
    globalInt = 1;
    globalInt = 2;
  }
}
do_cobegin();

proc do_coforall() {
  coforall i in 1..N with (ref globalInt) {
    // yes this is a ridiculous race condition
    globalInt = i;
  }
}
do_coforall();

proc do_on() {
  on Locales[numLocales-1] {
    globalInt = N;
  }
}
do_on();

proc do_coforall_on() {
  coforall i in 0..#numLocales with (ref globalInt) {
    on Locales[i] {
      // yes this is a ridiculous race condition
      globalInt = i;
    }
  }
}
do_coforall_on();

proc do_forall() {
  forall i in 1..N with (ref globalInt) {
    // yes this is a ridiculous race condition
    globalInt = i;
  }
}
do_forall();
