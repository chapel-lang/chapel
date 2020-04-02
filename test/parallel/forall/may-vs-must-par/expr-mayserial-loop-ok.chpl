// []-expression

use iters;
use datas;

proc multiply(arg1: int, arg2: int)  { writeln("mul=", arg1*arg2); }
proc tuplify(arg1: 2*int, arg2: int) { writeln("tup=", (arg1(0)+arg1(1))*arg2); }

writeln();


/*--- iterators --- */

/////////// single iterable ///////////

writeln("iter1ser()");
[ idx in ( [ IND in iter1ser() ] IND ) ]
  multiply(idx,0);
showCountsI;

writeln("iter1SA()");
[ idx in ( [ IND in iter1SA()  ] IND ) ] //bug: ignored standalone iterator
  multiply(idx,0);
showCountsI;

writeln("iter1LF()");
[ idx in ( [ IND in iter1LF()  ] IND ) ]
  multiply(idx,0);
showCountsI;

writeln("iter1par()");
[ idx in ( [ IND in iter1par() ] IND ) ] //bug: ignored standalone iterator
  multiply(idx,0);
showCountsI;

/////////// zippered ///////////

writeln("zip(iter1ser(),iter2ser())");
[ idx in ( [ IND in zip(iter1ser(), iter2ser()) ] IND ) ]
  tuplify(idx,1);
showCountsI;

writeln("zip(iter1SA(),iter2SA())");
[ idx in ( [ IND in zip(iter1SA(),  iter2SA())  ] IND ) ]
  tuplify(idx,1);
showCountsI;

writeln("zip(iter1LF(),iter2LF())");
[ idx in ( [ IND in zip(iter1LF(),  iter2LF())  ] IND ) ]
  tuplify(idx,1);
showCountsI;

writeln("zip(iter1par(),iter2par())");
[ idx in ( [ IND in zip(iter1par(), iter2par()) ] IND ) ]
  tuplify(idx,1);
showCountsI;


/*--- datatypes --- */

/////////// single iterable ///////////

writeln("inst1ser");
[ idx in ( [ IND in inst1ser ] IND ) ]
  multiply(idx,0);
showCountsD;

writeln("inst1SA");
[ idx in ( [ IND in inst1SA  ] IND ) ] //bug: ignored standalone iterator
  multiply(idx,0);
showCountsD;

writeln("inst1LF");
[ idx in ( [ IND in inst1LF  ] IND ) ]
  multiply(idx,0);
showCountsD;

writeln("inst1par");
[ idx in ( [ IND in inst1par ] IND ) ]
  multiply(idx,0);
showCountsD;

/////////// zippered ///////////

writeln("zip(inst1ser,inst2ser)");
[ idx in ( [ IND in zip(inst1ser.borrow(), inst2ser.borrow()) ] IND ) ]
  tuplify(idx,1);
showCountsD;

writeln("zip(inst1SA,inst2SA)");
[ idx in ( [ IND in zip(inst1SA.borrow(),  inst2SA.borrow())  ] IND ) ]
  tuplify(idx,1);
showCountsD;

writeln("zip(inst1LF,inst2LF)");
[ idx in ( [ IND in zip(inst1LF.borrow(),  inst2LF.borrow())  ] IND ) ]
  tuplify(idx,1);
showCountsD;

writeln("zip(inst1par,inst2par)");
[ idx in ( [ IND in zip(inst1par.borrow(), inst2par.borrow()) ] IND ) ]
  tuplify(idx,1);
showCountsD;
