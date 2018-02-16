config param nObjects = 1024 * 1024;

extern proc chpl_qsbr_defer_deletion(c_void_ptr);
extern proc chpl_qsbr_checkpoint();

for i in 1 .. nObjects {
	chpl_qsbr_defer_deletion(new object() : c_void_ptr);
}
chpl_qsbr_checkpoint();
