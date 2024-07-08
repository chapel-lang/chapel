use IO;

on Locales[0] {
    var reader = openReader("./input.bin", locking=false);

    on Locales[1] {
        var b = b"";

        on Locales[2] {
            reader.readBinary(b, 40);
            write(b);
        }
    }
}
