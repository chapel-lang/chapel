
class Gen {
  type t;
}

var Glob = (new owned Gen(real)).borrow();

iter rets(): unmanaged Gen {
  yield Glob;
}

var x = rets();
compilerWarning(x.type:string);
compilerError("done");
