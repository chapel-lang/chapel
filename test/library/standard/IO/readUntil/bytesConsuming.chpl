use IO, List;

const methionine = b"ATG",
      fileName = "hemoglobinB.txt";

// -- while readUntil(myBytes) -- pattern:
// consume the separator, don't include it in the returned bytes
writeln(testConsumingReadUntil(true, false));
// consume the separator, include it in the returned string
writeln(testConsumingReadUntil(true, true));

// -- var myBytes = readUntil(bytes) -- pattern:
// consume the separator, don't include it in the returned bytes
writeln(testConsumingReadUntil(false, false));
// consume the separator, include it in the returned bytes
writeln(testConsumingReadUntil(false, true));

proc testConsumingReadUntil(param reuseBuffer: bool, includes) {

  // split the input at methionine using readUntil
  const composed = readUntilIntoBytes(reuseBuffer, includes);

  // do the same using readAll and bytes.split
  const all = openreader(fileName).readAll(bytes).split(methionine);

  // compare results
  return all.size == composed.size &&
    (&& reduce [i in 0..<all.size]
      composed[i] ==
      (all[i] + (if includes && i != all.size-1 then methionine else b""))
    );
}

proc readUntilIntoBytes(param reuseBuffer: bool, includes) where reuseBuffer == true {
  var composed = new list(bytes),
      next: bytes,
      r = openreader(fileName);

  while r.readUntil(next, methionine, -1, true, includes) {
    composed.append(next);
  }

  return composed;
}

proc readUntilIntoBytes(param reuseBuffer: bool, includes) where reuseBuffer == false {
  var composed =  new list(bytes),
      r = openreader(fileName);

  while true {
    const next = r.readUntil(bytes, methionine, -1, true, includes);
    if next.isEmpty() then break;
    composed.append(next);
  }

  return composed;
}
