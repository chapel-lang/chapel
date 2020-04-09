// reduce expression

// Legend: the writeln() statements above the promoted expressions show:
//  first word:  the name of the iterator, implying the available varieties,
//  second word: which variety should be chosen.
// Ex: "SA serial" => available: serial+SA; chosen: serial.
//  "par leader+followers" => available serial+SA+LF, chosen: LF.

use iters;
use datas;

writeln();


/*--- iterators --- */

/////////// single iterable ///////////

writeln("ser serial");
writeln(+reduce iter1ser());
showCountsI;

writeln("SA standalone");
writeln(+reduce iter1SA());
showCountsI;

writeln("LF leader+follower");
writeln(+reduce iter1LF());
showCountsI;

writeln("par standalone");
writeln(+reduce iter1par());
showCountsI;

/////////// zippered ///////////

// bug: compiler crashes
//writeln("ser serial");
//writeln(+reduce zip(iter1ser(), iter2ser()));
//showCountsI;

// bug: compiler crashes
//writeln("SA serial");
//writeln(+reduce zip(iter1SA(), iter2SA()));
//showCountsI;

writeln("LF leader+followers");
writeln(+reduce zip(iter1LF(), iter2LF()));
showCountsI;

writeln("par leader+followers");
writeln(+reduce zip(iter1par(), iter2par()));
showCountsI;


/*--- datatypes --- */

/////////// single iterable ///////////

writeln("ser serial");
writeln(+reduce inst1ser.borrow());
showCountsD;

writeln("SA standalone");
writeln(+reduce inst1SA.borrow());
showCountsD;

writeln("LF leader+follower");
writeln(+reduce inst1LF.borrow());
showCountsD;

writeln("par standalone");
writeln(+reduce inst1par.borrow());
showCountsD;

/////////// zippered ///////////

// bug: compiler crashes
//writeln("ser serial");
//writeln(+reduce zip(inst1ser.borrow(), inst2ser.borrow()));
//showCountsD;

// bug: compiler crashes
//writeln("SA serial");
//writeln(+reduce zip(inst1SA.borrow(), inst2SA.borrow()));
//showCountsD;

writeln("LF leader+followers");
writeln(+reduce zip(inst1LF.borrow(), inst2LF.borrow()));
showCountsD;

writeln("par leader+followers");
writeln(+reduce zip(inst1par.borrow(), inst2par.borrow()));
showCountsD;
