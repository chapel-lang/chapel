// this is a test from issue #11987

proc mult10( ref i: int ): void {
    i *= 10;
}

proc test2( fn ) {
    var y = 5;
    fn( y );                // at time test created, was compilation error
    writeln( "y = ", y );   // expected result = 50
}

var x = 7;
mult10( x );
writeln( "x = ", x ); // gives 70

test2( mult10 );  // gives 50
