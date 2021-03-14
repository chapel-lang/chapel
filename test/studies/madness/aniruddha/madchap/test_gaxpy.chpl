use MRA;
use MadAnalytics;

proc main() {
    var npt = 10;

    writeln("Mad Chapel -- Gaxpy Test\n");

    var fcn  : [1..3] unmanaged AFcn = ((new unmanaged Fn_Test1()):unmanaged AFcn,  (new unmanaged Fn_Test2()):unmanaged AFcn,  (new unmanaged Fn_Test3()):unmanaged AFcn);
    var dfcn : [1..3] unmanaged AFcn = ((new unmanaged Fn_dTest1()):unmanaged AFcn, (new unmanaged Fn_dTest2()):unmanaged AFcn, (new unmanaged Fn_dTest3()):unmanaged AFcn);

    for i in fcn.domain {
        writeln("** Testing function ", i);
        var F = new unmanaged Function(k=5, thresh=1e-5, f=fcn[i]);
        var G = new unmanaged Function(k=5, thresh=1e-5, f=fcn[i]);

        writeln("\nAdding F", i, " ...");
        var H = F + G;
        if verbose then H.summarize();

        writeln("\nEvaluating F+G on [0, 1]:");
        H.evalNPT(npt);

        delete H;

        writeln("\nSubtracting F", i, " ...");
        H = F - G;
        if verbose then H.summarize();

        writeln("\nEvaluating F-G on [0, 1]:");
        H.evalNPT(npt);

        if i < 3 then writeln("\n======================================================================\n");

        delete F;
        delete G;
        delete H;
    }

    for (f,d) in zip(fcn,dfcn) {
      delete f;
      delete d;
    }
}
