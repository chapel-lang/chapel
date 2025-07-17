//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE_0 */
// example of an attribute without a tool name
@attributeName(arg1="value", arg2=1, arg3=1.0, arg4=true, arg5=1..10)
proc foo() { }

// example of an attribute with a tool name
@toolName.attributeName(arg1="value", arg2=1, arg3=true)
proc foo() { }
/* STOP_EXAMPLE_0 */
