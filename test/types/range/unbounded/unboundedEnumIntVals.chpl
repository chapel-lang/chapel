enum numbers { mn = min(int), one = 1, two = 2, three = 3, mx = max(int)}
use numbers;

proc testRange(r) {
  var count = 0;
  for i in r {
    writeln(i);
    count += 1;
    if count > 5 {
      writeln("This isn't working");
      break;
    }
  }
  writeln();
}

testRange(mn..);
testRange(mn.. by 2);
testRange(one.. by 2);
testRange(..mx by -1);
testRange(..mx by -2);
testRange(..three by -2);
