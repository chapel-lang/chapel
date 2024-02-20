use IO;

var b = b"hello world!\nI'm a bytes!\nFrom Locale 0";

on Locales[1] {
    var r = openBytesReader(b);
    write(r.readLine());

    on Locales[2] do
        write(r.readLine())

    on Locales[0] do
        writeln(r.readLine())
}
