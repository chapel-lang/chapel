
class Gen {
  type t;
}

var Glob = (new owned Gen(real)).borrow();

proc rets(): unmanaged Gen {
  return Glob;
}

var x = rets();
compilerWarning(x.type:string);
compilerError("done");
