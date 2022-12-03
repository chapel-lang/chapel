use IO, Set;

var Rucksacks = readItems();
//writeln(Rucksacks);

var sum = 0;

var allchars = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

/*
forall (s1, s2) in Rucksacks with (+ reduce sum) {
*/
for (s1, s2) in Rucksacks {
//  writeln((s1, s2));
  var chars: set(uint(8));
  for b in s1 {
    chars.add(charToInt(b));
  }
  for (b,i) in zip(s2,0..) {
    const b2 = charToInt(b);
    if chars.contains(b2) {
//      writeln("Found duplicate: ", b2);
//      writeln("(", allchars[b2], ")");
      sum += b2;
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
  var line: bytes;
  while readLine(line) {
    const len = line.size-1,
          mid = len / 2;
    yield (line[0..<mid], line[mid..<len]);
  }
}
