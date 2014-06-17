//
//    IRBASE.H -- compiler IR interface
//
//        Base classes
//
//    Copyright (c) 2004-2013, Cray Inc.
//    Copyright (c) 2013, Unicorn Enterprises SA
//

#ifndef _IRBASE_H_
#define _IRBASE_H_

  struct IRType {
      void *info;
      };

  struct IRValue {
      void *info;
      };

  class IRBuilder {
  public:
      IRBuilder() { }
      virtual ~IRBuilder() { }
  public:
    // top-level
      virtual void startProg() = 0;
      virtual void endProg() = 0;
      virtual void startHeader() = 0;
      virtual void endHeader() = 0;
      virtual void addConfig(
          const char *var,
          const char *type,
          const char *module) = 0;
      virtual void genGlobalDefClassId(const char *name, int id) = 0;
      virtual void addFtable(const char *name) = 0;
      virtual void setMaxVMT(int max) = 0;
      virtual void startVMT(const char *name) = 0;
      virtual void addVMT(const char *name) = 0;
      virtual void endVMT() = 0; 
      virtual void genGlobalsRegistry(int size) = 0;
      virtual void addMemDesc(const char *desc) = 0;
      virtual int addPBT(const char *name) = 0;
      virtual void genGlobalString(const char *name, const char *value) = 0;
      virtual void genGlobalInt(const char *name, int value) = 0;
  public:
    // types
      virtual void startEnum(const char *name, IRType type) = 0;
      virtual void addEnum(const char *name, IRValue init) = 0;
      virtual void endEnum() = 0;
      virtual void defStarTuple(
          const char *name, IRType base, int length) = 0;
      virtual void defFixedString(const char *name, int size) = 0;
      virtual void defRef(const char *name, IRType base) = 0;
      virtual void defDataClass(const char *name, IRType base) = 0;
      virtual void startStruct(const char *name) = 0;
      virtual void startUnion(const char *name) = 0;
      virtual void startClass(const char *name) = 0;
      virtual void startSuper() = 0;
      virtual void addSuper(IRType type) = 0;
      virtual void endSuper() = 0;
      virtual void endStruct() = 0;
      virtual void endUnion() = 0;
      virtual void endClass() = 0;
      virtual void protoClass(const char *cname, const char *oname) = 0;
      virtual void externType(const char *cname) = 0;
      virtual IRType genCPtr(IRType type) = 0;
      virtual IRValue genTypeStructureIndex(const char *name) = 0;
  public:
    // symbols
      virtual void defVar(const char *name, IRType type) = 0;
      virtual void startFn() = 0;
      virtual void endFn() = 0;
      virtual void startProtoFn() = 0;
      virtual void endProtoFn() = 0;
      virtual void startFnHeader(const char *name, IRType ret) = 0;
      virtual void addFormal(const char *name, IRType type) = 0; 
      virtual void endFnHeader() = 0;
      virtual void startFnType(IRType ret) = 0;
      virtual void addFormalType(IRType type) = 0; 
      virtual IRType endFnType() = 0;
      virtual void startModule(const char *name) = 0;
      virtual void endModule() = 0;
      virtual IRValue genVar(const char *name) = 0;
      virtual IRValue genArg(const char *name) = 0;
      virtual IRType genType(const char *name) = 0;
      virtual IRValue genFn(const char *name) = 0;
      virtual IRValue genEnum(const char *name) = 0;
      virtual IRValue genModule(const char *name) = 0;
      virtual IRValue genLabel(const char *name) = 0;
      virtual IRValue genUnresolved(const char *name) = 0;
  public:
    // immediate
      virtual IRValue genBool(bool v) = 0;
      virtual IRValue genInt8(int8_t v) = 0;
      virtual IRValue genInt16(int16_t v) = 0;
      virtual IRValue genInt32(int32_t v) = 0;
      virtual IRValue genInt64(int64_t v) = 0;
      virtual IRValue genUint8(uint8_t v) = 0;
      virtual IRValue genUint16(uint16_t v) = 0;
      virtual IRValue genUint32(uint32_t v) = 0;
      virtual IRValue genUint64(uint64_t v) = 0;
      virtual IRValue genFloat32(float v) = 0;
      virtual IRValue genFloat64(double v) = 0;
      virtual IRValue genComplex64(float r, float i) = 0;
      virtual IRValue genComplex128(double r, double i) = 0;
      virtual IRValue genString(const char *v) = 0;
  public:
    // statements
      virtual void startBlock() = 0;
      virtual void endBlock() = 0;
      virtual void startWhiledo(IRValue cond) = 0;
      virtual void endWhiledo() = 0;
      virtual void startDowhile(IRValue cond) = 0;
      virtual void endDowhile() = 0;
      virtual void startFor(IRValue cond) = 0;
      virtual void endFor() = 0;
      virtual void startXmtForall(IRValue x, IRValue y) = 0;
      virtual void endXmtForall() = 0;
      virtual void startIf(IRValue cond) = 0;
      virtual void startElse() = 0;
      virtual void endIf() = 0;
      virtual void genGoto(const char *name) = 0;
      virtual void genCopy(IRValue to, IRValue from, IRValue size) = 0;
      virtual void genStore(IRValue to, IRValue from) = 0;
      virtual void genStmtExpr(IRValue x) = 0;
      virtual void defLabel(const char *name) = 0;
  public:
    // expressions
      virtual IRValue genLogicalOr(IRValue x, IRValue y) = 0;
      virtual IRValue genLogicalAnd(IRValue x, IRValue y) = 0;
      virtual IRValue genEq(IRValue x, IRValue y) = 0;
      virtual IRValue genNe(IRValue x, IRValue y) = 0;
      virtual IRValue genLe(IRValue x, IRValue y) = 0;
      virtual IRValue genGe(IRValue x, IRValue y) = 0;
      virtual IRValue genLt(IRValue x, IRValue y) = 0;
      virtual IRValue genGt(IRValue x, IRValue y) = 0;
      virtual IRValue genAdd(IRValue x, IRValue y) = 0;
      virtual IRValue genSub(IRValue x, IRValue y) = 0;
      virtual IRValue genNeg(IRValue x) = 0; 
      virtual IRValue genMul(IRValue x, IRValue y) = 0;
      virtual IRValue genDiv(IRValue x, IRValue y) = 0;
      virtual IRValue genMod(IRValue x, IRValue y) = 0;
      virtual IRValue genLsh(IRValue x, IRValue y) = 0;
      virtual IRValue genRsh(IRValue x, IRValue y) = 0;
      virtual IRValue genAnd(IRValue x, IRValue y) = 0;
      virtual IRValue genOr(IRValue x, IRValue y) = 0;
      virtual IRValue genXor(IRValue x, IRValue y) = 0;
      virtual IRValue genTernary(
          IRValue cond, IRValue x, IRValue y) = 0;
      virtual IRValue genIsZero(IRValue x, bool is_raddr) = 0;
      virtual IRValue genIsNotZero(IRValue x, bool is_raddr) = 0;
      virtual IRValue genPlus(IRValue x) = 0;
      virtual IRValue genNot(IRValue x) = 0;
      virtual IRValue genTempVar(const char *type) = 0;
      virtual IRValue genLoad(IRValue x) = 0;
      virtual IRValue genNullPtr() = 0;
      virtual IRValue genSizeof(const char *type) = 0;
      virtual IRValue genSizeof(IRType type) = 0;
      virtual IRValue genSizeof(IRValue value) = 0;
      virtual IRValue genCast(const char *type, IRValue value) = 0;
      virtual IRValue genCast(IRType type, IRValue value) = 0;
      virtual IRValue genCastToVoidPtr(IRValue x) = 0;
      virtual IRValue genCastToCharPtr(IRValue x) = 0;  
      virtual IRValue genGlobal(const char *global) = 0;
      virtual IRValue genElemPtr(IRValue base, IRValue index) = 0;
      virtual IRValue genFieldPtr(IRValue base, const char *field) = 0;
      virtual void startCall(IRValue fn) = 0;
      virtual void addArg(IRValue arg) = 0;
      virtual IRValue endCall() = 0; 
      virtual IRValue genReturn() = 0;
      virtual IRValue genReturn(IRValue x) = 0;
      virtual IRValue genWideAddr(
          IRType type, IRValue locale, IRValue raddr) = 0;
      virtual IRValue genRaddr(IRValue wide, bool is_ptr) = 0;
      virtual IRValue genRlocale(IRValue wide, bool is_ptr) = 0;
      virtual IRValue genLocaleID(
          IRType type, IRValue node, IRValue subloc) = 0;
      virtual IRValue genFtablePtr(
          const char *type1,
          bool is_ptr1, 
          const char *type2,
          bool is_ptr2, 
          IRValue index) = 0;
      virtual void genMtaAssertNoalias(const char *name) = 0;
      virtual void genInvalidPrim(const char *name) = 0;
  public:
    // logging
      virtual void logLine(int lineno, const char *filename) = 0;
      virtual void logId(int id) = 0;
      };

#endif    // _IRBASE_H_
