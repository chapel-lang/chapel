config var N = 1000000;
config var trials = 10;

for i in 1..trials {
  var dom : domain(int);
  var data : [dom] int;
  forall j in 1..N {
    data[j] = j*j;
  }
  for j in 1..N do
    assert(data[i] == i*i);
}
writeln("SUCCESS");
