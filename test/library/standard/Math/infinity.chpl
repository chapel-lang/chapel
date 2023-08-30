var x = inf;
var y = -inf;
var z = nan;
var x32 = inf: real(32);
var y32 = -inf: real(32);
var z32 = nan: real(32);

if (isInf(x)) then writeln("inf okay.");
if (isFinite(x)) then writeln("inf not okay.");
if (isNan(x)) then writeln("inf not okay.");

if (isInf(y)) then writeln("-inf okay.");
if (isFinite(y)) then writeln("-inf not okay.");
if (isNan(y)) then writeln("-inf not okay.");

if (isInf(z)) then writeln("nan not okay.");
if (isFinite(z)) then writeln("nan not okay.");
if (isNan(z)) then writeln("nan okay.");

if (isInf(x32)) then writeln("inf32 okay.");
if (isFinite(x32)) then writeln("inf32 not okay.");
if (isNan(x32)) then writeln("inf32 not okay.");

if (isInf(y32)) then writeln("-inf32 okay.");
if (isFinite(y32)) then writeln("-inf32 not okay.");
if (isNan(y32)) then writeln("-inf32 not okay.");

if (isInf(z32)) then writeln("nan32 not okay.");
if (isFinite(z32)) then writeln("nan32 not okay.");
if (isNan(z32)) then writeln("nan32 okay.");

if (isFinite(12345)) then writeln("Finite test passes.");
