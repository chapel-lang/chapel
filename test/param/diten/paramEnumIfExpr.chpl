config param choice = false;
enum E { value };

var a = if choice then 1 else E.value;

writeln(a);
