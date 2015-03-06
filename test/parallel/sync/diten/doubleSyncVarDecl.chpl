var si: sync int;
var ssi: sync sync int;

si = 1;
ssi = si;
writeln(ssi.readFE().readFE());
