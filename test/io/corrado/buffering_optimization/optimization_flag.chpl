use CTypes;

extern var qio_write_unbuffered_threshold:c_ssize_t;
extern var qio_read_unbuffered_threshold:c_ssize_t;

// setting `IOSkipBufferingForLargeOps=false` should set these thresholds
//  to their maximum value, deactivating the optimization
writeln(qio_write_unbuffered_threshold == max(c_ssize_t));
writeln(qio_write_unbuffered_threshold == max(c_ssize_t));
