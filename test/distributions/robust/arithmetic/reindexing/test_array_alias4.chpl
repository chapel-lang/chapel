use driver_arrays;
config const verbose = false;
const c1 = 987654321;
const c2 = -123456789;
const c3 = -987654321;
const c4 = 123456789;
const c2d32 = max(int(32));

ref AA = A1D;
AA = c1;
var errs = 0;
for i in Dom1D do
  if A1D[i] != c1 then errs += 1;
if verbose then writeln(A1D);
writeln("alias1: ", errs, " errors");

ref AA2 = A2D;
AA2 = c1;
errs = 0;
for i in Dom2D do
  if A2D[i] != c1 then errs += 1;
if verbose then writeln(A2D);
writeln("alias1 2D: ", errs, " errors");

ref AA3 = A3D;
AA3 = c3;
errs = 0;
for i in Dom3D do
  if A3D[i] != c3 then errs += 1;
if verbose then writeln(A3D);
writeln("alias1 3D: ", errs, " errors");

ref AA4 = A4D;
AA4 = c4;
errs = 0;
for i in Dom4D do
  if A4D[i] != c4 then errs += 1;
if verbose then writeln(A4D);
writeln("alias1 4D: ", errs, " errors");

ref AA2D32 = A2D32;
AA2D32 = c2d32;
errs = 0;
for i in Dom2D32 do
  if A2D32[i] != c2d32 then errs += 1;
if verbose then writeln(A2D32);
writeln("alias1 2D32: ", errs, " errors");


const TD1D: domain(1) dmapped Dist1D = Space1.translate(-o5);
var TA1D: [TD1D] int;
ref AA11 = TA1D;
AA11 = c1;
errs = 0;
for i in TD1D do
  if TA1D[i] != c1 then errs += 1;
if verbose then writeln(TA1D);
writeln("alias2: ", errs, " errors");

const TD2D: domain(2) dmapped Dist2D = Space2.translate(-o5, -o5);
var TA2D: [TD2D] int;
ref AA22 = TA2D;
AA22 = c2;
errs = 0;
for i in TD2D do
  if TA2D[i] != c2 then errs += 1;
if verbose then writeln(TA2D);
writeln("alias2 2D: ", errs, " errors");

const TD3D: domain(3) dmapped Dist3D = Space3.translate(-o5, -o5, -o5);
var TA3D: [TD3D] int;
ref AA33 = TA3D;
AA33 = c3;
errs = 0;
for i in TD3D do
  if TA3D[i] != c3 then errs += 1;
if verbose then writeln(TA3D);
writeln("alias2 3D: ", errs, " errors");

const TD4D: domain(4) dmapped Dist4D = Space4.translate(-o5, -o5, -o5, -o5);
var TA4D: [TD4D] int;
ref AA44 = TA4D;
AA44 = c4;
errs = 0;
for i in TD4D do
  if TA4D[i] != c4 then errs += 1;
if verbose then writeln(TA4D);
writeln("alias2 4D: ", errs, " errors");

const TD2D32: domain(2, int(32)) dmapped Dist2D32 = Space2D32.chpl__unTranslate(o5:int(32), o5:int(32));
var TA2D32: [TD2D32] int;
ref AA22D32 = TA2D32;
AA22D32 = c2d32;
errs = 0;
for i in TD2D32 do
  if TA2D32[i] != c2d32 then errs += 1;
if verbose then writeln(TA2D32);
writeln("alias2 2D32: ", errs, " errors");
