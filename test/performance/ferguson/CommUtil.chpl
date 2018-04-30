use CommDiagnostics;
use Time;

config var printCounts = false;
config var printAllCounts = false;

// this is a config param just so it can be in COMPOPTS
config param checkMaxAttained = false;

var timer:Timer;

proc start() {
  resetCommDiagnostics();
  startCommDiagnostics();
  timer.clear();
  timer.start();
}

proc stop() {
  timer.stop();
  stopCommDiagnostics();
}

/*
  Total the number of any operation on any locale. Returns a total that still
  splits up the operations by type.
 */
proc totalCounts(D:[LocaleSpace] commDiagnostics) : commDiagnostics {
  var t:commDiagnostics;
  t.get = 0;
  t.get_nb = 0;
  t.put = 0;
  t.put_nb = 0;
  t.test_nb = 0;
  t.wait_nb = 0;
  t.try_nb = 0;
  t.execute_on = 0;
  t.execute_on_fast = 0;
  t.execute_on_nb = 0;
  for x in D {
    t.get += x.get;
    t.get_nb += x.get_nb;
    t.put += x.put;
    t.put_nb += x.put_nb;
    t.test_nb += x.test_nb;
    t.wait_nb += x.wait_nb;
    t.try_nb += x.try_nb;
    t.execute_on += x.execute_on;
    t.execute_on_fast += x.execute_on_fast;
    t.execute_on_nb += x.execute_on_nb;
  }
  return t;
}

/*
  Get the total number of GET or non-blocking GET operations on any locale.
 */
proc totalGets(D:[LocaleSpace] commDiagnostics) : int {
  var t = totalCounts(D);
  return (t.get + t.get_nb):int;
}
/*
  Get the total number of PUT or non-blocking PUT operations on any locale.
 */
proc totalPuts(D:[LocaleSpace] commDiagnostics) : int {
  var t = totalCounts(D);
  return (t.put + t.put_nb):int;
}
/*
  Get the total number of execute_on, execute_on_fast, or execute_on_nb
  operations on any locale.
 */
proc totalOns(D:[LocaleSpace] commDiagnostics) : int {
  var t = totalCounts(D);
  return (t.execute_on + t.execute_on_fast + t.execute_on_nb):int;
}


// Checks anything with a passed maximum != max(int) is below the maximum.
// Prints out counts if the passed maximum == max(int) (or is not passed).
// Prints out all counts if called with no arguments.
proc report(maxGets=max(int), maxPuts=max(int), maxOns=max(int)) {
  var counts = getCommDiagnostics();

  if printAllCounts || printCounts then
    writeln("seconds elapsed: ", timer.elapsed());

  if printAllCounts then
    writeln(counts);

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
