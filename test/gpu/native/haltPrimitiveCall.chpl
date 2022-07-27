config const shouldHalt = true;
proc f() {
  on here.getChild(1) {
    foreach i in 1..100 {
      if i == 42 && shouldHalt {
        __primitive("chpl_error", c"error error error");
      }
    }
  }
}

f();
