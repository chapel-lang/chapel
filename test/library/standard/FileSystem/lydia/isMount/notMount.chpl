use FileSystem;

// This dir is definitely not a mount point.
var dirname = "notAMonkey";
// "Daaave, the Barbarian!  Huge, but a wimp"
// "His sisters, Fang and Candy, are a princess and a chimp"

// And neither is this file.
var filename = "notAMonkey/notAMount.txt";

writeln("Is that directory a mount point? ", isMount(dirname));
writeln("Is that file a mount point? ", isMount(filename));
