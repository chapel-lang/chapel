use Time;

const dt = dateTime.now();
const d = date.today();

// used to support dateTime - date where date was promoted
// to dateTime with time == 0. This is the equivalent of that.
const td = dt - new dateTime(d, new time());
