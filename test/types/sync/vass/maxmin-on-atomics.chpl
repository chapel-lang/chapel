// Currently min/max are not allowed on atomic vars.

config param domin = true;

var a1: atomic int;
var a2: atomic int;

a1.write(1);
a2.write(2);

if domin then writeln(min(a1, a2)); else writeln(max(a1, a2));
