record R {
  var x:int;
}


proc addone(in arg:R) {
  arg.x += 1;
  return arg;
}

var A:[1..4] R;
for i in 1..4 do
  A[i].x = i;

var B = addone(A);

writeln(A);
writeln(B);
