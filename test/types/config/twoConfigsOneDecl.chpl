config type randType = uint(32),
            char = int(8);

var r = 42: randType;
var c = ascii('\n'): char;

writeln("r: ", r, ": ", r.type:string);
writeln("c: ", c, ": ", c.type:string);
