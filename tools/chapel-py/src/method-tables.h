/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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
// TYPEFN is the type of the method, not including the receiver. It should
//        be written as the C++ function type, ReturnType(ArgTypes...).
//
#ifndef METHOD
#define METHOD(NODE, NAME, DOCSTR, TYPEFN, BODY)
#endif

#ifndef PLAIN_GETTER
#define PLAIN_GETTER(NODE, NAME, DOCSTR, RETTYPE, BODY) \
  METHOD(NODE, NAME, DOCSTR, RETTYPE(), (void) args; BODY)
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
               const char*, return intentToString(node->intent()))
  PLAIN_GETTER(AnonFormal, type_expression, "Get the type expression for this AnonFormal node",
               const chpl::uast::AstNode*, return node->typeExpression())
CLASS_END(AnonFormal)

CLASS_BEGIN(As)
  PLAIN_GETTER(As, symbol, "Get the symbol for this As node",
               const chpl::uast::AstNode*, return node->symbol())
  PLAIN_GETTER(As, rename, "Get the rename for this As node",
               const chpl::uast::AstNode*, return node->rename())
CLASS_END(As)

CLASS_BEGIN(Array)
  PLAIN_GETTER(Array, exprs, "Get the expressions from this Array node",
               IterAdapterBase*, return mkIterPair(node->exprs()))
  PLAIN_GETTER(Array, has_trailing_comma, "Check if this Array node has a trailing comma",
               bool, return node->hasTrailingComma())
  PLAIN_GETTER(Array, is_associative, "Check if this Array node is associative",
               bool, return node->isAssociative())
CLASS_END(Array)

CLASS_BEGIN(Attribute)
  METHOD_PROTOTYPE(Attribute, actuals, "Get the actuals for this Attribute node")
  PLAIN_GETTER(Attribute, name, "Get the name of this Attribute node",
               chpl::UniqueString, return node->name())
CLASS_END(Attribute)

CLASS_BEGIN(AttributeGroup)
  PLAIN_GETTER(AttributeGroup, is_unstable, "Check if this AttributeGroup contains the 'unstable' attribute",
               bool, return node->isUnstable())
  PLAIN_GETTER(AttributeGroup, is_deprecated, "Check if this AttributeGroup contains the 'deprecated' attribute",
               bool, return node->isDeprecated())
CLASS_END(AttributeGroup)

CLASS_BEGIN(Break)
  PLAIN_GETTER(Break, target, "Get the target from this Break node",
               const chpl::uast::AstNode*, return node->target())
CLASS_END(Break)

CLASS_BEGIN(Catch)
  PLAIN_GETTER(Catch, target, "Get the error from this Catch node",
               const chpl::uast::AstNode*, return node->error())
  PLAIN_GETTER(Catch, body, "Get the body from this Catch node",
               const chpl::uast::AstNode*, return node->body())
  PLAIN_GETTER(Catch, has_parens_around_error, "Check if this Catch uses parentheses",
               bool, return node->hasParensAroundError())
CLASS_END(Catch)

CLASS_BEGIN(Cobegin)
  PLAIN_GETTER(Cobegin, with_clause, "Get the WithClause from this Cobegin node",
               const chpl::uast::AstNode*, return node->withClause())
  PLAIN_GETTER(Cobegin, task_bodies, "Get tasks from this Cobegin node",
               IterAdapterBase*, return mkIterPair(node->taskBodies()))
CLASS_END(Cobegin)

CLASS_BEGIN(Conditional)
  PLAIN_GETTER(Conditional, condition, "Get the condition of this Conditional node",
               const chpl::uast::AstNode*, return node->condition())
  PLAIN_GETTER(Conditional, else_block, "Get the else block of this Conditional node or None if no else block",
               const chpl::uast::AstNode*, return node->elseBlock())
  PLAIN_GETTER(Conditional, is_expression_level, "Checks if this Conditional node is expression-level",
               bool, return node->isExpressionLevel())
  PLAIN_GETTER(Conditional, then_block, "Get the then block of this Conditional node",
               const chpl::uast::AstNode*, return node->thenBlock())
CLASS_END(Conditional)

CLASS_BEGIN(Comment)
  PLAIN_GETTER(Comment, text, "Get the text from this Comment node",
               const char*, return node->c_str())
CLASS_END(Comment)

CLASS_BEGIN(Continue)
  PLAIN_GETTER(Continue, target, "Get the target from this Continue node",
               const chpl::uast::AstNode*, return node->target())
CLASS_END(Continue)

CLASS_BEGIN(Delete)
  PLAIN_GETTER(Delete, exprs, "Get the expressions from this Delete node",
               IterAdapterBase*, return mkIterPair(node->exprs()))
CLASS_END(Delete)

CLASS_BEGIN(Domain)
  PLAIN_GETTER(Domain, exprs, "Get the expressions from this Domain node",
               IterAdapterBase*, return mkIterPair(node->exprs()))
  PLAIN_GETTER(Domain, used_curly_braces, "Check if the Domain node used curly braces",
               bool, return node->usedCurlyBraces())
CLASS_END(Domain)

CLASS_BEGIN(Dot)
  PLAIN_GETTER(Dot, field, "Get the field accessed in the Dot node",
               chpl::UniqueString, return node->field())
  PLAIN_GETTER(Dot, field_location, "Get the textual location of the Dot node's field expression",
               chpl::Location, return chpl::parsing::locateDotFieldWithAst(context, node))
  PLAIN_GETTER(Dot, receiver, "Get the receiver of the Dot node",
               const chpl::uast::AstNode*, return node->receiver())
  PLAIN_GETTER(Dot, to_node, "Get the AST node that this Dot node refers to",
               const chpl::uast::AstNode*, return nodeOrNullFromToId(context, node))
CLASS_END(Dot)

CLASS_BEGIN(ExternBlock)
  PLAIN_GETTER(ExternBlock, code, "Get C code of this ExternBlock node",
               std::string, return node->code())
CLASS_END(ExternBlock)

CLASS_BEGIN(FunctionSignature)
  PLAIN_GETTER(FunctionSignature, formals, "Get the formals for this FunctionSignature node",
               IterAdapterBase*, return mkIterPair(node->formals()))
  PLAIN_GETTER(FunctionSignature, is_parenless, "Check if this FunctionSignature node is parenless",
               bool, return node->isParenless())
  PLAIN_GETTER(FunctionSignature, kind, "Get the kind of this FunctionSignature node",
               const char*, return Function::kindToString(node->kind()))
  PLAIN_GETTER(FunctionSignature, return_intent, "Get the return intent of this FunctionSignature node",
               const char*,  return intentToString(node->returnIntent()))
  PLAIN_GETTER(FunctionSignature, return_type, "Get the return type for this FunctionSignature node",
               const chpl::uast::AstNode*, return node->returnType())
  PLAIN_GETTER(FunctionSignature, this_formal, "Get the this formal for this FunctionSignature node",
               const chpl::uast::AstNode*, return node->thisFormal())
  PLAIN_GETTER(FunctionSignature, throws, "Check if this FunctionSignature node is marked throws",
               bool, return node->throws())
CLASS_END(FunctionSignature)

CLASS_BEGIN(Implements)
  PLAIN_GETTER(Implements, interface_name, "Get the interface name of this Implements node",
               chpl::UniqueString, return node->interfaceName())
  PLAIN_GETTER(Implements, type_ident, "Get the type identifier from this Implements node",
               const chpl::uast::AstNode*, return node->typeIdent())
  PLAIN_GETTER(Implements, interface_expr, "Get the interface expression from this Implements node",
               const chpl::uast::AstNode*, return node->interfaceExpr())
  PLAIN_GETTER(Implements, is_expression_level, "Check if this Implements node is expression level",
               bool, return node->isExpressionLevel())
CLASS_END(Implements)

CLASS_BEGIN(Identifier)
  PLAIN_GETTER(Identifier, name, "Get the name of this Identifier node",
               chpl::UniqueString, return node->name())
  PLAIN_GETTER(Identifier, to_node, "Get the AST node that this Identifier node refers to",
               const chpl::uast::AstNode*, return nodeOrNullFromToId(context, node))
CLASS_END(Identifier)

CLASS_BEGIN(Import)
  PLAIN_GETTER(Import, visibility, "Get the visibility of this Import node",
               const char*, return Decl::visibilityToString(node->visibility()))
  PLAIN_GETTER(Import, visibility_clauses, "Get the visibility clauses of this Import node",
               IterAdapterBase*, return mkIterPair(node->visibilityClauses()))
CLASS_END(Import)

CLASS_BEGIN(Include)
  PLAIN_GETTER(Include, name, "Get the name of this Include node",
               chpl::UniqueString, return node->name())
  PLAIN_GETTER(Include, is_prototype, "Check if this Include node is for a prototype module",
               bool, return node->isPrototype())
  PLAIN_GETTER(Include, visibility, "Get the visibility of this Include node",
               const char*, return Decl::visibilityToString(node->visibility()))
CLASS_END(Include)

CLASS_BEGIN(Init)
  PLAIN_GETTER(Init, target, "Get the target of this Init node",
               const chpl::uast::AstNode*, return node->target())
CLASS_END(Init)

CLASS_BEGIN(Label)
  PLAIN_GETTER(Label, loop, "Get the loop this Label node is attached to",
               const chpl::uast::AstNode*, return node->loop())
  PLAIN_GETTER(Label, name, "Get the name of this Label node",
               chpl::UniqueString, return node->name())
CLASS_END(Label)

CLASS_BEGIN(New)
  PLAIN_GETTER(New, management, "Get the management style for this New node",
               const char*, return New::managementToString(node->management()))
  PLAIN_GETTER(New, type_expression, "Get the type expression for this New node",
               const chpl::uast::AstNode*, return node->typeExpression())
CLASS_END(New)

CLASS_BEGIN(Range)
  PLAIN_GETTER(Range, lower_bound, "Get the lower bound of this Range node",
               const chpl::uast::AstNode*, return node->lowerBound())
  PLAIN_GETTER(Range, op_kind, "Get the op kind of this Range node",
               const char*, return opKindToString(node->opKind()))
  PLAIN_GETTER(Range, upper_bound, "Get the upper bound of this Range node",
               const chpl::uast::AstNode*, return node->upperBound())
CLASS_END(Range)

CLASS_BEGIN(Require)
  PLAIN_GETTER(Require, exprs, "Get the expressions of this Require node",
               IterAdapterBase*, return mkIterPair(node->exprs()))
CLASS_END(Require)

CLASS_BEGIN(Return)
  PLAIN_GETTER(Return, value, "Get the expression returned by this Return node",
               const chpl::uast::AstNode*, return node->value())
CLASS_END(Return)

CLASS_BEGIN(Select)
  PLAIN_GETTER(Select, exprs, "Get the expression of this Select node",
               const chpl::uast::AstNode*, return node->expr())
  PLAIN_GETTER(Select, when_stmts, "Get the When statements of this Select node",
               IterAdapterBase*, return mkIterPair(node->whenStmts()))
CLASS_END(Select)

CLASS_BEGIN(Throw)
  PLAIN_GETTER(Throw, error_expression, "Get the expression thrown by this Throw node",
               const chpl::uast::AstNode*, return node->errorExpression())
CLASS_END(Throw)

CLASS_BEGIN(Try)
  PLAIN_GETTER(Try, body, "Get the body of this Try node",
               const chpl::uast::AstNode*, return node->body())
  PLAIN_GETTER(Try, handlers, "Get the Catch node handlers of this Try node",
               IterAdapterBase*, return mkIterPair(node->handlers()))
  PLAIN_GETTER(Try, is_expression_level, "Check if this Try node is expression level",
               bool, return node->isExpressionLevel())
  PLAIN_GETTER(Try, is_try_bang, "Check if this Try node is a 'try!'",
               bool, return node->isTryBang())
CLASS_END(Try)

CLASS_BEGIN(Use)
  PLAIN_GETTER(Use, visibility, "Get the visibility of this Use node",
               const char*, return Decl::visibilityToString(node->visibility()))
  PLAIN_GETTER(Use, visibility_clauses, "Get the visibility clauses of this Use node",
               IterAdapterBase*, return mkIterPair(node->visibilityClauses()))
CLASS_END(Use)

CLASS_BEGIN(VisibilityClause)
  PLAIN_GETTER(VisibilityClause, symbol, "Get the symbol referenced by this VisibilityClause node",
               const chpl::uast::AstNode*, return node->symbol())
CLASS_END(VisibilityClause)

CLASS_BEGIN(WithClause)
  PLAIN_GETTER(WithClause, exprs, "Get the expressions of this WithClause node",
               IterAdapterBase*, return mkIterPair(node->exprs()))
CLASS_END(WithClause)

CLASS_BEGIN(Yield)
  PLAIN_GETTER(Yield, value, "Get the expression yielded by this Yield node",
               const chpl::uast::AstNode*, return node->value())
CLASS_END(Yield)

CLASS_BEGIN(SimpleBlockLike)
  PLAIN_GETTER(SimpleBlockLike, block_style, "Get the block style of this SimpleBlockLike node",
               const char*, return blockStyleToString(node->blockStyle()))
CLASS_END(SimpleBlockLike)

CLASS_BEGIN(Begin)
  PLAIN_GETTER(Begin, with_clause, "Get the WithClause of this Begin node",
               const chpl::uast::AstNode*, return node->withClause())
CLASS_END(Begin)

CLASS_BEGIN(Local)
  PLAIN_GETTER(Local, condition, "Get the condition of this Local node",
               const chpl::uast::AstNode*, return node->condition())
CLASS_END(Local)

CLASS_BEGIN(Manage)
  PLAIN_GETTER(Manage, managers, "Get the managers of this Manage node",
               IterAdapterBase*, return mkIterPair(node->managers()))
CLASS_END(Manage)

CLASS_BEGIN(On)
  PLAIN_GETTER(On, destination, "Get the destination of this On node",
               const chpl::uast::AstNode*, return node->destination())
CLASS_END(On)

CLASS_BEGIN(Serial)
  PLAIN_GETTER(Serial, condition, "Get the condition of this Serial node",
               const chpl::uast::AstNode*, return node->condition())
CLASS_END(Serial)

CLASS_BEGIN(When)
  PLAIN_GETTER(When, block_style, "Get the block style of this When node",
               const char*, return blockStyleToString(node->blockStyle()))
  PLAIN_GETTER(When, body, "Get the body of this When node",
               const chpl::uast::AstNode*, return node->body())
  PLAIN_GETTER(When, case_exprs, "Get the case expressions of this When node",
               IterAdapterBase*, return mkIterPair(node->caseExprs()))
  PLAIN_GETTER(When, is_otherwise, "Check if this When node uses the otherwise keyword",
               bool, return node->isOtherwise())
CLASS_END(When)

CLASS_BEGIN(Loop)
  PLAIN_GETTER(Loop, block_style, "Get the block style of this Loop node",
               const char*, return blockStyleToString(node->blockStyle()))
  PLAIN_GETTER(Loop, body, "Get the body of this Loop node",
               const chpl::uast::AstNode*, return node->body())
CLASS_END(Loop)

CLASS_BEGIN(DoWhile)
  PLAIN_GETTER(DoWhile, condition, "Get the condition of this DoWhole node",
               const chpl::uast::AstNode*, return node->condition())
CLASS_END(DoWhile)

CLASS_BEGIN(While)
  PLAIN_GETTER(While, condition, "Get the condition of this While node",
               const chpl::uast::AstNode*, return node->condition())
CLASS_END(While)

CLASS_BEGIN(IndexableLoop)
  PLAIN_GETTER(IndexableLoop, index, "Get the index of this IndexableLoop node",
               const chpl::uast::AstNode*, return node->index())
  PLAIN_GETTER(IndexableLoop, is_expression_level, "Check if this IndexableLoop node is expression level",
               bool, return node->isExpressionLevel())
  PLAIN_GETTER(IndexableLoop, iterand, "Get the iterand of this IndexableLoop node",
               const chpl::uast::AstNode*, return node->iterand())
  PLAIN_GETTER(IndexableLoop, with_clause, "Get the WithClause of this IndexableLoop node",
               const chpl::uast::AstNode*, return node->withClause())
CLASS_END(IndexableLoop)

CLASS_BEGIN(BracketLoop)
  PLAIN_GETTER(BracketLoop, is_maybe_array_type, "Check if this BracketLoop node may actually be a type",
               bool, return node->isMaybeArrayType())
CLASS_END(BracketLoop)

CLASS_BEGIN(For)
  PLAIN_GETTER(For, is_param, "Check if this For node is a param",
               bool, return node->isParam())
CLASS_END(For)

CLASS_BEGIN(BoolLiteral)
  PLAIN_GETTER(BoolLiteral, value, "Get the value of this BoolLiteral node",
               const char*, return (node->value() ? "true" : "false"))
CLASS_END(BoolLiteral)

CLASS_BEGIN(ImagLiteral)
  PLAIN_GETTER(ImagLiteral, text, "Get the value of this ImagLiteral node",
               chpl::UniqueString, return node->text())
CLASS_END(ImagLiteral)

CLASS_BEGIN(IntLiteral)
  PLAIN_GETTER(IntLiteral, text, "Get the value of this IntLiteral node",
               chpl::UniqueString, return node->text())
CLASS_END(IntLiteral)

CLASS_BEGIN(RealLiteral)
  PLAIN_GETTER(RealLiteral, text, "Get the value of this RealLiteral node",
               chpl::UniqueString, return node->text())
CLASS_END(RealLiteral)

CLASS_BEGIN(UintLiteral)
  PLAIN_GETTER(UintLiteral, text, "Get the value of this UintLiteral node",
               chpl::UniqueString, return node->text())
CLASS_END(UintLiteral)

CLASS_BEGIN(StringLikeLiteral)
  PLAIN_GETTER(StringLikeLiteral, value, "Get the value of this StringLikeLiteral node",
               chpl::UniqueString, return node->value())
CLASS_END(StringLikeLiteral)

CLASS_BEGIN(Call)
  PLAIN_GETTER(Call, actuals, "Get the arguments to this Call node",
               IterAdapterBase*, return mkIterPair(node->actuals()))
  PLAIN_GETTER(Call, called_expression, "Get the expression invoked by this Call node",
               const chpl::uast::AstNode*, return node->calledExpression())
  PLAIN_GETTER(Call, formal_actual_mapping, "Get the index of the function's formal for each of the call's actuals.",
               std::vector<int>, return actualOrderForNode(context, node))
CLASS_END(Call)

CLASS_BEGIN(FnCall)
  METHOD_PROTOTYPE(FnCall, actuals, "Get the actuals of this FnCall node")
  PLAIN_GETTER(FnCall, used_square_brackets, "Check if this FnCall was made using square brackets",
               bool, return node->callUsedSquareBrackets())
CLASS_END(FnCall)

CLASS_BEGIN(OpCall)
  PLAIN_GETTER(OpCall, is_binary_op, "Check if this OpCall node is a binary op",
               bool, return node->isBinaryOp())
  PLAIN_GETTER(OpCall, is_unary_op, "Check if this OpCall node is an unary op",
               bool, return node->isUnaryOp())
  PLAIN_GETTER(OpCall, op, "Get the op string for this OpCall node",
               chpl::UniqueString, return node->op())
CLASS_END(OpCall)

CLASS_BEGIN(PrimCall)
  PLAIN_GETTER(PrimCall, prim, "Get the primitive name for this PrimCall node",
               const char*, return chpl::uast::primtags::primTagToName(node->prim()))
CLASS_END(PrimCall)

CLASS_BEGIN(Reduce)
  PLAIN_GETTER(Reduce, iterand, "Get the iterand for this Reduce node",
               const chpl::uast::AstNode*, return node->iterand())
  PLAIN_GETTER(Reduce, op, "Get the op for this Reduce node",
               const chpl::uast::AstNode*, return node->op())
CLASS_END(Reduce)

CLASS_BEGIN(Scan)
  PLAIN_GETTER(Scan, iterand, "Get the iterand for this Scan node",
               const chpl::uast::AstNode*, return node->iterand())
  PLAIN_GETTER(Scan, op, "Get the op for this Scan node",
               const chpl::uast::AstNode*, return node->op())
CLASS_END(Scan)

CLASS_BEGIN(Decl)
  PLAIN_GETTER(Decl, linkage, "Get the linkage of this Decl node",
               const char*, return Decl::linkageToString(node->linkage()))
  PLAIN_GETTER(Decl, linkage_name, "Get the linkage name of this Decl node",
               const chpl::uast::AstNode*, return node->linkageName())
  PLAIN_GETTER(Decl, visibility, "Get the visibility of this Decl node",
               const char*, return Decl::visibilityToString(node->visibility()))
CLASS_END(Decl)

CLASS_BEGIN(TupleDecl)
  PLAIN_GETTER(TupleDecl, decls, "Get the declarations for this TupleDecl node",
               IterAdapterBase*, return mkIterPair(node->decls()))
  PLAIN_GETTER(TupleDecl, init_expression, "Get the init expression of this TupleDecl node",
               const chpl::uast::AstNode*, return node->typeExpression())
  PLAIN_GETTER(TupleDecl, intent_or_kind, "Get the intent or kind of this TupleDecl node",
               const char*, return TupleDecl::intentOrKindToString(node->intentOrKind()))
  PLAIN_GETTER(TupleDecl, type_expression, "Get the type expression of this TupleDecl node",
               const chpl::uast::AstNode*, return node->initExpression())
CLASS_END(TupleDecl)

CLASS_BEGIN(ForwardingDecl)
  PLAIN_GETTER(ForwardingDecl, expr, "Get the expression of this ForwardingDecl node",
               const chpl::uast::AstNode*, return node->expr())
CLASS_END(ForwardingDecl)

CLASS_BEGIN(NamedDecl)
  PLAIN_GETTER(NamedDecl, name, "Get the name of this NamedDecl node",
               chpl::UniqueString, return node->name())
  PLAIN_GETTER(NamedDecl, name_location, "Get the textual location of the NamedDecl node's name",
               chpl::Location, return chpl::parsing::locateDeclNameWithAst(context, node))
CLASS_END(NamedDecl)

CLASS_BEGIN(EnumElement)
  PLAIN_GETTER(EnumElement, init_expression, "Get the init expression of this EnumElement node",
               const chpl::uast::AstNode*, return node->initExpression())
CLASS_END(EnumElement)

CLASS_BEGIN(Function)
  PLAIN_GETTER(Function, formals, "Get the formals for this Function node",
               IterAdapterBase*, return mkIterPair(node->formals()))
  PLAIN_GETTER(Function, body, "Get the body for this function",
               const chpl::uast::AstNode*, return node->body())
  METHOD(Function, formal, "Get the n'th Formal of this Function node",
         const chpl::uast::AstNode*(int), return node->formal(std::get<0>(args)))
  PLAIN_GETTER(Function, is_anonymous, "Check if this Function node is anonymous",
               bool, return node->isAnonymous())
  PLAIN_GETTER(Function, is_inline, "Check if this Function node is marked inline",
               bool, return node->isInline())
  PLAIN_GETTER(Function, is_method, "Check if this Function node is a method",
               bool, return node->isMethod())
  PLAIN_GETTER(Function, is_override, "Check if this Function node is an override",
               bool, return node->isOverride())
  PLAIN_GETTER(Function, is_parenless, "Check if this Function node is parenless",
               bool, return node->isParenless())
  PLAIN_GETTER(Function, is_primary_method, "Check if this Function node is a primary method",
               bool, return node->isPrimaryMethod())
  PLAIN_GETTER(Function, kind, "Get the kind of this Function node",
               const char*, return Function::kindToString(node->kind()))
  PLAIN_GETTER(Function, lifetime_clauses, "Get the lifetime clauses for this Function node",
               IterAdapterBase*, return mkIterPair(node->lifetimeClauses()))
  PLAIN_GETTER(Function, num_formals, "Get the number of formals for this Function node",
               int, return node->numFormals())
  PLAIN_GETTER(Function, return_intent, "Get the return intent of this Function node",
               const char*,  return intentToString(node->returnIntent()))
  PLAIN_GETTER(Function, return_type, "Get the return type for this Function node",
               const chpl::uast::AstNode*, return node->returnType())
  PLAIN_GETTER(Function, this_formal, "Get the this formal for this Function node",
               const chpl::uast::AstNode*, return node->thisFormal())
  PLAIN_GETTER(Function, throws, "Check if this Function node is marked throws",
               bool, return node->throws())
  PLAIN_GETTER(Function, where_clause, "Get the where clause for this Function node",
               const chpl::uast::AstNode*, return node->whereClause())
CLASS_END(Function)

CLASS_BEGIN(Interface)
  PLAIN_GETTER(Interface, stmts, "Get the statements for this Interface node",
               IterAdapterBase*, return mkIterPair(node->stmts()))
  PLAIN_GETTER(Interface, formals, "Get the formals for this Interface node",
               IterAdapterBase*, return mkIterPair(node->formals()))
  PLAIN_GETTER(Interface, is_formal_list_explicit, "Check if this Interface node has an explicit formal list",
               bool, return node->isFormalListExplicit())
CLASS_END(Interface)

CLASS_BEGIN(Module)
  PLAIN_GETTER(Module, kind, "Get the kind of this Module node",
               const char*, return Module::moduleKindToString(node->kind()))
CLASS_END(Module)

CLASS_BEGIN(ReduceIntent)
  PLAIN_GETTER(ReduceIntent, op, "Get the op for this ReduceIntent node",
               const chpl::uast::AstNode*, return node->op())
CLASS_END(ReduceIntent)

CLASS_BEGIN(VarLikeDecl)
  PLAIN_GETTER(VarLikeDecl, init_expression, "Get the init expression of this VarLikeDecl node",
               const chpl::uast::AstNode*, return node->initExpression())
  PLAIN_GETTER(VarLikeDecl, storage_kind, "Get the storage kind of this VarLikeDecl node",
               const char*, return qualifierToString(node->storageKind()))
  PLAIN_GETTER(VarLikeDecl, type_expression, "Get the type expression of this VarLikeDecl node",
               const chpl::uast::AstNode*, return node->typeExpression())
  PLAIN_GETTER(VarLikeDecl, intent, "Get the intent for this VarLikeDecl node",
               const char*, return intentToString(node->storageKind()))
CLASS_END(VarLikeDecl)

CLASS_BEGIN(VarArgFormal)
  PLAIN_GETTER(VarArgFormal, count, "Get the count expression of this VarArgFormal node",
               const chpl::uast::AstNode*, return node->count())
CLASS_END(VarArgFormal)

CLASS_BEGIN(Variable)
  PLAIN_GETTER(Variable, is_config, "Check if this Variable node is a config variable",
               bool, return node->isConfig())
  PLAIN_GETTER(Variable, is_field, "Check if this Variable node is a class field variable",
               bool, return node->isField())
  PLAIN_GETTER(Variable, kind, "Get the kind of this Variable node",
               const char*, return qualifierToString(node->storageKind()))
CLASS_END(Variable)

CLASS_BEGIN(AggregateDecl)
  PLAIN_GETTER(AggregateDecl, decls_or_comments, "Get the declarations and comments of this AggregateDecl node",
               IterAdapterBase*, return mkIterPair(node->declOrComments()))
  PLAIN_GETTER(AggregateDecl, inherit_exprs, "Get the inherit expressions of this AggregateDecl node",
               IterAdapterBase*, return mkIterPair(node->inheritExprs()))
CLASS_END(AggregateDecl)

CLASS_BEGIN(BoolParam)
  PLAIN_GETTER(BoolParam, value, "Get the value of this boolean Param",
               bool, return node->value())
CLASS_END(BoolParam)

CLASS_BEGIN(EnumParam)
  PLAIN_GETTER(EnumParam, value, "Get the value of this enum Param",
               const chpl::uast::AstNode*, return parsing::idToAst(context, node->value()))
CLASS_END(EnumParam)

CLASS_BEGIN(IntParam)
  PLAIN_GETTER(IntParam, value, "Get the value of this integer Param",
               int, return node->value())
CLASS_END(IntParam)

CLASS_BEGIN(StringParam)
  PLAIN_GETTER(StringParam, value, "Get the value of this string Param",
               chpl::UniqueString, return node->value())
CLASS_END(StringParam)

CLASS_BEGIN(CompositeType)
  PLAIN_GETTER(CompositeType, decl, "Get the chpl::uast::AstNode that declares this CompositeType",
               const chpl::uast::AstNode*, return parsing::idToAst(context, node->id()))
CLASS_END(CompositeType)

CLASS_BEGIN(Context)
  PLAIN_GETTER(Context, introspect_parsed_files, "Inspect the list of files that have been parsed by the Context",
               std::vector<chpl::UniqueString>, return parsing::introspectParsedFiles(context))

  METHOD(Context, parse, "Parse a top-level AST node from the given file",
         std::vector<const chpl::uast::AstNode*>(chpl::UniqueString),

         auto fileNameUS = std::get<0>(args);
         auto parentPathUS = chpl::UniqueString();
         auto& builderResult =
           parsing::parseFileToBuilderResultAndCheck(context, fileNameUS, parentPathUS);
         std::vector<const chpl::uast::AstNode*> topLevelNodes;
         for (auto i = 0; i < builderResult.numTopLevelExpressions(); i++) {
           topLevelNodes.push_back(builderResult.topLevelExpression(i));
         }
         return topLevelNodes)
  METHOD(Context, set_module_paths, "Set the module path arguments to the given lists of module paths and filenames",
         void(std::vector<std::string>, std::vector<std::string>),

         auto& paths = std::get<0>(args);
         auto& filenames = std::get<1>(args);
         parsing::setupModuleSearchPaths(context, false, false, paths, filenames))
  METHOD(Context, is_bundled_path, "Check if the given file path is within the bundled (built-in) Chapel files",
         bool(chpl::UniqueString),

         auto pathUS = std::get<0>(args);
         return parsing::filePathIsInInternalModule(context, pathUS) ||
                parsing::filePathIsInStandardModule(context, pathUS) ||
                parsing::filePathIsInBundledModule(context, pathUS))
  METHOD(Context, advance_to_next_revision, "Advance the context to the next revision",
         void(bool),

         auto prepareToGc = std::get<0>(args);
         context->advanceToNextRevision(prepareToGc))

  METHOD_PROTOTYPE(Context, _get_pyi_file, "Generate a stub file for the Chapel AST nodes")
  METHOD_PROTOTYPE(Context, track_errors, "Return a context manager that tracks errors emitted by this Context")
CLASS_END(Context)

//
// Cleanup and undefine all macros
//
#undef CLASS_BEGIN
#undef CLASS_END
#undef METHOD
#undef PLAIN_GETTER
#undef METHOD_PROTOTYPE
