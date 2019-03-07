
class Gen {
  type t;
}

var Glob = new borrowed Gen(real);

proc rets(): unmanaged Gen {
  return Glob;
}

var x = rets();
compilerWarning(x.type:string);
compilerError("done");
