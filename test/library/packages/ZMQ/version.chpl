use ZMQ;

var (major, minor, patch) = ZMQ.version;
writef("Current 0MQ version is %i.%i.%i\n", major, minor, patch);
