enum thingsToTakeScubaDiving { WetSuit, Fins, AirTank };

var thing1: thingsToTakeScubaDiving = thingsToTakeScubaDiving.Fins;
var thing2: thingsToTakeScubaDiving = thingsToTakeScubaDiving.WetSuit;

writeln("I brought: ", thing1, " ", thing2);
writeln("I forgot:  ", if (thing1 != thingsToTakeScubaDiving.AirTank && thing2 != thingsToTakeScubaDiving.AirTank) then thingsToTakeScubaDiving.AirTank:string else "Nothing");
