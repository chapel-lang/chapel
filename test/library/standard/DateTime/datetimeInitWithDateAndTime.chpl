use DateTime;

const dt = datetime.now();
const d = date.today();

// used to support datetime - date where date was promoted
// to datetime with time == 0. This is the equivalent of that.
const td = dt - new datetime(d, new time());
