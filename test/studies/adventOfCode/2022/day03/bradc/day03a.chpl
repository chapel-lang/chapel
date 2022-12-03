// WARNING:  This is a hot mess because I've been spending all my
// time writing blogs rather than cleaning up my solutions

use IO, Set;

var Rucksacks = readItems();
//writeln(Rucksacks);

var sum = 0;

var allchars = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

/*
forall (s1, s2) in Rucksacks with (+ reduce sum) {
*/
for (s1, s2, s3) in Rucksacks {
//  writeln((s1, s2, s3));
  var chars: set(uint(8)),
      chars2: set(uint(8));;
  for b in s1[0..<s1.size] {
    chars.add(charToInt(b));
  }
  for b in s2[0..<s2.size] {
    const b2 = charToInt(b);
    if chars.contains(b2) {
      chars2.add(b2);
    }
  }
  for b in s3[0..<s3.size] {
    const b3 = charToInt(b);
    if (chars2.contains(b3)) {
//      writeln("Found duplicate: ", b3);
//      writeln("(", allchars[b3], ")");
      sum += b3;
      break;
    }
  }
}
writeln(sum);

proc charToInt(char) {
  param a = b"a".toByte(),
        z = b"z".toByte(),
        A = b"A".toByte(),
	Z = b"Z".toByte();
  if char <= Z {
    return char-A + 27;
  } else {
    return char-a + 1;
  }
}

iter readItems() {
  var group1, group2, group3: bytes;
  while readLine(group1) {
    readLine(group2);
    readLine(group3);
    yield (group1, group2, group3);
  }
}
