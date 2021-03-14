config var x: int;
writeln("x is: ", x);

{
  config var y: int;
  writeln("y is: ", y);
}

for i in 1..10 {
  config var z: int;
  writeln("z is: ", z);
}
