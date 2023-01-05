use IO;

on Locales[0] {
    var reader = openreader("./input.bin");

    on Locales[1] {
        var s: string = createStringWithNewBuffer("");

        on Locales[2] {
            reader.readBinary(s, 40);
            write(s);
        }
    }
}
