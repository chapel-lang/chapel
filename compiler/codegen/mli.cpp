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

//
// TODO: Multi-Locale Interop prototype code!
//
// I decided to make a separate source file just to handle generation of the
// MLI wrapper code (this includes RPC and marshalling).
//
// --
// --
// --
//

const char* out_mli_marshalling = "chpl_mli_marshalling";
const char* out_mli_client_bundle = "chpl_mli_client_bundle";
const char* out_mli_server_bundle = "chpl_mli_server_bundle";

class MLIContext {
private:

  std::vector<FnSymbol*> exps;
  std::map<Type*, int64_t> tmap;
  fileinfo fiMarshalling;
  fileinfo fiClientBundle;
  fileinfo fiServerBundle;
  GenInfo* info;

public:

  MLIContext(bool separateHeaders=false, bool separateCompilation=false);
  ~MLIContext();

  void emit(ModuleSymbol* md);
  void emit(FnSymbol* fn);
  const std::vector<FnSymbol*>& getExports(void);
  const std::map<Type*, int64_t>& getTypeMap(void);
  void emitClientPrelude(void);
  void emitServerPrelude(void);
  void emitMarshallingRoutines(const std::map<Type*, int64_t>& tmap);
  void emitServerMain(const std::vector<FnSymbol*>& fns);

private:

  void setOutput(fileinfo* fi);
  void setOutputAndWrite(fileinfo* fi, const std::string& out);
  void write(const std::string& code);
  int64_t assignUniqueTypeID(Type* tp);
  void emitClientWrapper(FnSymbol* fn);
  void emitServerWrapper(FnSymbol* fn);
  std::string genServerDispatchSwitch(const std::vector<FnSymbol*>& fns);

};

void codegenMultiLocaleInteropWrappers(void) {
  Vec<ModuleSymbol*> &mds = allModules;

  MLIContext mli;

  mli.emitClientPrelude();
  mli.emitServerPrelude();

  forv_Vec(ModuleSymbol, md, mds) {
    mli.emit(md);
  }

  // These pieces have to be called manually after we're done.
  const std::vector<FnSymbol*>& exps = mli.getExports();
  const std::map<Type*, int64_t>& tmap = mli.getTypeMap();

  mli.emitMarshallingRoutines(tmap);
  mli.emitServerMain(exps);

  return;
}

MLIContext::MLIContext(bool separateHeaders, bool separateCompilation) {

  openCFile(&this->fiMarshalling, out_mli_marshalling, "c");
  openCFile(&this->fiClientBundle, out_mli_client_bundle, "c");
  openCFile(&this->fiServerBundle, out_mli_server_bundle, "c");
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
  }

  return;
}

void MLIContext::emit(FnSymbol* fn) {
  if (not fn->hasFlag(FLAG_EXPORT)) { return; }
 
  this->emitClientWrapper(fn);
  this->emitServerWrapper(fn);

  return;
}

const std::vector<FnSymbol*>& MLIContext::getExports(void) {
  return this->exps;
}

const std::map<Type*, int64_t>& MLIContext::getTypeMap(void) {
  return this->tmap;
}

void MLIContext::emitClientPrelude(void) {
  std::string out;

  out += "// TODO: Include generated CHPL header?\n";
  out += "// TODO: Include ZMQ header!\n";
  out += "#include \"chpl_mli_marshalling.c\"\n";
  out += "\n";

  this->setOutputAndWrite(&this->fiClientBundle, out);

  return;
}

void MLIContext::emitServerPrelude(void) {
  std::string out;

  out += "// TODO: Include generated _main.c file!\n";
  out += "// TODO: Include ZMQ header!\n";
  out += "// TODO: Override regularly generated chpl_gen_main!\n";
  out += "#include \"chpl_mli_marshalling.c\"\n";
  out += "\n";

  this->setOutputAndWrite(&this->fiServerBundle, out);

  return;
}

//
// TODO
//
// Generate marshalling routines for every key value pair in this map. For
// safety, assert that every value (int64 value) is unique?
//
// Marshalling routines are generated according to a simple algorithm which
// is reminiscent of how cycles are detected when verifying the members
// of value types (I'd think? Well, we shall soon find out).
//
//
void
MLIContext::emitMarshallingRoutines(const std::map<Type*, int64_t>& tmap) {
  return;
}

//
// TODO
//
// The server main consists of some initialization code, followed by a listen
// loop on the inbound socket port.
//
// Within the listen loop, the server waits for an int64 to be read off the
// wire. This int64 contains the unique number of a given function that is
// assigned at code generation.
//
// After receiving the ID of the function to call, the server passes the ID
// to a giant switch.
//
// Each exported function has a case in this switch. The body of the case is
// a call to the server wrapper for that function.
//
// The wrapper to call can be found by "chpl_mli_swrapper_" + str(ID).
// The wrapper takes the inbound ZMQ port as an argument.
//
// The process of implementing the inbound RPC protocol is invisible to the
// main function, it is nothing more than a glorified switch.
//
// We will need a function which enables the user to remotely shut down the
// server.
//
void MLIContext::emitServerMain(const std::vector<FnSymbol*>& fns) {
  std::string out;

  // The dispatch switch is in a separate function for now.
  out += this->genServerDispatchSwitch(fns);
  out += "\n";

  // Heaaaappps of ugly, manually written code.
  out += "int chpl_mli_smain(int argc, char** argv) {\n";
  out += "\t// Server initialization code should go here.\n";
  out += "\n";
  out += "\tint64_t function_id = -1;\n";
  out += "\tint err = 0;\n";
  out += "\n";
  out += "\twhile (1) {\n";
  out += "\t\t// Server listen action should go here.\n";
  out += "\t\t// IE: \"function_id = read_int64_from_zmq(zmq);\"\n";
  out += "\t\terr = chpl_mli_sdispatch(function_id, zmq);\n";
  out += "\t\tif (err) {\n";
  out += "\t\t\t// TODO: Respond to errors.\n";
  out += "\t\t\tchpl_halt(\"Error in dispatch loop!\");\n";
  out += "\t\t}\n";
  out += "\t}\n";
  out += "\n";
  out += "\n";
  out += "\treturn 0;\n";
  out += "}\n";

  this->setOutputAndWrite(&this->fiServerBundle, out);

  return;
}

void MLIContext::setOutput(fileinfo* fi) {
  this->info->cfile = fi->fptr;
  return;
}

void MLIContext::setOutputAndWrite(fileinfo* fi, const std::string& out) {
  this->setOutput(fi);
  this->write(out);
}

void MLIContext::write(const std::string& out) {
  fprintf(this->info->cfile, out.c_str());
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
// generate marshalling routines for all types in the map.
//
// --
//
int64_t MLIContext::assignUniqueTypeID(Type* tp) {
  return 0;
}

//
// TODO
//
// Client wrappers need access to the outbound ZMQ port. We can expose this
// in the marshalling header or just roll it into the top of the client
// library wrapper.
//
// The outbound ZMQ connection needs to be initialized by calling an init
// function. The connection also needs to be closed so that the server can
// be killed off. Is this a separate function from library init, or not?
//
// Client wrappers implement the outbound half of the RPC protocol, which is
// as follows:
//
//  0. Allocate space for the return type on the stack.
//  1. Push this function's unique ID onto the wire.
//  2. For each formal, emit a call to the appropriate marshalling pack
//      routine. These are generated lazily for each new type as requested.
//      Outside of primitive types, these are implemented in terms of each
//      other, so the only thing that need be known is the name (to emit
//      the call).
//  3. Call the appropriate unpack routine for the return type.
//  4. Return.
//
void MLIContext::emitClientWrapper(FnSymbol* fn) {
  std::string out;

  out += " ";
  out += "{\n";
  out += "  ";
  out += "// Empty client wrapper body, outbound RPC code here.\n";
  // TODO: Outbound RPC code generated here.
  out += "}\n";
  out += "\n";

  this->setOutput(&this->fiClientBundle);
  fn->codegenHeaderC();
  this->write(out);
  
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
//  4. Emit a call to the appropriate marshalling pack routine for the
//      return type.
//  5. Return.
//
void MLIContext::emitServerWrapper(FnSymbol* fn) {
  std::string out;

  // Big long, silly block of manual code generation.
  out += "// ";
  out += toString(fn);
  out += "\n";
  out += "void chpl_mli_swrapper_";
  out += std::to_string((int64_t) fn->id);
  out += "(ZMQPlaceholder* zmq) ";
  out += "{\n";
  out += "\t// Empty server wrapper body, inbound RPC code here.\n";
  // TODO: Inbound RPC code generated here.
  out += "}\n";
  out += "\n";

  this->setOutputAndWrite(&this->fiServerBundle, out);
 
  return;
}

//
// TODO
//
// This is about as simple as it gets, just a switch on the unique ID read
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
  std::string out;

  out += "int chpl_mli_sdispatch";
  out += "(int64_t function_id, ZMQPlaceholder* zmq) {\n";
  out += "\tswitch (function_id) {\n";

  for_vector(FnSymbol, fn, fns) {
    if (not fn->hasFlag(FLAG_EXPORT)) { continue; }
    const std::string ids = std::to_string((int64_t) fn->id);
    out += "\tcase ";
    out += ids;
    out += ": ";
    out += "chpl_mli_swrapper_";
    out += ids;
    out += "(zmq); break;\n";
  }

  out += "\tdefault: return -1; break;\n";
  out += "\t}\n";
  out += "\treturn 0;\n";
  out += "}\n";

  return out;
}

