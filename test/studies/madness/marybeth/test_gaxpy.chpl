use MRA;
use MadAnalytics;

def main() {
    var npt = 10;

    writeln("Mad Chapel -- Gaxpy Test\n");

    var fcn  : [1..3] AFcn = (Fn_Test1():AFcn,  Fn_Test2():AFcn,  Fn_Test3():AFcn);
    var dfcn : [1..3] AFcn = (Fn_dTest1():AFcn, Fn_dTest2():AFcn, Fn_dTest3():AFcn);

    for i in fcn.domain {
        writeln("** Testing function ", i);
        var F = Function(k=5, thresh=1e-5, f=fcn[i]);
        var G = Function(k=5, thresh=1e-5, f=fcn[i]);

        writeln("\nAdding F", i, " ...");
        var H = F + G;
        if verbose then H.summarize();

        writeln("\nEvaluating F+G on [0, 1]:");
        H.evalNPT(npt);

        writeln("\nSubtracting F", i, " ...");
        H = F - G;
        if verbose then H.summarize();

        writeln("\nEvaluating F-G on [0, 1]:");
        H.evalNPT(npt);

        if i < 3 then writeln("\n======================================================================\n");
    }

}
