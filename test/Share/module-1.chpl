

module rand;

var some_number : integer = 56;
var some_other_number : integer = 57;


module main;  -- what is the name of the module containing "main" ??

use rand only some_number;

writenln("expect 56 ", some_number);
