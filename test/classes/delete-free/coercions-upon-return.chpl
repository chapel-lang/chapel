
class Gen {
  type t;
}

var Glob = new Gen(real);

proc rets(): unmanaged Gen(?) {
  return Glob.borrow();
}

var x = rets();
compilerWarning(x.type:string);
compilerError("done");
