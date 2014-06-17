//
//    GENC.H -- Code generator (C back-end)
//
//        Declarations
//
//    Copyright (c) 2004-2013, Cray Inc.
//    Copyright (c) 2013, Unicorn Enterprises SA
//

#ifndef _GENC_H_
#define _GENC_H_

  template<class TYPE> class TempBuf {
  public:
      TempBuf() {
          mData = NULL;
          mCap = 0;
          mLen = 0;
          }
      ~TempBuf() {
          if (mData != NULL)
              delete[] mData;
          }
  public:
      void push(TYPE value) {
          if (mLen >= mCap) {
              int c = (mCap > 0) ? (mCap * 2) : 1024;
              TYPE *d = new TYPE[c];
              if (mLen > 0)
                  memcpy(d, mData, mLen*sizeof(TYPE));
              if (mData != NULL)
                  delete[] mData;
              mData = d;
              mCap = c;
              }
          mData[mLen] = value;
          mLen++;
          }
      const TYPE *data(int pos) const
          { return &mData[pos]; }
      int len() const
          { return mLen; }
      void setLen(int pos)
          { mLen = pos; }
  private:
      TYPE *mData;
      int mCap;
      int mLen;
      };

#define GenRet GenC_GenRet

  struct GenRet {
      std::string c;
      Type *chplType;
      uint8_t isLVPtr;
      bool isUnsigned;
      
      GenRet() {
          chplType = NULL;
          isLVPtr = GEN_VAL;
          isUnsigned = false;
          }
      };

  class GenC {
  public:
      GenC();
      ~GenC();
  public:
      void codegen();
  private:
      void genComment(const char *comment, bool push=false);
      void genIdComment(int id);
      void flushStatements();
      int maxVMT()
          { return gMaxVMT; }
      static std::string numToString(int64_t num);
      static std::string int64_to_string(int64_t i);
      static std::string uint64_to_string(uint64_t i);
  private:  
      int infoLineno()
          { return lineno; }
      const char *infoFilename()
          { return filename; }
      void setInfoLineno(int lineno)
          { this->lineno = lineno; }
      void setInfoFilename(const char *filename)
          { this->filename = filename; }
      void clearStatements()
          { cStatements.clear(); }
      void clearLocalDecls()
          { cLocalDecls.clear(); }
      void writeStatement(const std::string &stmt)
          { cStatements.push_back(stmt); }
      void writeLocalDecl(const std::string &decl)
          { cLocalDecls.push_back(decl); }
      void writeC(const char *fmt, ...);
  private:
      GenRet AList_codegen(AList *self, const char *separator=", ");
      GenRet Expr_codegen(Expr *self);
  private:
      void codegen_header();
      void codegen_header_addons();
      void codegen_config();
  private:
      void collectTypes(Vec<TypeSymbol *> &types);
      void collectGlobals(Vec<VarSymbol *> &globals);
      void collectFunctions(Vec<FnSymbol *> &functions);
      void uniquifyTypeNames(
          Vec<TypeSymbol *> &types, Vec<const char *> &cnames);
      void uniquifyEnumNames(
          Vec<TypeSymbol *> &types, Vec<const char *> &cnames);
      void uniquifyFieldNames(
          Vec<TypeSymbol *> &types, Vec<const char *> &cnames);
      void uniquifyGlobalNames(
          Vec<VarSymbol *> &globals, Vec<const char *> &cnames);
      void uniquifyFunctionNames(
          Vec<FnSymbol *> &functions, Vec<const char *> &cnames);
      void uniquifyFormalNames(Vec<const char *> &cnames);
      void uniquifyLocalNames(Vec<const char *> &cnames);
      void genClassIDs(Vec<TypeSymbol *> &typeSymbols);
      void genClassPrototypes(Vec<TypeSymbol *> &types);
      void genEnumeratedTypes(Vec<TypeSymbol *> &types);
      void genAggregates(Vec<TypeSymbol *> &types);
      void genOtherClasses(Vec<TypeSymbol *> &types);
      void genClassDefs();
      void codegen_aggregate_def(ClassType *ct);
      bool shouldCodegenAggregate(ClassType *ct);
  private:
      void genCompilationInfo();
      void genIncludes();
      void genGlobalDefClassId(const char *cname, int id);
      void genFunctionPrototypes(Vec<FnSymbol *> &functions);
      void genFunctionPointerTable(Vec<FnSymbol *> &functions);
      void genFtable(Vec<FnSymbol *> &fSymbols);
      void genVirtualMethodTable(Vec<TypeSymbol *> &types);
      void genGlobalVariables(Vec<VarSymbol *> &globals);
      void genGlobalString(const char *cname, const char *value);
      void genGlobalInt(const char *cname, int value);
  private:
      const char *uniquifyName(
          const char *name, 
          Vec<const char *> *set1, 
          Vec<const char *> *set2=NULL);
      static int compareSymbol(const void *v1, const void *v2);
      static void legalizeName(Symbol *sym);
      static const char *subChar(
          Symbol *sym, const char *ch, const char *x);          
      void reservedSymbolNames(Vec<const char *> &cnames);
  private:
    // GenInfo
      FILE *cfile;
      std::vector<std::string> cLocalDecls;
      std::vector<std::string> cStatements;
      int lineno;
      const char *filename;
      bool parseOnly;
  private:
      int gMaxVMT;
      fileinfo gAllExternCode;
  private:
      static const int maxUniquifyAddedChars = 25;
  private:
      int fMaxCIdentLen;
      char *longCNameReplacementBuffer;
      Map<const char *, int> uniquifyNameCounts;
//
//    Type
//
  private:
      GenRet Type_codegen(Type *self);
      void Type_codegenDef(Type *self);
      void Type_codegenPrototype(Type *self);
      int Type_codegenStructure(
          Type *self, FILE *outfile, const char *baseoffset);
      GenRet ClassType_codegenClassStructType(ClassType *self);
  private:
      void registerTypeToStructurallyCodegen(TypeSymbol *type);
      GenRet genTypeStructureIndex(TypeSymbol *typesym);
      void codegenTypeStructures(FILE *hdrfile);
      void codegenTypeStructureInclude(FILE *outfile);
  private:
      void PrimitiveType_codegenDef(PrimitiveType *self);
      int PrimitiveType_codegenStructure(
          PrimitiveType *self, FILE *outfile, const char *baseoffset);
      void EnumType_codegenDef(EnumType *self);
      int EnumType_codegenStructure(
          EnumType *self, FILE *outfile, const char *baseoffset);
      void ClassType_codegenDef(ClassType *self);
      void ClassType_codegenPrototype(ClassType *self);
      int ClassType_codegenStructure(
          ClassType *self, FILE *outfile, const char *baseoffset);
      int ClassType_codegenFieldStructure(
          ClassType *self,
          FILE *outfile, 
          bool nested, 
          const char *baseoffset);
  private:
      const char *genNewBaseOffsetString(
          TypeSymbol *typesym, 
          int fieldnum,
          const char *baseoffset, 
          Symbol *field,
          ClassType *classtype);
      const char *genSizeofStr(TypeSymbol *typesym);
      const char *genChplTypeEnumString(TypeSymbol *typesym);
      const char *genUnderscore(Symbol *sym);
      static int compareCnames(const void *v1, const void *v2);
  private:
      Vec<TypeSymbol*> typesToStructurallyCodegen;
      Vec<TypeSymbol*> typesToStructurallyCodegenList;
      int maxFieldsPerType;
//
//    Symbol
//
  private:
      GenRet Symbol_codegen(Symbol *self);
      void Symbol_codegenDef(Symbol *self);
      void Symbol_codegenPrototype(Symbol *self);
      void VarSymbol_codegenGlobalDef(VarSymbol *self);
      GenRet FnSymbol_codegenCast(FnSymbol *self, GenRet fnPtr);
  private:
      GenRet VarSymbol_codegen(VarSymbol *self);
      void VarSymbol_codegenDef(VarSymbol *self);
      void VarSymbol_codegenDefC(VarSymbol *self);
      GenRet ArgSymbol_codegen(ArgSymbol *self);
      GenRet ArgSymbol_codegenType(ArgSymbol *self);
      GenRet TypeSymbol_codegen(TypeSymbol *self);
      void TypeSymbol_codegenDef(TypeSymbol *self);
      void TypeSymbol_codegenPrototype(TypeSymbol *self);
      GenRet FnSymbol_codegen(FnSymbol *self);
      void FnSymbol_codegenDef(FnSymbol *self);
      void FnSymbol_codegenPrototype(FnSymbol *self);
      void FnSymbol_codegenHeaderC(FnSymbol *self);
      GenRet FnSymbol_codegenFunctionType(FnSymbol *self, bool forHeader);
      void EnumSymbol_codegenDef(EnumSymbol *self);
      void ModuleSymbol_codegenDef(ModuleSymbol *self);
      void LabelSymbol_codegenDef(LabelSymbol *self);
  private:
      static int compareLineno(const void *v1, const void *v2);      
//
//    Stmt
//
  private:
      void codegenStmt(Expr *stmt);
      GenRet BlockStmt_codegen(BlockStmt *self);
      GenRet CondStmt_codegen(CondStmt *self);
      GenRet GotoStmt_codegen(GotoStmt *self);
      GenRet ExternBlockStmt_codegen(ExternBlockStmt *self);
//
//    Expr
//
  private:
      GenRet SymExpr_codegen(SymExpr *self);
      GenRet UnresolvedSymExpr_codegen(UnresolvedSymExpr *self);
      GenRet DefExpr_codegen(DefExpr *self);
      GenRet CallExpr_codegen(CallExpr *self);
      GenRet NamedExpr_codegen(NamedExpr *self);
  private:
      GenRet primUnknown(CallExpr *self);
      void primArraySet(CallExpr *self);
      void primArrayAlloc(CallExpr *self);
      void primGpuAlloc(CallExpr *self);
      void primCopyHostGpu(CallExpr *self);
      void primCopyGpuHost(CallExpr *self);
      void primGpuFree(CallExpr *self);
      void primArrayFree(CallExpr *self);
      void primArrayFreeElts(CallExpr *self);
      GenRet primMove(CallExpr *self);
      GenRet primAddrOf(CallExpr *self);
      GenRet primRefToString(CallExpr *self);
      GenRet primReturn(CallExpr *self);
      GenRet primUnaryMinus(CallExpr *self);
      GenRet primUnaryPlus(CallExpr *self);
      GenRet primUnaryNot(CallExpr *self);
      GenRet primUnaryLnot(CallExpr *self);
      GenRet primAdd(CallExpr *self);
      GenRet primSubtract(CallExpr *self);
      GenRet primMult(CallExpr *self);
      GenRet primDiv(CallExpr *self);
      GenRet primMod(CallExpr *self);
      GenRet primLsh(CallExpr *self);
      GenRet primRsh(CallExpr *self);
      GenRet primEqual(CallExpr *self);
      GenRet primNotequal(CallExpr *self);
      GenRet primLessorequal(CallExpr *self);
      GenRet primGreaterorequal(CallExpr *self);
      GenRet primLess(CallExpr *self);
      GenRet primGreater(CallExpr *self);
      GenRet primAnd(CallExpr *self);
      GenRet primOr(CallExpr *self);
      GenRet primXor(CallExpr *self);
      GenRet primPow(CallExpr *self);
      GenRet primMin(CallExpr *self);
      GenRet primMax(CallExpr *self);
      void primSetcid(CallExpr *self);
      GenRet primGetcid(CallExpr *self);
      GenRet primTestcid(CallExpr *self);
      void primSetUnionId(CallExpr *self);
      GenRet primGetUnionId(CallExpr *self);
      void primSetSvecMember(CallExpr *self);
      GenRet primGetMember(CallExpr *self);
      GenRet primGetSvecMember(CallExpr *self);
      void primSetMember(CallExpr *self);
      void primCheckNil(CallExpr *self);
      void primLocalCheck(CallExpr *self);
      void primSyncInit(CallExpr *self);
      void primSyncLock(CallExpr *self);
      void primSyncUnlock(CallExpr *self);
      void primSyncWaitFull(CallExpr *self);
      void primSyncWaitEmpty(CallExpr *self);
      void primSyncSignalFull(CallExpr *self);
      void primSyncSignalEmpty(CallExpr *self);
      void primSingleInit(CallExpr *self);
      void primSingleLock(CallExpr *self);
      void primSingleUnlock(CallExpr *self);
      void primSingleWaitFull(CallExpr *self);
      void primSingleSignalFull(CallExpr *self);
      void primWriteef(CallExpr *self);
      void primWriteff(CallExpr *self);
      void primSyncReset(CallExpr *self);
      GenRet primReadfe(CallExpr *self);
      GenRet primSyncIsFull(CallExpr *self);
      void primSingleWriteef(CallExpr *self);
      void primSingleReset(CallExpr *self);
      GenRet primSingleReadff(CallExpr *self);
      GenRet primSingleIsFull(CallExpr *self);
      void primProcessTaskList(CallExpr *self);
      void primExecuteTasksInList(CallExpr *self);
      void primFreeTaskList(CallExpr *self);
      GenRet primGetSerial(CallExpr *self);
      void primSetSerial(CallExpr *self);
      GenRet primGetSublocId(CallExpr *self);
      void primSetSublocId(CallExpr *self);
      GenRet primLocGetNode(CallExpr *self);
      GenRet primLocGetSubloc(CallExpr *self);
      void primChplCommGet(CallExpr *self);
      void primChplCommGetStrd(CallExpr *self);
      GenRet primChplAlloc(CallExpr *self);
      void primChplFree(CallExpr *self);
      GenRet primCast(CallExpr *self);
      GenRet primDynamicCast(CallExpr *self);
      GenRet primNodeId(CallExpr *self);
      GenRet primOnLocaleNum(CallExpr *self);
      void primAllocGvr(CallExpr *self);
      void primHeapRegisterGlobalVar(CallExpr *self);
      void primHeapBroadcastGlobalVars(CallExpr *self);
      void primPrivateBroadcast(CallExpr *self);
      void primIntError(CallExpr *self);
      GenRet primStringCopy(CallExpr *self);
      GenRet primRtError(CallExpr *self);
      void primNewPrivClass(CallExpr *self);
      GenRet primNumPrivClasses(CallExpr *self);
      GenRet primFtableCall(CallExpr *self);
      GenRet primVmtCall(CallExpr *self);
      void primBlockXmtPragmaNoalias(CallExpr *self);
      void invalidPrim(CallExpr *self);
  private:
      void genBeginBlock(CallExpr *self, FnSymbol *fn);
      void genCobeginOrCoforallBlock(CallExpr *self, FnSymbol *fn);
      void genOnBlock(CallExpr *self, FnSymbol *fn);
      GenRet genFnCall(CallExpr *self, FnSymbol *fn);
  private:
      GenRet startCudaCall(CallExpr *self, GenRet base);
      void endCudaCall(CallExpr *self);
  private:
      GenRet eval(Expr *expr);
      GenRet evalArg(CallExpr *call, int i)
          { return eval(call->get(i)); }
      GenRet evalSym(Symbol *sym);
      GenRet evalInt(int x) 
          { return evalSym(new_IntSymbol(x, INT_SIZE_64)); }
      GenRet evalStr(const char *str) 
          { return evalSym(new_StringSymbol(str)); }
  private:
      static Type *getFieldType(Type *type, const char *name)
          { return type->getField(name)->type; }
      static Type *getFieldType(Expr *expr, const char *name)
          { return expr->typeInfo()->getField(name)->type; }
      static bool hasFlag(Type *type, Flag flag)
          { return type->symbol->hasFlag(flag); }
      static bool hasFlag(Expr *expr, Flag flag)
          { return expr->typeInfo()->symbol->hasFlag(flag); }
      static bool hasEitherFlag(Type *type, Flag aflag, Flag bflag)
          { return type->symbol->hasEitherFlag(aflag, bflag); }
      static bool hasEitherFlag(Expr *expr, Flag aflag, Flag bflag)
          { return expr->typeInfo()->symbol->hasEitherFlag(aflag, bflag); } 
  private:
      GenRet codegenFieldPtr(GenRet base, Expr *field);
      GenRet codegenFieldPtr(GenRet base, Symbol *field);
      GenRet codegenFieldPtr(GenRet base, const char *field);
      GenRet codegenFieldCidPtr(GenRet base);
      GenRet codegenFieldUidPtr(GenRet base);
      GenRet createTempVar(Type *t);
      GenRet codegenValuePtr(GenRet r);
      GenRet codegenAddrOf(GenRet r);
      GenRet codegenLocalAddrOf(GenRet r);
      GenRet codegenLocalDeref(GenRet r);
      GenRet codegenDeref(GenRet r);
      GenRet codegenDynamicCastCheck(GenRet cid, Type *type);
      GenRet codegenArgForFormal(
          GenRet arg,
          ArgSymbol *formal,
          bool defaultToValues,
          bool isExtern);
      GenRet codegenBasicPrimitiveExpr(CallExpr *call);
      GenRet codegenZero();
      GenRet codegenOne();
      GenRet codegenSizeof(Type *t);
      static bool isTupleOfTuple(BaseAST *e);
      GenRet codegenExprMinusOne(Expr *expr);
      void callExprHelper(CallExpr *call, BaseAST *arg);
      GenRet codegenUseGlobal(std::string str);
      GenRet codegenUseGlobal(const char *global);
      GenRet codegenUseCid(Type *classType);
      GenRet codegenGetNodeID();
      GenRet codegenGetSublocID();
      GenRet codegenGetLocaleID();
      GenRet codegenWideHere(GenRet addr, Type *wideType=NULL);
      GenRet codegenRaddr(GenRet wide);
      GenRet codegenRlocale(GenRet wide);
      GenRet codegenRnode(GenRet wide);
      GenRet codegenRsubloc(GenRet wide);               
  private:
      GenRet codegenElementPtr(GenRet base, GenRet index);
      GenRet codegenFieldPtr(
          GenRet base,
          const char *c_field_name,
          const char *chpl_field_name,
          int special);
      void codegenAssign(GenRet to_ptr, GenRet from);
      GenRet codegenWideAddr(
          GenRet locale, GenRet raddr, Type *wideType=NULL);
      GenRet codegenRaddrMaybePtr(GenRet wide);
      GenRet codegenRlocaleMaybePtr(GenRet wide);
  private:
      GenRet codegenEquals(GenRet a, GenRet b);
      GenRet codegenNotEquals(GenRet a, GenRet b);
      GenRet codegenLogicalOr(GenRet a, GenRet b);
      GenRet codegenLogicalAnd(GenRet a, GenRet b);
      GenRet codegenAdd(GenRet a, GenRet b);
      GenRet codegenSub(GenRet a, GenRet b);
      GenRet codegenNeg(GenRet a);
      GenRet codegenMul(GenRet a, GenRet b);
      GenRet codegenTernary(GenRet cond, GenRet ifTrue, GenRet ifFalse);
      GenRet codegenIsZero(GenRet x);
      GenRet codegenIsNotZero(GenRet x);
      GenRet createTempVar(const char *ctype);
      GenRet createTempVarWith(GenRet v);
      GenRet codegenValue(GenRet r);
      GenRet codegenNullPointer();
      GenRet codegenSizeof(const char *name);
      GenRet codegenCast(Type *t, GenRet value, bool Cparens=true);
      GenRet codegenCast(const char *typeName, GenRet value, bool Cparens=true);
      GenRet codegenCastToVoidStar(GenRet value);
      GenRet codegenCastToCharStar(GenRet value);
      GenRet codegenCallExpr(
          GenRet function,
          int num,
          const GenRet *args,
          FnSymbol *fSym,
          bool defaultToValues);
      GenRet codegenLocaleID(GenRet node, GenRet subloc);
  private:
      GenRet codegenCallExpr(
          const char *fnName,
          int num,
          const GenRet *args,
          bool defaultToValues=true);
      void codegenCall(const char *fnName, int num, const GenRet *args);
      GenRet codegenCallExpr(const char *fnName);
      GenRet codegenCallExpr(const char *fnName, GenRet a1);
      GenRet codegenCallExpr(const char *fnName, GenRet a1, GenRet a2);
      GenRet codegenCallExpr(
          const char *fnName, 
          GenRet a1, 
          GenRet a2, 
          GenRet a3);
      GenRet codegenCallExpr(
          const char *fnName, 
          GenRet a1, 
          GenRet a2, 
          GenRet a3, 
          GenRet a4);
      GenRet codegenCallExpr(
          const char *fnName, 
          GenRet a1, 
          GenRet a2, 
          GenRet a3,
          GenRet a4, 
          GenRet a5);
/* 
      void codegenCall(const char *fnName);
*/
      void codegenCall(const char *fnName, GenRet a1);
      void codegenCall(const char *fnName, GenRet a1, GenRet a2);
      void codegenCall(
          const char *fnName, 
          GenRet a1, 
          GenRet a2, 
          GenRet a3);
      void codegenCall(
          const char *fnName, 
          GenRet a1, 
          GenRet a2, 
          GenRet a3, 
          GenRet a4);
      void codegenCall(
          const char *fnName, 
          GenRet a1, 
          GenRet a2, 
          GenRet a3,
          GenRet a4, 
          GenRet a5);
/*
      void codegenCall(
          const char *fnName, 
          GenRet a1, 
          GenRet a2, 
          GenRet a3,
          GenRet a4, 
          GenRet a5, 
          GenRet a6);
      void codegenCall(
          const char *fnName, 
          GenRet a1, 
          GenRet a2, 
          GenRet a3,
          GenRet a4, 
          GenRet a5, 
          GenRet a6, 
          GenRet a7);
*/
      void codegenCall(
          const char *fnName, 
          GenRet a1, 
          GenRet a2, 
          GenRet a3,
          GenRet a4, 
          GenRet a5, 
          GenRet a6, 
          GenRet a7, 
          GenRet a8);
/*
      void codegenCall(
          const char *fnName, 
          GenRet a1, 
          GenRet a2, 
          GenRet a3,
          GenRet a4, 
          GenRet a5, 
          GenRet a6, 
          GenRet a7, 
          GenRet a8,
          GenRet a9);
      void codegenCall(
          const char *fnName, 
          GenRet a1, 
          GenRet a2, 
          GenRet a3,
          GenRet a4, 
          GenRet a5, 
          GenRet a6, 
          GenRet a7, 
          GenRet a8,
          GenRet a9, 
          GenRet a10);
*/
      void codegenCall(
          const char *fnName, 
          GenRet a1, 
          GenRet a2, 
          GenRet a3,
          GenRet a4, 
          GenRet a5, 
          GenRet a6, 
          GenRet a7, 
          GenRet a8,
          GenRet a9, 
          GenRet a10, 
          GenRet a11);
  private:
      int startArg()
          { return argBuf.len(); }
      void pushArg(GenRet arg)
          { argBuf.push(arg); }
      void endArg(int mark) {
          argLen = argBuf.len();
          argBuf.setLen(mark);
          }
      const GenRet *argPtr() 
          { return argBuf.data(argBuf.len()); }
      int argNum()
          { return (argLen - argBuf.len()); }
  private:
      enum {
          field_normal,
          field_cid,
          field_uid,
          field_other
          };
  private:
      int codegen_tmp;
      TempBuf<GenRet> argBuf;
      int argLen;
      };

#endif    // _GENC_H_
