// forall-expression

use iters;

var arrIter2par = forall IND in zip(iter1par(), iter2par()) do IND;
writeln(arrIter2par);
