use IO, Map;

config const steps = 10,
             debug = false;

// TODO: bytes + array of uint(8) would be more appropriate, storage-wise

var polyTemplStr: string;
readLine(polyTemplStr);  
polyTemplStr = polyTemplStr.strip();

var allChars: domain(string);
var prodMap: map(string, string);

try {
  var line: string;
  readln();
  while readLine(line) {
    prodMap.add(line[0..1], line[6]);
    // TODO: This has got to be too conservative:
    allChars.add[line[0]];
    allChars.add[line[1]];
    allChars.add[line[6]];
  }
} catch {
}

var allPairs: domain(string);
forall ch in allChars with (ref allPairs) do
  forall ch2 in allChars with (ref allPairs) do
    allPairs.add(ch + ch2);

writeln(polyTemplStr);
if debug {
  writeln(prodMap);
  writeln(allPairs);
}

var counts: [allPairs] atomic int;
forall i in polyTemplStr.indices do
  if i != polyTemplStr.indices.last then
    counts[polyTemplStr[i]+polyTemplStr[i+1]].add(1);

if debug then
  for (p,c) in zip(allPairs, counts) do
    writeln(p, ": ", c);

processTemplate(counts, steps);

proc processTemplate(in counts, stepsLeft) {
  if stepsLeft == 0 {
    if debug then
      for (p,c) in zip(allPairs, counts) do
        writeln(p, ": ", c);
    var finalCounts: [allChars] atomic int;
    forall p in allPairs do
      finalCounts[p(0)].add(counts[p].read());
    // add 1 for the second character in the last pair, which never changed
    finalCounts[polyTemplStr[polyTemplStr.size-1]].add(1);
    if debug then
      for (ch,c) in zip(allChars, finalCounts) do
        writeln(ch, ": ", c);
    calcStats(finalCounts);
  } else {
    writeln("Step ", stepsLeft);
    var newCounts: [allPairs] atomic int;
    forall p in allPairs {
      const count = counts[p].read();
      const firstchar = p(0);
      const secondchar = p(1);
      var midchar = prodMap.getValue(p);
      newCounts[firstchar + midchar].add(count);
      newCounts[midchar + secondchar].add(count);
    }
    processTemplate(newCounts, stepsLeft-1);
  }
}

proc calcStats(counts) {
  // BUG/FEATURE REQUEST: maxloc over string domain -- currently requires max()
  // on strings to break ties.  See futures/day14-maxlocstringinds.chpl
  var largest = max reduce counts.read();
  var smallest = min reduce counts.read();
  writeln(largest-smallest);
}
