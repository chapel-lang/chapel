use IO, List;

const methionine = "ATG",
      fileName = "hemoglobinB.txt";

// -- while readUntil(myString) -- pattern:
// consume the separator, don't include it in the returned string
writeln(testConsumingReadUntil(true, false));
// consume the separator, include it in the returned string
writeln(testConsumingReadUntil(true, true));

// -- var myString = readUntil(string) -- pattern:
// consume the separator, don't include it in the returned string
writeln(testConsumingReadUntil(false, false));
// consume the separator, include it in the returned string
writeln(testConsumingReadUntil(false, true));

proc testConsumingReadUntil(param reuseBuffer: bool, includes) {

  // split the input at methionine using readUntil
  const composed = readUntilIntoStrings(reuseBuffer, includes);

  // do the same using readAll and string.split
  const all = openreader(fileName).readAll(string).split(methionine);

  // compare results
  return all.size == composed.size &&
    (&& reduce [i in 0..<all.size]
      composed[i] ==
      (all[i] + (if includes && i != all.size-1 then methionine else ""))
    );
}

proc readUntilIntoStrings(param reuseBuffer: bool, includes) where reuseBuffer == true {
  var composed = new list(string),
      next: string,
      r = openreader(fileName);

  while r.readUntil(next, methionine, -1, true, includes) {
    composed.append(next);
  }

  return composed;
}

proc readUntilIntoStrings(param reuseBuffer: bool, includes) where reuseBuffer == false {
  var composed =  new list(string),
      r = openreader(fileName);

  while true {
    const next = r.readUntil(string, methionine, -1, true, includes);
    if next.isEmpty() then break;
    composed.append(next);
  }

  return composed;
}
