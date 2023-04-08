use IO;

on Locales[0] {
    var reader = openReader("./input.bin");

    on Locales[1] {
        var b: bytes = bytes.createWithNewBuffer(b"");

        on Locales[2] {
            reader.readBinary(b, 40);
            write(b);
        }
    }
}
