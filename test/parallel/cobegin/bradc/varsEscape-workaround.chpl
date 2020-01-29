class C {
  var left, right: unmanaged C?;
  
  proc countNodes(): int {
    var lnodes, rnodes: int;
    cobegin with (ref lnodes, ref rnodes) {
            lnodes = if left == nil then 0 else left!.countNodes();
            rnodes = if right == nil then 0 else right!.countNodes();
    }
    return lnodes + rnodes + 1;
  }
}

var c = new unmanaged C(
              new unmanaged C(),
              new unmanaged C(new unmanaged C(), new unmanaged C()));

writeln("c has ", c.countNodes(), " nodes");

delete c.left;
delete c.right!.left;
delete c.right!.right;
delete c.right;
delete c;
