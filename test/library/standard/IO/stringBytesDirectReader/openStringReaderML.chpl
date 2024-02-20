use IO;

var s = "hello world!\nI'm a string!\nFrom Locale 0";

on Locales[1] {
    var r = openStringReader(s);
    write(r.readLine());

    on Locales[2] do
        write(r.readLine())

    on Locales[0] do
        writeln(r.readLine())
}
