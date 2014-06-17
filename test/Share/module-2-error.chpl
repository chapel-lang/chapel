

module rand;

var some_number : int = 56;
var some_other_number : int = 57;


module main;  // what is the name of the module containing "main" ??

use rand only some_number;

writenln("expect 56 ", some_other_number);
