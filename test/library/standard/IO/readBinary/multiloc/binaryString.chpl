use IO;

on Locales[0] {
    var reader = openreader("./input.bin");

    on Locales[1] {
        var s: string = "";

        on Locales[2] {
            reader.readBinary(s, 32);
            writeln(s);
        }
    }
}
