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
}

test();
