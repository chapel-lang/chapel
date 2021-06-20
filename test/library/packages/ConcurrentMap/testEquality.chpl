use ConcurrentMap;

var m1 = new ConcurrentMap(int, int);
var m2 = new ConcurrentMap(int, int);

var atok = m1.getToken();
var btok = m2.getToken();
for i in 1..10 {
  m1.add(i, -i, atok);
  m2.add(i, -i, btok);
}

writeln(m1 == m2);
writeln(m1 != m2);


m2.set(3, 3, btok);

writeln(m1 == m2);
writeln(m1 != m2);
