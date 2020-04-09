// forall-expression

use datas;

var arrInst2par = forall IND in zip(inst1par.borrow(), inst2par.borrow()) do IND;
writeln(arrInst2par);
