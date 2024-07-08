
use IO;
use arrays;

use Time;

proc main() {
  {
    var x = 1..5;
    test(x);
  }
  {
    var x = {1..10, 1..10};
    test(x);
  }
  {
    var x = new date(2002, 3, 1); 
    test(x);
  }
  {
    var x = new dateTime(2002, 3, 1); 
    test(x);
  }
}
