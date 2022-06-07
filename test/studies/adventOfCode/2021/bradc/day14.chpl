use IO, Map;

config const steps = 10,
             debug = false;

// TODO: bytes + array of uint(8) would be more appropriate, storage-wise

var polyTemplStr: string;
readLine(polyTemplStr);  
polyTemplStr = polyTemplStr.strip();

var polyTempl = [ch in polyTemplStr] ch;

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

writeln(polyTemplStr);
writeln(polyTempl);
if debug then
  writeln(prodMap);
processTemplate(polyTempl, steps);

proc processTemplate(templ: [?templInds], stepsLeft) {
  if stepsLeft == 0 {
    writeln(templ);
    writeln(templ.size);
    calcStats(templ);
  } else {
    writeln("Step ", stepsLeft);
    var next: [0..<(templ.size*2-1)] string;
    writeln(templInds);
    forall i in templInds {
      const first = templ[i];
      next[i*2] = first;
      if i != templInds.high {
        const second = templ[i+1];
        var substr = first + second;
        if debug then
          writeln("Trying to look up ", substr);
        next[i*2+1] = prodMap.getValue(substr);
      }
    }
    processTemplate(next, stepsLeft-1);
  }
}

proc calcStats(templ: [?templInds]) {
  var counts = [ch in allChars] + reduce (templ == ch);
  if debug then
    forall (ch, count) in zip(allChars, counts) do
      writeln((ch, count));
  // BUG/FEATURE REQUEST: maxloc over string domain -- currently requires max()
  // on strings to break ties.  See futures/day14-maxlocstringinds.chpl
  var largest = max reduce counts;
  var smallest = min reduce counts;
  writeln(largest-smallest);
}
