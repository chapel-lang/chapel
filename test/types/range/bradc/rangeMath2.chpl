const r = 1..4;

const r2 = r*4 + 1;

writeln("r is: ", r);
writeln("r2 is: ", r2);

def *(r:range, i) {
  writeln("In my multiply routine");
  return r;
}

def +(r:range, i) {
  writeln("In my addition routine");
  return r;
}
