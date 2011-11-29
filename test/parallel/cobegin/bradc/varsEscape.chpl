class C {
  var left, right: C;
  
  proc countNodes(): int {
    if (this == nil) then return 0;

    cobegin {
      const lnodes = left.countNodes();
      const rnodes = right.countNodes();
    }
    return lnodes + rnodes + 1;
  }
}

var c = new C(C(), C(C(), C()));

writeln("c has ", c.countNodes(), " nodes");

