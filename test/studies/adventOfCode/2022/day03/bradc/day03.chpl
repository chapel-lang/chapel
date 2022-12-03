use IO, Set;

var Rucksacks = readRucksacks();
writeln(sumOfPriorities(Rucksacks));

iter readRucksacks() {
  var rucksack: bytes;

  while readLine(rucksack) {
    const len = rucksack.size-1,
          mid = len / 2;

    yield (rucksack[0..<mid], rucksack[mid..<len]);
  }
}

proc sumOfPriorities(Rucksacks) {
  var sum = 0;

  forall (compartment1, compartment2) in Rucksacks with (+ reduce sum) {
    var items: set(uint(8));

    for item in compartment1 do
      items.add(item);

    for item in compartment2 {
      if items.contains(item) {
        sum += itemToPriority(item);
        break;
      }
    }
  }

  return sum;
}

proc itemToPriority(item) {
  param A = b"A".toByte(),
        Z = b"Z".toByte(),
        a = b"a".toByte();

  if item <= Z {
    return item-A + 27;
  } else {
    return item-a + 1;
  }
}

