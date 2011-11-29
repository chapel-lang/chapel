proc tupleToArray(t:?eltType...3) : [1..3] eltType
{
        var retr : [1..3] eltType;
        for i in 1..3
        {
            retr(i) = t(i);
        }

        return retr;
}

var x = tupleToArray(1, 2, 3);
var y = tupleToArray(1.0, 2.0, 3.0);
var z = tupleToArray("one", "two", "three");

writeln("x is: ", x);
writeln("y is: ", y);
writeln("z is: ", z);
