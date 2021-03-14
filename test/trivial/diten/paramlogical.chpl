config param abool = false;
proc foo(param i:int, param j:int) param return i/j > 2;

param x = abool && foo(3,0);
param y = !abool || foo(4, 0);
param z = (x || y) && (y || foo(3,0));

writeln(x, " ", y, " ", z);

