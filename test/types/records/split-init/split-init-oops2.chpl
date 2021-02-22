config const cond = true;

var g: int;

proc test() {
  var b;
  if cond {
    b = 1.0;
  } else {
    g = 1; // oops, I meant b
  }
  compilerWarning(b.type:string);
}

test();
