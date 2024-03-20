use Set;

var combinations : set(int);

forall i in 1..1000 with (+ reduce combinations) {
  for j in 1..100 {
    combinations.add(i*j);
  }
}

var serialcombs : set(int);
for i in 1..1000 do
  for j in 1..100 do
    serialcombs.add(i*j);

writeln(combinations == serialcombs);


var foos: set(string);

var first = [ "f", "fo", "foo", "fooo", "foooo", "fooooo" ];
var second = [ "oo", "obar", "oobar" ];

forall i in first with (+ reduce foos) {
  for j in second do
    foos.add(i+j);
}

var serialfoos: set(string);
for i in first {
  for j in second do
    serialfoos.add(i+j);
}

writeln(foos == serialfoos);
