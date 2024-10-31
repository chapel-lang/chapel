record R {
  param flag = true;
}

proc bar(x: R(true)) {
  writeln("In bar: ", x.type:string);
}

proc bar(x: R(false)) {
  writeln("In bar: ", x.type:string);
}

for param flag in false..true {
  var r = new R(flag);
  bar(r);
}
