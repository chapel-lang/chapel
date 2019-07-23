use Sets;

record testRecord {
  var dummy: int = 0;
}

class testClass {
  var dummy: int = 0;
}

var s1 = new set(int);
var s2: set(testRecord);
var s3: set(testClass);

writeln(s1);
writeln(s2);
writeln(s3);
