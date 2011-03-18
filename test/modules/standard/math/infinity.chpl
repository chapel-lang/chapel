var x = INFINITY;
var y = -INFINITY;
var z = NAN;

if (isinf(x)) then writeln("INFINITY okay.");
if (isfinite(x)) then writeln("INFINITY not okay.");
if (isnan(x)) then writeln("INFINITY not okay.");

if (isinf(y)) then writeln("-INFINITY okay.");
if (isfinite(y)) then writeln("-INFINITY not okay.");
if (isnan(y)) then writeln("-INFINITY not okay.");

if (isinf(z)) then writeln("NAN not okay.");
if (isfinite(z)) then writeln("NAN not okay.");
if (isnan(z)) then writeln("NAN okay.");

if (isfinite(12345)) then writeln("Finite test passes.");
