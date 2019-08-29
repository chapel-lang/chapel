var b: atomic bool;                            writeln(b);
b.compareExchange(false, true);                writeln(b);
b.compareExchangeStrong(true, false);          writeln(b);
while !b.compareExchangeWeak(false, true) { }  writeln(b);

var i: atomic int;                             writeln(i);
i.compareExchange(0, 1);                       writeln(i);
i.compareExchangeStrong(1, 2);                 writeln(i);
while !i.compareExchangeWeak(2, 3) { }         writeln(i);
