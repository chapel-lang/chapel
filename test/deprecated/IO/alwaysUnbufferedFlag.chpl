use IO;

var fr = openReader("alwaysUnbufferedFlag.chpl", hints=ioHintSet.fromFlag(QIO_CH_ALWAYS_UNBUFFERED), locking=false);
