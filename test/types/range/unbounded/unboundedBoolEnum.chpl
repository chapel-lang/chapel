enum color { red, green, blue };

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

testRange(false..);
testRange(false.. by 2);
testRange(color.red..);
testRange(color.red.. by 2);
testRange(..true by -1);
testRange(..true by -2);
testRange(..color.blue by -1);
testRange(..color.blue by -2);
