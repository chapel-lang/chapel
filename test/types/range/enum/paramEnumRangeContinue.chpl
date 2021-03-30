enum number {one=1, two=2, three=3, four=4, five=5};

for param i in number.one..number.five {
  if (i:int %2 == 0) then
    continue;
  writeln(i);
}

writeln("---");

for param i in number.one..number.five {
  if (i:int %2 == 1) then
    continue;
  writeln(i);
}

writeln("---");

for param i in number.one..number.five {
  if (i:int %2 == 0) then
    break;
  writeln(i);
}

writeln("---");

for param i in number.one..number.five by -1 {
  if (i:int %2 == 0) then
    continue;
  writeln(i);
}

writeln("---");

for param i in number.one..number.five by -1 {
  if (i:int %2 == 1) then
    continue;
  writeln(i);
}

writeln("---");

for param i in number.one..number.five by -1 {
  if (i:int %2 == 0) then
    break;
  writeln(i);
}
