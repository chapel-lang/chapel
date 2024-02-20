use IO;

on Locales[0] {
    var reader = openReader("./input.bin", locking=false);

    on Locales[1] {
        var s = "";

        on Locales[2] {
            reader.readBinary(s, 40);
            write(s);
        }
    }
}
