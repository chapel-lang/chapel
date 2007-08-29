use Function1d;
use Fn1d;
use MadFn1d;

class Square: Fn1d {
    var f: Fn1d;
    def this(x: real): real {
        return f(x)**2;
    }
}

def main() {
    var npt = 10;

    writeln("Mad Chapel -- Multiplication Test\n");

    var fcn  : [1..3] Fn1d = (Fn_Test1():Fn1d,  Fn_Test2():Fn1d,  Fn_Test3():Fn1d);
    var dfcn : [1..3] Fn1d = (Fn_dTest1():Fn1d, Fn_dTest2():Fn1d, Fn_dTest3():Fn1d);

    for i in fcn.domain {
        writeln("** Testing function ", i);
        var F = Function1d(k=5, thresh=1e-5, f=fcn[i]);
        var G = Function1d(k=5, thresh=1e-5, f=fcn[i]);

        writeln("\nMultiplying F", i, " ...");
        var H = F * G;
        H.f = Square(fcn[i]);
        if verbose then H.summarize();

        writeln("\nEvaluating F*G on [0, 1]:");
        H.evalNPT(npt);

        if i < fcn.domain.dim(1).high then
            writeln("\n======================================================================\n");
    }

}
