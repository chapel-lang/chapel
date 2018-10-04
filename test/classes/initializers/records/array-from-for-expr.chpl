
class CCC {
  const yyy: int;

  proc init(idy: int) {
    writeln("CCC.init ", idy);
    yyy = idy;
  }

  proc deinit() {
    writeln("CCC.deinit ", yyy);
  }
}

record RRR {
  const xxx: int;
  const cls: owned CCC;

  proc init(idx: int) {
    writeln("RRR.init ", idx);
    xxx = idx;
    cls = new owned CCC(idx);
  }

  proc deinit() {
    //delete cls; // not needed when it is 'owned'
    writeln("RRR.deinit ", xxx);
  }
}

proc helper(x: int) {
  return new RRR(x);
}

proc main {
  const array = for i in 1..3 do helper(i);
  writeln(array);
}
