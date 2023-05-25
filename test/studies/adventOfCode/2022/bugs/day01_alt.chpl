
use IO;

var line: string;
while readLine(line) {
    write("readLine: ", line);
    if line == "\n" {
        try {
            var x = line: int;
        } catch e: IllegalArgumentError {
            writeln("cannot cast a newline to 'int' 👍");
        } catch e {
            writeln("illegal cast threw wrong error: ", e);
        }
    } else {
        var x = line: int;
        writeln("cast to int: ", x);
    }

}
