config param dropX = true;

proc bar(type T) {
  var x : T;
  if dropX then
    return 5;
  else
    return x;
}

config var flag = false;

{
  writeln("case 1");
  var buf = for i in 0..9 do bar([0..1] int);
  writeln(buf);
}

{
  writeln("case 2");
  for i in 0..9 do
    writeln(bar([0..1] int));
}

{
  writeln("case 3");
  if flag {
    writeln(bar([0..1] int));
  }
  else {
    writeln(bar([0..1] int));
  }
}

{
  writeln("case 4");
  var tripped = false;
  while !tripped {
    writeln(bar([0..1] int));
    tripped = true;
  }
}

{
  writeln("case 5");
  var tripped = false;
  do {
    writeln(bar([0..1] int));
    tripped = true;
  } while !tripped;
}
