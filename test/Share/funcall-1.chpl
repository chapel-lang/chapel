-- is this legal?? we have code like this with print on line 94 of SSCA #2

function f x : string {
  return x.length;
}

writeln("expect 5 ", f "hello");
