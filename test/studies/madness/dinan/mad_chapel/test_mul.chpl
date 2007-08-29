use Function1d;
use AnalyticFcn;
use MadAnalytics;

class Square: AFcn {
    var f: AFcn;
    def this(x: real): real {
        return f(x)**2;
    }
}

def main() {
    var npt = 10;

    writeln("Mad Chapel -- Multiplication Test\n");

    var fcn  : [1..3] AFcn = (Fn_Test1():AFcn,  Fn_Test2():AFcn,  Fn_Test3():AFcn);
    var dfcn : [1..3] AFcn = (Fn_dTest1():AFcn, Fn_dTest2():AFcn, Fn_dTest3():AFcn);

    for i in fcn.domain {
        writeln("** Testing function ", i);
        var F = Function1d(k=5, thresh=1e-5, f=fcn[i]);
        var G = Function1d(k=5, thresh=1e-5, f=fcn[i]);

        writeln("\nMultiplying F", i, " ...");
        var H = F * G;
        H.f = Square(fcn[i]):AFcn;
        if verbose then H.summarize();

        writeln("\nEvaluating F*G on [0, 1]:");
        H.evalNPT(npt);

        if i < fcn.domain.dim(1).high then
            writeln("\n======================================================================\n");
    }

}
