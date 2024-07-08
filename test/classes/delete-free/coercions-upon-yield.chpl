
class Gen {
  type t;
}

var Glob = new Gen(real);

iter rets(): unmanaged Gen(?) {
  yield Glob.borrow();
}

var x = rets();
compilerWarning(x.type:string);
compilerError("done");
