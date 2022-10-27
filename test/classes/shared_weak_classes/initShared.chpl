
class basicClass {
    var x: int;
}

writeln("1");
var bc1 = new shared basicClass(1);
info(bc1);
{
    bc1.x += 1;

    writeln("2");
    var bc2 = shared.create(bc1);
    info(bc1);
    info(bc2);
    {
        bc2.x += 1;

        writeln("3");
        var bc3 = shared.create(bc1);
        info(bc1);
        info(bc2);
        info(bc3);

    }
    bc2.x -= 1;

    writeln("2");
    info(bc1);
    info(bc2);
}
bc1.x -= 1;

writeln("1");
info(bc1);

proc info(x) {
    writeln("\tvalue: '", x, "' \ttype: '", x.type:string, "' \tsc: ", x.chpl_pn!.strongCount());
}
