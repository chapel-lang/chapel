proc foo(x) {
  writeln("In foo with type ", x.type:string);
}

record R {
  param i: int;
}

for param i in 1..257 {
  var myR = new R(i);
  foo(myR);
}
