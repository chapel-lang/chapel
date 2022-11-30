use IO;

on Locales[0] {
    var w = open("./a.bin", iomode.cw).writer();

    on Locales[1] {
        var a = [1, 2, 4, 8, 16, 32, 64, 128] : uint(8);

        on Locales[2] {
            w.writeBinary(a);
        }
    }
}
