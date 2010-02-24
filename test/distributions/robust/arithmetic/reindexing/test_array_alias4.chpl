use driver_arrays;
config const verbose = false;
const c1 = 987654321;
const c2 = -123456789;

var AA => A1D;
AA = c1;
var errs = 0;
for i in Dom1D do
  if A1D[i] != c1 then errs += 1;
if verbose then writeln(A1D);
writeln("alias1: ", errs, " errors");

var AA2 => A2D;
AA2 = c1;
errs = 0;
for i in Dom2D do
  if A2D[i] != c1 then errs += 1;
if verbose then writeln(A2D);
writeln("alias1 2D: ", errs, " errors");

const TD1D: domain(1) distributed Dist1D = Space1 - (o5);
var TA1D: [TD1D] int;
AA => TA1D;
AA = c2;
errs = 0;
for i in TD1D do
  if TA1D[i] != c2 then errs += 1;
if verbose then writeln(TA1D);
writeln("alias2: ", errs, " errors");

const TD2D: domain(2) distributed Dist2D = Space2 - (o5, o5);
var TA2D: [TD2D] int;
AA2 => TA2D;
AA2 = c2;
errs = 0;
for i in TD2D do
  if TA2D[i] != c2 then errs += 1;
if verbose then writeln(TA2D);
writeln("alias2 2D: ", errs, " errors");

