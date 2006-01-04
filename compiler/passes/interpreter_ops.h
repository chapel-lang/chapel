#ifndef _INTERPRETER_OPS_H_
#define _INTERPRETER_OPS_H_

class InterpreterOp;

#ifndef _EXTERN
#define _EXTERN extern
#define _INIT
#endif

_EXTERN InterpreterOp *unimplemented_interpreter_op _INIT;
_EXTERN InterpreterOp *init_interpreter_op _INIT;
_EXTERN InterpreterOp *alloc_interpreter_op _INIT;
_EXTERN InterpreterOp *fopen_interpreter_op _INIT;
_EXTERN InterpreterOp *fclose_interpreter_op _INIT;
_EXTERN InterpreterOp *strerror_interpreter_op _INIT;
_EXTERN InterpreterOp *fprintf_interpreter_op _INIT;
_EXTERN InterpreterOp *fscanf_interpreter_op _INIT;
_EXTERN InterpreterOp *array_index_interpreter_op _INIT;
_EXTERN InterpreterOp *array_set_interpreter_op _INIT;
_EXTERN InterpreterOp *done_interpreter_op _INIT;
//_EXTERN InterpreterOp *x_interpreter_op _INIT;

#endif
