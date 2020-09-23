config const n = 5;
config const noisy = false;
const n2 = n/2;
const np = n*(n-1);
const np2 = n2*(n2-1);

inline proc sumOfI(n) return n*(n+1)/2;

if n < 4 then halt("n must be > 4");

iter blah(numTasks=n) {
  for i in 1..numTasks do yield i;
}

if noisy then writeln("=== serial iter ===");
var sum = 0;
for i in blah() do sum += i;
if sum != sumOfI(n) then
  writeln("ERROR: sum is ", sum, " (should be ", sumOfI(n), ")");

sum = 0;
for i in blah(n2) do sum += i;
if sum != sumOfI(n2) then
  writeln("ERROR: sum is ", sum, " (should be ", sumOfI(n2), ")");

iter blah(param tag: iterKind, numTasks=n)
  where tag==iterKind.leader {
  coforall i in 1..numTasks do
    yield (i-1)*(numTasks-1)..#(numTasks-1);
}

iter blah(param tag: iterKind, followThis, numTasks=n)
  where tag==iterKind.follower {
  for i in followThis do yield i+numTasks;
}

if noisy then writeln("=== parallel iter ===");
var asum: atomic int;
forall i in blah() do asum.add(i-(n-1));
if asum.read() != sumOfI(np) then
  writeln("ERROR: sum is ", sum, " (should be ", sumOfI(np), ")");

asum.write(0);
forall i in blah(n2) do asum.add(i-(n2-1));
if asum.read() != sumOfI(np2) then
  writeln("ERROR: sum is ", sum, " (should be ", sumOfI(np2), ")");

iter forwardBlah(numTasks=n) {
  for i in blah(numTasks) do yield i;
}

if noisy then writeln("=== serial forwarded iter ===");
sum = 0;
for i in forwardBlah() do sum += i;
if sum != n*(n+1)/2 then
  writeln("ERROR: sum is ", sum, " (should be ", n*(n+1)/2, ")");

sum = 0;
for i in forwardBlah(n2) do sum += i;
if sum != n2*(n2+1)/2 then
  writeln("ERROR: sum is ", sum, " (should be ", n2*(n2+1)/2, ")");

iter forwardBlah(param tag: iterKind, numTasks=n)
  where tag==iterKind.leader {
  for i in blah(iterKind.leader, numTasks) do yield i;
}

iter forwardBlah(param tag: iterKind, followThis, numTasks=n)
  where tag==iterKind.follower {
  for i in blah(iterKind.follower, followThis, numTasks) do yield i;
}

if noisy then writeln("=== parallel forwarded iter ===");
asum.write(0);
forall i in forwardBlah() do asum.add(i-(n-1));
if asum.read() != sumOfI(np) then
  writeln("ERROR: sum is ", sum, " (should be ", sumOfI(np), ")");

asum.write(0);
forall i in forwardBlah(n2) do asum.add(i-(n2-1));
if asum.read() != sumOfI(np2) then
  writeln("ERROR: sum is ", sum, " (should be ", sumOfI(np2), ")");
