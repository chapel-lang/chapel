use Map;

var m1 = new map(int, int),
    m2 = new map(int, int);

for i in 1..10 {
  m1[i] = -i;
  m2[i] = -i;
}


writeln(m1 == m2);
writeln(m1 != m2);

m2[3] = 3;

writeln(m1 == m2);
writeln(m1 != m2);
