use MRA;
use MadAnalytics;

class Sum: AFcn {
    var f: unmanaged AFcn;
    var g: unmanaged AFcn;
    override proc this(x) {
        return f(x)+g(x);
    }
}

class Product: AFcn {
    var f: unmanaged AFcn;
    var g: unmanaged AFcn;
    override proc this(x) {
        return f(x)*g(x);
    }
}

proc main() {
    // Run the code on each test function in turn

    // Note that test3 does not satisfy the boundary condition
    // assumed by the derivative operator that the function is
    // zero at and beyond the boundary ... thus, close to the
    // boundary, the derivative will be incorrect.  It will
    // be correct in the interior.

    var npt = 20;      // No. points to sample on test printing
    var k   = 5;       // order of wavelet
    var thresh = 1e-5; // truncation threshold

    var tests  : [1..3] unmanaged AFcn = (new unmanaged Fn_Test1():unmanaged AFcn,  new unmanaged Fn_Test2():unmanaged AFcn,  new unmanaged Fn_Test3():unmanaged AFcn);
    var dtests : [1..3] unmanaged AFcn = (new unmanaged Fn_dTest1():unmanaged AFcn, new unmanaged Fn_dTest2():unmanaged AFcn, new unmanaged Fn_dTest3():unmanaged AFcn);

    var buf = "                           ";

    for (test, dtest) in zip(tests, dtests) {
        writeln("\n\n");
        var f = new unmanaged Function(k, thresh, test);
        writeln("norm of function is ", f.norm2());
        delete f;
    }

    for (test, dtest) in zip(tests, dtests) {
      delete test;
      delete dtest;
    }
}
