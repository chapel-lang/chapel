use driver_arrays;
config const verbose = false;
const c1 = 987654321;
const c2 = -123456789;
const c3 = -987654321;
const c4 = 123456789;
const c2d64 = max(int);

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

var AA3 => A3D;
AA3 = c3;
errs = 0;
for i in Dom3D do
  if A3D[i] != c3 then errs += 1;
if verbose then writeln(A3D);
writeln("alias1 3D: ", errs, " errors");

var AA4 => A4D;
AA4 = c4;
errs = 0;
for i in Dom4D do
  if A4D[i] != c4 then errs += 1;
if verbose then writeln(A4D);
writeln("alias1 4D: ", errs, " errors");

var AA2D64 => A2D64;
AA2D64 = c2d64;
errs = 0;
for i in Dom2D64 do
  if A2D64[i] != c2d64 then errs += 1;
if verbose then writeln(A2D64);
writeln("alias1 2D64: ", errs, " errors");


const TD1D: domain(1) dmapped Dist1D = Space1.translate(-o5);
var TA1D: [TD1D] int;
var AA11 => TA1D;
AA11 = c1;
errs = 0;
for i in TD1D do
  if TA1D[i] != c1 then errs += 1;
if verbose then writeln(TA1D);
writeln("alias2: ", errs, " errors");

const TD2D: domain(2) dmapped Dist2D = Space2.translate(-o5, -o5);
var TA2D: [TD2D] int;
var AA22 => TA2D;
AA22 = c2;
errs = 0;
for i in TD2D do
  if TA2D[i] != c2 then errs += 1;
if verbose then writeln(TA2D);
writeln("alias2 2D: ", errs, " errors");

const TD3D: domain(3) dmapped Dist3D = Space3.translate(-o5, -o5, -o5);
var TA3D: [TD3D] int;
var AA33 => TA3D;
AA33 = c3;
errs = 0;
for i in TD3D do
  if TA3D[i] != c3 then errs += 1;
if verbose then writeln(TA3D);
writeln("alias2 3D: ", errs, " errors");

const TD4D: domain(4) dmapped Dist4D = Space4.translate(-o5, -o5, -o5, -o5);
var TA4D: [TD4D] int;
var AA44 => TA4D;
AA44 = c4;
errs = 0;
for i in TD4D do
  if TA4D[i] != c4 then errs += 1;
if verbose then writeln(TA4D);
writeln("alias2 4D: ", errs, " errors");

const TD2D64: domain(2, int(64)) dmapped Dist2D64 = Space2D64.translate(-o5:int(64), -o5:int(64));
var TA2D64: [TD2D64] int;
var AA22D64 => TA2D64;
AA22D64 = c2d64;
errs = 0;
for i in TD2D64 do
  if TA2D64[i] != c2d64 then errs += 1;
if verbose then writeln(TA2D64);
writeln("alias2 2D64: ", errs, " errors");
