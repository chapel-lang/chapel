record Foo
{
    var vec : [1..3] int;
}

proc main()
{
    var arr : [1..2] Foo;

    arr[ 1 ].vec = 100;
    arr[ 2 ].vec = 200;
    writeln( arr );  // (vec = 100 100 100) (vec = 200 200 200)

    // arr.vec = 500;        // error: cannot iterate over values of type int(64) (L1)
    arr[ .. ].vec = 500;  // error: cannot iterate over values of type int(64) (L2)
    // arr.vec[ .. ] = 500;        // internal error (E1)
    // arr[ .. ].vec[ .. ] = 500;  // internal error (E2)
    writeln( arr );
}
