// Verify that an error is reported when assigning to a would-be formal.

#include "support-decls.cpp"
#include "var-decls.cpp"

#define FUNNAME funBlank
#define INTENT
#include "var-fun-with-assigns.cpp"

#define FUNNAME funConst
#define INTENT const
#include "var-fun-with-assigns.cpp"

#define FUNNAME funConstIn
#define INTENT const in
#include "var-fun-with-assigns.cpp"

#define FUNNAME funIn
#define INTENT in
#include "var-fun-with-assigns.cpp"

#define FUNNAME funInOut
#define INTENT inout
#include "var-fun-with-assigns.cpp"

#define FUNNAME funOut
#define INTENT out
#include "var-fun-with-assigns.cpp"

#define FUNNAME funRef
#define INTENT ref
#include "var-fun-with-assigns.cpp"

#define FUNNAME funConstRef
#define INTENT const ref
#include "var-fun-with-assigns.cpp"
