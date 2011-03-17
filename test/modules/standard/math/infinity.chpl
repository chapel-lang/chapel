var x = INFINITY;
var y = -INFINITY;

if (10000.0001 < x) then writeln("INFINITY okay.");
if (1.0/0.0 == x) then writeln("INFINITY okay.");

if (10000.0001 < y) then writeln("-INFINITY not okay.");
                    else writeln("-INFINITY okay.");
if (log(0) == y) then writeln("-INFINITY okay.");
