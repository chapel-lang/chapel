use CPtr;
extern type sPtr;
extern proc getNullSPtr(): sPtr;
extern proc getNonNullSPtr(): sPtr;

writeln(is_c_nil(getNullSPtr()));
writeln(is_c_nil(getNonNullSPtr()));
