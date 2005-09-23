var d : _adomain(2);
d._set(1, 1..5);
d._set(2, 2..8 by 2);
writeln(d);

writeln(d._forall(1));
writeln(d._forall(2));
writeln(d._forall());
