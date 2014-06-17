const r = 1..2;
assert(true && r == r && true);
if (r == r) then writeln("range OK");

const d = {1..2};
assert (d == d && true);
if (d == d) then writeln("domain OK");
