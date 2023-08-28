use Time;

// the enums themselves
var x : dayOfWeek = dayOfWeek.Monday; // deprecation warning for changed behavior
var y : isoDayOfWeek = isoDayOfWeek.Monday; // deprecated
writeln(x:int);
writeln(y:int);

// date methods using the enums
var a = date.today().weekday(); // deprecation warning for changed behavior
var b = date.today().isoWeekday(); // deprecated
writeln(a:int == b:int);

// dateTime methods using the enums
a = dateTime.now().weekday(); // deprecation warning for changed behavior
b = dateTime.now().isoWeekday(); // deprecated
writeln(a:int == b:int);
