testit(42..#0);
testit(1..#0);
testit(0..#0);
testit(1:uint..#0);
testit(0:uint..#0);
testit(0:uint(8)..#0);

testit(..42#0);
testit(..1#0);
testit(..0#0);
testit(..1:uint#0);
testit(..0:uint#0);
testit(..max(uint)#0);
testit(..max(uint(8))#0);

testit((42..by 2)#0);
testit((42..by 2 align 43)#0);
testit((1.. by 2)#0);
testit((1:uint(8).. by 2)#0);
testit((0:uint(8).. by 2)#0);
testit((1:uint(8).. by 3)#0);
testit((1:uint(8).. by 3 align 0)#0);

testit((..42 by -2)#0);
testit((..42 by -2 align 41)#0);
testit((..1  by -2)#0);
testit((..max(uint(8)) by -2)#0);


proc testit(r) {
  writeln(r, ": ", r.type:string);
  if (r.low > min(r.idxType)+2 || r.high < max(r.idxType)-2) {
    for i in r do
      write(" ", i);
  }
  writeln();
}
