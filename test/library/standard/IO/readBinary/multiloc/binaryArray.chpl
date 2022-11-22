use IO;

on Locales[0] {
    var reader = openreader("../input/tu8.bin");

    on Locales[1] {
        var a_expected = [1, 2, 4, 8, 16, 32, 64, 128] : uint(8);
        var a : [0..<8] uint(8);

        on Locales[2] {
            reader.readBinary(a, ioendian.native);

            const correct = && reduce (a == a_expected);
            writeln(correct);
        }
    }
}
