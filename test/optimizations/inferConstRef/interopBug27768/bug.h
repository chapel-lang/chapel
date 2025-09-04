#ifndef BUG_H
#define BUG_H

// C typedef: pointer to struct
// The struct is a record implemented in Chapel
typedef struct MyObject* MyHandle;

// Callback function type
typedef int (*MyCreateCallback)(MyHandle* handle);
typedef int (*MyModifyCallback)(MyHandle* handle);

// Functions that accept callbacks
int testCreateCallback(MyCreateCallback callback);
int testModifyCallback(MyModifyCallback callback, MyHandle handle);

#endif // BUG_H
