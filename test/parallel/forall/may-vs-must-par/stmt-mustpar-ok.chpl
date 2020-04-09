// forall-statement

use iters;
use datas;

writeln();

// Legend: the writeln() statements above the forall loops show:
//  first word:  the name of the iterator, implying the available varieties,
//  second word: which variety should be chosen.
// Ex: "SA standalone" => available: serial+SA; chosen: SA.
//  "par leader+followers" => available serial+SA+LF, chosen: LF.


/*--- iterators --- */

/////////// single iterable ///////////

// error - parallel iterator is not provided
//writeln("ser serial");
//forall IND in iter1ser() do
//  writeln("forall IND in iter1ser");

writeln("SA standalone");
forall IND in iter1SA() do
  writeln("forall IND in iter1SA");
showCountsI;

writeln("LF leader+follower");
forall IND in iter1LF() do
  writeln("forall IND in iter1LF");
showCountsI;

writeln("par standalone");
forall IND in iter1par() do
  writeln("forall IND in iter1par");
showCountsI;

/////////// zippered ///////////

// error - parallel iterator is not provided
//writeln("ser serial");
//forall IND in zip(iter1ser(), iter2ser()) do
//  writeln("forall IND in zip(iter1ser(), iter2ser())");

// error - leader is not provided
//writeln("SA leader");
//forall IND in zip(iter1SA(), iter2SA()) do
//  writeln("forall IND in zip(iter1SA(), iter2SA())");

writeln("LF leader+followers");
forall IND in zip(iter1LF(), iter2LF()) do
  writeln("forall IND in zip(iter1LF(), iter2LF())");
showCountsI;

writeln("par leader+followers");
forall IND in zip(iter1par(), iter2par()) do
  writeln("forall IND in zip(iter1par(), iter2par())");
showCountsI;


/*--- datatypes --- */

/////////// single iterable ///////////

// error - parallel iterator is not provided
//writeln("ser serial");
//forall IND in inst1ser do
//  writeln("forall IND in inst1ser");

writeln("SA standalone");
forall IND in inst1SA do
  writeln("forall IND in inst1SA");
showCountsD;

writeln("LF leader+follower");
forall IND in inst1LF do
  writeln("forall IND in inst1LF");
showCountsD;

writeln("par standalone");
forall IND in inst1par do
  writeln("forall IND in inst1par");
showCountsD;

/////////// zippered ///////////

// error - parallel iterator is not provided
//writeln("ser serial");
//forall IND in zip(inst1ser, inst2ser) do
//  writeln("forall IND in zip(inst1ser, inst2ser)");

// error - leader is not provided
//writeln("SA leader");
//forall IND in zip(inst1SA, inst2SA) do
//  writeln("forall IND in zip(inst1SA, inst2SA)");

writeln("LF leader+followers");
forall IND in zip(inst1LF.borrow(), inst2LF.borrow()) do
  writeln("forall IND in zip(inst1LF, inst2LF)");
showCountsD;

writeln("par leader+followers");
forall IND in zip(inst1par.borrow(), inst2par.borrow()) do
  writeln("forall IND in zip(inst1par, inst2par)");
showCountsD;
