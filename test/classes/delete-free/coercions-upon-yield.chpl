
class Gen {
  type t;
}

var Glob = new borrowed Gen(real);

iter rets(): unmanaged Gen {
  yield Glob;
}

var x = rets();
compilerWarning(x.type:string);
compilerError("done");
