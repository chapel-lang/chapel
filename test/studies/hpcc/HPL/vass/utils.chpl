use Time, Memory;

// vmsg()

  config var vPrintOff   = false;
  config var vPrintOn    = true;
  proc       vDoPrint      return vPrintOn && !vPrintOff;
  config var vPrintStart = false;
  config var vPrintCurr  = true;
//  config var vPrintMem   = memTrack;
param vPrintMem = false;

  const vUserSep = "  ";
  const vGB = (1024**3):real;
  const vStartTime = getCurrentTime();
  var   vLapTime   = vStartTime;
  var   vLapMem    = getCurrentMem();

  proc getCurrentMem()
    return if memTrack then memoryUsed():real / vGB else 0;

  proc getCurrentTimeLoc() {  // go to the same node for the timer
    var result: vLapTime.type;
    on vLapTime.locale do result = getCurrentTime();
    return result;
  }

  proc vPrintHelp()
    return
      (if vPrintStart || vPrintCurr then vPrintHelpTime() else "") +
      (if vPrintMem                 then vPrintHelpMem()  else "");

  proc vPrintHelpTime() {
    var currTime = getCurrentTimeLoc();
    const prevTime = vLapTime;
    vLapTime = currTime;
    return
      (if vPrintStart then "%{###.##}".format(currTime - vStartTime) else "") +
      (if vPrintCurr  then "%{####.###}".format(currTime - prevTime)  else "") +
      (if vPrintMem then "" else vUserSep);
  }

  // assumes vPrintMem==true
  proc vPrintHelpMem() {
    const currMem = getCurrentMem();
    const prevMem = vLapMem;
    vLapMem = currMem;
    return "%{####.###}".format(currMem - prevMem) + vUserSep;
  }

  proc vmsg(args...) { if vDoPrint then writeln(vPrintHelp(), (...args));  }

config var vmore = false;
proc vmsgmore(args...) { if vmore then vmsg((...args)); }


/////////////////////////////////////////////////////////////////////////////
// printCommDiagnostics()

// the default 'format' for printCommDiagnostics()
config var defaultCommDiagnosticsFormat = 8;

//
// Print comm diagnostics one comm type per line.
// 'dest' - the channel to print to.
// 'format' - indicates how to print each number:
//   -1 (see below)
//   0  preceeded by a tab
//   1  preceeded by a space
//   N  occupying at least N positions (N is a positive int)
//
// When format=-1, all comm diagnostics are printed as a sequence of records,
// one record per locale, all on a single line.
//
proc printCommDiagnostics(format=defaultCommDiagnosticsFormat, dest=stdout)
{
  var nstyle = new iostyle(), pad = false;

  select true {
    when format == -1 {
      dest.writeln(getCommDiagnostics());
      return;
    }
    when format > 1 {
      nstyle.min_width = format:(nstyle.min_width.type);
      pad = true;
    }
    when format < 0 {
      halt("printCommDiagnostics(): invalid format ", format);
    }
    }

  proc show(num: integral) {
    select format {
      when 0 do dest.write("\t", num);
      when 1 do dest.write(" ", num);
      otherwise dest.write(num, nstyle);
      }
  }
  // 'h2' is the header 'h1' padded to a given length (currently 7)
  proc show(h1: string, h2: string) {
    dest.write(if pad then h2 else h1);
  }

  show("get", "get    ");
  for loc in Locales do on loc do show(chpl_numCommGets());
  dest.writeln();

  show("put", "put    ");
  for loc in Locales do on loc do show(chpl_numCommPuts());
  dest.writeln();

  show("fork", "fork   ");
  for loc in Locales do on loc do show(chpl_numCommForks());
  dest.writeln();

  show("fork_f", "fork_f ");
  for loc in Locales do on loc do show(chpl_numCommFastForks());
  dest.writeln();

  show("fork_nb","fork_nb");
  for loc in Locales do on loc do show(chpl_numCommNBForks());
  dest.writeln();

  show("get_nb", "get_nb ");
  for loc in Locales do on loc do show(chpl_numCommNBGets());
  dest.writeln();
/*
  show("get_nbt", "get_nbt");
  for loc in Locales do on loc do show(chpl_numCommTestNBGets());
  dest.writeln();

  show("get_nbw", "get_nbw");
  for loc in Locales do on loc do show(chpl_numCommWaitNBGets());
  dest.writeln();
*/
}

/////////////////////////////////////////////////////////////////////////////

config var VTimerQuiet = false;
config var VTimerFatal = false;
proc VTimerError(args...) {
  if VTimerFatal then halt((...args)); else writeln("Error: ", (...args));
}

// How many decimals when printing time; the width when printing timers' names.
config var VTimerDigits: int(32) = 2;
config var VTimerAutoWidth = true;
var VTimerWidth = 1;
proc VTimerAdjustWidth(name: string) {
  VTimerWidth = max(VTimerWidth, name.size);
}

record VTimerG {
  // parameters
  var name: string;
  var  singleUse: bool;
  proc cumulative: bool  return !singleUse;
  var printOnStop: bool;
  var verbPrint: bool;
  var resetOnPrint: bool;

  // constructor
  proc VTimerG(name: string, singleUse = true, printOnStop = singleUse,
               verbPrint = !singleUse, resetOnPrint = false)
  {
    this.name         = name;
    this.singleUse    = singleUse;
    this.printOnStop  = printOnStop;
    this.verbPrint    = verbPrint;
    this.resetOnPrint = resetOnPrint;
    VTimerAdjustWidth(name);
  }

  // implementation
  var timer: Timer;
  var hasBeenStarted: bool = false;
  var invCount = 0;
  var totElapsed = 0:real;

  proc elapsed()  return timer.elapsed();
  proc total()    return totElapsed;

  // does not reset the total
  proc reset() {
    timer.clear();
  }

  proc start(singleUse = this.singleUse) {
    if hasBeenStarted then
      VTimerError("timer ", name, " is started repeatedly");
    if singleUse then
      hasBeenStarted = true;
    timer.start();
  }

  proc stop(printOnStop = this.printOnStop) {
    timer.stop();
    invCount += 1;
    totElapsed += elapsed();
    if printOnStop then print();
    timer.clear();
  }

  proc print(resetOnPrint = this.resetOnPrint, verbPrint = this.verbPrint) {
    if VTimerQuiet then return;
    const fmt = "%-" + VTimerWidth:string + "s";
    const name = fmt.format(this.name);
    const style = new iostyle(realfmt = 1, precision = VTimerDigits);
    if verbPrint then
      writeln(name, " : cnt ", invCount,"  elapsed ", elapsed(),
              "  total ", total(), style);
    else
      writeln(name, " : elapsed ", elapsed(), style);
    if resetOnPrint then reset();
  }

  // a future extention
  proc printTotal() {
    //if VTimerQuiet then return;
    print();
  }

}  // record VTimerG

proc VTimerSelfTest() {

  var tm1 = vt1("single-use");
  var tmc = vtc("cumulative timer");
  var tm3 = vt1("test");

  tm1.start();
  tmc.start();
  sleep(2);
  tmc.stop();
  tm1.stop();

  tm3.start();
  tmc.start();
  sleep(3);
  tm3.stop();
  tmc.stop(true);
  tmc.print();
}

/////////////////////////////////////////////////////////////////////////////

// conveniences: 1-use vs. Cumulative vs. repeated
proc vt1(name: string) return new VTimerG(name, singleUse = true);
proc vtc(name: string) return new VTimerG(name, singleUse = false);
proc vtr(name: string) return new VTimerG(name, singleUse = false,
                           printOnStop = true, resetOnPrint = true);

var tLF1iter   = vtr("LUFactorize");
var tSC1call   = vtr("schurComplement");
var tPS1iter   = vtr("panelSolve");
var tUBR1iter  = vtr("updateBlockRow");
var tBScall    = vtr("backwardSub");
var tVer = vt1("verifyResults");
var tInit      = vtr("initAB");
