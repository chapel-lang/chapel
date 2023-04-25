use Time;

// Make sure dateTime.now() is within a day of date.today(). They could be
// different if the time passes midnight betwen the two calls.

var now = dateTime.now();
var today = date.today();
var diff = now - new dateTime(today.year, today.month, today.day);

assert(diff.days == 0 || diff.days == -1);
