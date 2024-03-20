use Time;

var curDateTuple : (int, int, int) = getCurrentDate();
var today : date = date.utcToday();
writeln(curDateTuple[0] == today.year);
writeln(curDateTuple[1] == today.month);
writeln(curDateTuple[2] == today.day);
