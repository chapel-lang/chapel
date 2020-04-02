// forall-expression

use iters;
use datas;

proc multiply(arg1: int, arg2: int)  { writeln("mul=", arg1*arg2); }
proc tuplify(arg1: 2*int, arg2: int) { writeln("tup=", (arg1(0)+arg1(1))*arg2); }

writeln();


/*--- iterators --- */

/////////// single iterable ///////////

// error - parallel iterator is not provided
//writeln("iter1ser()");
//[ idx in (forall IND in iter1ser() do IND) ]
//  multiply(idx,0);

writeln("iter1SA()");
[ idx in (forall IND in iter1SA()  do IND) ] //bug: ignored standalone iterator
  multiply(idx,0);
showCountsI;

writeln("iter1LF()");
[ idx in (forall IND in iter1LF()  do IND) ]
  multiply(idx,0);
showCountsI;

writeln("iter1par()");
[ idx in (forall IND in iter1par() do IND) ] //bug: ignored standalone iterator
  multiply(idx,0);
showCountsI;

/////////// zippered ///////////

// error - leader is not provided
//writeln("zip(iter1ser(),iter2ser())");
//[ idx in (forall IND in zip(iter1ser(), iter2ser()) do IND) ]
//  tuplify(idx,1);

// error - leader is not provided
//writeln("zip(iter1SA(),iter2SA())");
//[ idx in (forall IND in zip(iter1SA(),  iter2SA())  do IND) ]
//  tuplify(idx,1);

writeln("zip(iter1LF(),iter2LF())");
[ idx in (forall IND in zip(iter1LF(),  iter2LF())  do IND) ]
  tuplify(idx,1);
showCountsI;

writeln("zip(iter1par(),iter2par())");
[ idx in (forall IND in zip(iter1par(), iter2par()) do IND) ]
  tuplify(idx,1);
showCountsI;


/*--- datatypes --- */

/////////// single iterable ///////////

// error - parallel iterator is not provided
//writeln("inst1ser");
//[ idx in (forall IND in inst1ser do IND) ]
//  multiply(idx,0);

writeln("inst1SA");
[ idx in (forall IND in inst1SA  do IND) ] //bug: ignored standalone iterator
  multiply(idx,0);
showCountsD;

writeln("inst1LF");
[ idx in (forall IND in inst1LF  do IND) ]
  multiply(idx,0);
showCountsD;

writeln("inst1par");
[ idx in (forall IND in inst1par do IND) ]
  multiply(idx,0);
showCountsD;

/////////// zippered ///////////

// error - leader is not provided
//writeln("zip(inst1ser,inst2ser)");
//[ idx in (forall IND in zip(inst1ser.borrow(), inst2ser.borrow()) do IND) ]
//  tuplify(idx,1);

// error - leader is not provided
//writeln("zip(inst1SA,inst2SA)");
//[ idx in (forall IND in zip(inst1SA.borrow(),  inst2SA.borrow())  do IND) ]
//  tuplify(idx,1);

writeln("zip(inst1LF,inst2LF)");
[ idx in (forall IND in zip(inst1LF.borrow(),  inst2LF.borrow())  do IND) ]
  tuplify(idx,1);
showCountsD;

writeln("zip(inst1par,inst2par)");
[ idx in (forall IND in zip(inst1par.borrow(), inst2par.borrow()) do IND) ]
  tuplify(idx,1);
showCountsD;
