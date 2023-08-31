use Time;

var timestamp = timeSinceEpoch().totalSeconds();
var dateFromTimestamp : date = date.createFromTimestamp(timestamp);
var dateTimeFromTimestamp : dateTime = dateTime.createUtcFromTimestamp(timestamp);
writeln(dateFromTimestamp == dateTimeFromTimestamp.getDate());

// Also test with a dateTime with 0 time, to make sure timezones aren't
// causing problems.
var d1 = new date(1999, 9, 19);
var delta = d1 - unixEpoch.getDate();
timestamp = delta.days * 60 * 60 * 24 + delta.seconds;
dateFromTimestamp = date.createFromTimestamp(timestamp);
dateTimeFromTimestamp = dateTime.createUtcFromTimestamp(timestamp);
writeln(dateFromTimestamp == dateTimeFromTimestamp.getDate());
