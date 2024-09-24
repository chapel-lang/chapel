// Once this generates a good error message,
// remove it and uncomment 'writeln()' in carray-void.chpl

use CTypes;

var arr: c_array(void, 5);
writeln(arr);
