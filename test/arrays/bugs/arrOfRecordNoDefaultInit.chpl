record Rec
{
    var num : int;
    proc init( n: int ) { num = n; }   // 0-arg init() not defined
}

proc main()
{
    var x = new Rec(n = 1);      // this works
    writeln( "x = ", x, ", type = ", x.type:string );

    var r = [ new Rec(n = 1) ];  // this fails
    writeln( "r = ", r, ", type = ", r.type:string );
}
