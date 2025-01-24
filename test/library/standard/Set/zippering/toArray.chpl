use Set;

var LocalSet= new set(int,parSafe = true);
LocalSet.add(1);
LocalSet.add(2);
LocalSet.add(3);
LocalSet.add(4);
LocalSet.add(5);

var A : [0..4] int;

writeln("A = ", A);
writeln("LocalSet = ", LocalSet);
forall (a,b) in zip (A,LocalSet.toArray()) {
  a=b;
}
writeln("A = ", A);
