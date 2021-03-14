config const cond = true;

proc test() {
  var uu = 1:uint(8);

  var x: int;
  if cond then
    x = 1:uint(8);
  else
    x = uu;

  writeln(x.type:string, " ", x);
}
test();
