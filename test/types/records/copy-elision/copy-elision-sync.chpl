class C { }
record R {
  var x: int = 0;
  var ptr: owned C = new owned C();
  proc init() {
    this.x = 0;
    writeln("init");
  }
  proc init(arg:int) {
    this.x = arg;
    writeln("init ", arg);
  }
  proc init=(other: R) {
    this.x = other.x;
    writeln("init= ", other.x);
  }
}
operator R.=(ref lhs:R, rhs:R) {
  writeln("= ", lhs.x, " ", rhs.x);
  lhs.x = rhs.x;
}


proc syncSplitInit() {
  writeln("syncSplitInit");
  var x: R;
 
  sync {
    x = new R(1); // split init applies to sync blocks
  }
}
syncSplitInit();

proc fOut(out arg: R) {
  arg = new R(2);
}

proc syncSplitInitOut() {
  writeln("syncSplitInitOut");
  var x: R;
 
  sync {
    fOut(x);
  }
}
syncSplitInitOut();


proc syncCopyElide() {
  writeln("syncCopyElide");
  var x = new R(1);
 
  sync {
    var y = x; // copy elision applies to sync blocks
  }
}
syncCopyElide();
