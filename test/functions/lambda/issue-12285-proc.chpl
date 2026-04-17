record R {
  param size: uint;
}
record S {
  param size: uint;
  var r: R(size);
}

// used to error: illegal access of first class function
proc S.funcA(const functor): string {
  return functor(r);
}

// used to error: unresolved call 'S(10:uint(64)).funcB(chpl__fcf_type_R_10_string)'
proc S.funcB(const functor): string
where functor.argTypes == 1*R(size) && functor.retType == string {
  return functor(r);
}

record T {}

record U {
  var t: T;
}

// Okay
proc U.func(const functor): string
where functor.argTypes == 1*T && functor.retType == string {
  return functor(t);
}

proc main() {
  var s: S(10);
  writeln(s.funcA(proc(r: R(10)){ return r.type:string; }));
  writeln(s.funcB(proc(r: R(10)){ return r.type:string; }));

  var u: U;
  writeln(u.func(proc(t: T){ return t.type:string; }));
}
