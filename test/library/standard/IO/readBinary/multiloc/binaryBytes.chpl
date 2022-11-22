use IO;

on Locales[0] {
    var reader = openreader("./input.bin");

    on Locales[1] {
        var b: bytes = b"";

        on Locales[2] {
            reader.readBinary(b, 32);
            writeln(b);
        }
    }
}
