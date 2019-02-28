config const N = 100;
var globalAtomicInt: atomic int;

// Explicit main included to reduce test maintenance
proc main() { }

proc blocking_loop_int_read() {
  while globalAtomicInt.read() == 0 { }
}

// how about task constructs with blocking operations?
proc begin_blocking() {
  begin {
    blocking_loop_int_read();
  }
}
begin_blocking();

proc begin_sync_blocking() {
  sync {
    begin {
      blocking_loop_int_read();
    }
  }
}
begin_sync_blocking();

proc cobegin_blocking() {
  cobegin {
    blocking_loop_int_read();
    blocking_loop_int_read();
  }
}
cobegin_blocking();

proc coforall_blocking() {
  coforall i in 1..N {
    blocking_loop_int_read();
  }
}
coforall_blocking();

proc on_blocking() {
  on Locales[numLocales-1] {
    blocking_loop_int_read();
  }
}
on_blocking();

proc coforall_on_blocking() {
  coforall i in 0..#numLocales {
    on Locales[i] {
      blocking_loop_int_read();
    }
  }
}
coforall_on_blocking();

proc forall_blocking() {
  forall i in 1..N {
    blocking_loop_int_read();
  }
}
forall_blocking();
