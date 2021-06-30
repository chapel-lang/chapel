/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
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

// This file lists the ASTType subclasses to help with macros generating code
// for each
//
// Each line consists of one of
//   TYPE_NODE(NAME)
//   TYPE_BEGIN_SUBCLASSES(NAME)
//   TYPE_END_SUBCLASSES(NAME)
//
// TYPE_NODE is for an ASTType subclass
// TYPE_BEGIN_SUBCLASSES/TYPE_END_SUBCLASSES mark subclasses of abstract classes

// These Type nodes are documented in their respective header files
// and only listed here.

// This file should store the Type nodes in groups according to the
// class hierarchy and otherwise be in sorted order.

// the following comment disables doxygen for these
/// \cond DO_NOT_DOCUMENT

TYPE_NODE(BuiltinType)
TYPE_NODE(ErroneousType)
TYPE_NODE(UnknownType)

TYPE_BEGIN_SUBCLASSES(DeclaredType)
  TYPE_NODE(EnumType)
  TYPE_NODE(OpaqueType)
  TYPE_NODE(FunctionType)
  TYPE_NODE(TupleType)

  TYPE_BEGIN_SUBCLASSES(AggregateType)
    TYPE_NODE(ClassType) // has field for management / generic management
    TYPE_NODE(RecordType)
    TYPE_NODE(UnionType)
  TYPE_END_SUBCLASSES(AggregateType)

TYPE_END_SUBCLASSES(DeclaredType)


/// \endcond

// this comment seems to be necessary for doxygen xml output to be well-formed
