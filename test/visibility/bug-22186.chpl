import CTypes;

type test_t = uint(64);
extern proc c_test(a: CTypes.c_ptr(test_t));
