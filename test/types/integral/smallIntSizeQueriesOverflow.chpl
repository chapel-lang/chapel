config type idxType = int(8);

config const m = 126:idxType;
config const n = 10:idxType;

const r = -m..m;
writeln(r.size);

const D = {1..n, 1..n, 1..n};
writeln(D.size);
