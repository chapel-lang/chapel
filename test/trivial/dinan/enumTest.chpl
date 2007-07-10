enum thingsToTakeScubaDiving { WetSuit, Fins, AirTank };

var thing1: thingsToTakeScubaDiving = Fins;
var thing2: thingsToTakeScubaDiving = WetSuit;

writeln("I brought: ", thing1, " ", thing2);
writeln("I forgot:  ", if (thing1 != AirTank && thing2 != AirTank) then AirTank else "Nothing");
