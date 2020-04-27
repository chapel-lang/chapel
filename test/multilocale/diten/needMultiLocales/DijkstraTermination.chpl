config param debug: bool = false;

//
// Simple, stand-alone implementation of EWD840:
// Derivation of a termination detection algorithm for distributed computations
// "Compiler" transformed user code starts at "use Time;". The transformations
// the compiler would apply are: 
//   1. Call setupTerminationDetection() at the beginning of execution
//   2. Insert a call to incEndCount before every begin statement
//   3. Insert a call to decEndCount at the end of every begin statement
//   4. Insert a call to turnBlack before every on statement
//   5. Add a "single" variable to wait upon at the end of main
//

enum TerminationColor {black, white};
enum WakeupType {beginFinish, tokenPass, die};

class MyEndCount {
  var count: sync int;
  var localColor: sync TerminationColor;
  var token: sync TerminationColor;
}

class WakeupSyncVarHack {
  var wakeup: sync WakeupType;
}

// keep them unmanaged as it makes it much easier with locale private
pragma "locale private" var wakeup: unmanaged WakeupSyncVarHack?;
pragma "locale private" var endCount: unmanaged MyEndCount?;

proc setupTerminationDetection() {
  for loc in Locales do on loc {
    endCount = new unmanaged MyEndCount();
    wakeup = new unmanaged WakeupSyncVarHack();
    if here.id == 0 {
      endCount!.count = 1;
      endCount!.localColor = TerminationColor.white;
      endCount!.token = TerminationColor.white;
    } else {
      endCount!.count = 0;
      endCount!.localColor = TerminationColor.white;
    }
    begin tokenWaiter();
  }
}

proc passToken(wakeupType) {
  var next = if here.id == 0 then numLocales-1 else here.id-1;
  var token = endCount!.token;
  if endCount!.localColor == TerminationColor.black then
    token = TerminationColor.black;
  if debug then
    writeln(here, ": Passing ", token, " token");
  on Locales(next) {
    endCount!.token = token;
    if wakeupType != WakeupType.die then
      wakeup!.wakeup = WakeupType.tokenPass;
    else
      wakeup!.wakeup = WakeupType.die;
  }
  endCount!.localColor = TerminationColor.white;
}

var finishedProg: single bool;

proc tokenWaiter() {
  var wakeupType = wakeup!.wakeup;
  while wakeupType != WakeupType.die {
    var nextWakeupType = wakeupType;
    if endCount!.token.isFull {
      if endCount!.count.readFF() == 0 {
        if here.id == 0 {
          if wakeupType == WakeupType.beginFinish {
            endCount!.localColor.writeXF(TerminationColor.white);
            endCount!.token.writeXF(TerminationColor.white);
          } else if wakeupType == WakeupType.tokenPass {
            if endCount!.token == TerminationColor.white {
              finishedProg = true;
              nextWakeupType = WakeupType.die;
              endCount!.localColor.writeXF(TerminationColor.white);
              endCount!.token.writeXF(TerminationColor.white);
            } else {
              endCount!.localColor.writeXF(TerminationColor.white);
              endCount!.token.writeXF(TerminationColor.white);
            }
          }
        }
        passToken(nextWakeupType);
      }
    }
    wakeupType = wakeup!.wakeup;
  }
  if here.id != 0 then
    passToken(WakeupType.die);
}

proc decEndCount {
  endCount!.count -= 1;
  if debug then
    writeln(here, ": decEndCount: It is now: ", endCount!.count.readFF());
  if endCount!.count.readFF() == 0 then
    wakeup!.wakeup = WakeupType.beginFinish;
}

proc incEndCount {
  endCount!.count += 1;
  if debug then
    writeln(here, ": incEndCount: It is now: ", endCount!.count.readFF());
}

proc turnBlack {
  if debug then
    writeln(here, ": turning black");
  endCount!.localColor.writeXF(TerminationColor.black);
}


var a: sync int = 0;

use Time;

proc foo() {
  // doWork3();
  turnBlack;
  on Locales(0) {
    incEndCount;
    begin {
      // doWork4();
      a += 1;
      decEndCount;
    }
  }
  // doWork5();
  a += 1;
}

//
// A possible main program after compiler transformations for termination
// detection.
//
proc main {
  setupTerminationDetection();
  incEndCount;
  begin {
    turnBlack;
    on Locales(1) {
      // doWork1();
      a += 1;
    }
    decEndCount;
  }
  incEndCount;
  begin {
    // doWork2();
    a += 1;
    decEndCount;
  }
  incEndCount;
  begin {
    turnBlack;
    on Locales(1) {
      incEndCount;
      begin {
        foo();
        a += 1;
        decEndCount;
      }
    }
    decEndCount;
  }

  decEndCount;
  finishedProg;
  write("program finished ");
  var aa = a;
  if aa == 5 then
    writeln("successfully");
  else
    writeln("unsuccessfully: a is ", aa);
}

proc deinit() {
  for l in Locales do on l {
    delete wakeup;
    delete endCount;
  }
}
