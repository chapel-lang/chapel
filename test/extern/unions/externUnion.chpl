/**********************************************************
/* This union example is also provided in
/* $CHPL_HOME/doc/rst/language/spec/interoperability.rst
/**********************************************************/

extern record _someUnion {
    var x: real(32);
    var y: real(64);
}

var myUnion: _someUnion;

myUnion.x = 1.5: real(32);
writeln("myUnion.x = ", myUnion.x);

myUnion.y = -2.6: real(64);
writeln("myUnion.y = ", myUnion.y);

var myA: [1..5] _someUnion;

for i in 1..5 {
    myA[i].x = i: real(32);
    writeln("myA", "[", i, "]", ".x = ", myA[i].x);
    myA[i].y = -i: real(64) / (5.0: real(64));
    writeln("myA", "[", i, "]", ".y = ", myA[i].y);
}
