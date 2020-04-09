proc tupleToArray(t:int...?k) : [1..k] int
{
        var retr : [1..k] int;
        for i in 1..k
        {
            retr(i) = t(i-1);
        }

        return retr;
}

var x = tupleToArray(1, 2, 3);
//var y = tupleToArray(1.0, 2.0, 3.0);
//var z = tupleToArray("one", "two", "three");

writeln("x is: ", x);
//writeln("y is: ", y);
//writeln("z is: ", z);
