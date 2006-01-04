#ifndef _INTERPRETER_OPS_H_
#define _INTERPRETER_OPS_H_

class InterpreterOp;

#ifndef _EXTERN
#define _EXTERN extern
#define _INIT
#endif

_EXTERN InterpreterOp *unimplemented_interpreter_op _INIT;
_EXTERN InterpreterOp *init_interpreter_op _INIT;

#endif
