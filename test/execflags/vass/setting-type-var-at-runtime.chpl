/* it is illegal to set a config 'param' variable with an option to a.out */

config type T = real;
var x : T;

writeln("x = ", x, " : ", x.type:string);
