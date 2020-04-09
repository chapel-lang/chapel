const a1 : [1..6] int = 1..6;
writeln("a1 = \n",a1,"\n");

const a3 : [1..6,1..3] int = [(i,j) in {1..6,1..3}] i+j;
writeln("a3 = \n",a3,"\n");

var a4 : [1..3] int;

//error: halt reached - zippered iterations have non-equal lengths
[i in 1..3] a4(i) = + reduce (a1*a3(.., i));
//[i in 1..3] a4(i) = + reduce (a1*a3(1..6,i));

writeln("a4 = \n",a4,"\n");
