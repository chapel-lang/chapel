use Function1d;
use MadFn1d;

def main() {
    var npt = 10;

    writeln("Mad Chapel -- Differentiation Test\n");

    var fcn  : [1..3] Fn1d = (Fn_Test1():Fn1d,  Fn_Test2():Fn1d,  Fn_Test3():Fn1d);
    var dfcn : [1..3] Fn1d = (Fn_dTest1():Fn1d, Fn_dTest2():Fn1d, Fn_dTest3():Fn1d);

    for i in fcn.domain {
        writeln("** Testing function ", i);
        var F = Function1d(k=5, thresh=1e-5, f=fcn[i]);

        writeln("F", i, ".norm2() = ", F.norm2());

        if verbose {
            F.summarize();
            writeln("Evaluating F", i, " on [0, 1]:");
            F.evalNPT(npt);
            writeln();
        }

        writeln("Compressing F", i, " ...");
        F.compress();
        if verbose then F.summarize();

        writeln("Reconstructing F", i, " ...");
        F.reconstruct();
        writeln("F", i, ".norm2() = ", F.norm2());
        if verbose then F.summarize();

        writeln("Evaluating F", i, " on [0, 1]:");
        F.evalNPT(npt);

        writeln("\nDifferentiating F", i, " ...");
        var dF = F.diff();
        dF.f = dfcn[i]:Fn1d; // Fudge it for the sake of evalNPT()
        if verbose then dF.summarize();

        writeln("\nEvaluating dF", i, " on [0, 1]:");
        dF.evalNPT(npt);

        if i < 3 then writeln("\n======================================================================\n");
    }
}
