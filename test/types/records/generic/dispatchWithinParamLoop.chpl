record R {
  param flag = true;
}

proc foo(x: R) {
  writeln("In foo: ", x.type:string);
}

proc bar(x: R(true)) {
  writeln("In bar: ", x.type:string);
}

proc bar(x: R(false)) {
  writeln("In bar: ", x.type:string);
}

proc baz(x: R(?)) {
  writeln("In baz: ", x.type:string);
}

// Works:

{
  var r = new R(false);
  bar(r);
  baz(r);
}

{
  var r = new R(true);
  foo(r);
  bar(r);
  baz(r);
}

// Doesn't work

for param flag in false..true {
  var r = new R(flag);
  if flag == true then foo(r);
  bar(r);
  baz(r);
}

