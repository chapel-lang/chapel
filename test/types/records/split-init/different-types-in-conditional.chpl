config const cond = true;

proc test() {

  var a;
  if cond {
    a = 1;
  } else {
    a = "hello";
  }
  
  var b;
  if cond {
    b = 1.0;
  } else {
    b = 1;
  }

  var c:int;
  if cond {
    c = 1.0;
  } else {
    c = "hello";
  }
}

test();
