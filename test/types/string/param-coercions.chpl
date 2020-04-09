proc myadd(param a:string, param b:string) param {
  return a + b;
}

proc myadd(a:string, b:string) {
  return a + b;
}

proc run() {
  param a:c_string = "a";
  param b:c_string = "b";
  var c = myadd(a,b);
  compilerWarning(c.type:string);
  param d:c_string = a + b;
  compilerWarning(d:string);
  
  var a_cs = a;
  var b_cs = b;
  myadd(a_cs, b_cs); // don't expect this to resolve
}
run();
