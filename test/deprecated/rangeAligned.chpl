// deprecated by Vass in 1.32


var r1 = 1..3;
var r2 = ..3 by 75;

writeln(r1.aligned);
writeln(r2.aligned);
writeln(r1.isAmbiguous());
writeln(r2.isAmbiguous());
writeln(r1.isNaturallyAligned());
writeln(r2.isNaturallyAligned());
