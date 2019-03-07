// Test record indefinite domain add/remove
type T = (int, int);

proc _indefinite_hash( t: T) {
  return 0;
}

var intd: domain(T);

for i in 1..10 {
  intd.add( (i, i));
}


intd.remove((4, 4));
intd.remove((8, 8));
icheck(0);
icheck(2);
icheck(53);
icheck(92);
icheck(5);
icheck(6);
icheck(4);
intd.add((4,4));
icheck(4);
const retval = intd.remove((53, 0));
writeln("# ind removed: ", retval);

proc icheck( i:int) {
  writeln( "contains(", (i,i), ") = ", intd.contains( (i,i)));
}
