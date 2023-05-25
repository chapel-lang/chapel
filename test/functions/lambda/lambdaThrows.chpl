const f1 = lambda(n: int): int throws { return n*n; };
const f2 = lambda(n: int): int throws { throw new Error(n:string); };

proc test() {
  try! writeln(f1(4));
  try! writeln(f2(8));
}
test();
