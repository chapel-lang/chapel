record R0 {
  var home: locale = here;

  proc init() { }
}
proc R0.init(r1:R1) {
  on r1.home {
    this.home = r1.home;
  }
}

record R1 {
  var home: locale = here;
}
proc R1.myR0() {
  var ret: R0;
  on Locales[0] /* this.home */ {
    ret = new R0(this);
  }
  return ret;
}

proc f(l) {
  var ret: R1;
  ret.home = l;
  return ret;
}

const r0: R0 = f(Locales[numLocales-1]).myR0();
writeln(r0);
