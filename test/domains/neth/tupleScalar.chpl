var scalarToTuple1 = makeRectangularDomain(0,(10,));
var scalarToTuple2 = makeRectangularDomain(0,(10,10));
var scalarToTuple3 = makeRectangularDomain(3,(10,10,10),inclusive=false);

var s2t1Correct = {0..10};
var s2t2Correct = {0..10,0..10};
var s2t3Correct = {3..<10,3..<10,3..<10};

if (scalarToTuple1 != s2t1Correct) {
    writeln("scalar to tuple domain failed for 1 dimensional tuple");
}
if (scalarToTuple2 != s2t2Correct) {
    writeln("scalar to tuple domain failed for 2 dimensional tuple");
}
if (scalarToTuple3 != s2t3Correct) {
    writeln("scalar to tuple domain failed for 3 dimensional tuple");
}

var tupleToScalar1 = makeRectangularDomain((0,),10);
var tupleToScalar2 = makeRectangularDomain((0,0),10);
var tupleToScalar3 = makeRectangularDomain((3,3,3),10,inclusive=false);

var t2s1Correct = {0..10};
var t2s2Correct = {0..10,0..10};
var t2s3Correct = {3..<10,3..<10,3..<10};

if (tupleToScalar1 != t2s1Correct) {
    writeln("tuple to scalar domain failed for 1 dimensional tuple");
}
if (tupleToScalar2 != t2s2Correct) {
    writeln("tuple to scalar domain failed for 2 dimensional tuple");
}
if (tupleToScalar3 != t2s3Correct) {
    writeln("tuple to scalar domain failed for 3 dimensional tuple");
}
