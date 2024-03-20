use Time;

const ordinal : int = 10000;
var dt = dateTime.createFromOrdinal(ordinal);

// check equivalence to forwarded-to method on date
assert(dt == new dateTime(date.createFromOrdinal(ordinal)));
assert(dt.getDate() == date.createFromOrdinal(ordinal));
