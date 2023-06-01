use CTypes;
extern type sPtr = c_ptr(opaque);
extern proc getNullSPtr(): sPtr;
extern proc getNonNullSPtr(): sPtr;

writeln(getNullSPtr() == nil);
writeln(getNonNullSPtr() == nil);
