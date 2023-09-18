class MyInt { var x: int; }

use List;
var l1 = new list((int, owned MyInt?));
var l2 = new list((int, shared MyInt?));
l1.pushBack((1, new owned MyInt?(1)));
l1.pushBack((2, new owned MyInt?(2)));
l2.pushBack((3, new shared MyInt?(3)));
l2.pushBack((4, new shared MyInt?(4)));
writeln(l1);
writeln(l2);
