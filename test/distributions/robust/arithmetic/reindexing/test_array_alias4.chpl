use driver;
const c1 = 987654321;
const c2 = -123456789;

var AA => A1D;
AA = c1;
var errs = 0;
for i in Dom1D do
  if A1D[i] != c1 then errs += 1;
writeln("alias1: ", errs, " errors");

const TD1D: domain(1) distributed Dist1D = Space1 - (o5);
var TA1D: [TD1D] int;
AA => TA1D;
AA = c2;
errs = 0;
for i in TD1D do
  if TA1D[i] != c2 then errs += 1;
writeln("alias2: ", errs, " errors");

