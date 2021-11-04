// This is from issue https://github.com/chapel-lang/chapel/issues/18656

use SortedSet;

enum E { A = 0, B, C, D, };

record R1 { proc key(a) return  a:int; };
record R2 { proc key(a) return -a:int; };

const comp1: R1, comp2: R2;

var v1 = new sortedSet(E, false, comp1);
var v2 = new sortedSet(E, false, comp2);

for l in E {
    v1.add(l);
    v2.add(l);
}

writeln(v1);  // [ A B C D ]
writeln(v2);  // [ D C B A ]

