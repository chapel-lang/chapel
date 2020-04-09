const Dbase = {1..3, 1..5};
const rd1 = 0..6 by 3, rd2 = 0..6 by 3 align 2;
const DbaseS = Dbase(rd1,rd2);
writeln("Do  ", DbaseS);
forall ix in DbaseS do writeln("Dp  ", ix);
for    ix in DbaseS do writeln("Ds  ", ix);

const E = {1..3 by 3 align 0, 1..5 by 3 align 2};
writeln("Eo  ", E);
forall ix in E do writeln("Ep  ", ix);
for    ix in E do writeln("Es  ", ix);
