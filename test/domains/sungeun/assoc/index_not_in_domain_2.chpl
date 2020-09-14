config param parSafe = true;

var D1: domain(int, parSafe=parSafe);
D1 += max(int);
D1 += max(int)/2;
D1 += max(int)/4;
D1 += max(int)/8;
printDomainSorted();

D1 -= max(int)/4;
printDomainSorted();

D1 -= max(int);
printDomainSorted();

D1 -= max(int)/8;
printDomainSorted();

D1 -= max(int)/2;
printDomainSorted();

D1 -= max(int)/4;
printDomainSorted();

const retval = D1.remove(max(int)/4);
assert(retval == 0);

proc printDomainSorted() {
  write("Indices in domain: ");
  writeln(D1.sorted());
}
