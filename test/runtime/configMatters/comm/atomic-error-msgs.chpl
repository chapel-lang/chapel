var areal: atomic real;
areal.fetchOr(1);
areal.or(1);
areal.fetchXor(1);
areal.xor(1);
areal.fetchAnd(1);
areal.and(1);

var astring: atomic string;

var aint: atomic int = 0;

aint = 1;
aint = aint + 1;
aint += 1;
aint -= 1;
aint = aint - 1;
aint = aint * 1;
aint *= 1;
aint = aint / 1;
aint /= 1;
aint = aint % 1;
aint %= 1;
