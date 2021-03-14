// From vass:
var w = 10..10 by 2 align 1;
writeln(w.hasFirst());  // false
writeln(w.hasLast());   // false
writeln(w.first);       // undefined (11 is OK)
writeln(w.last);        // undefined (9 is OK)
writeln(w.size);      // 0
