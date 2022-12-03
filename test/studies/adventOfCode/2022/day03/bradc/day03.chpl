use IO, Set;

var Rucksacks = readItems();

var sum = 0;

/*
forall (s1, s2) in Rucksacks with (+ reduce sum) {
*/
for (compartment1, compartment2) in Rucksacks {
  var items: set(uint(8));
  for item in compartment1 {
    items.add(itemToPriority(item));
  }
  for item in compartment2 {
    const priority = itemToPriority(item);
    if items.contains(priority) {
      sum += priority;
      break;
    }
  }
}
writeln(sum);

proc itemToPriority(char) {
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
  var rucksack: bytes;
  while readLine(rucksack) {
    const len = rucksack.size-1,
          mid = len / 2;
    yield (rucksack[0..<mid], rucksack[mid..<len]);
  }
}
