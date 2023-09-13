

writeln(( 0..  by 2 align 0 )[ 0..  by 2 align 1]);   // low,low -> low
writeln(( 0..  by 2 align 0 )[  ..  by 2 align 1]);   // low,unb -> low

writeln((  ..0 by 2 align 0 )[  ..0 by 2 align 1]);   // high,high -> high
writeln((  ..0 by 2 align 0 )[  ..  by 2 align 1]);   // high,unb  -> high

writeln((  ..  by 2 align 0 )[ 0..  by 2 align 1]);   // unb,low  -> low
writeln((  ..  by 2 align 0 )[  ..0 by 2 align 1]);   // unb,high -> high
writeln((  ..  by 2 align 0 )[  ..  by 2 align 1]);   // unb,unb  -> unb

/*
use boundKind;

proc st(param b1, param b2) param {
  const r1: range(bounds=b1);
  const r2: range(bounds=b2);
  const sli = r1[r2];
  return sli.bounds: string;
}

for param b1 in both..neither {
  compilerWarning("| ", b1:string, " | ", st(b1, both), " | ", st(b1, low),
                  " | ", st(b1, high), " | ", st(b1, neither), " |");
}
*/


/*
proc show(r1,r2) {
  r1.displayRepresentation("r1 = ");
  r2.displayRepresentation("r2 = ");
  (r1[r2]).
     displayRepresentation("[] = ");
  writeln();
}

show(.. by 3 align 0, .. by 4 align 1);    // .. by 12 align 9
show(..0 by 3 align 0, ..1 by 4 align 1);  // ..0 by 12 align 9
*/

/*

unbounded aligned | unbounded unaligned -- always OK
unbounded aligned | unbounded aligned -- always OK

unbounded aligned | low-bounded


low-bounded | low-bounded -- always OK


both | ... -always OK
low | high
high | low


*/


/*
proc f(): domain(?) {
  var d: domain(1);
  return d;
}
writeln(f());
*/
/* compiler crash 2
var d: domain(?) = {1..34};
f(d);

proc f(arg: domain) {
  writeln(arg);
}
*/
