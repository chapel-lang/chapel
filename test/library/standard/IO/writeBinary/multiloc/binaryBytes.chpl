use IO;

on Locales[0] {
    var w = open("./bb.bin", iomode.cw).writer();

    on Locales[1] {
        var d = b"some very interesting output data \xF0\x9F\x91\x8D\n";

        on Locales[2] {
            w.writeBinary(d);
        }
    }
}
