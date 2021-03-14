class C { }
class D { type t; }
proc tm() type return C;
proc tn(type arg) type return D(arg);
type CN = C?;

param Cm = "C.meth";
proc C.meth param  return Cm;
proc type C.tmeth type  return D(int);
proc type (shared C).tmeth type  return D(real);
class X { var fld; }
proc type C.xmeth type  return X;

param Dtm = "D.tdmeth";
proc type D.tdmeth param  return Dtm;

var z20a = new C().tmeth;
compilerError(z20a.type:string);
var z20c = new C()?.tmeth;
compilerError(z20c.type:string);
