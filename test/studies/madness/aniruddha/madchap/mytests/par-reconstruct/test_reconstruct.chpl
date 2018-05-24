use MRA;
use MadAnalytics;

proc main() {
    var npt = 10;

    writeln("Mad Chapel -- Concurrent Reconstruction Test\n");

    var fcn  : [1..4] unmanaged AFcn = ((new unmanaged Fn_Test1():unmanaged AFcn),  (new unmanaged Fn_Test2():unmanaged AFcn),  (new unmanaged Fn_Test3():unmanaged AFcn), (new unmanaged Fn_Unity():unmanaged AFcn));
    var dfcn : [1..4] unmanaged AFcn = ((new unmanaged Fn_dTest1():unmanaged AFcn), (new unmanaged Fn_dTest2():unmanaged AFcn), (new unmanaged Fn_dTest3():unmanaged AFcn), (new unmanaged Fn_dUnity():unmanaged AFcn));

    for i in fcn.domain {
        writeln("** Testing function ", i);
        var F = new unmanaged Function(k=5, thresh=1e-5, f=fcn[i]);
        writeln("F", i, ".norm2() = ", F.norm2());

        writeln("Compressing F", i, " ...");
        F.compress();

        writeln("Reconstructing F", i, " ...");
        F.reconstruct();
        
        writeln("F", i, ".norm2() = ", F.norm2());
        
        F.summarize();

        writeln("Evaluating F", i, " on [0, 1]:");
        F.evalNPT(npt);

        if i < fcn.domain.dim(1).high then
            writeln("\n======================================================================\n");
        delete F;
    }

    for (f,d) in zip(fcn,dfcn) {
      delete f;
      delete d;
    }
}
