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

pragma "private" var wakeup: WakeupSyncVarHack;
pragma "private" var endCount: MyEndCount;

def setupTerminationDetection() {
  for loc in Locales do on loc do {
    endCount = new MyEndCount();
    wakeup = new WakeupSyncVarHack();
    if here.id == 0 {
      endCount.count = 1;
      endCount.localColor = TerminationColor.white;
      endCount.token = TerminationColor.white;
    } else {
      endCount.count = 0;
      endCount.localColor = TerminationColor.white;
    }
    begin tokenWaiter();
  }
}

def passToken() {
  var next = if here.id == 0 then numLocales-1 else here.id-1;
  var token = endCount.token;
  if endCount.localColor == TerminationColor.black then
    token = TerminationColor.black;
  if debug then
    writeln(here, ": Passing ", token, " token");
  on Locales(next) {
    endCount.token = token;
    wakeup.wakeup = WakeupType.tokenPass;
  }
  endCount.localColor = TerminationColor.white;
}

var finishedProg: single bool;

def tokenWaiter() {
  var wakeupType = wakeup.wakeup;
  while wakeupType != WakeupType.die {
    if endCount.token.isFull {
      if endCount.count.readFF() == 0 {
        if here.id == 0 {
          if wakeupType == WakeupType.beginFinish {
            endCount.localColor.writeXF(TerminationColor.white);
            endCount.token.writeXF(TerminationColor.white);
          } else if wakeupType == WakeupType.tokenPass {
            if endCount.token == TerminationColor.white {
              finishedProg = true;
            } else {
              endCount.localColor.writeXF(TerminationColor.white);
              endCount.token.writeXF(TerminationColor.white);
            }
          }
        }
        passToken();
      }
    }
    wakeupType = wakeup.wakeup;
  }
}

def decEndCount {
  endCount.count -= 1;
  if debug then
    writeln(here, ": decEndCount: It is now: ", endCount.count.readFF());
  if endCount.count.readFF() == 0 then
    wakeup.wakeup = WakeupType.beginFinish;
}

def incEndCount {
  endCount.count += 1;
  if debug then
    writeln(here, ": incEndCount: It is now: ", endCount.count.readFF());
}

def turnBlack {
  if debug then
    writeln(here, ": turning black");
  endCount.localColor.writeXF(TerminationColor.black);
}


var a: sync int = 0;

use Time;

def foo() {
  sleep(5);
  turnBlack;
  on Locales(0) {
    incEndCount;
    begin {
      sleep(3);
      a += 1;
      decEndCount;
    }
  }
  sleep(4);
  a += 1;
}

//
// A possible main program after compiler transformations for termination
// detection.
//
def main {
  setupTerminationDetection();
  incEndCount;
  begin {
    turnBlack;
    on Locales(1) {
      sleep(3);
      a += 1;
    }
    decEndCount;
  }
  incEndCount;
  begin {
    sleep(4);
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
  exit(if aa == 5 then 0 else 1);
}
