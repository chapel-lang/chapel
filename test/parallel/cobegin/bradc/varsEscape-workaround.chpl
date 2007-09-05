class C {
  var left, right: C;
  
  def countNodes(): int {
    if (this == nil) then return 0;
    var lnodes, rnodes: int;
    cobegin {
            lnodes = left.countNodes();
            rnodes = right.countNodes();
    }
    return lnodes + rnodes + 1;
  }
}

var c = C(C(), C(C(), C()));

writeln("c has ", c.countNodes(), " nodes");

