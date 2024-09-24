use IO, PrecisionSerializer;

var x = 3.14159;

stdout.withSerializer(new precisionSerializer(precision=7, padding=12)).writeln(x);
