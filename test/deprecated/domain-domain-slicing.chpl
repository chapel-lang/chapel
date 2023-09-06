// deprecated by Vass in 1.32

use BlockDist;

var D5 = {1..5};
var D9 = {1..9};
var DB = Block.createDomain(1..7);

var sli1 = D9[D5];
var sli2 = D9[DB];
var sli3 = DB[D5];

writeln(sli1, " : ", sli1.type:string);
writeln(sli2, " : ", sli2.type:string);
writeln(sli3, " : ", sli3.type:string);
