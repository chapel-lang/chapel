use Time;


var timestamp = 1693332842.123456; // August 29, 2023, 6:14:02.123456 PM
var myDateTime = dateTime.createUtcFromTimestamp(timestamp);
var myDate = myDateTime.getDate();
var myTime = myDateTime.getTime();

writeln(myDate.isoFormat());
writeln(myTime.isoFormat());
writeln(myDateTime.isoFormat());
writeln(myDateTime.isoFormat("")); // no separator

// ensure replacement functionality (cast) has same behavior
writeln(myDate.isoFormat() == myDate:string);
writeln(myTime.isoFormat() == myTime:string);
writeln(myDateTime.isoFormat() == myDateTime:string);
