use MRA;
use MadAnalytics;

class Square: AFcn {
    var f: unmanaged AFcn;
    override proc this(x: real): real {
        return f(x)**2;
    }
}

proc main() {
    var npt = 10;

    writeln("Mad Chapel -- Multiplication Test\n");

    var fcn  : [1..4] unmanaged AFcn = (new unmanaged Fn_Test1():unmanaged AFcn, new unmanaged Fn_Test2():unmanaged AFcn,  new unmanaged Fn_Test3():unmanaged AFcn, new unmanaged Fn_Unity():unmanaged AFcn);

    for i in fcn.domain {
        writeln("** Testing function ", i);
        var F1 = new unmanaged Function(k=5, thresh=1e-5, f=fcn[i], autorefine=false);
        var F2 = new unmanaged Function(k=5, thresh=1e-5, f=fcn[i]);
        var G = new unmanaged Function(k=5, thresh=1e-5, f=new unmanaged Fn_Unity());

        writeln("\nMultiplying F", i, "*Unity ...");
        var H1 = F1 * G;
        H1.f = fcn[i];
        if verbose then H1.summarize();

        writeln("\nEvaluating F*Unity on [0, 1]:");
        H1.evalNPT(npt);

        writeln("\nMultiplying F",i,"*F",i," ...");
        var H2 = F1 * F1;
        H2.f = new unmanaged Square(fcn[i]):unmanaged AFcn;
        delete H2.f;
        H2.f = fcn[i];
        if verbose then H2.summarize();

        writeln("\nEvaluating F*F on [0, 1]:");
        H2.evalNPT(npt);

        if i < fcn.domain.dim(0).high then
            writeln("\n======================================================================\n");

        delete F1;
        delete F2;
        delete G.f;
        delete G;
        delete H1;
        delete H2;
    }

    for f in fcn do
      delete f;

}
