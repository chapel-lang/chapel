use Set;

var s = new set(int);
s.add(-1);

var initialSize = s._htb.tableSize;

for i in 0..#initialSize/2 do
  s.add(i);

assert(s._htb.tableSize > initialSize);

for i in 0..#2+initialSize/4 {
  assert(s._htb.tableSize > initialSize);
  s.remove(i);
}

assert(s._htb.tableSize == initialSize);
