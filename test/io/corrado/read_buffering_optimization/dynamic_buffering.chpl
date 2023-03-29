use IO, CTypes, FileSystem;

config const unbuffThreshold = 64,
             iobuffSize = 64,
             smallWriteSize = 0;

// modify the unbuffered threshold and io buffer size
extern var qio_read_unbuffered_threshold:c_ssize_t;
extern var qbytes_iobuf_size:c_size_t;
qio_read_unbuffered_threshold = unbuffThreshold:c_ssize_t;
qbytes_iobuf_size = iobuffSize:c_size_t;

const r = openReader("./db.bin", locking=false, hints = ioHintSet.mmap(false));


// read something small
