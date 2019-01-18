var D = {1..8};
writeln("D = ", D);

class CD { const D; }

var cd = new unmanaged CD(D);
writeln("CD = ", cd);
delete cd;

////////////////////////////

var SD: sparse subdomain(D) = (1, 3);
writeln("SD = ", SD);

class CS { const SD; }

var cs = new unmanaged CS(SD);
writeln("CSD = ", cs);
delete cs;
