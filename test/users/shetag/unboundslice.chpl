const a1 : [1..6] int = 1..6;
writeln("a1 = \n",a1,"\n");

var a2 : [1..6,1..3] int;

//error: halt reached - array index out of bounds: (7, 1)
[i in 1..3] a2(..,i) = a1;
//[i in 1..3] a2(1..6,i) = a1;

writeln("a2 = \n",a2,"\n");
