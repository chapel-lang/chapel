/*
 * Copyright 2023-2026 Hewlett Packard Enterprise Development LP
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

// See method-tables.h for a top-level description of what the X-macros
// in this file do.

// The order here should be kept in sync with the order in the types list.
// Not all types need methods exposed to python, so not all type classes are listed here.
// See list in frontend/include/chpl/types/type-classes-list.h.

//
// Inside each method table, methods should be listed in alphabetical order
//

CLASS_BEGIN(ChapelType)
CLASS_END(ChapelType)

CLASS_BEGIN(CompositeType)
  PLAIN_GETTER(CompositeType, decl, "Get the chpl::uast::AstNode that declares this CompositeType",
               Nilable<const chpl::uast::AstNode*>,

               // For completely builtin types, the ID could be empty.
               // They don't have code-level declarations, so return 'None'.
               auto& id = node->id();
               if (id.isEmpty()) return nullptr;

               return parsing::idToAst(context, id))
  PLAIN_GETTER(CompositeType, name, "Get name of this CompositeType",
               chpl::UniqueString,
               return node->name())
CLASS_END(CompositeType)

CLASS_BEGIN(ArrayType)
  PLAIN_GETTER(ArrayType, domain_type, "Get the domain type of this ArrayType",
               Nilable<const chpl::types::DomainType*>,
               auto ty = node->domainType().type();
               if (!ty || !ty->isDomainType()) return {};
               return ty->toDomainType())
  PLAIN_GETTER(ArrayType, elt_type, "Get the element type of this ArrayType",
               Nilable<const chpl::types::Type*>,
               auto ty = node->eltType().type();
               if (!ty) return {};
               return ty)
CLASS_END(ArrayType)

CLASS_BEGIN(DomainType)
 PLAIN_GETTER(DomainType, kind, "Get the kind of this DomainType (e.g. rectangular, associative, etc.)",
              const char*,
              return chpl::types::DomainType::kindToString(node->kind()))
  PLAIN_GETTER(DomainType, rank, "Get the rank of this DomainType as an integer",
               int,
               return node->rankInt())
  PLAIN_GETTER(DomainType, idx_type, "Get the index type of this DomainType",
                Nilable<const chpl::types::Type*>,
                auto ty = node->idxType().type();
                if (!ty) return {};
                return ty)
  PLAIN_GETTER(DomainType, strides, "Get the param that represents the strides of this DomainType, if it exists",
               Nilable<const chpl::types::Param*>,
               auto ty = node->strides().param();
               if (!ty) return {};
               return ty)
CLASS_END(DomainType)

CLASS_BEGIN(EnumType)
  PLAIN_GETTER(EnumType, name, "Get name of this EnumType",
               chpl::UniqueString,
               return node->name())
  PLAIN_GETTER(EnumType, decl, "Get the chpl::uast::AstNode that declares this EnumType",
               Nilable<const chpl::uast::AstNode*>,
               // For completely builtin types, the ID could be empty.
               // They don't have code-level declarations, so return 'None'.
               auto& id = node->id();
               if (id.isEmpty()) return nullptr;
               return parsing::idToAst(context, id))
CLASS_END(EnumType)

CLASS_BEGIN(ClassType)
  PLAIN_GETTER(ClassType, manageable_type, "Get the type managed by this ClassType",
               Nilable<const chpl::types::ManageableType*>,
               return node->manageableType())
  PLAIN_GETTER(ClassType, basic_class_type, "Get the basic class type for this ClassType",
               Nilable<const chpl::types::BasicClassType*>,
               return node->basicClassType())
CLASS_END(ClassType)

CLASS_BEGIN(PlaceholderType)
  PLAIN_GETTER(PlaceholderType, originator, "Get the AST node corresponding to this PlaceholderType's ID",
               Nilable<const chpl::uast::AstNode*>,
               auto& id = node->id();
               if (id.isEmpty()) return nullptr;
               return parsing::idToAst(context, id))
CLASS_END(PlaceholderType)
