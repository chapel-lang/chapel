type mutex_p = opaque;
type mutex_t = opaque;
_extern proc mutex_init(inout lock);
_extern proc mutex_lock(inout lock): int(32);
_extern proc mutex_unlock(inout lock);
