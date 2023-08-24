var x = inf;
var y = -inf;
var z = nan;
var x32 = inf: real(32);
var y32 = -inf: real(32);
var z32 = nan: real(32);

if (isinf(x)) then writeln("inf okay.");
if (isFinite(x)) then writeln("inf not okay.");
if (isnan(x)) then writeln("inf not okay.");

if (isinf(y)) then writeln("-inf okay.");
if (isFinite(y)) then writeln("-inf not okay.");
if (isnan(y)) then writeln("-inf not okay.");

if (isinf(z)) then writeln("nan not okay.");
if (isFinite(z)) then writeln("nan not okay.");
if (isnan(z)) then writeln("nan okay.");

if (isinf(x32)) then writeln("inf32 okay.");
if (isFinite(x32)) then writeln("inf32 not okay.");
if (isnan(x32)) then writeln("inf32 not okay.");

if (isinf(y32)) then writeln("-inf32 okay.");
if (isFinite(y32)) then writeln("-inf32 not okay.");
if (isnan(y32)) then writeln("-inf32 not okay.");

if (isinf(z32)) then writeln("nan32 not okay.");
if (isFinite(z32)) then writeln("nan32 not okay.");
if (isnan(z32)) then writeln("nan32 okay.");

if (isFinite(12345)) then writeln("Finite test passes.");
