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

        f.evalNPT(npt);
    
        writeln("coefficients before compressing");
        f.summarize();

        f.compress();

        writeln("\ncoefficients after compressing");
        f.summarize();
    
        f.reconstruct();
        writeln("\ncoefficients after reconstructing");
        f.summarize();

        var df = f.diff();
        df.f = dtest;

        df.evalNPT(npt);

        delete f;
        delete df;
    }

    // addition test which in turn tests gaxpy
   
    for tf2 in tests {
        var tf1 = tests[1];
        writeln("\n\n");
        var f1 = new unmanaged Function(k, thresh, tf1);
        writeln("norm of f1 is ", f1.norm2());
        var f2 = new unmanaged Function(k, thresh, tf2);
        writeln("norm of f2 is ", f2.norm2());
        var f3 = f1 + f2;
        f3.f = new unmanaged Sum(tf1, tf2):unmanaged AFcn;
        writeln("norm of f3 = f1 + f2 is ", f3.norm2());
        f3.summarize();

        f3.evalNPT(npt);

        delete f1;
        delete f2;
        delete f3.f;
        delete f3;
    }
    
    
    // multiplication test which in turn tests gaxpy

    for tf2 in tests {
        var tf1 = tests[1];
        writeln("\n\n");
        var f1 = new unmanaged Function(k, thresh, tf1);
        writeln("norm of f1 is ", f1.norm2());
        var f2 = new unmanaged Function(k, thresh, tf2);
        writeln("norm of f2 is ", f2.norm2());
        var f3 = f1 * f2;
        f3.f = new unmanaged Product(tf1, tf2):unmanaged AFcn;
        writeln("norm of f3 = f1 * f2 is ", f3.norm2());
        f3.summarize();

        f3.evalNPT(npt);

        delete f1;
        delete f2;
        delete f3.f;
        delete f3;
    }

    for (test,dtest) in zip(tests,dtests) {
      delete test;
      delete dtest;
    }
}
