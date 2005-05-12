#ifndef _CHPL_ENUM_H_
#define _CHPL_ENUM_H_

enum moduleSet {
  MODULES_USER = 0x1,
  MODULES_CODEGEN = 0x7,
  MODULES_ALL = 0x15
};

enum paramType {
  PARAM_BLANK = 0,
  PARAM_IN,
  PARAM_INOUT,
  PARAM_OUT,
  PARAM_CONST,
  PARAM_REF,

  NUM_PARAM_TYPES
};

enum ioCallType {
  IO_WRITE = 0, 
  IO_WRITELN, 
  IO_READ
};

#endif
