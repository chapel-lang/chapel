use CTypes;
extern type sPtr;
extern proc getNullSPtr(): sPtr;
extern proc getNonNullSPtr(): sPtr;

writeln(getNullSPtr() == nil);
writeln(getNonNullSPtr() == nil);
