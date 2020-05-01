config const cond = true;

var g: int;

proc test() {

  var a;
  if cond {
    a = 1;
  } else {
    g = 1; // oops, I meant a
  }
  compilerWarning(a.type:string);
  
  var b;
  if cond {
    b = 1.0;
  } else {
    g = 1; // oops, I meant b
  }
  compilerWarning(b.type:string);
}

test();
