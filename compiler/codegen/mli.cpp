/*
 * Copyright 2004-2020 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mli.h"

#include "fixupExports.h"
#include "library.h"
#include "ModuleSymbol.h"
#include "FnSymbol.h"
#include "beautify.h"
#include "codegen.h"
#include "driver.h"
#include "expr.h"
#include "stlUtil.h"
#include "stringutil.h"

#include <cstring>
#include <map>
#include <sstream>

const char* gen_mli_marshalling = "chpl_mli_marshalling";
const char* gen_mli_client = "chpl_mli_client";
const char* gen_mli_server = "chpl_mli_server";
static const char* client_main = "chpl_client.main";
static const char* client_arg = "chpl_client.arg";
static const char* client_res = "chpl_client.res";
static const char* server_arg = "chpl_server.arg";
static const char* server_res = "chpl_server.res";
static const char* marshal_push_prefix = "chpl_mli_mtpush_";
static const char* marshal_pull_prefix = "chpl_mli_mtpull_";
static const char* socket_push_name = "chpl_mli_push";
static const char* socket_pull_name = "chpl_mli_pull";
static const char* scope_begin = "{\n";
static const char* scope_end = "}\n";

class MLIContext {
public:

  MLIContext(bool debugPrint=false);
  ~MLIContext();

  void emit(ModuleSymbol* md);
  void emit(FnSymbol* fn);
  void emitClientPrelude(void);
  void emitServerPrelude(void);
  void emitMarshalRoutines(void);
  void emitServerDispatchRoutine(void);

private:

  bool debugPrint;
  std::vector<FnSymbol*> exps;
  std::vector<FnSymbol*> throws;
  std::map<Type*, int64_t> typeMap;
  fileinfo fiMarshalling;
  fileinfo fiClientBundle;
  fileinfo fiServerBundle;
  GenInfo* info;

  bool shouldEmit(FnSymbol* fn);
  void setOutput(fileinfo* fi);
  void setOutputAndWrite(fileinfo* fi, const std::string& gen);
  void write(const std::string& code);
  int64_t assignUniqueTypeID(Type* t);
  void emitClientWrapper(FnSymbol* fn);
  void emitServerWrapper(FnSymbol* fn);
  bool isSupportedType(Type* t);
  void verifyPrototype(FnSymbol* fn);
  Type* getTypeFromFormal(ArgSymbol* as);
  Type* getTypeFromFormal(FnSymbol* fn, int i);
  bool isTypeRequiringAlloc(Type* t);

  std::string genMarshalBodyPrimitiveScalar(Type* t, bool out);
  std::string genMarshalBodyString(Type* t, bool out);
  std::string genMarshalBodyChplBytesWrapper(Type* t, bool out);
  std::string genComment(const char* msg, const char* pfx="");
  std::string genNote(const char* msg);
  std::string genTodo(const char* msg);
  std::string genDefine(const char* val);
  std::string genHeaderInc(const char* header, bool system=false);
  std::string genMarshalRoutineProto(Type* t, bool out);
  std::string genMarshalRoutine(Type* t, bool out);
  std::string genMarshalPushRoutine(Type* t);
  std::string genMarshalPullRoutine(Type* t);
  std::string genServerDispatchSwitch(const std::vector<FnSymbol*>& fns);
  std::string genDebugPrintCall(FnSymbol* fn);
  std::string genDebugPrintCall(const char* msg);
  std::string genDebugPrintCall1Arg(const char* msg, const char* uintArg);
  std::string genFuncNumericID(FnSymbol* fn);
  std::string genServerWrapperCall(FnSymbol* fn);
  std::string genClientsideRPC(FnSymbol* fn);
  std::string genServersideRPC(FnSymbol* fn);
  std::string genMemCleanup(Type* t, const char* var);
  std::string genMarshalCall(const char* skt, const char* var, Type* t,
                             bool out);
  std::string genMarshalPushCall(const char* skt, const char* var, Type* t);
  std::string genMarshalPullCall(const char* skt, const char* var, Type* t);
  std::string genTypeName(Type* t);
  std::string genSocketCallBuffer(const char* skt, const char* var,
                                  const char* len, bool out);
  std::string genSocketCall(const char* skt, const char* var, const char* len,
                            bool out);
  std::string genSocketCall(const char* skt, const char* var, bool out);
  std::string genSocketPushCall(const char* skt, const char* var);
  std::string genSocketPullCall(const char* skt, const char* var);
  std::string genAddressOf(const char* var);
  std::string genAddressOf(std::string& var);
  std::string genSizeof(const char* var);
  std::string genSizeof(std::string& var);
  std::string genNewDecl(const char* t, const char* n);
  std::string genNewDecl(Type* t, const char* n);

};

//
// Generic helper method replacing C++11 std::to_string() method.
//
template <typename T>
std::string str(T value) {
  std::ostringstream tmp;
  tmp << value;
  return tmp.str();
}

//
// This is the main entrypoint for MLI code generation, call this if the
// necessary conditions are met in codegen().
//
void codegenMultiLocaleInteropWrappers(void) {
  Vec<ModuleSymbol*> &mds = allModules;

  MLIContext mli(fMultiLocaleLibraryDebug);

  mli.emitClientPrelude();
  mli.emitServerPrelude();

  forv_Vec(ModuleSymbol, md, mds) {
    mli.emit(md);
  }

  mli.emitMarshalRoutines();
  mli.emitServerDispatchRoutine();

  return;
}

MLIContext::MLIContext(bool debugPrint) {

  this->debugPrint = debugPrint;

  openCFile(&this->fiMarshalling, gen_mli_marshalling, "c");
  openCFile(&this->fiClientBundle, gen_mli_client, "c");
  openCFile(&this->fiServerBundle, gen_mli_server, "c");
  this->info = gGenInfo;

  return;
}

MLIContext::~MLIContext() {

  closeCFile(&this->fiMarshalling, true);
  closeCFile(&this->fiClientBundle, true);
  closeCFile(&this->fiServerBundle, true);

  return;
}

bool MLIContext::shouldEmit(FnSymbol* fn) {
  return (fn->hasFlag(FLAG_EXPORT) && isUserRoutine(fn));
}

void MLIContext::emit(ModuleSymbol* md) {
  const std::vector<FnSymbol*> fns = md->getTopLevelFunctions(true);

  for_vector(FnSymbol, fn, fns) {
    if (!this->shouldEmit(fn)) { continue; }

    this->exps.push_back(fn);
    this->emit(fn);
  }

  return;
}

void MLIContext::emit(FnSymbol* fn) {
  if (!this->shouldEmit(fn)) { return; }

  this->verifyPrototype(fn); 
  this->emitClientWrapper(fn);
  this->emitServerWrapper(fn);

  return;
}

void MLIContext::emitClientPrelude(void) {
  std::string gen;

  gen += this->debugPrint ? this->genDefine("CHPL_MLI_DEBUG_PRINT") : "";
  gen += this->genDefine("CHPL_MLI_IS_CLIENT");
  gen += this->genHeaderInc("mli/client_runtime.c");
  gen += this->genHeaderInc("chpl_mli_marshalling.c");
  gen += "\n";

  this->setOutputAndWrite(&this->fiClientBundle, gen);

  return;
}

std::string MLIContext::genHeaderInc(const char* header, bool system) {
  std::string gen;

  gen += "#include ";
  gen += system ? "<" : "\"";
  gen += header;
  gen += system ? ">" : "\"";
  gen += "\n";

  return gen;
}

std::string MLIContext::genComment(const char* msg, const char* pfx) {
  std::string gen;

  gen += "// ";
  gen += pfx;
  gen += strcmp(pfx, "") ? ": " : "";
  gen += msg;
  gen += "\n";

  return gen;
}

std::string MLIContext::genNote(const char* msg) {
  return this->genComment(msg, "NOTE");
}

std::string MLIContext::genTodo(const char* msg) {
  return this->genComment(msg, "TODO");
}

std::string MLIContext::genDefine(const char* val) {
  std::string gen;

  gen += "#define ";
  gen += val;
  gen += "\n";

  return gen;
}

void MLIContext::emitServerPrelude(void) {
  std::string gen;

  gen += this->debugPrint ? this->genDefine("CHPL_MLI_DEBUG_PRINT") : "";
  gen += this->genDefine("CHPL_MLI_IS_SERVER");
  gen += this->genHeaderInc("mli/server_runtime.c");
  gen += this->genHeaderInc("chpl_mli_marshalling.c");
  gen += this->genHeaderInc("_main.c");
  gen += "\n";
  
  this->setOutputAndWrite(&this->fiServerBundle, gen);

  return;
}

void MLIContext::emitMarshalRoutines(void) {
  std::map<Type*, int64_t>::iterator i;
  std::string gen;

  gen += this->debugPrint ? this->genDefine("CHPL_MLI_DEBUG_PRINT") : "";
  gen += this->genHeaderInc("stdio.h", true);
  gen += this->genHeaderInc("zmq.h", true);
  gen += this->genHeaderInc("chpl__header.h");
  gen += this->genHeaderInc("mli/common_code.c");
  gen += "\n";

  for (i = this->typeMap.begin(); i != this->typeMap.end(); ++i) {
    if (this->debugPrint) {
      std::string tpn = this->genTypeName(i->first);
      gen += this->genComment(tpn.c_str());
    }

    gen += this->genMarshalPushRoutine(i->first);
    gen += this->genMarshalPullRoutine(i->first);
  }

  this->setOutputAndWrite(&this->fiMarshalling, gen);

  return;
}

std::string MLIContext::genMarshalBodyPrimitiveScalar(Type* t, bool out) {
  std::string gen;

  // On pack, target buffer is input parameter. On unpack, the temporary.
  const char* target = out ? "obj" : "result";

  // Move the raw bytes of the type to/from the wire.
  gen += "skt_err=";
  gen += this->genSocketCall("skt", target, out);

  // Generate a null frame in the opposite direction for the ACK.
  gen += this->genSocketCall("skt", NULL, !out);

  return gen;
}

//
// TODO: Generalize this method to become something akin to
// "genMarshalVariableWidthBuffer", of which a string is nothing more than a
// specific instance of.
// This will help us later down the line when we have to support other types
// that push variable width buffers (arrays).
//
std::string MLIContext::genMarshalBodyString(Type* t, bool out) {
  const char* target = out ? "obj" : "buffer";
  std::string gen;

  // Declare temporaries for bytecount, buffer, and memory errors.
  gen += this->genNewDecl("uint64_t", "bytes");
  gen += this->genNewDecl("void*", "buffer");
  gen += this->genNewDecl("int64_t", "mem_err");

  if (out) {
    // Compute and push length of string.
    gen += "bytes = strlen(obj);\n";

    if (this->debugPrint) {
      gen += this->genDebugPrintCall1Arg("Pushing length: ", "bytes");
    }
  }

  gen += this->genSocketCall("skt", "bytes", out);

  if (!out) {
    if (this->debugPrint) {
      gen += this->genDebugPrintCall1Arg("Received intended length: ", "bytes");
    }
    // Attempt to allocate buffer.
    gen += "buffer = mli_malloc(bytes + 1);\n";

    // Set ACK value (non-zero if memory allocation failed).
    gen += "mem_err = (buffer == NULL);\n";
  }

  // Push/pull possible allocation error on ACK.
  gen += this->genSocketCall("skt", "mem_err", !out);

  // If error, terminate client/server.
  gen += "if (mem_err) chpl_mli_terminate(CHPL_MLI_CODE_EMEMORY);\n";

  // Move the string over the wire, using length.
  gen += this->genSocketCallBuffer("skt", target, "bytes", out);

  // Generate a null frame in the opposite direction for the ACK.
  gen += this->genSocketCall("skt", NULL, !out);

  if (!out) {
    // Null terminate the string we just received.
    gen += "((char*) buffer)[bytes] = '\\0';\n";

    if (t == dtStringC) {
      // Cast buffer to const char.
      gen += "result = ((const char*) buffer);\n";
    } else if (t->symbol->hasFlag(FLAG_C_PTR_CLASS) &&
               getDataClassType(t->symbol)->typeInfo() == dtInt[INT_SIZE_8]) {
      // Cast buffer to int8*
      Type* underlyingType = getDataClassType(t->symbol)->typeInfo();
      const char* underlyingTypeName = underlyingType->symbol->cname;
      gen += "result = ((";
      gen += underlyingTypeName;
      gen += "*) buffer);\n";
    } else {
      INT_FATAL("Unknown type passed to %s, %s", __FUNCTION__,
                t->symbol->name);
    }
  }

  return gen;
}

std::string MLIContext::genMarshalBodyChplBytesWrapper(Type* t, bool out) {
  const char* target = out ? "obj" : "result";
  std::string gen;

  // Create expressions for each "chpl_byte_buffer" struct field.
  const char* fieldIsOwned = astr(target, ".isOwned");
  const char* fieldSize = astr(target, ".size");
  const char* fieldData = astr(target, ".data");

  // Generate a temporary to hold memory errors.
  gen += this->genNewDecl("int64_t", "mem_err");

  // Push/pull the "isOwned" field.
  gen += this->genSocketCall("skt", fieldIsOwned, out);

  // Generate a null frame for the ACK.
  gen += this->genSocketCall("skt", NULL, !out);

  // Push/pull the "size" field.
  gen += this->genSocketCall("skt", fieldSize, out);

  if (!out) {
    // Attempt to allocate buffer.
    gen += fieldData;
    gen += " = mli_malloc(";
    gen += + fieldSize;
    gen += " + 1);\n";

    // Set ACK value (non-zero if memory allocation failed).
    gen += "mem_err = (";
    gen += fieldData;
    gen += " == NULL);\n";
  }

  // Push/pull possible allocation error on ACK.
  gen += this->genSocketCall("skt", "mem_err", !out);

  // If error, terminate client/server.
  gen += "if (mem_err) chpl_mli_terminate(CHPL_MLI_CODE_EMEMORY);\n";

  // Move the bytes over the wire, using length.
  gen += this->genSocketCallBuffer("skt", fieldData, fieldSize, out);

  // Generate a null frame for the ACK.
  gen += this->genSocketCall("skt", NULL, !out);

  // Null terminate the bytes buffer if we are pulling.
  if (!out) {
    gen += fieldData;
    gen += "[";
    gen += fieldSize;
    gen += "] = '\\0';\n";
  }

  return gen;
}

std::string MLIContext::genMarshalRoutineProto(Type* t, bool out) {
  int64_t id = this->assignUniqueTypeID(t);
  std::string gen;

  // Push returns nothing, while pull returns the type being read in.
  if (out) {
    gen += "void ";
  } else {
    gen += this->genTypeName(t);
    gen += " ";
  }

  // Select appropriate prefix for function name based on direction.
  gen += out ? marshal_push_prefix : marshal_pull_prefix;
  gen += str(id);
  gen += "(void* skt";

  // Push routines expect the type as a parameter (named "obj").
  if (out) {
    gen += ",";
    gen += this->genTypeName(t);
    gen += " obj";
  }

  gen += ")";

  return gen;
}

std::string MLIContext::genMarshalRoutine(Type* t, bool out) {
  std::string proto;
  std::string gen;

  // Generate a forward declaration.
  proto = this->genMarshalRoutineProto(t, out);
  gen += proto;
  gen += ";\n";

  // Followed by the actual definition.
  gen += proto;
  gen += scope_begin;

  // Always declare a variable to catch socket errors.
  gen += this->genNewDecl("int", "skt_err");

  // If unpacking, declare a temporary for the return value.
  if (!out) { gen += this->genNewDecl(t, "result"); }

  // Insert a debug message if appropriate.
  if (this->debugPrint) {
    std::string msg;

    msg += out ? "Pushing type: " : "Pulling type: ";
    msg += this->genTypeName(t);

    gen += this->genDebugPrintCall(msg.c_str());
  }

  //
  // Handle translation of different type classes here. Note that right now
  // what we can translate is limited.
  //
  if (isPrimitiveScalar(t) && !is_complex_type(t)) {
    gen += this->genMarshalBodyPrimitiveScalar(t, out);
  } else if (t == dtStringC) {
    gen += this->genMarshalBodyString(t, out);
  } else if (t->symbol->hasFlag(FLAG_C_PTR_CLASS) &&
             getDataClassType(t->symbol)->typeInfo() == dtInt[INT_SIZE_8]) {
    // A different strategy will be needed if we ever intend to support
    // c_ptr(int8)s that weren't originally Chapel strings.
    gen += this->genMarshalBodyString(t, out);
  } else if (t->getValType() == exportTypeChplByteBuffer) {
    gen += this->genMarshalBodyChplBytesWrapper(t, out); 
  } else {
    USR_FATAL(t, "Multi-locale libraries do not support type: %s",
              t->name());
  }

  // If we are unpacking, return our temporary.
  if (!out) { gen += "return result;\n"; }
  
  gen += scope_end;
  gen += "\n";

  return gen;
}

std::string MLIContext::genMarshalPushRoutine(Type* t) {
  return this->genMarshalRoutine(t, true);
}

std::string MLIContext::genMarshalPullRoutine(Type* t) {
  return this->genMarshalRoutine(t, false);
}

void MLIContext::emitServerDispatchRoutine(void) {
  std::string gen;

  gen += this->genServerDispatchSwitch(this->exps);
  gen += "\n";
  
  this->setOutputAndWrite(&this->fiServerBundle, gen);

  return;
}

void MLIContext::setOutput(fileinfo* fi) {
  this->info->cfile = fi->fptr;
  return;
}

void MLIContext::setOutputAndWrite(fileinfo* fi, const std::string& gen) {
  this->setOutput(fi);
  this->write(gen);
}

void MLIContext::write(const std::string& gen) {
  fprintf(this->info->cfile, "%s", gen.c_str());
  return;
}

//
// We can (as I understand it) count on Type* being unique across the entire
// symbol table (IE, you'll never encounter two different Type objects that
// both end up describing the same concrete type).
//
int64_t MLIContext::assignUniqueTypeID(Type* t) {
  // Prepare a new ID based on the map size (will never overflow).
  int64_t result = (int64_t) this->typeMap.size();

  if (this->typeMap.find(t) != this->typeMap.end()) {
    result = this->typeMap[t];
  } else {
    this->typeMap[t] = result;
  }

  return result;
}

void MLIContext::emitClientWrapper(FnSymbol* fn) {
  std::string gen;

  this->setOutput(&this->fiClientBundle);
  fn->codegenHeaderC();

  gen += scope_begin;
  gen += this->genClientsideRPC(fn);
  gen += scope_end;
  gen += "\n";

  this->write(gen);
  
  return;
}

void MLIContext::emitServerWrapper(FnSymbol* fn) {
  std::string gen;
  std::string prototype;

  gen += this->genComment(toString(fn));
  prototype += "int64_t chpl_mli_swrapper_";
  prototype += this->genFuncNumericID(fn);
  prototype += "(void)";

  // Generate a prototype for the function
  gen += prototype + ";\n";

  // Start the function
  gen += prototype;
  gen += scope_begin;

  gen += this->genServersideRPC(fn);
  gen += "return 0;\n";
 
  gen += scope_end;
  gen += "\n";

  this->setOutputAndWrite(&this->fiServerBundle, gen);
 
  return;
}

std::string MLIContext::genDebugPrintCall(FnSymbol* fn) {
  std::string msg;

  msg += "Calling: ";
  msg += toString(fn);

  return this->genDebugPrintCall(msg.c_str());
}

std::string MLIContext::genDebugPrintCall(const char* msg) {
  std::string gen;

  gen += "chpl_mli_debugf(\"%s\\n\", \"";
  gen += msg;
  gen += "\");\n";

  return gen;
}

std::string MLIContext::genDebugPrintCall1Arg(const char* msg,
                                              const char* uintArg) {
  std::string gen;
  gen += "chpl_mli_debugf(\"%s%llu\\n\", \"";
  gen += msg;
  gen += "\", ";
  gen += uintArg;
  gen += ");\n";

  return gen;
}

std::string MLIContext::genFuncNumericID(FnSymbol* fn) {
  return str((int64_t) fn->id);
}

std::string MLIContext::genServerWrapperCall(FnSymbol* fn) {
  std::string gen;

  gen += "chpl_mli_swrapper_";
  gen += this->genFuncNumericID(fn);
  gen += "();\n";

  return gen;
}
  
std::string
MLIContext::genServerDispatchSwitch(const std::vector<FnSymbol*>& fns) {
  std::string gen;

  gen += "int64_t chpl_mli_sdispatch";
  gen += "(int64_t function)";
  gen += scope_begin;
  gen += this->genNewDecl("int", "err");
  gen += "switch (function)";
  gen += scope_begin;

  for_vector(FnSymbol, fn, fns) {
    INT_ASSERT(this->shouldEmit(fn));

    gen += "case ";
    gen += this->genFuncNumericID(fn);
    gen += ": ";

    gen += scope_begin;
    
    if (this->debugPrint) {
      gen += this->genDebugPrintCall(fn);
    }
    
    gen += "err = ";
    gen += this->genServerWrapperCall(fn);
    gen += scope_end;
    gen += "break;\n";
  }

  gen += "default: return CHPL_MLI_CODE_ENOFUNC; break;\n";
  gen += scope_end;
  gen += "return err;\n";
  gen += scope_end;

  return gen;
}

//
// This filter will change as we support more and more type classes.
//
bool MLIContext::isSupportedType(Type* t) {
  return (
      (isPrimitiveScalar(t) && !is_complex_type(t)) ||
      t == dtStringC ||
      t == exportTypeChplByteBuffer
  );
}

void MLIContext::verifyPrototype(FnSymbol* fn) {

  if (fn->retType != dtVoid && !isSupportedType(fn->retType) &&
      exportedStrRets.find(fn) == exportedStrRets.end()) {
    // We only allow c_ptr(int8) if it was originally a Chapel string return
    Type* t = fn->retType;
    USR_FATAL(fn, "Multi-locale libraries do not support return type: %s",
              t->name());
  }

  for (int i = 1; i <= fn->numFormals(); i++) {
    ArgSymbol* as = fn->getFormal(i);
    if (!this->isSupportedType(as->type)) {
      Type* t = as->type;
      USR_FATAL(fn, "Multi-locale libraries do not support formal type: %s",
                t->name());
    }
  }

  return;
}

Type* MLIContext::getTypeFromFormal(ArgSymbol* as) {
  INT_ASSERT(as);
  return as->type;
}

Type* MLIContext::getTypeFromFormal(FnSymbol* fn, int i) {
  return getTypeFromFormal(fn->getFormal(i));
}

std::string MLIContext::genClientsideRPC(FnSymbol* fn) {
  bool hasVoidReturnType = fn->retType == dtVoid;
  bool hasFormals = fn->numFormals() != 0;
  std::string gen;

  // Declare the unique ID for this function.
  gen += "int64_t id = ";
  gen += this->genFuncNumericID(fn);
  gen += ";\n";

  // Declare a int64 for the server return value.
  gen += this->genNewDecl("int64_t", "st");

  // Declare a temporary for the return value, if necessary.
  if (!hasVoidReturnType) {
    gen += this->genNewDecl(fn->retType, "result");
  }

  if (this->debugPrint) {
    gen += this->genDebugPrintCall(fn);
  }

  // Push function to call.
  gen += this->genSocketPushCall(client_main, "id");

  // Pull server confirmation.
  gen += this->genSocketPullCall(client_main, "st");

  // If we are void/void, then there's nothing left to do.
  if (hasVoidReturnType && !hasFormals) {
    gen += this->genComment("Routine is void/void!");
    return gen;
  }

  // Issue pack call for each formal.
  for (int i = 1; i <= fn->numFormals(); i++) {
    ArgSymbol* as = fn->getFormal(i);
    Type* t = getTypeFromFormal(as);

    gen += this->genMarshalPushCall(client_arg, as->name, t);
  }

  // Pull and return result if applicable.
  if (!hasVoidReturnType) {
    gen += "result = ";
    gen += this->genMarshalPullCall(client_res, "result", fn->retType);
    gen += "return result;\n";
  }

  return gen;
}

std::string MLIContext::genServersideRPC(FnSymbol* fn) {
  std::map<int, std::string> formalTempNames;
  bool hasVoidReturnType = fn->retType == dtVoid;
  bool hasFormals = fn->numFormals() != 0;
  std::string gen;

  // Emit void/void calls immediately, then return.
  if (hasVoidReturnType && !hasFormals) {
    gen += fn->cname;
    gen += "();\n";
    return gen;
  }

  // Declare a temporary for the return value, if necessary.
  if (!hasVoidReturnType) { gen += genNewDecl(fn->retType, "result"); }

  // Declare temporaries, issue unpack call for each formal.
  for (int i = 1; i <= fn->numFormals(); i++) {
    Type* t = this->getTypeFromFormal(fn, i);
    std::string tmp;

    // Map temp names to formal indices (shifted down one).
    tmp += "tmp_";
    tmp += fn->getFormal(i)->name;
    formalTempNames[i] = tmp;

    // Declare each temporary and initialize with a pack call.
    gen += this->genTypeName(t);
    gen += " ";
    gen += tmp;
    gen += "=";
    gen += this->genMarshalPullCall(server_arg, tmp.c_str(), t);
  }

  // Only generate LHS target if necessary.
  if (!hasVoidReturnType) {
    gen += "result=";
  }

  // Make the unwrapped call.
  gen += fn->cname;
  gen += "(";

  // Pass in temporaries as arguments to call.
  if (hasFormals) {
    for (int i = 1; i <= fn->numFormals() - 1; i++) {
      gen += formalTempNames.at(i);
      gen += ",";
    }
    gen += formalTempNames.at(fn->numFormals());
  }

  gen += ");\n";

  // If there is a result, issue a pack call for it.
  if (!hasVoidReturnType) {
    gen += this->genMarshalPushCall(server_res, "result", fn->retType);
  }

  //
  // TODO: Generalize this later. Ideally, any type that requires dynamic
  // allocation should have functions generated for it which perform these
  // steps automatically as well, but since we're only supporting c_string
  // for now, we do this manually.
  //
  // Note that we only do this for the server, _after_ making the unwrapped
  // call. This is because values received by the server are considered to
  // be read only. On the client, it is the _user's_ responsibility to ensure
  // that any allocated data is freed.
  //
  for (int i = 1; i <= fn->numFormals(); i++) {
    Type* t = this->getTypeFromFormal(fn, i);
    if (this->isTypeRequiringAlloc(t)) {
      gen += this->genMemCleanup(t, formalTempNames[i].c_str());
    }
  }

  //
  // TODO: As it stands today, the server leaks memory allocated for return
  // values. This is _obviously_ not ideal, however uncommenting the
  // below code causes tests related to `c_string` to fail with a segfault.
  // So for now, knowingly leak the memory.
  //
  /*
  if (!hasVoidReturnType && this->isTypeRequiringAlloc(fn->retType)) {
    gen += this->genMemCleanup(fn->retType, "result");
  }
  */

  return gen;
}

std::string MLIContext::genMemCleanup(Type* t, const char* var) {
  std::string gen;

  if (t == dtStringC) {
    gen += "mli_free(";
    gen += "((void*) ";
    gen += var;
    gen += "));\n";
  } else if (t == exportTypeChplByteBuffer) {
    gen += "chpl_byte_buffer_free(";
    gen += var;
    gen += ");\n";
  } else {
    INT_FATAL("Unsupported type %s expects deallocation", t->symbol->name);
  }
       
  return gen;
}

std::string MLIContext::genMarshalCall(const char* skt, const char* var,
                                       Type* t, bool out) {
  std::string gen;
  int64_t id = this->assignUniqueTypeID(t);

  gen += out ? marshal_push_prefix : marshal_pull_prefix;
  gen += str(id);
  gen += "(";
  gen += skt;
  
  if (out) {
    gen += ",";
    gen += var;
  }

  gen += ");\n";

  return gen;
}

//
// TODO: These calls pass value types (cheaper to pass pointer).
//
std::string
MLIContext::genMarshalPushCall(const char* skt, const char* var, Type* t) {
  return this->genMarshalCall(skt, var, t, true);
}

//
// TODO: These calls pass value types (cheaper to pass pointer).
//
std::string
MLIContext::genMarshalPullCall(const char* skt, const char* var, Type* t) {
  return this->genMarshalCall(skt, var, t, false);
}

std::string MLIContext::genTypeName(Type* t) {
  return t->codegen().c;
}

//
// Bit of duplication, but "genSocketCall()" sadly assumes a value type, which
// means it doesn't work very well with buffers.
//
std::string
MLIContext::genSocketCallBuffer(const char* skt, const char* var,
                                const char* len, bool out) {
  std::string gen;

  gen += out ? socket_push_name : socket_pull_name;
  gen += "(";
  gen += skt;
  gen += ", ";
  // To get rid of _discards qualifiers_ warnings.
  gen += "((void*) ";
  gen += var;
  gen += ")";
  gen += ", ";
  gen += len;
  gen += ", 0);\n";

  return gen;
}

std::string
MLIContext::genSocketCall(const char* skt, const char* var, const char* len,
                          bool out) {
  std::string gen;

  gen += out ? socket_push_name : socket_pull_name;
  gen += "(";
  gen += skt;
  gen += ", ";
  gen += var ? this->genAddressOf(var) : "((void*)\"\")";
  gen += ", ";
  gen += len ? len : (var ? this->genSizeof(var) : "0");
  gen += ", 0);\n";

  return gen;
}

std::string
MLIContext::genSocketCall(const char* skt, const char* var, bool out) {
  return this->genSocketCall(skt, var, NULL, out);
}

std::string MLIContext::genSocketPushCall(const char* skt, const char* var) {
  return this->genSocketCall(skt, var, true);
}


std::string MLIContext::genSocketPullCall(const char* skt, const char* var) {
  return this->genSocketCall(skt, var, false);
}

std::string MLIContext::genAddressOf(const char* var) {
  std::string gen;

  gen += "&";
  gen += var;

  return gen;
}

std::string MLIContext::genAddressOf(std::string& var) {
  return this->genAddressOf(var.c_str());
}

std::string MLIContext::genSizeof(const char* var) {
  std::string gen;

  gen += "sizeof(";
  gen += var;
  gen += ")";

  return gen;
}

std::string MLIContext::genSizeof(std::string& var) {
  return this->genAddressOf(var.c_str());
}

bool MLIContext::isTypeRequiringAlloc(Type* t) {
  return
      // TODO: Do we just assume that all CPTRs require allocation?
      t->symbol->hasFlag(FLAG_C_PTR_CLASS) ||
      t == dtStringC ||
      t->getValType() == exportTypeChplByteBuffer;
}

std::string MLIContext::genNewDecl(const char* t, const char* v) {
  std::string gen;

  gen += t;
  gen += " ";
  gen += v;
  gen += ";\n";

  return gen;
}

std::string MLIContext::genNewDecl(Type* t, const char* v) {
  std::string gen = this->genTypeName(t);
  gen = this->genNewDecl(gen.c_str(), v);
  return gen;
}
