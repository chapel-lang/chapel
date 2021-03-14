// Test integer indefinite domain remove
var intd: domain(int);
for i in 1..10 {
  intd.add(i*2);
}

intd.remove(4);
intd.remove(8);
icheck(0);
icheck(2);
icheck(53);
icheck(92);
icheck(5);
icheck(6);
icheck(4);
intd.add(4);
icheck(4);
const retval = intd.remove(53);
writeln("# ind removed: ", retval);

proc icheck( i:int) {
  writeln( "contains(", i, ") = ", intd.contains( i));
}
