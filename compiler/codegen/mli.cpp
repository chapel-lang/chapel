/*
 * Copyright 2004-2019 Cray Inc.
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


#include <cstring>
#include "library.h"
#include "ModuleSymbol.h"
#include "FnSymbol.h"
#include "beautify.h"
#include "codegen.h"
#include "driver.h"
#include "expr.h"
#include "stlUtil.h"
#include "stringutil.h"
#include <map>


//
// TODO: Multi-Locale Interop prototype code!
//
// I decided to make a separate source file just to handle generation of the
// MLI wrapper code (this includes RPC and marshalling).
//
// TODO: Sooooo, it looks like all the formatting I've done may not matter
// at all, because files can be auto "beautified" on close. So after we
// get things working, we'll want to do another pass and remove any
// formatting characters we've generated, since the "beautify" function can
// do that for us.
// 
// --
// --
// --
//


const char* gen_mli_marshalling = "chpl_mli_marshalling";
const char* gen_mli_client_bundle = "chpl_mli_client_bundle";
const char* gen_mli_server_bundle = "chpl_mli_server_bundle";

static const char* client_main = "chpl_client.main";
static const char* client_arg = "chpl_client.arg";
static const char* client_res = "chpl_client.res";
static const char* server_main = "chpl_server.main";
static const char* server_arg = "chpl_server.arg";
static const char* server_res = "chpl_server.res";

static const char* marshal_push_prefix = "chpl_mli_mtpush_";
static const char* marshal_pull_prefix = "chpl_mli_mtpull_";

static const char* socket_push_name = "chpl_mli_push";
static const char* socket_pull_name = "chpl_mli_pull";


class MLIContext {
private:

bool injectDebugPrintlines;
bool separateHeaders;
std::vector<FnSymbol*> exps;
std::vector<FnSymbol*> throws;
std::map<Type*, int64_t> typeMap;
fileinfo fiMarshalling;
fileinfo fiClientBundle;
fileinfo fiServerBundle;
GenInfo* info;

public:

MLIContext(bool injectDebugPrintlines=false, bool separateHeaders=false);
~MLIContext();

void emit(ModuleSymbol* md);
void emit(FnSymbol* fn);
const std::vector<FnSymbol*>& getExports(void);
const std::map<Type*, int64_t>& getTypeMap(void);
void emitClientPrelude(void);
void emitServerPrelude(void);
void emitMarshalRoutines(void);
void emitServerDispatchRoutine(void);

private:

void setOutput(fileinfo* fi);
void setOutputAndWrite(fileinfo* fi, const std::string& gen);
void write(const std::string& code);
int64_t assignUniqueTypeID(Type* t);
void emitClientWrapper(FnSymbol* fn);
void emitServerWrapper(FnSymbol* fn);
bool structContainsOnlyPrimitiveScalars(Type* t);
bool isSupportedType(Type* t);
void verifyPrototype(FnSymbol* fn);
Type* getTypeFromFormal(ArgSymbol* as);
Type* getTypeFromFormal(FnSymbol* fn, int i);
 
private:

std::string genComment(const char* msg);
std::string genNote(const char* msg);
std::string genTodo(const char* msg);
std::string genHeaderInc(const char* header, bool system=false);
std::string genMarshalRoutine(Type* t, bool out);
std::string genMarshalPushRoutine(Type* t);
std::string genMarshalPullRoutine(Type* t);
std::string genServerDispatchSwitch(const std::vector<FnSymbol*>& fns);
std::string genFuncToSetServerGlobals(void);
std::string genClientCallPrologue(FnSymbol* fn);
std::string genClientCallEpilogue(FnSymbol *fn);
std::string genDebugPrintCall(FnSymbol* fn, const char* pfx="");
std::string genDebugPrintCall(const char* msg, const char* pfx="");
std::string genFuncNumericID(FnSymbol* fn);
std::string genServerWrapperCall(FnSymbol* fn);
std::string genClientsideRPC(FnSymbol* fn);
std::string genServersideRPC(FnSymbol* fn);
std::string genMarshalCall(const char* s, const char* v, Type* t, bool out);
std::string genMarshalPushCall(const char* s, const char* v, Type* t);
std::string genMarshalPullCall(const char* s, const char* v, Type* t);
std::string genTypeName(Type* t);
std::string genSocketCall(const char* s, const char* v, bool out);
std::string genSocketPushCall(const char* s, const char* v);
std::string genSocketPullCall(const char* s, const char* v);
std::string genAddressOf(const char* var);
std::string genAddressOf(std::string& var);
std::string genSizeof(const char* var);
std::string genSizeof(std::string& var);

};


//
// This is the main entrypoint for MLI code generation, call this if the
//  necessary conditions are met in codegen().
// --
//
void codegenMultiLocaleInteropWrappers(void) {
  Vec<ModuleSymbol*> &mds = allModules;

  // Insert all kinds of debug printlines into generated code for now.
  MLIContext mli(true);

  mli.emitClientPrelude();
  mli.emitServerPrelude();

  forv_Vec(ModuleSymbol, md, mds) {
    mli.emit(md);
  }

  mli.emitMarshalRoutines();
  mli.emitServerDispatchRoutine();

  return;
}


MLIContext::MLIContext(bool injectDebugPrintlines, bool separateHeaders) {

  // Yes, I know this isn't the most optimal way to initialize these!
  this->injectDebugPrintlines = injectDebugPrintlines;
  this->separateHeaders = separateHeaders;

  openCFile(&this->fiMarshalling, gen_mli_marshalling, "c");
  openCFile(&this->fiClientBundle, gen_mli_client_bundle, "c");
  openCFile(&this->fiServerBundle, gen_mli_server_bundle, "c");
  this->info = gGenInfo;

  return;
}


MLIContext::~MLIContext() {

  // Don't beautify YET!
  closeCFile(&this->fiMarshalling, false);
  closeCFile(&this->fiClientBundle, false);
  closeCFile(&this->fiServerBundle, false);

  return;
}


void MLIContext::emit(ModuleSymbol* md) {
  if (md->modTag == MOD_INTERNAL) { return; }

  const std::vector<FnSymbol*> fns = md->getTopLevelFunctions(true);

  for_vector(FnSymbol, fn, fns) {
    this->emit(fn);
    this->exps.push_back(fn);
    // We might like to generate bridge code for exceptions later.
    if (fn->throwsError()) { this->throws.push_back(fn); }
  }

  return;
}


void MLIContext::emit(FnSymbol* fn) {
  if (not fn->hasFlag(FLAG_EXPORT) || fn->hasFlag(FLAG_GEN_MAIN_FUNC)) {
    return;
  }

  this->verifyPrototype(fn); 
  this->emitClientWrapper(fn);
  this->emitServerWrapper(fn);

  return;
}


const std::vector<FnSymbol*>& MLIContext::getExports(void) {
  return this->exps;
}


const std::map<Type*, int64_t>& MLIContext::getTypeMap(void) {
  return this->typeMap;
}


void MLIContext::emitClientPrelude(void) {
  std::string gen;

  gen += this->genHeaderInc("chpl_mli_marshalling.c");
  gen += this->genNote("We use Makefile magic to make this visible!");
  gen += this->genHeaderInc("mli_client_runtime.c");
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


std::string MLIContext::genComment(const char* msg) {
  std::string gen;

  gen += "// ";
  gen += msg;
  gen += "\n";

  return gen;
}


std::string MLIContext::genNote(const char* msg) {
  std::string gen;

  gen += "// NOTE: ";
  gen += msg;
  gen += "\n";

  return gen;
}


std::string MLIContext::genTodo(const char* msg) {
  std::string gen;

  gen += "// TODO: ";
  gen += msg;
  gen += "\n";

  return gen;
}


std::string MLIContext::genFuncToSetServerGlobals(void) {
  std::string gen;

  gen += "void chpl_mli_server_set_conf(void) {\n";
  gen += "\tchpl_server_conf.debug = ";
  gen += this->injectDebugPrintlines ? "1" : "0";
  gen += ";\n";
  gen += "}\n";
  
  return gen;
}


void MLIContext::emitServerPrelude(void) {
  std::string gen;

  gen += this->genHeaderInc("chpl_mli_marshalling.c");
  gen += this->genNote("We use Makefile magic to make this visible!");
  gen += this->genHeaderInc("mli_server_runtime.c");
  gen += this->genHeaderInc("_main.c");
  gen += "\n";
  
  // The server will call this function to set globals appropriately.
  gen += this->genFuncToSetServerGlobals();
  gen += "\n";

  this->setOutputAndWrite(&this->fiServerBundle, gen);

  return;
}

//
// TODO
//
// Generate marshalling rgenines for every key value pair in this map. For
// safety, assert that every value (int64 value) is unique?
//
// Marshalling rgenines are generated according to a simple algorithm which
// is reminiscent of how cycles are detected when verifying the members
// of value types (I'd think? Well, we shall soon find gen).
// --
//
void MLIContext::emitMarshalRoutines(void) {
  std::map<Type*, int64_t>::iterator i;
  std::string gen;

  // Bunch of stuff included in generated header as bridge code.
  gen += this->genHeaderInc("stdlib.h", true);
  gen += this->genHeaderInc("stdio.h", true);
  gen += this->genHeaderInc("zmq.h", true);
  gen += this->genHeaderInc("chpl__header.h");
  gen += this->genNote("We use Makefile magic to make this visible!");
  gen += this->genHeaderInc("mli_common_code.c");
  gen += "\n";

  for (i = this->typeMap.begin(); i != this->typeMap.end(); ++i) {
    if (this->injectDebugPrintlines) {
      std::string tpn = this->genTypeName(i->first);
      gen += this->genComment(tpn.c_str());
    }

    gen += this->genMarshalPushRoutine(i->first);
    gen += this->genMarshalPullRoutine(i->first);
  }

  this->setOutputAndWrite(&this->fiMarshalling, gen);

  return;
}


std::string MLIContext::genMarshalRoutine(Type* t, bool out) {
  int64_t id = this->assignUniqueTypeID(t);
  std::string gen;

  // Push returns nothing, while pull returns the type being read in.
  if (out) {
    gen += "void ";
  } else {
    gen += this->genTypeName(t);
    gen += " ";
  }

  gen += out ? marshal_push_prefix : marshal_pull_prefix;
  gen += std::to_string(id);
  gen += "(void* skt";

  if (out) {
    gen += ", ";
    gen += this->genTypeName(t);
    gen += " var";
  }

  gen += ") {\n";

  // Declare a variable to catch socket errors.
  gen += "\tint skt_err = 0;\n";

  // If we are unpacking, declare a temporary for the return value.
  if (not out) {
    gen += "\t";
    gen += this->genTypeName(t);
    gen += " result;\n";
  }

  if (this->injectDebugPrintlines) {
    std::string msg;

    msg += out ? "Pushing type:" : "Pulling type:";
    msg += " ";
    msg += this->genTypeName(t);

    gen += "\t";
    gen += this->genDebugPrintCall(msg.c_str(), "[RPC]");
    gen += ";\n";
  }

  //
  // TODO
  //
  // Handle translation of different type classes here. Note that right now
  // about the only things we can translate are primitive scalars, pointers,
  // and cstrings (eventually).
  //
  // For right now, we'll focus only on emitting code for primitive scalars.
  // --
  //
  if (isPrimitiveScalar(t)) {
    
    // On pack, target buffer is input parameter. On unpack, the temporary.
    const char* target = out ? "var" : "result";
    
    gen += "\tskt_err = ";
    gen += this->genSocketCall("skt", target, out);
    gen += ";\n";

    if (this->injectDebugPrintlines) {
      gen += "\tif (skt_err < 0) {\n";
      gen += "\t\t";
      gen += this->genDebugPrintCall("Socket error!", "[RPC]");
      gen += ";\n";
      gen += "\t}\n";
    }
    
  } else {
    USR_FATAL("MLI does not support code generation for type", t);
  }

  // Generate a null frame in the opposite direction for the ACK.
  gen += "\t";
  gen += this->genSocketCall("skt", NULL, not out);
  gen += ";\n";

  // If we are unpacking, return our temporary.
  if (not out) {
    gen += "\treturn result;\n";
  }
  
  gen += "}\n\n";

  return gen;
}


//
// TODO: If these mirror, we can create a single bridge routine.
//
std::string MLIContext::genMarshalPushRoutine(Type* t) {
  return this->genMarshalRoutine(t, true);
}


//
// TODO: If these mirror, we can create a single bridge routine.
//
std::string MLIContext::genMarshalPullRoutine(Type* t) {
  return this->genMarshalRoutine(t, false);
}


void MLIContext::emitServerDispatchRoutine(void) {
  std::string gen;

  // The dispatch switch is in a separate function for now.
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
  fprintf(this->info->cfile, gen.c_str());
  return;
}


//
// TODO
//
// This should assign a unique ID to every new _fully qualified type_
// encountered, and return the already assigned ID otherwise.
//
// Use it to lazily stash IDs.
//
// The map built from repeated calls to this function can be used to
// generate marshalling rgenines for all types in the map.
//
// We can (as I understand it) cound on Type* being unique across the entire
// symbol table (IE, you'll never two different Type that both end up
// describing the same concrete type.
// --
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


//
// Assumes scope level is 1 (and generates 1 tab).
//
std::string MLIContext::genClientCallPrologue(FnSymbol* fn) {
  std::string gen;

  gen += "\tint64_t id = ";
  gen += this->genFuncNumericID(fn);
  gen += ";\n";

  // Stores the server return value.
  gen += "\tint64_t st = 0;\n";

  if (this->injectDebugPrintlines) {
    gen += "\t";
    gen += this->genDebugPrintCall(fn, "[Client]");
    gen += ";\n";
  }

  // Send out request.
  gen += "\t";
  gen += this->genSocketPushCall(client_main, "id");
  gen += ";\n";

  // Read out server confirmation.
  gen += "\t";
  gen += this->genSocketPullCall(client_main, "id");
  gen += ";\n"; 
 
  // Server return value "st" already declared in prologue.
  gen += "\t";
  gen += this->genTodo("Handle server errors.");
  gen += "\tif (st) { ;;; }\n"; 
  
  return gen;
}


//
// Assumes scope level is 1 (and generates 1 tab).
//
std::string MLIContext::genClientCallEpilogue(FnSymbol* fn) {
  std::string gen;
  return gen;
}


//
// TODO
//
// Client wrappers need access to the genbound ZMQ port. We can expose this
// in the marshalling header or just roll it into the top of the client
// library wrapper.
//
// The genbound ZMQ connection needs to be initialized by calling an init
// function. The connection also needs to be closed so that the server can
// be killed off. Is this a separate function from library init, or not?
//
// Client wrappers implement the genbound half of the RPC protocol, which is
// as follows:
//
//  0. Allocate space for the return type on the stack.
//  1. Push this function's unique ID onto the wire.
//  2. For each formal, emit a call to the appropriate marshalling pack
//      rgenine. These are generated lazily for each new type as requested.
//      Outside of primitive types, these are implemented in terms of each
//      other, so the only thing that need be known is the name (to emit
//      the call).
//  3. Call the appropriate unpack rgenine for the return type.
//  4. Return.
//
void MLIContext::emitClientWrapper(FnSymbol* fn) {
  std::string gen;

  this->setOutput(&this->fiClientBundle);
  fn->codegenHeaderC();

  gen += " ";
  gen += "{\n";
  gen += this->genClientCallPrologue(fn);
  gen += "\n";

  gen += this->genClientsideRPC(fn);
  gen += "\n";

  gen += this->genClientCallEpilogue(fn);
  gen += "}\n";
  gen += "\n";

  this->write(gen);
  
  return;
}


//
// TODO
//
// Server wrappers need access to the inbound ZMQ port. We can expose this
// in the marshalling header or just roll it into server main.
//
// The inbound ZMQ connection is a bit different, not sure of how the two-way
// handshake will take place yet.
//
// Server wrappers implement the inbound half of the RPC protocol, which is
// as follows:
//
//  0. Allocate space for all parameters and the return type on the stack.
//  1. For each formal, emit a call to the appropriate marshalling unpack
//      these are generated lazily for each new type as requested. Outside
//      of primitive types, these are implemented in terms of each other,
//      so the only thing that need be known is the name (to emit the call).
//  2. Pass the unpacked parameters to the actual implementation of the
//      function hosted by the server.
//  3. The result of the call is stored in the temporary allocated earlier.
//  4. Emit a call to the appropriate marshalling pack rgenine for the
//      return type.
//  5. Return.
//
void MLIContext::emitServerWrapper(FnSymbol* fn) {
  std::string gen;

  // Big long, silly block of manual code generation.
  gen += this->genComment(toString(fn));
  gen += "int64_t chpl_mli_swrapper_";
  gen += this->genFuncNumericID(fn);
  gen += "(void) ";
  gen += "{\n";
  
  gen += this->genServersideRPC(fn);
  gen += "\n";

  gen += "\treturn 0;\n";
  gen += "}\n";
  gen += "\n";

  this->setOutputAndWrite(&this->fiServerBundle, gen);
 
  return;
}


std::string MLIContext::genDebugPrintCall(FnSymbol* fn, const char* pfx) {
  std::string msg;

  msg += "Calling: ";
  msg += toString(fn);

  return this->genDebugPrintCall(msg.c_str(), pfx);
}


std::string MLIContext::genDebugPrintCall(const char* msg, const char* pfx) {
  std::string gen;

  gen += "printf(\"";

  if (pfx && pfx != "") {
    gen += pfx;
    gen += " ";
  }

  gen += "%%s\\n\", \"";
  gen += msg;
  gen += "\")";

  return gen;
}


std::string MLIContext::genFuncNumericID(FnSymbol* fn) {
  return std::to_string((int64_t) fn->id);
}


std::string MLIContext::genServerWrapperCall(FnSymbol* fn) {
  std::string gen;

  gen += "chpl_mli_swrapper_";
  gen += this->genFuncNumericID(fn);
  gen += "();";

  return gen;
}

  
//
// TODO
//
// This is abgen as simple as it gets, just a switch on the unique ID read
// from the inbound ZMQ port.
//
// The serverside wrapper to call is implicit in the function ID. The naming
// convention is: "chpl_mli_swrapper_" + str(id)
//
// For now, the default case should halt. Later, it should communicate an
// error to the client.
//
std::string
MLIContext::genServerDispatchSwitch(const std::vector<FnSymbol*>& fns) {
  std::string gen;

  gen += "int64_t chpl_mli_sdispatch";
  gen += "(int64_t function) {\n";
  gen += "\tint err = 0;\n";
  gen += "\n";
  gen += "\tswitch (function) {\n";

  for_vector(FnSymbol, fn, fns) {
    if (not fn->hasFlag(FLAG_EXPORT) || fn->hasFlag(FLAG_GEN_MAIN_FUNC)) {
      continue;
    }

    gen += "\tcase ";
    gen += this->genFuncNumericID(fn);
    gen += ": ";

    if (this->injectDebugPrintlines) {
      gen += "{\n";
      gen += "\t\t";
      gen += this->genDebugPrintCall(fn, "[MLI-Server]");
      gen += ";\n";
      gen += "\t\terr = ";
      gen += this->genServerWrapperCall(fn);
      gen += "\n";
      gen += "\t} break;\n";
    } else {
      gen += "err = ";
      gen += this->genServerWrapperCall(fn);
      gen += " break;\n";
    }
  }

  gen += "\tdefault: return CHPL_MLI_ERROR_NOFUNC; break;\n";
  gen += "\t}\n";
  gen += "\n";
  gen += "\treturn err;\n";
  gen += "}\n";

  return gen;
}


bool MLIContext::structContainsOnlyPrimitiveScalars(Type *t) {
  return false;
}


//
// TODO: This filter will change as we support more and more type classes.
//
bool MLIContext::isSupportedType(Type* t) {
  return isPrimitiveScalar(t);
}


void MLIContext::verifyPrototype(FnSymbol* fn) {

  if (fn->retType != dtVoid && not isSupportedType(fn->retType)) {
    USR_FATAL("MLI does not support code generation for type", fn->retType);
  }

  // Loop through all formals and error gen if a type is unsupported.
  for (int i = 1; i <= fn->numFormals(); i++) {
    ArgSymbol* as = fn->getFormal(i);
    if (isSupportedType(as->type)) { continue; }
    USR_FATAL("MLI does not support code generation for type", as->type);
  }

  return;
}


Type* MLIContext::getTypeFromFormal(ArgSymbol* as) {
  if (as == NULL) { return NULL; }
  return as->type;
}


Type* MLIContext::getTypeFromFormal(FnSymbol* fn, int i) {
  return getTypeFromFormal(fn->getFormal(i));
}


//
// TODO
//
// NOTE - This assumes scope level is 1 (and generates 1 tab). Only matters
// from a formatting POV. Whiiiiich doesn't actually matter, because we can
// just use "beautify" on close.
//
// 
//
std::string MLIContext::genClientsideRPC(FnSymbol* fn) {
  std::string gen;
  std::map<int, std::string> formalTempNames;
  bool hasVoidReturnType = fn->retType == dtVoid;
  bool hasFormals = fn->numFormals() != 0;

  // If we are void/void, then there's nothing left to do.
  if (hasVoidReturnType and not hasFormals) {
    gen += "\t";
    gen += this->genComment("Routine is void/void!");
    return gen;
  }

  // Declare a temporary for the return value, if necessary.
  if (not hasVoidReturnType) {
    gen += "\t";
    gen += this->genTypeName(fn->retType);
    gen += " result;\n";
  }

  // Issue pack call for each formal.
  for (int i = 1; i <= fn->numFormals(); i++) {
    ArgSymbol* as = fn->getFormal(i);
    Type* t = getTypeFromFormal(as);

    gen += "\t";
    gen += this->genMarshalPushCall(client_arg, as->name, t);
    gen += ";\n"; 
  }

  // Pull and return result if applicable.
  if (not hasVoidReturnType) {
    gen += "\tresult = ";
    gen += this->genMarshalPullCall(client_res, "result", fn->retType);
    gen += ";\n";
    gen += "\treturn result;\n";
  }
 
  return gen;
}


//
// TODO
//
// NOTE - This assumes scope level is 1 (and generates 1 tab). Only matters
// from a formatting POV. Whiiiiich doesn't actually matter, because we can
// just use "beautify" on close.
//
//
//
std::string MLIContext::genServersideRPC(FnSymbol* fn) {
  std::string gen;
  std::map<int, std::string> formalTempNames;
  bool hasVoidReturnType = fn->retType == dtVoid;
  bool hasFormals = fn->numFormals() != 0;

  // Emit void/void calls immediately.
  if (hasVoidReturnType and not hasFormals) {
    gen += "\t";
    gen += fn->cname;
    gen += "();\n";
    return gen;
  }

  // Declare a temporary for the return value, if necessary.
  if (not hasVoidReturnType) {
    gen += "\t";
    gen += this->genTypeName(fn->retType);
    gen += " result;\n";
  }

  // Declare temporaries, issue unpack call for each formal.
  for (int i = 1; i <= fn->numFormals(); i++) {
    std::string tmp;

    Type* t = this->getTypeFromFormal(fn, i);

    gen += "\t";
    gen += this->genTypeName(t);
    gen += " ";

    // Map temp names to formal indices (shifted down one).
    tmp += "tmp_";
    tmp += std::to_string(i - 1);
    formalTempNames[i] = tmp;

    // Emit a unpack call to initialize each temporary.
    gen += tmp;
    gen += " = ";
    gen += this->genMarshalPullCall(server_arg, tmp.c_str(), t);
    gen += ";\n";
  }

  gen += '\t';

  // Only generate LHS target if necessary.
  if (not hasVoidReturnType) {
    gen += "result = ";
  }

  // Make the unwrapped call.
  gen += fn->cname;
  gen += "(";

  // Pass in temporaries as arguments to call.
  if (hasFormals) {
    for (int i = 1; i <= fn->numFormals() - 1; i++) {
      gen += formalTempNames.at(i);
      gen += ", ";
    }
    gen += formalTempNames.at(fn->numFormals());
  }

  gen += ");\n";

  // If there is a result, issue a pack call for it.
  if (not hasVoidReturnType) {
    gen += '\t';
    gen += this->genMarshalPushCall(server_res, "result", fn->retType);
    gen += ";\n";
  }

  return gen;
}


std::string MLIContext::genMarshalCall(const char* s, const char* v, Type* t,
                                       bool out) {
  std::string gen;
  int64_t id = this->assignUniqueTypeID(t);

  gen += out ? marshal_push_prefix : marshal_pull_prefix;
  gen += std::to_string(id);
  gen += "(";
  gen += s;
  
  if (out) {
    gen += ", ";
    gen += v;
  }

  gen += ")";

  return gen;
}

//
// TODO: These calls pass value types (cheaper to pass pointer).
//
std::string
MLIContext::genMarshalPushCall(const char* s, const char* v, Type* t) {
  return this->genMarshalCall(s, v, t, true);
}


//
// TODO: These calls pass value types (cheaper to pass pointer).
//
std::string
MLIContext::genMarshalPullCall(const char* s, const char* v, Type* t) {
  (void) v;
  return this->genMarshalCall(s, v, t, false);
}


std::string MLIContext::genTypeName(Type* t) {
  return t->codegen().c;
}


std::string
MLIContext::genSocketCall(const char* s, const char* v, bool out) {
  std::string gen;

  gen += out ? socket_push_name : socket_pull_name;
  gen += "(";
  gen += s;
  gen += ", ";
  gen += v ? this->genAddressOf(v) : "\"\"";
  gen += ", ";
  gen += v ? this->genSizeof(v) : "0";
  gen += ", 0)";

  return gen;
}


std::string MLIContext::genSocketPushCall(const char* s, const char* v) {
  return this->genSocketCall(s, v, true);
}


std::string MLIContext::genSocketPullCall(const char* s, const char* v) {
  return this->genSocketCall(s, v, false);
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

