proc show(r,m) {
  write(m, " ->");
  for i in r do write(" ", i);
  writeln();
}
proc test(r1,r2,m1,m2) {
  show(r1,m1);
  show(r2,m2);
  writeln(if r1==r2 then "EQUAL" else "NOT equal");
  writeln();
}

writeln("represented sequence = (0)\n");

test( 0..0 by 3 , 0..0 by 4 ,
     "0..0 by 3","0..0 by 4");

test( 0..1 by 3 , 0..1 by 4 ,
     "0..1 by 3","0..1 by 4");

test( 0..0 , 0..0 by -1 ,
     "0..0","0..0 by -1");

test( 0..1 by 3 , -1..0 by -3 align 0 ,
     "0..1 by 3","-1..0 by -3 align 0");

writeln("represented sequence is empty\n");

test( 1..0 , 1..0 by 2 ,
     "1..0","1..0 by 2");

test( 1..0 , 1..0 by -1 ,
     "1..0","1..0 by -1");

test( 1..0 , 1..0 by -2 ,
     "1..0","1..0 by -2");

test( 1..0 , 2..0 ,
     "1..0","2..0");

test( 1..0 , 1..-1 ,
     "1..0","1..-1");
