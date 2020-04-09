// Verifies that use of enum does not prevent normal enum access
enum blondie {oneWay, orAnother, oneDay, maybeNextWeek};

use blondie;

var first: blondie;
var second: blondie = blondie.orAnother;
var third = blondie.oneDay;
writeln(first, ", ", second, " I'm gonna find ya");
writeln("I'm gonna getcha getcha getcha getcha");
writeln(third, ", maybe next week . . .");
