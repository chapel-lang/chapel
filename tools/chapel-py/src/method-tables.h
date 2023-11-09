/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

//
// This file defines nearly all the methods exposed to python from the frontend.
//
// To use these definitions, define the macro and then `#include` this file.
// See `chapel.cpp` for examples. If the macros below are not defined, they are
// defined to be empty. Only the ones that are used at the time of include need
// to be defined.
//

//
// Defines the beginning of an ast node
//
#ifndef CLASS_BEGIN
#define CLASS_BEGIN(TAG)
#endif

//
// Defines the end of an ast node
//
#ifndef CLASS_END
#define CLASS_END(TAG)
#endif

//
// Defines a simple getter that takes no arguments
//
// BODY is an inline definition of the getter. Available variables:
// - contextObject: the current frontend context
// - node: the ast node
// TYPESTR is the return type. Possible values:
// - "O" = python object
// - "s" = string, can be a C string, C++ string, or chpl UniqueStr
// - "b" = bool
//
#ifndef PLAIN_GETTER
#define PLAIN_GETTER(NODE, NAME, DOCSTR, TYPESTR, BODY)
#endif

//
// Declares a simple getter that takes no arguments. The definition is expected
// elsewhere in a function named `NODEObject_NAME`. See the `ACTUAL_ITERATOR`
// macro for an example
//
#ifndef METHOD_PROTOTYPE
#define METHOD_PROTOTYPE(NODE, NAME, DOCSTR)
#endif


//
// The order here should be kept in sync with the order in the uast list.
// Not all nodes need methods exposed to python, so not all uast nodes are listed here.
// See list in frontend/include/chpl/uast/uast-classes-list.h.
//
// Inside each method table, methods should be listed in alphabetical order
//

CLASS_BEGIN(AnonFormal)
  PLAIN_GETTER(AnonFormal, intent, "Get the intent for this AnonFormal node",
               const char*(void), return intentToString(node->intent()))
  PLAIN_GETTER(AnonFormal, type_expression, "Get the type expression for this AnonFormal node",
               const AstNode*(void), return node->typeExpression())
CLASS_END(AnonFormal)


CLASS_BEGIN(Array)
  PLAIN_GETTER(Array, exprs, "Get the expressions from this Array node",
               IterAdapterBase*(void), return mkIterPair(node->exprs()))
  PLAIN_GETTER(Array, has_trailing_comma, "Check if this Array node has a trailing comma",
               bool(void), return node->hasTrailingComma())
  PLAIN_GETTER(Array, is_associative, "Check if this Array node is associative",
               bool(void), return node->isAssociative())
CLASS_END(Array)

CLASS_BEGIN(Attribute)
  METHOD_PROTOTYPE(Attribute, actuals, "Get the actuals for this Attribute node")
  PLAIN_GETTER(Attribute, name, "Get the name of this Attribute node",
               UniqueString(void), return node->name())
CLASS_END(Attribute)

CLASS_BEGIN(AttributeGroup)
  PLAIN_GETTER(AttributeGroup, is_unstable, "Check if this AttributeGroup contains the 'unstable' attribute",
               bool(void), return node->isUnstable())
CLASS_END(AttributeGroup)

CLASS_BEGIN(Break)
  PLAIN_GETTER(Break, target, "Get the target from this Break node",
               const AstNode*(void), return node->target())
CLASS_END(Break)

CLASS_BEGIN(Conditional)
  PLAIN_GETTER(Conditional, condition, "Get the condition of this Conditional node",
               const AstNode*(void), return node->condition())
  PLAIN_GETTER(Conditional, else_block, "Get the else block of this Conditional node or None if no else block",
               const AstNode*(void), return node->elseBlock())
  PLAIN_GETTER(Conditional, is_expression_level, "Checks if this Conditional node is expression level",
               bool(void), return node->isExpressionLevel())
  PLAIN_GETTER(Conditional, then_block, "Get the then block of this Conditional node",
               const AstNode*(void), return node->thenBlock())
CLASS_END(Conditional)

CLASS_BEGIN(Comment)
  PLAIN_GETTER(Comment, text, "Get the text from this Comment node",
               const char*(void), return node->c_str())
CLASS_END(Comment)

CLASS_BEGIN(Continue)
  PLAIN_GETTER(Continue, target, "Get the target from this Continue node",
               const AstNode*(void), return node->target())
CLASS_END(Continue)

CLASS_BEGIN(Delete)
  PLAIN_GETTER(Delete, exprs, "Get the expressions from this Delete node",
               IterAdapterBase*(void), return mkIterPair(node->exprs()))
CLASS_END(Delete)

CLASS_BEGIN(Domain)
  PLAIN_GETTER(Domain, exprs, "Get the expressions from this Domain node",
               IterAdapterBase*(void), return mkIterPair(node->exprs()))
  PLAIN_GETTER(Domain, used_curly_braces, "Check if the Domain node used curly braces",
               bool(void), return node->usedCurlyBraces())
CLASS_END(Domain)

CLASS_BEGIN(Dot)
  PLAIN_GETTER(Dot, field, "Get the field accessed in the Dot node",
               UniqueString(), return node->field())
  PLAIN_GETTER(Dot, receiver, "Get the receiver of the Dot node",
               const AstNode*(void), return node->receiver())
CLASS_END(Dot)

CLASS_BEGIN(FunctionSignature)
  PLAIN_GETTER(FunctionSignature, formals, "Get the formals for this FunctionSignature node",
               IterAdapterBase*(void), return mkIterPair(node->formals()))
  PLAIN_GETTER(FunctionSignature, is_parenless, "Check if this FunctionSignature node is parenless",
               bool(void), return node->isParenless())
  PLAIN_GETTER(FunctionSignature, kind, "Get the kind of this FunctionSignature node",
               const char*(void), return chpl::uast::Function::kindToString(node->kind()))
  PLAIN_GETTER(FunctionSignature, return_intent, "Get the return intent of this FunctionSignature node",
               const char*(void),  return intentToString(node->returnIntent()))
  PLAIN_GETTER(FunctionSignature, return_type, "Get the return type for this FunctionSignature node",
               const AstNode*(void), return node->returnType())
  PLAIN_GETTER(FunctionSignature, this_formal, "Get the this formal for this FunctionSignature node",
               const AstNode*(void), return node->thisFormal())
  PLAIN_GETTER(FunctionSignature, throws, "Check if this FunctionSignature node is marked throws",
               bool(void), return node->throws())
CLASS_END(FunctionSignature)

CLASS_BEGIN(Identifier)
  PLAIN_GETTER(Identifier, name, "Get the name of this Identifier node",
               UniqueString(void), return node->name())
CLASS_END(Identifier)

CLASS_BEGIN(Init)
  PLAIN_GETTER(Init, target, "Get the target of this Init node",
               const AstNode*(void), return node->target())
CLASS_END(Init)

CLASS_BEGIN(Label)
  PLAIN_GETTER(Label, loop, "Get the loop this Label node is attached to",
               const AstNode*(void), return node->loop())
  PLAIN_GETTER(Label, name, "Get the name of this Label node",
               UniqueString(void), return node->name())
CLASS_END(Label)

CLASS_BEGIN(New)
  PLAIN_GETTER(New, management, "Get the management style for this New node",
               const char*(void), return chpl::uast::New::managementToString(node->management()))
  PLAIN_GETTER(New, type_expression, "Get the type expression for this New node",
               const AstNode*(void), return node->typeExpression())
CLASS_END(New)

CLASS_BEGIN(Range)
  PLAIN_GETTER(Range, lower_bound, "Get the lower bound of this Range node",
               const AstNode*(void), return node->lowerBound())
  PLAIN_GETTER(Range, op_kind, "Get the op kind of this Range node",
               const char*(void), return opKindToString(node->opKind()))
  PLAIN_GETTER(Range, upper_bound, "Get the upper bound of this Range node",
               const AstNode*(void), return node->upperBound())
CLASS_END(Range)

CLASS_BEGIN(Return)
  PLAIN_GETTER(Return, value, "Get the expression returned by this Return node",
               const AstNode*(void), return node->value())
CLASS_END(Return)

CLASS_BEGIN(Throw)
  PLAIN_GETTER(Throw, error_expression, "Get the expression thrown by this Throw node",
               const AstNode*(void), return node->errorExpression())
CLASS_END(Throw)

CLASS_BEGIN(VisibilityClause)
  PLAIN_GETTER(VisibilityClause, symbol, "Get the symbol referenced by this VisibilityClause node",
               const AstNode*(void), return node->symbol())
CLASS_END(VisibilityClause)

CLASS_BEGIN(WithClause)
  PLAIN_GETTER(WithClause, exprs, "Get the expressions of this WithClause node",
               IterAdapterBase*(void), return mkIterPair(node->exprs()))
CLASS_END(WithClause)

CLASS_BEGIN(Yield)
  PLAIN_GETTER(Yield, value, "Get the expression yielded by this Yield node",
               const AstNode*(void), return node->value())
CLASS_END(Yield)

CLASS_BEGIN(START_SimpleBlockLike)
  PLAIN_GETTER(SimpleBlockLike, block_style, "Get the block style of this SimpleBlockLike node",
               const char*(void), return blockStyleToString(node->blockStyle()))
CLASS_END(START_SimpleBlockLike)

CLASS_BEGIN(Begin)
  PLAIN_GETTER(Begin, with_clause, "Get the WithClause of this Begin node",
               const AstNode*(void), return node->withClause())
CLASS_END(Begin)

CLASS_BEGIN(Local)
  PLAIN_GETTER(Local, condition, "Get the condition of this Local node",
               const AstNode*(void), return node->condition())
CLASS_END(Local)

CLASS_BEGIN(On)
  PLAIN_GETTER(On, destination, "Get the destination of this On node",
               const AstNode*(void), return node->destination())
CLASS_END(On)

CLASS_BEGIN(Serial)
  PLAIN_GETTER(Serial, condition, "Get the condition of this Serial node",
               const AstNode*(void), return node->condition())
CLASS_END(Serial)

CLASS_BEGIN(START_Loop)
  PLAIN_GETTER(Loop, block_style, "Get the block style of this Loop node",
               const char*(void), return blockStyleToString(node->blockStyle()))
  PLAIN_GETTER(Loop, body, "Get the body of this Loop node",
               const AstNode*(void), return node->body())
CLASS_END(START_Loop)

CLASS_BEGIN(DoWhile)
  PLAIN_GETTER(DoWhile, condition, "Get the condition of this DoWhole node",
               const AstNode*(void), return node->condition())
CLASS_END(DoWhile)

CLASS_BEGIN(While)
  PLAIN_GETTER(While, condition, "Get the condition of this While node",
               const AstNode*(void), return node->condition())
CLASS_END(While)

CLASS_BEGIN(START_IndexableLoop)
  PLAIN_GETTER(IndexableLoop, index, "Get the index of this IndexableLoop node",
               const AstNode*(void), return node->index())
  PLAIN_GETTER(IndexableLoop, is_expression_level, "Check if this IndexableLoop node is expression level",
               bool(void), return node->isExpressionLevel())
  PLAIN_GETTER(IndexableLoop, iterand, "Get the iterand of this IndexableLoop node",
               const AstNode*(void), return node->iterand())
  PLAIN_GETTER(IndexableLoop, with_clause, "Get the WithClause of this IndexableLoop node",
               const AstNode*(void), return node->withClause())
CLASS_END(START_IndexableLoop)

CLASS_BEGIN(BracketLoop)
  PLAIN_GETTER(BracketLoop, is_maybe_array_type, "Check if this BracketLoop node may actually be a type",
               bool(void), return node->isMaybeArrayType())
CLASS_END(BracketLoop)

CLASS_BEGIN(For)
  PLAIN_GETTER(For, is_param, "Check if this For node is a param",
               bool(void), return node->isParam())
CLASS_END(For)

CLASS_BEGIN(BoolLiteral)
  PLAIN_GETTER(BoolLiteral, value, "Get the value of this BoolLiteral node",
               const char*(void), return (node->value() ? "true" : "false"))
CLASS_END(BoolLiteral)

CLASS_BEGIN(ImagLiteral)
  PLAIN_GETTER(ImagLiteral, text, "Get the value of this ImagLiteral node",
               UniqueString(void), return node->text())
CLASS_END(ImagLiteral)

CLASS_BEGIN(IntLiteral)
  PLAIN_GETTER(IntLiteral, text, "Get the value of this IntLiteral node",
               UniqueString(void), return node->text())
CLASS_END(IntLiteral)

CLASS_BEGIN(RealLiteral)
  PLAIN_GETTER(RealLiteral, text, "Get the value of this RealLiteral node",
               UniqueString(void), return node->text())
CLASS_END(RealLiteral)

CLASS_BEGIN(UintLiteral)
  PLAIN_GETTER(UintLiteral, text, "Get the value of this UintLiteral node",
               UniqueString(void), return node->text())
CLASS_END(UintLiteral)

CLASS_BEGIN(START_StringLikeLiteral)
  PLAIN_GETTER(StringLikeLiteral, value, "Get the value of this StringLikeLiteral node",
               UniqueString(void), return node->value())
CLASS_END(START_StringLikeLiteral)

CLASS_BEGIN(START_Call)
  PLAIN_GETTER(Call, actuals, "Get the arguments to this Call node",
               IterAdapterBase*(void), return mkIterPair(node->actuals()))
  PLAIN_GETTER(Call, called_expression, "Get the expression invoked by this Call node",
               const AstNode*(void), return node->calledExpression())
CLASS_END(START_Call)

CLASS_BEGIN(FnCall)
  METHOD_PROTOTYPE(FnCall, actuals, "Get the actuals of this FnCall node")
  PLAIN_GETTER(FnCall, used_square_brackets, "Check if this FnCall was made using square brackets",
               bool(void), return node->callUsedSquareBrackets())
CLASS_END(FnCall)

CLASS_BEGIN(OpCall)
  PLAIN_GETTER(OpCall, is_binary_op, "Check if this OpCall node is a binary op",
               bool(void), return node->isBinaryOp())
  PLAIN_GETTER(OpCall, is_unary_op, "Check if this OpCall node is an unary op",
               bool(void), return node->isUnaryOp())
  PLAIN_GETTER(OpCall, op, "Get the op string for this OpCall node",
               UniqueString(void), return node->op())
CLASS_END(OpCall)

CLASS_BEGIN(Reduce)
  PLAIN_GETTER(Reduce, iterand, "Get the iterand for this Reduce node",
               const AstNode*(void), return node->iterand())
  PLAIN_GETTER(Reduce, op, "Get the op for this Reduce node",
               const AstNode*(void), return node->op())
CLASS_END(Reduce)

CLASS_BEGIN(START_Decl)
  PLAIN_GETTER(Decl, linkage, "Get the linkage of this Decl node",
               const char*(void), return chpl::uast::Decl::linkageToString(node->linkage()))
  PLAIN_GETTER(Decl, linkage_name, "Get the linkage name of this Decl node",
               const AstNode*(void), return node->linkageName())
  PLAIN_GETTER(Decl, visibility, "Get the visibility of this Decl node",
               const char*(void), return chpl::uast::Decl::visibilityToString(node->visibility()))
CLASS_END(START_Decl)

CLASS_BEGIN(TupleDecl)
  PLAIN_GETTER(TupleDecl, decls, "Get the declarations for this TupleDecl node",
               IterAdapterBase*(void), return mkIterPair(node->decls()))
  PLAIN_GETTER(TupleDecl, init_expression, "Get the init expression of this TupleDecl node",
               const AstNode*(void), return node->typeExpression())
  PLAIN_GETTER(TupleDecl, intent_or_kind, "Get the intent or kind of this TupleDecl node",
               const char*(void), return chpl::uast::TupleDecl::intentOrKindToString(node->intentOrKind()))
  PLAIN_GETTER(TupleDecl, type_expression, "Get the type expression of this TupleDecl node",
               const AstNode*(void), return node->initExpression())
CLASS_END(TupleDecl)

CLASS_BEGIN(START_NamedDecl)
  PLAIN_GETTER(NamedDecl, name, "Get the name of this NamedDecl node",
               UniqueString(void), return node->name())
CLASS_END(START_NamedDecl)

CLASS_BEGIN(EnumElement)
  PLAIN_GETTER(EnumElement, init_expression, "Get the init expression of this EnumElement node",
               const AstNode*(void), return node->initExpression())
CLASS_END(EnumElement)

CLASS_BEGIN(Function)
  PLAIN_GETTER(Function, formals, "Get the formals for this Function node",
               IterAdapterBase*(void), return mkIterPair(node->formals()))
  PLAIN_GETTER(Function, body, "Get the body for this function",
               const AstNode*(void), return node->body())
  PLAIN_GETTER(Function, is_anonymous, "Check if this Function node is anonymous",
               bool(void), return node->isAnonymous())
  PLAIN_GETTER(Function, is_inline, "Check if this Function node is marked inline",
               bool(void), return node->isInline())
  PLAIN_GETTER(Function, is_method, "Check if this Function node is a method",
               bool(void), return node->isMethod())
  PLAIN_GETTER(Function, is_override, "Check if this Function node is an override",
               bool(void), return node->isOverride())
  PLAIN_GETTER(Function, is_parenless, "Check if this Function node is parenless",
               bool(void), return node->isParenless())
  PLAIN_GETTER(Function, is_primary_method, "Check if this Function node is a primary method",
               bool(void), return node->isPrimaryMethod())
  PLAIN_GETTER(Function, kind, "Get the kind of this Function node",
               const char*(void), return chpl::uast::Function::kindToString(node->kind()))
  PLAIN_GETTER(Function, lifetime_clauses, "Get the lifetime clauses for this Function node",
               IterAdapterBase*(void), return mkIterPair(node->lifetimeClauses()))
  PLAIN_GETTER(Function, return_intent, "Get the return intent of this Function node",
               const char*(void),  return intentToString(node->returnIntent()))
  PLAIN_GETTER(Function, return_type, "Get the return type for this Function node",
               const AstNode*(void), return node->returnType())
  PLAIN_GETTER(Function, this_formal, "Get the this formal for this Function node",
               const AstNode*(void), return node->thisFormal())
  PLAIN_GETTER(Function, throws, "Check if this Function node is marked throws",
               bool(void), return node->throws())
  PLAIN_GETTER(Function, where_clause, "Get the where clause for this Function node",
               const AstNode*(void), return node->whereClause())
CLASS_END(Function)

CLASS_BEGIN(Module)
  PLAIN_GETTER(Module, kind, "Get the kind of this Module node",
               const char*(void), return chpl::uast::Module::moduleKindToString(node->kind()))
CLASS_END(Module)

CLASS_BEGIN(ReduceIntent)
  PLAIN_GETTER(ReduceIntent, op, "Get the op for this ReduceIntent node",
               const AstNode*(void), return node->op())
CLASS_END(ReduceIntent)

CLASS_BEGIN(START_VarLikeDecl)
  PLAIN_GETTER(VarLikeDecl, init_expression, "Get the init expression of this VarLikeDecl node",
               const AstNode*(void), return node->initExpression())
  PLAIN_GETTER(VarLikeDecl, storage_kind, "Get the storage kind of this VarLikeDecl node",
               const char*(void), return chpl::uast::qualifierToString(node->storageKind()))
  PLAIN_GETTER(VarLikeDecl, type_expression, "Get the type expression of this VarLikeDecl node",
               const AstNode*(void), return node->typeExpression())
CLASS_END(START_VarLikeDecl)

CLASS_BEGIN(Formal)
  PLAIN_GETTER(Formal, intent, "Get the intent for this Formal node",
               const char*(void), return intentToString(node->intent()))
CLASS_END(Formal)

CLASS_BEGIN(TaskVar)
  PLAIN_GETTER(TaskVar, intent, "Get the intent of this TaskVar node",
               const char*(void), return intentToString(node->intent()))
CLASS_END(TaskVar)

CLASS_BEGIN(Variable)
  PLAIN_GETTER(Variable, is_config, "Check if this Variable node is a config variable",
               bool(void), return node->isConfig())
  PLAIN_GETTER(Variable, is_field, "Check if this Variable node is a class field variable",
               bool(void), return node->isField())
  PLAIN_GETTER(Variable, kind, "Get the kind of this Variable node",
               const char*(void), return chpl::uast::qualifierToString(node->storageKind()))
CLASS_END(Variable)

CLASS_BEGIN(START_AggregateDecl)
  PLAIN_GETTER(AggregateDecl, decls_or_comments, "Get the declarations and comments of this AggregateDecl node",
               IterAdapterBase*(void), return mkIterPair(node->declOrComments()))
  PLAIN_GETTER(AggregateDecl, inherit_exprs, "Get the inherit expressions of this AggregateDecl node",
               IterAdapterBase*(void), return mkIterPair(node->inheritExprs()))
CLASS_END(START_AggregateDecl)

//
// Cleanup and undefine all macros
//
#undef CLASS_BEGIN
#undef CLASS_END
#undef PLAIN_GETTER
#undef METHOD_PROTOTYPE
