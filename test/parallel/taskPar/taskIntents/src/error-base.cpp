// Verify that an error is reported when assigning to a would-be global.
// This needs #include "support-decls.cpp" and #include "var-decls.cpp".
// Can be placed in any scope.

begin {
#include "var-assigns.cpp"
}

cobegin {
#include "var-assigns.cpp"
}

coforall jjjjj in 1..3 {
#include "var-assigns.cpp"
}
