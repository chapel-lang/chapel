// Any auxilliary functions, classes etc should go here

use MDTypes;
use configs;
use Time;

inline proc dot(const in v1:real3, const in v2:real3) : real {
  var r : real = 0.0;
  for i in 0..2 do r += v1(i) * v2(i);
  return r;
}

proc postfix!(A:[]) { var B = for a in A do a!; return B; } //#15080

class Ticker {
  var name : string;
  var tick : Timer;
  var duration : real;
  var times : int;

  proc init(const in tName : string) {
    this.complete();
    name = tName;
    tick = new Timer();
    tick.clear();
    duration = 0.0;
    times = 0;
  }

  inline proc start() {
    tick.start();
  }

  inline proc stop() : real {
    tick.stop();
    var elapsed : real = tick.elapsed();
    duration += elapsed;
    times += 1;
    tick.clear();
    return elapsed;
  }
	
  inline proc print(loopTime: real) : void {
    writef("%s %8di %12.4dr %12.4dr %12.2dr\n", name, times, duration/times, duration, duration/loopTime * 100);
  }
}

const timers = 19;
enum timerEnum {TOTAL = 1, INIT, FCREATE, INITGRID, EPILOGUE, REDIST1, F1, LOOP, TIMESTEP, POSITION, VELOCITY, REDIST, ATOMHALO, SORT, FORCE, FORCE1, FORCE2, EAMHALO, COMMREDUCE};
var tArray : [timerEnum.TOTAL..timerEnum.COMMREDUCE] unmanaged Ticker = 
(	new unmanaged Ticker("total              "),
	new unmanaged Ticker("init               "),
	new unmanaged Ticker("  createForce      "),
	new unmanaged Ticker("  initGrid         "),
	new unmanaged Ticker("  replicateForce   "),
	new unmanaged Ticker("  redistributeInit "),
	new unmanaged Ticker("  forceInit        "),
	new unmanaged Ticker("loop               "),
	new unmanaged Ticker("timestep           "),
	new unmanaged Ticker("  position         "),
	new unmanaged Ticker("  velocity         "),
	new unmanaged Ticker("  redistribute     "),
	new unmanaged Ticker("    atomHalo       "),
	new unmanaged Ticker("    sortAtoms      "),
	new unmanaged Ticker("  force            "),
	new unmanaged Ticker("    force1         "),
	new unmanaged Ticker("    force2         "),
	new unmanaged Ticker("    eamHalo        "),
	new unmanaged Ticker("reductions         "));
