config const cond = true;

proc test() {
  var x: int;
  if cond then
    x = "one";
  else
    x = "two";

  writeln(x.type:string, " ", x);
}
test();
