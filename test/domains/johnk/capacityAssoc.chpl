use Random;

var D : domain(string);
var arr : [D] int;

D += "foo";
D += "bar";

arr["foo"] = 4;
arr["bar"] = 5;

//Pick a random initial capacity for the associative domain
var rs = new RandomStream();
var min = 5;
var max = 3500;
var capacity = (rs.getNext() * (max - min) + min): int;
D.requestCapacity(capacity);


if (D._value.tableSize > 2 * (capacity + 1)) &&
   arr["foo"] == 4 && 
   arr["bar"] == 5 
{
    writeln("SUCCESS");
} else {
    writeln("FAILURE");
}
