const r1 = 1..4;
const r2 = 1..4;
const r3 = ..;
const idx = 3;

// Testing 3 overloads of boundsCheck
r1.boundsCheck(idx);
r1.boundsCheck(r2);
r1.boundsCheck(r3);

// Testing assignment between unbounded ranges
const r4: range(bool, boundKind.neither) = r3;
const r5: range(bool, boundKind.neither, strides = strideKind.any) = .. by 2;

// Testing .dist deprecation
const d1 = {1..idx, 1..idx};
d1.dist;
const d2 = {1..idx, 1..idx, idx..idx};
d2.dist;
