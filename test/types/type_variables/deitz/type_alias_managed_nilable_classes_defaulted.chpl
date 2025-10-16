

class C {
  type t = int;
  var x: t;
}

proc foo(type t) {
  compilerWarning("t     is ", t:string);
  type inner = t(real);
  compilerWarning("inner is ", inner:string);
}

foo(unmanaged C(?)?);
foo(borrowed C(?)?);
foo(owned C(?)?);
foo(shared C(?)?);
foo(unmanaged C(?));
foo(borrowed C(?));
foo(owned C(?));
foo(shared C(?));
