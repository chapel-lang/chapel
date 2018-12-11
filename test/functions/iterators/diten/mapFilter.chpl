use FunctionalOperations;

proc imaginate(x: int) {
  return x*1.0i;
}

writeln("range empty");
for i in (1..0).these().map(imaginate) {
  writeln(i);
}

writeln("range forward");
for i in (1..10).these().map(imaginate) {
  writeln(i);
}

writeln("range backward");
for i in (1..10 by -1).these().map(imaginate) {
  writeln(i);
}

writeln("domain empty");
for i in {1..0}.these().map(imaginate) {
  writeln(i);
}

writeln("domain forward");
for i in {1..10}.these().map(imaginate) {
  writeln(i);
}

writeln("domain backward");
for i in {1..10 by -1}.these().map(imaginate) {
  writeln(i);
}

writeln("array empty");
var A: [1..0] int;
for i in A.these().map(imaginate) {
  writeln(i);
}

writeln("array forward");
for i in [1,2,3,4,5,6,7,8,9,10].these().map(imaginate) {
  writeln(i);
}

iter udIter() {
  var i = 1;
  while i <= 10 {
    yield i;
    i += 1;
  }
}

writeln("user-defined iterator");
for i in udIter().map(imaginate) {
  writeln(i);
}

proc threevenPlusOne(x: int) return x%3 == 1;

writeln("chained on user-defined iterator");
for i in udIter().filter(threevenPlusOne).map(imaginate) {
  writeln(i);
}

proc writeit(im: imag) {
  writeln(im);
}

writeln("chained with consume");
udIter().filter(threevenPlusOne).map(imaginate).consume(writeit);
