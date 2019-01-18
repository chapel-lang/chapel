var x = INFINITY;
var y = -INFINITY;
var z = NAN;
var x32 = INFINITY: real(32);
var y32 = -INFINITY: real(32);
var z32 = NAN: real(32);

if (isinf(x)) then writeln("INFINITY okay.");
if (isfinite(x)) then writeln("INFINITY not okay.");
if (isnan(x)) then writeln("INFINITY not okay.");

if (isinf(y)) then writeln("-INFINITY okay.");
if (isfinite(y)) then writeln("-INFINITY not okay.");
if (isnan(y)) then writeln("-INFINITY not okay.");

if (isinf(z)) then writeln("NAN not okay.");
if (isfinite(z)) then writeln("NAN not okay.");
if (isnan(z)) then writeln("NAN okay.");

if (isinf(x32)) then writeln("INFINITY32 okay.");
if (isfinite(x32)) then writeln("INFINITY32 not okay.");
if (isnan(x32)) then writeln("INFINITY32 not okay.");

if (isinf(y32)) then writeln("-INFINITY32 okay.");
if (isfinite(y32)) then writeln("-INFINITY32 not okay.");
if (isnan(y32)) then writeln("-INFINITY32 not okay.");

if (isinf(z32)) then writeln("NAN32 not okay.");
if (isfinite(z32)) then writeln("NAN32 not okay.");
if (isnan(z32)) then writeln("NAN32 okay.");

if (isfinite(12345)) then writeln("Finite test passes.");
