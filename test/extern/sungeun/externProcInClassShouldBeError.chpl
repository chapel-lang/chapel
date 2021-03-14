// This very similar to the test parallel/taskPar/sungeun/private.chpl
//  except that the extern declaration is accidentally placed at the
//  class-level.

// An example of how to create task private variables.
//
// This example should work for forall loop iterating over default
// arrays/domains (you could drop the dmap), as well as Block, Cyclic,
// and probably BlockCyclic arrays/domains.
//
use BlockDist, PrivateDist;

extern proc system(s: c_string): int;

record taskPrivateData {
  var tid$: sync chpl_taskID_t = chpl_nullTaskID;
  var x: int;
  var y: [0..#numLocales] real;

  // need our version of writeThis so we can print the sync field
  proc writeThis(x: Writer) throws {
    x.write("(", tid$.readXX(), ": ", x, "  ", y, ")");
  }
};

inline proc =(ref a: chpl_taskID_t, b: chpl_taskID_t) { __primitive("=", a, b); }
inline proc !=(a: chpl_taskID_t, b: chpl_taskID_t) return __primitive("!=", a, b);
class localePrivateData {
  type myStuff;
  // assumes maxTaskPar is the same across all locales
  const numTasks = if dataParTasksPerLocale==0 then here.maxTaskPar
    else dataParTasksPerLocale;
  var slot: sync bool;
  var r = {0..#numTasks};
  var temps: [r] myStuff;
  extern proc chpl_task_getId(): chpl_taskID_t;
  proc gettid() {
    var mytid = chpl_task_getId();
    var slot = (mytid:uint % (numTasks:uint)):int;
    // Would be nice to have CAS
    var tid: chpl_taskID_t = temps[slot].tid$; // lock
    while ((tid != chpl_nullTaskID) && (tid != mytid)) {
      temps[slot].tid$ = tid;                  // unlock
      slot = (slot+1)%numTasks;
      tid = temps[slot].tid$;                  // lock
    }
    temps[slot].tid$ = mytid;                  // unlock
    return slot;
  }
}

var localePrivate: [PrivateSpace] localePrivateData(taskPrivateData);
forall l in localePrivate do l = new localePrivateData(taskPrivateData);

// an example use
config param nPerLocale = 113;
config const printTemps = false;
const D = {0..#nPerLocale*numLocales} dmapped Block(boundingBox={0..#nPerLocale*numLocales});
forall d in D {
  // my copy of the task private vars
  var lp = localePrivate[here.id];

  var slot = lp.gettid();

  lp.temps[slot].x += slot+1;

  // make a system call to skew timing a bit
  if d%7==0 then system("sleep 1");

  lp.temps[slot].y[here.id] += 1;

}

if printTemps then writeln(localePrivate.temps);

const numTasks = if dataParTasksPerLocale==0 then here.maxTaskPar
  else dataParTasksPerLocale;

for l in 0..#numLocales {
  var lp = localePrivate[l];
  var y =  0.0;
  for slot in 0..#numTasks {
    y += + reduce lp.temps[slot].y;
    for l2 in 0..#numLocales {
      if l != l2 {
        if lp.temps[slot].y[l2] != 0.0 then
          halt("localePrivate[",l,"].temps[",slot,"].y[", l2, "]!=0.0! (",
               lp.temps[slot].y[l2],")");
      }
    }
  }
  if y != nPerLocale then
    halt("localePrivate[",l,"].temps[].y value is incorrect! (y=",y,")");
}


module blah {
  class C {
    const numTasks = if dataParTasksPerLocale==0 then here.maxTaskPar
      else dataParTasksPerLocale;
    extern proc chpl_task_getId(): chpl_taskID_t;
    proc bar() {
      var mytid = chpl_task_getId();
    }
  }
  proc foo() {
    var myC: [3..9] C;
    for c in myC do c = new C();
  }
}

if false {

extern class C {
  const mydom;
  extern proc chpl_task_getId(): chpl_taskID_t;
  proc bar() {
    var mytid = chpl_task_getId();
  }
}

const D = {5..9};
 var c: C;
// var c: C(D.type);
// c = new C(D);
// c.bar();

}

forall l in localePrivate do delete l;
