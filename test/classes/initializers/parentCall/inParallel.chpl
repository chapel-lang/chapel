class ParaSuper {
  var f: uint;

  proc init(fVal: uint) {
    cobegin {
      super.init();
      f = fVal;
    }
  }
}

proc main() {
  var c: ParaSuper = new ParaSuper(2);
  writeln(c);
  delete c;
}
