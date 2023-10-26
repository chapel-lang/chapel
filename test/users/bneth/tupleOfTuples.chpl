var tuple1 = (1,2,3);
var tuple2 = ('c', "string", 6);
var tuple3 = (8.0,);

var tupleOfTuples = (tuple1, tuple2, tuple3);

for tuple in tupleOfTuples {
    for element in tuple {
        writeln(element);
    }
}