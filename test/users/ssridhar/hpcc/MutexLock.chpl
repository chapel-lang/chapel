type mutex_p = opaque;
type mutex_t = opaque;
_extern def mutex_init(inout lock);
_extern def mutex_lock(inout lock): int(32);
_extern def mutex_unlock(inout lock);
