record R {
  var x;
  proc type tt type {
    return int;
  }
}

proc f(type t, type u = t.tt) {
  writeln("t=", t:string, " u=", u:string);
}
f(R);

// see also test/classes/bradc/paramInClass/weirdParamInit4
