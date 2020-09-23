use DateTime;

// Make sure datetime.now() is within a day of date.today(). They could be
// different if the time passes midnight betwen the two calls.

var now = datetime.now();
var today = date.today();
var diff = now - new datetime(today.year, today.month, today.day);

assert(diff.days == 0 || diff.days == -1);
