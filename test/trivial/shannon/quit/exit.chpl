module exit_test { // wrapping to avoid name conflict with test name
var gems : string = "platinum and sapphires";
var treasure : int = 1;
var found : int = 1;

if (treasure == found) then {
   writeln("The first piece has been found!");
   writeln("It's the dragonfly, made of ", gems, ".");
   exit(0);
}
writeln("Unfortunately, four tokens are missing.");
}
