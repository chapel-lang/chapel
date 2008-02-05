var (x,y) = stdin.read(int, real);

writeln("x is: ", x);
writeln("y is: ", y);

def file.readln(type t ...?numTypes) where numTypes > 1 {
  var tupleVal: t;
  for param i in 1..numTypes-1 do
    tupleVal(i) = this.read(t(i));
  tupleVal(numTypes) = this.readln(t(numTypes));
  return tupleVal;
}

def file.read(type t ...?numTypes) where numTypes > 1 {
  var tupleVal: t;
  for param i in 1..numTypes do
    tupleVal(i) = this.read(t(i));
  return tupleVal;
}

def readln(type t ...?numTypes) where numTypes > 1
  return stdin.readln(t);

def read(type t ...?numTypes) where numTypes > 1
  return stdin.read(t);
