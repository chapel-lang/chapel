use MRA;
use MadAnalytics;

config const k      = 5;
config const thresh = 1e-5;

def main() {
    var npt = 10;

    writeln("Mad Chapel -- Show me the boxes\n");

    var fcn  = Fn_Test1();
    var dfcn = Fn_dTest1();

    var F = Function(k=k, thresh=thresh, f=fcn);
    writeln("F.norm2() = ", F.norm2());
    F.summarize();
    F.evalNPT(npt);
    
    writeln("\nDumping the boxes at each level:");
    for lvl in 0..F.max_level { 
        write("\n **",lvl,": ");
        //for (n, l) in F.s.indices do
        //    if n == lvl then write(" [",n,", ",l,"]");
        for (n, l) in F.s.idx_iter(lvl) do
            write(" [",n,", ",l,"]");
    }
}
