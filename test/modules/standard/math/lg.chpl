config var ns = 32;
config var nu = 64:uint;
config var nf = 128.1;

writeln("lg(ns) = ", lg(ns));
writeln("lg(nu) = ", lg(nu));
writeln("lg(nf) = ", lg(nf));

ns = -1;
//nu = -2;
nf = -1.2;

writeln("lg(ns) = ", lg(ns));
//writeln("lg(nu) = ", lg(nu));
writeln("lg(nf) = ", lg(nf));

ns = 0;
nu = 0;
nf = 0.1;

writeln("lg(ns) = ", lg(ns));
writeln("lg(nu) = ", lg(nu));
writeln("lg(nf) = ", lg(nf));

ns = 1;
nu = 1;
nf = 1.1;

writeln("lg(ns) = ", lg(ns));
writeln("lg(nu) = ", lg(nu));
writeln("lg(nf) = ", lg(nf));

