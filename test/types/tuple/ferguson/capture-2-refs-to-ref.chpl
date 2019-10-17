var GLOBAL1: int;
var GLOBAL2: int;
proc singleVar1() ref { return GLOBAL1; }
proc singleVar2() ref { return GLOBAL2; }

proc test( ref x:(int, int) ) {
    x(0) = 1;
    x(1) = 2;
}

proc main {

    test( (singleVar1(), singleVar2()) );

    writeln( GLOBAL1, GLOBAL2 );
}
