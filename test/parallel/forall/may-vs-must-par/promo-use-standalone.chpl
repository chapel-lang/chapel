use iters;
use datas;

proc multiply(arg1: int, arg2: int)   { writeln("mul=", arg1*arg2); }

/* Currently these always use the serial iterator:
var arrIter1SA  = [ IND in iter1SA()  ] IND;
var arrIter1par = [ IND in iter1par() ] IND;
var arrInst1SA  = [ IND in inst1SA  ] IND;
var arrInst1par = [ IND in inst1par ] IND;
*/

multiply([ IND in iter1SA()  ] IND, 0);
multiply([ IND in iter1par() ] IND, 0);
multiply([ IND in inst1SA.borrow()  ] IND, 0);
multiply([ IND in inst1par.borrow() ] IND, 0);
