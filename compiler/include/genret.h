/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

#ifndef GENRET_H
#define GENRET_H

#include <string>

#ifdef HAVE_LLVM
namespace llvm {
  class MDNode;
  class Value;
  class Type;
}
#endif

#define GEN_VAL      0
#define GEN_PTR      1
#define GEN_WIDE_PTR 2

class BaseAST;
class Type;
class GenRet;

extern GenRet baseASTCodegen(BaseAST* ast);
extern GenRet baseASTCodegenInt(int x);
extern GenRet baseASTCodegenString(const char* str);

/* The different codegen methods return a GenRet.
   This class exists so that they can return partial expressions and support
   both C and LLVM intermediate results.

   When codegen-ing, we will either be producing C or LLVM output.

   GenRet structure includes a field - isLVPtr - which indicates whether or not
   the value is actually a pointer to the value we would normally be talking
   about
     - if it is GEN_VAL it can only be read (e.g. an integer constant);
     - if it is GEN_PTR it is a local pointer (e.g. to stack variable or array
       member);i
     - if it is GEN_WIDE, it is a wide pointer.
   For example, if we have
     var x:int;
     x = x + 5;

   in 'x = x + 5' the LHS would be a GenRet storing '&x' with isLVPtr=GEN_PTR.
   The 5 would be a GenRet storing '5' with isLVPtr=GEN_VAL, and the x on the
   RHS would be just like the other x: '&x' with isLVPtr=GEN_PTR. Then, when
   we code generate +, we will codegenValue both its arguments (since it is
   reading them), which will dereference '&x' to form 'x' with isLVPtr=GEN_VAL,
   and leave '5' alone since it is already a value.

   The approach is similar for wide pointers - isLVPtr=GEN_WIDE means that we
   have a wide pointer, and if we codegenValue it we will code generate a get;
   if we codegenAssign it to something we will generate a put.

   Note that as far as the code generator is concerned, Chapel references,
   classes, and _ddata arrays are all mutable pointers to something; that is if
   we had a variable of that type, we would normally create a
   pointer-to-the-pointer and set isLVPtr=GEN_PTR - so that what is being
   pointed to can be changed. Put another way, references and class types are
   'value types' as far as the code generator is concerned.

   In particular note that a variable of reference type can be changed to point
   to something else. Sometimes the code generator is required to do that, even
   though it does not normally make sense with the Chapel reference semantics.

   Lastly, note that GenRet is implicitly convertible from
   BaseAST* (code generate some Chapel thing)
   const char* (generate a string)
   int (generate an int)

 */
class GenRet {
public:
  // When generating c, this should be set.
  // expression or type goes here
  std::string c;

#ifdef HAVE_LLVM
  // one of the following is set when generating LLVM
  llvm::Value *val; // use val->getType() to obtain LLVM type
  llvm::Type *type; // set when generating a type only
  Type *surroundingStruct; // surrounding structure, if this is a field
  uint64_t fieldOffset; // byte offset of this field within struct
  llvm::MDNode *fieldTbaaTypeDescriptor;
  llvm::MDNode *aliasScope;
  llvm::MDNode *noalias;
#else
  // Keeping same layout for non-LLVM builds
  void* val;
  void* type;
  void* surroundingStruct;
  uint64_t fieldOffset;
  void* fieldTbaaTypeDescriptor;
  void* aliasScope;
  void* noalias;
#endif

  // Used to mark variables as const after they are stored
  // Specifically: use "llvm.invariant.start"
  bool canBeMarkedAsConstAfterStore;

  // Mark pointers we already stored to, used to assert
  // the assumption that store to const memory
  // is the only store to that memory
  bool alreadyStored;

  // always set if available
  // note that the chplType of a GenRet corresponds to the Chapel
  // type of the result of codegenValue on it - that is, chplType
  // corresponds to the case when isLVPtr == GEN_VAL, and does not change
  // if isLVPtr is GEN_PTR or GEN_WIDE_PTR.
  Type *chplType;
  uint8_t isLVPtr; // for some L-value expression, we set isLVPtr
                   // if the generated expression is a possible lvalue
                   // If isLVPtr is set, the expression is the address
                   // of the e.g. variable we are referring to.
                   //
                   // If it's a wide pointer, we use put/get and set
                   //  isLVPtr = GEN_WIDE_PTR > 0. If it's a local pointer,
                   //  isLVPtr = GEN_PTR > 0.
   
  bool isUnsigned; // Is this expression unsigned?
                   // Needed for LLVM code generation in order to
                   // properly coerce call arguments into the correct
                   // called type, since LLVM native integer types do not
                   // include signed-ness.
                   
  GenRet() : c(), val(NULL), type(NULL), surroundingStruct(NULL),
             fieldOffset(0), fieldTbaaTypeDescriptor(NULL),
             aliasScope(NULL), noalias(NULL),
             canBeMarkedAsConstAfterStore(false), alreadyStored(false),
             chplType(NULL), isLVPtr(GEN_VAL), isUnsigned(false) { }

  // Allow implicit conversion from AST elements.
  GenRet(BaseAST* ast) {
    *this = baseASTCodegen(ast);
  }
  GenRet(int x) {
    *this = baseASTCodegenInt(x);
  }
  GenRet(const char* str) {
    *this = baseASTCodegenString(str);
  }

  // Return true if this GenRet is empty
  bool isEmpty() const {
    return c.empty() && val == NULL && type == NULL;
  }
};


#endif //GENRET_H
