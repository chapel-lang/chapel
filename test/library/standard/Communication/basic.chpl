use Communication;
use CTypes;

var A = [i in 1..10] i;


var B: [1..10] int;

get(c_ptrTo(B), c_ptrTo(A), A.locale.id, A.size*numBytes(A.eltType));

writeln(B);
