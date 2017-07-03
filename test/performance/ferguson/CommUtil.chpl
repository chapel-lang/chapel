use CommDiagnostics;

config var printCounts = false;
config var printAllCounts = false;

// this is a config param just so it can be in COMPOPTS
config param checkMaxAttained = false;

proc start() {
  resetCommDiagnostics();
  startCommDiagnostics();
}

proc stop() {
  stopCommDiagnostics();
}

// Checks anything with a passed maximum != max(int) is below the maximum.
// Prints out counts if the passed maximum == max(int) (or is not passed).
// Prints out all counts if 
proc report(maxGets=max(int), maxPuts=max(int), maxOns=max(int)) {
  var counts = getCommDiagnostics();

  var gets = totalGets(counts);
  var puts = totalPuts(counts);
  var ons = totalOns(counts);

  var fixGets = false;
  var fixPuts = false;
  var fixOns = false;

  if (printCounts && maxGets==max(int)) || printAllCounts  then
    writeln("GETs: ", gets);
  if gets > maxGets then
    writeln("ERROR -- TOO MANY GETs, ", gets, " EXCEEDS MAX OF ", maxGets);
  if checkMaxAttained && maxGets!=max(int) && gets < maxGets then
    fixGets = true;

  if (printCounts && maxPuts==max(int)) || printAllCounts  then
    writeln("PUTs: ", puts);
  if puts > maxPuts then
    writeln("ERROR -- TOO MANY PUTs, ", puts, " EXCEEDS MAX OF ", maxPuts);
  if checkMaxAttained && maxPuts!=max(int) && puts < maxPuts then
    fixPuts = true;


  if (printCounts && maxOns==max(int)) || printAllCounts then
    writeln("ONs: ", ons);
  if ons > maxOns then
    writeln("ERROR -- TOO MANY ONs, ", ons, " EXCEEDS MAX OF ", maxOns);
  if checkMaxAttained && maxOns!=max(int) && ons < maxOns then
    fixOns = true;

  if (fixGets || fixPuts || fixOns) {
    writeln("upper bound comm count is not tight in wost-case configuration\n",
            "This is good! Please update this test to detect regressions.\n");

    if fixGets then
      writeln(gets, " < max of ", maxGets, " GETs");
    if fixPuts then
      writeln(puts, " < max of ", maxPuts, " PUTs");
    if fixOns then
      writeln(ons, " < max of ", maxOns, " ONs");
    writeln();

    var needComma = false;
    write("report(");
    if maxGets!=max(int) {
      write("maxGets=",gets);
      needComma=true;
    }
    if maxPuts!=max(int) {
      if needComma then write(", ");
      write("maxPuts=",puts);
      needComma=true;
    }
    if maxOns!=max(int) {
      if needComma then write(", ");
      write("maxOns=",ons);
    }
    writeln(");");
    writeln();
  }
}
