use IO;

on Locales[0] {
    var reader = openReader("../input/tu8.bin");

    on Locales[1] {
        const a_expected = [1, 2, 4, 8, 16, 32, 64, 128] : uint(8);
        var a : [0..<8] uint(8);

        on Locales[2] {
            const numRead = reader.readBinary(a, ioendian.native),
                  correct = && reduce (a == a_expected);
            writeln(numRead, " ", correct);
        }
    }
}
