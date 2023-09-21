config const op = 0;

if op == 1 then writeln(( 0..  by 2 align 0 )[ 0..  by 2 align 1]);   // low,low -> low
if op == 2 then writeln(( 0..  by 2 align 0 )[  ..  by 2 align 1]);   // low,unb -> low

if op == 3 then writeln((  ..0 by 2 align 0 )[  ..0 by 2 align 1]);   // high,high -> high
if op == 4 then writeln((  ..0 by 2 align 0 )[  ..  by 2 align 1]);   // high,unb  -> high

if op == 5 then writeln((  ..  by 2 align 0 )[ 0..  by 2 align 1]);   // unb,low  -> low
if op == 6 then writeln((  ..  by 2 align 0 )[  ..0 by 2 align 1]);   // unb,high -> high
if op == 7 then writeln((  ..  by 2 align 0 )[  ..  by 2 align 1]);   // unb,unb  -> unb

const r1 = .. by 2 align 0;
const r2 = .. by 2 align 1;
const r3 = r1(r2);
writeln(r3);
