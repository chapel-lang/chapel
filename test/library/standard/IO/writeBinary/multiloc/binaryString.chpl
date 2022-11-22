use IO;

on Locales[0] {
    var w = open("./bs.bin", iomode.cw).writer();

    on Locales[1] {
        var d = "some very interesting output data 👍\n";

        on Locales[2] {
            w.writeBinary(d);
        }
    }
}
