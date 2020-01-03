config const cond = true;

record R { }

proc test() {
  var x: R;
  if cond then
    x = 1;
  else
    x = 1.0;

  writeln(x.type:string, " ", x);
}
test();
