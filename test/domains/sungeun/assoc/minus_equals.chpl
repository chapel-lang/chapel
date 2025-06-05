import Sort;
config param parSafe = true;

var D1: domain(int, parSafe=parSafe);
D1 += max(int);
D1 += max(int)/2;
D1 += max(int)/4;
D1 += max(int)/8;
writeln(Sort.sorted(D1));

var D2: domain(int, parSafe=parSafe);
D2 += max(int)/8;
D2 += max(int)/4;
D2 += max(int)/2;
D2 += max(int);
D2 += max(int)/16;
D2 += max(int)/32;
D2 += max(int)/64;
writeln(Sort.sorted(D2));

serial !parSafe do D2 -= D1;
writeln(Sort.sorted(D2));
