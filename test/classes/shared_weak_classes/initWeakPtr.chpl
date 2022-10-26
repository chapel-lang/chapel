class myClass {
    var x :int;
}

var wmc;

{
    {
        writeln("create a new shared class instance");
        var mc = new shared myClass(0);
        info(mc);

        writeln("create weak pointer via downgrading");
        wmc = mc.downgrade();
        info(wmc);

        writeln("create a new shared from the first");
        var mc2 = shared.create(mc);
        info(mc);
        info(wmc);

        writeln("create a new shared by upgrading a weak pointer");
        var mc3 = wmc.upgrade() : shared myClass;
        info(mc3);
        info(wmc);
    }
    writeln("all shared instances deinitialized here");
    info(wmc);

    writeln("copy-init a new weak pointer");
    var wmc2 = wmc;
    info(wmc2);

    // upgrades now result in 'nil'
    writeln("try to upgrade a weak pointer");
    var wmc_up = wmc.upgrade();
    info(wmc_up);
}
writeln("second weak pointer deinitialized here");
info(wmc);

proc info(x) {
    writeln("\t\tvalue: '", x, "' \ttype: '", x.type:string, "'");
}
