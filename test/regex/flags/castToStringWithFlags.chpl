
use Regex;

var r: regex(string);

r = new regex("0f*", nonGreedy=true);
writeln(r:string);

r = new regex("0f*", multiLine=true);
writeln(r:string);

r = new regex("0f*", nonGreedy=true, multiLine=true);
writeln(r:string);
