// Test a sync var in a conditional.  Using cobegin threads this time.

use Time;

var done: sync bool;
var data = 2;

cobegin with (ref data) {
  { 
    sleep( 2);
    data = 5;
    done = true;
  }
  if done then
    writeln( data);
}
