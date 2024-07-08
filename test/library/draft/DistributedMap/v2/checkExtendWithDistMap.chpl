use DistributedMap;

var m1 = new distributedMap(int, int);
var m2 = new distributedMap(int, int);

for i in 1..20 {
  m1.add(i, -i);
}

writeln(m1.size);
writeln(m2.size);

m2.extend(m1);
writeln("Check after extend");
writeln(m2.size);

var allHere: bool = true;
for i in 1..20 {
  if m2.contains(i) != true {
    allHere = false;
    break;
  }
}
writeln(allHere);
