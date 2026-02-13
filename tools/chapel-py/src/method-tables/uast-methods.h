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

// The order here should be kept in sync with the order in the uast list.
// Not all nodes need methods exposed to python, so not all uast nodes are listed here.
// See list in frontend/include/chpl/uast/uast-classes-list.h.

//
// Inside each method table, methods should be listed in alphabetical order
//

CLASS_BEGIN(AstNode)
  ITER_PROTOTYPE(AstNode, const chpl::uast::AstNode*)
  PLAIN_GETTER(AstNode, dump, "Dump the internal representation of the given AST node",
               void, node->dump())
  PLAIN_GETTER(AstNode, tag, "Get a string representation of the AST node's type",
               const char*, return chpl::uast::asttags::tagToString(node->tag()))
  PLAIN_GETTER(AstNode, attribute_group, "Get the attribute group, if any, associated with this node",
               Nilable<const chpl::uast::AttributeGroup*>, return node->attributeGroup())
  PLAIN_GETTER(AstNode, location, "Get the location of this AST node in its file",
               chpl::Location, return chpl::parsing::locateAst(context, node))
  PLAIN_GETTER(AstNode, parent, "Get the parent node of this AST node",
               Nilable<const chpl::uast::AstNode*>, return chpl::parsing::parentAst(context, node))
  PLAIN_GETTER(AstNode, parent_symbol, "Get the parent symbol of this AST node (e.g., module, variable, etc.)",
               Nilable<const chpl::uast::AstNode*>,

               auto id = node->id();
               auto parentId = id.parentSymbolId(context);
               if (parentId.isEmpty()) return nullptr;
               return parsing::idToAst(context, parentId))
  PLAIN_GETTER(AstNode, pragmas, "Get the pragmas of this AST node",
               std::set<std::string>,

               std::set<std::string> toReturn;
               if (auto ag = node->attributeGroup()) {
                 for (auto p : ag->pragmas()) {
                   toReturn.insert(pragmatags::pragmaTagToName(p));
                 }
               }
               return toReturn)
  PLAIN_GETTER(AstNode, unique_id, "Get a unique identifier for this AST node",
               std::string,

               std::stringstream ss;
               node->id().stringify(ss, CHPL_SYNTAX);
               return ss.str())
  PLAIN_GETTER(AstNode, scope, "Get the scope for this AST node",
               std::optional<ScopeObject*>,

               return ScopeObject::tryCreate(contextObject, resolution::scopeForId(context, node->id())))
  PLAIN_GETTER(AstNode, creates_scope, "Returns true if this AST node creates a scope",
               bool, return chpl::resolution::createsScope(node->tag()))
  PLAIN_GETTER(AstNode, type, "Get the type of this AST node, as a 3-tuple of (kind, type, param).",
               std::optional<QualifiedTypeTuple>,

               auto qt = typeForNode(context, node);
               if (qt.isUnknown()) {
                 return {};
               }

               return std::make_tuple(intentToString(qt.kind()), qt.type(), qt.param()))
  PLAIN_GETTER(AstNode, called_fn, "Get the function being invoked by this node",
               Nilable<const chpl::uast::AstNode*>, return calledFnForNode(context, node))
  PLAIN_GETTER(AstNode, resolve, "Perform resolution on code surrounding this node to determine its type and other information.",
               std::optional<ResolvedExpressionObject*>,

               auto r = resolveResultsForNode(context, node);
               return ResolvedExpressionObject::tryCreate(contextObject, r))
  PLAIN_GETTER(AstNode, scope_resolve, "Perform scope resolution on code surrounding this node to retrieve its to-ID and collect errors.",
               std::optional<ResolvedExpressionObject*>,

               auto r = scopeResolveResultsForNode(context, node);
               return ResolvedExpressionObject::tryCreate(contextObject, r))
  METHOD(AstNode, resolve_via, "Use a given function's type information to determine the information of this node.",
         std::optional<ResolvedExpressionObject*>(TypedSignatureObject*),

         auto sigObj = std::get<0>(args);
         // TODO: Might need to do frame rewinding here.
         resolution::ResolutionContext rcval(context);
         auto resolvedFn = resolution::resolveFunction(&rcval, sigObj->value_.signature, sigObj->value_.poiScope);
         if (!resolvedFn) return {};

         auto r = resolvedFn->byAstOrNull(node);
         return ResolvedExpressionObject::tryCreate(contextObject, r))
  PLAIN_GETTER(AstNode, block_header, "Get the header Location of this block-like AstNode node",
               std::optional<chpl::Location>,
               auto loc = chpl::parsing::locateBlockHeaderWithAst(context, node);
               return getValidLocation(loc))
  PLAIN_GETTER(AstNode, curly_braces_location, "Get the Location of the curly braces of this AstNode node",
               std::optional<chpl::Location>,
               auto loc = chpl::parsing::locateCurlyBracesWithAst(context, node);
               return getValidLocation(loc))
  PLAIN_GETTER(AstNode, paren_location, "Get the Location of the parentheses of this AstNode node",
               std::optional<chpl::Location>,
               auto loc = chpl::parsing::locateExprParenWithAst(context, node);
               return getValidLocation(loc))
CLASS_END(AstNode)

CLASS_BEGIN(AnonFormal)
  PLAIN_GETTER(AnonFormal, intent, "Get the intent for this AnonFormal node",
               const char*, return intentToString(node->intent()))
  PLAIN_GETTER(AnonFormal, type_expression, "Get the type expression for this AnonFormal node",
               Nilable<const chpl::uast::AstNode*>, return node->typeExpression())
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
  METHOD(AttributeGroup, get_attribute_named, "Get the attribute with a particular name, if any",
         Nilable<const chpl::uast::AstNode*>(chpl::UniqueString),
         return node->getAttributeNamed(std::get<0>(args)))
  PLAIN_GETTER(AttributeGroup, is_unstable, "Check if this AttributeGroup contains the 'unstable' attribute",
               bool, return node->isUnstable())
  PLAIN_GETTER(AttributeGroup, is_deprecated, "Check if this AttributeGroup contains the 'deprecated' attribute",
               bool, return node->isDeprecated())
CLASS_END(AttributeGroup)

CLASS_BEGIN(Break)
  PLAIN_GETTER(Break, target, "Get the target from this Break node",
               Nilable<const chpl::uast::Identifier*>, return node->target())
CLASS_END(Break)

CLASS_BEGIN(Catch)
  PLAIN_GETTER(Catch, target, "Get the error from this Catch node",
               Nilable<const chpl::uast::Variable*>, return node->error())
  PLAIN_GETTER(Catch, body, "Get the body from this Catch node",
               const chpl::uast::Block*, return node->body())
  PLAIN_GETTER(Catch, has_parens_around_error, "Check if this Catch uses parentheses",
               bool, return node->hasParensAroundError())
CLASS_END(Catch)

CLASS_BEGIN(Cobegin)
  PLAIN_GETTER(Cobegin, with_clause, "Get the WithClause from this Cobegin node",
               Nilable<const chpl::uast::WithClause*>, return node->withClause())
  PLAIN_GETTER(Cobegin, task_bodies, "Get tasks from this Cobegin node",
               IterAdapterBase*, return mkIterPair(node->taskBodies()))
CLASS_END(Cobegin)

CLASS_BEGIN(Conditional)
  PLAIN_GETTER(Conditional, condition, "Get the condition of this Conditional node",
               const chpl::uast::AstNode*, return node->condition())
  PLAIN_GETTER(Conditional, then_block, "Get the then block of this Conditional node",
               const chpl::uast::Block*, return node->thenBlock())
  PLAIN_GETTER(Conditional, then_stmts, "Get the statements in the then block of this Conditional node",
               IterAdapterBase*, return mkIterPair(node->thenStmts()))
  PLAIN_GETTER(Conditional, num_then_stmts, "Get the number of statements in the then block of this Conditional node",
               int, return node->numThenStmts())
  METHOD(Conditional, then_stmt, "Get the i'th statement in the then block of this Conditional node",
         const chpl::uast::AstNode*(int), return node->thenStmt(std::get<0>(args)))
  PLAIN_GETTER(Conditional, then_block_style, "Get the block style of the then block of this Conditional node",
               const char*, return blockStyleToString(node->thenBlockStyle()))
  PLAIN_GETTER(Conditional, then_keyword_location, "Get the Location of the 'then' keyword of this Conditional node",
               std::optional<chpl::Location>,
               auto loc = chpl::parsing::locateThenKeywordWithAst(context, node);
               return getValidLocation(loc))
  PLAIN_GETTER(Conditional, has_else_block, "Check if this Conditional node has an else block",
               bool, return node->hasElseBlock())
  PLAIN_GETTER(Conditional, else_block, "Get the else block of this Conditional node or None if no else block",
               Nilable<const chpl::uast::Block*>, return node->elseBlock())
  PLAIN_GETTER(Conditional, else_stmts, "Get the statements in the else block of this Conditional node",
               IterAdapterBase*, return mkIterPair(node->elseStmts()))
  PLAIN_GETTER(Conditional, num_else_stmts, "Get the number of statements in the else block of this Conditional node",
               int, return node->numElseStmts())
  METHOD(Conditional, else_stmt, "Get the i'th statement in the else block of this Conditional node",
         const chpl::uast::AstNode*(int), return node->elseStmt(std::get<0>(args)))
  PLAIN_GETTER(Conditional, else_block_style, "Get the block style of the else block of this Conditional node",
               const char*, return blockStyleToString(node->elseBlockStyle()))
  PLAIN_GETTER(Conditional, else_keyword_location, "Get the Location of the 'else' keyword of this Conditional node",
               std::optional<chpl::Location>,
               auto loc = chpl::parsing::locateElseKeywordWithAst(context, node);
               return getValidLocation(loc))
  PLAIN_GETTER(Conditional, is_expression_level, "Checks if this Conditional node is expression-level",
               bool, return node->isExpressionLevel())
CLASS_END(Conditional)

CLASS_BEGIN(Comment)
  PLAIN_GETTER(Comment, text, "Get the text from this Comment node",
               const char*, return node->c_str())
CLASS_END(Comment)

CLASS_BEGIN(Continue)
  PLAIN_GETTER(Continue, target, "Get the target from this Continue node",
               Nilable<const chpl::uast::Identifier*>, return node->target())
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
               Nilable<const chpl::uast::AstNode*>, return nodeOrNullFromToId(context, node))
CLASS_END(Dot)

CLASS_BEGIN(ExternBlock)
  PLAIN_GETTER(ExternBlock, code, "Get C code of this ExternBlock node",
               std::string, return node->code())
CLASS_END(ExternBlock)

CLASS_BEGIN(FunctionSignature)
  PLAIN_GETTER(FunctionSignature, formals, "Get the formals for this FunctionSignature node",
               TypedIterAdapterBase<const chpl::uast::Decl*>*, return mkIterPair(node->formals()))
  PLAIN_GETTER(FunctionSignature, is_parenless, "Check if this FunctionSignature node is parenless",
               bool, return node->isParenless())
  PLAIN_GETTER(FunctionSignature, kind, "Get the kind of this FunctionSignature node",
               const char*, return Function::kindToString(node->kind()))
  PLAIN_GETTER(FunctionSignature, return_intent, "Get the return intent of this FunctionSignature node",
               const char*,  return intentToString(node->returnIntent()))
  PLAIN_GETTER(FunctionSignature, return_type, "Get the return type for this FunctionSignature node",
               Nilable<const chpl::uast::AstNode*>, return node->returnType())
  PLAIN_GETTER(FunctionSignature, this_formal, "Get the this formal for this FunctionSignature node",
               Nilable<const chpl::uast::Formal*>, return node->thisFormal())
  PLAIN_GETTER(FunctionSignature, throws, "Check if this FunctionSignature node is marked throws",
               bool, return node->throws())
CLASS_END(FunctionSignature)

CLASS_BEGIN(Implements)
  PLAIN_GETTER(Implements, interface_name, "Get the interface name of this Implements node",
               chpl::UniqueString, return node->interfaceName())
  PLAIN_GETTER(Implements, type_ident, "Get the type identifier from this Implements node",
               Nilable<const chpl::uast::Identifier*>, return node->typeIdent())
  PLAIN_GETTER(Implements, interface_expr, "Get the interface expression from this Implements node",
               const chpl::uast::AstNode*, return node->interfaceExpr())
  PLAIN_GETTER(Implements, is_expression_level, "Check if this Implements node is expression level",
               bool, return node->isExpressionLevel())
CLASS_END(Implements)

CLASS_BEGIN(Identifier)
  PLAIN_GETTER(Identifier, name, "Get the name of this Identifier node",
               chpl::UniqueString, return node->name())
  PLAIN_GETTER(Identifier, to_node, "Get the AST node that this Identifier node refers to",
               Nilable<const chpl::uast::AstNode*>, return nodeOrNullFromToId(context, node))
CLASS_END(Identifier)

CLASS_BEGIN(Import)
  PLAIN_GETTER(Import, visibility, "Get the visibility of this Import node",
               const char*, return Decl::visibilityToString(node->visibility()))
  PLAIN_GETTER(Import, visibility_clauses, "Get the visibility clauses of this Import node",
               TypedIterAdapterBase<const chpl::uast::VisibilityClause*>*, return mkIterPair(node->visibilityClauses()))
CLASS_END(Import)

CLASS_BEGIN(Include)
  PLAIN_GETTER(Include, name, "Get the name of this Include node",
               chpl::UniqueString, return node->name())
  PLAIN_GETTER(Include, is_prototype, "Check if this Include node is for a prototype module",
               bool, return node->isPrototype())
  PLAIN_GETTER(Include, visibility, "Get the visibility of this Include node",
               const char*, return Decl::visibilityToString(node->visibility()))
  PLAIN_GETTER(Include, to_node, "Get the AST node that this Include node refers to",
               Nilable<const chpl::uast::AstNode*>, return nodeOrNullFromToId(context, node))
  PLAIN_GETTER(Include, name_location, "Get the location of the name in this Include node",
               std::optional<chpl::Location>,
               auto loc = chpl::parsing::locateIncludeNameWithAst(context, node);
               return getValidLocation(loc))
CLASS_END(Include)

CLASS_BEGIN(Init)
  PLAIN_GETTER(Init, target, "Get the target of this Init node",
               const chpl::uast::Identifier*, return node->target())
CLASS_END(Init)

CLASS_BEGIN(Label)
  PLAIN_GETTER(Label, loop, "Get the loop this Label node is attached to",
               const chpl::uast::Loop*, return node->loop())
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
               Nilable<const chpl::uast::AstNode*>, return node->lowerBound())
  PLAIN_GETTER(Range, op_kind, "Get the op kind of this Range node",
               const char*, return opKindToString(node->opKind()))
  PLAIN_GETTER(Range, upper_bound, "Get the upper bound of this Range node",
               Nilable<const chpl::uast::AstNode*>, return node->upperBound())
CLASS_END(Range)

CLASS_BEGIN(Require)
  PLAIN_GETTER(Require, exprs, "Get the expressions of this Require node",
               IterAdapterBase*, return mkIterPair(node->exprs()))
CLASS_END(Require)

CLASS_BEGIN(Return)
  PLAIN_GETTER(Return, value, "Get the expression returned by this Return node",
               Nilable<const chpl::uast::AstNode*>, return node->value())
CLASS_END(Return)

CLASS_BEGIN(Select)
  PLAIN_GETTER(Select, exprs, "Get the expression of this Select node",
               const chpl::uast::AstNode*, return node->expr())
  PLAIN_GETTER(Select, when_stmts, "Get the When statements of this Select node",
               TypedIterAdapterBase<const chpl::uast::When*>*, return mkIterPair(node->whenStmts()))
CLASS_END(Select)

CLASS_BEGIN(Throw)
  PLAIN_GETTER(Throw, error_expression, "Get the expression thrown by this Throw node",
               const chpl::uast::AstNode*, return node->errorExpression())
CLASS_END(Throw)

CLASS_BEGIN(Try)
  PLAIN_GETTER(Try, body, "Get the body of this Try node",
               Nilable<const chpl::uast::Block*>, return node->body())
  PLAIN_GETTER(Try, handlers, "Get the Catch node handlers of this Try node",
               TypedIterAdapterBase<const chpl::uast::Catch*>*, return mkIterPair(node->handlers()))
  PLAIN_GETTER(Try, is_expression_level, "Check if this Try node is expression level",
               bool, return node->isExpressionLevel())
  PLAIN_GETTER(Try, is_try_bang, "Check if this Try node is a 'try!'",
               bool, return node->isTryBang())
CLASS_END(Try)

CLASS_BEGIN(Use)
  PLAIN_GETTER(Use, visibility, "Get the visibility of this Use node",
               const char*, return Decl::visibilityToString(node->visibility()))
  PLAIN_GETTER(Use, visibility_clauses, "Get the visibility clauses of this Use node",
               TypedIterAdapterBase<const chpl::uast::VisibilityClause*>*, return mkIterPair(node->visibilityClauses()))
CLASS_END(Use)

CLASS_BEGIN(VisibilityClause)
  PLAIN_GETTER(VisibilityClause, symbol, "Get the symbol referenced by this VisibilityClause node",
               const chpl::uast::AstNode*, return node->symbol())
  PLAIN_GETTER(VisibilityClause, limitation_kind, "Get the limitation kind of this VisibilityClause node",
               const char*, return VisibilityClause::limitationKindToString(node->limitationKind()))
  PLAIN_GETTER(VisibilityClause, limitations, "Get the limitations of this VisibilityClause node",
               IterAdapterBase*, return mkIterPair(node->limitations()))
CLASS_END(VisibilityClause)

CLASS_BEGIN(WithClause)
  PLAIN_GETTER(WithClause, exprs, "Get the expressions of this WithClause node",
               IterAdapterBase*, return mkIterPair(node->exprs()))
CLASS_END(WithClause)

CLASS_BEGIN(Yield)
  PLAIN_GETTER(Yield, value, "Get the expression yielded by this Yield node",
               Nilable<const chpl::uast::AstNode*>, return node->value())
CLASS_END(Yield)

CLASS_BEGIN(SimpleBlockLike)
  PLAIN_GETTER(SimpleBlockLike, stmts, "Get the statements contained in this SimpleBlockLike.",
               IterAdapterBase*, return mkIterPair(node->stmts()))
  PLAIN_GETTER(SimpleBlockLike, num_stmts, "Get the number of statements contained in this SimpleBlockLike.",
               int, return node->numStmts())
  METHOD(SimpleBlockLike, stmt, "Get the i'th statement contained in this SimpleBlockLike.",
         const chpl::uast::AstNode*(int), return node->stmt(std::get<0>(args)))
  PLAIN_GETTER(SimpleBlockLike, block_style, "Get the block style of this SimpleBlockLike node",
               const char*, return blockStyleToString(node->blockStyle()))
CLASS_END(SimpleBlockLike)

CLASS_BEGIN(Begin)
  PLAIN_GETTER(Begin, with_clause, "Get the WithClause of this Begin node",
               Nilable<const chpl::uast::WithClause*>, return node->withClause())
CLASS_END(Begin)

CLASS_BEGIN(Local)
  PLAIN_GETTER(Local, condition, "Get the condition of this Local node",
               Nilable<const chpl::uast::AstNode*>, return node->condition())
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
               Nilable<const chpl::uast::AstNode*>, return node->condition())
CLASS_END(Serial)

CLASS_BEGIN(When)
  PLAIN_GETTER(When, block_style, "Get the block style of this When node",
               const char*, return blockStyleToString(node->blockStyle()))
  PLAIN_GETTER(When, body, "Get the body of this When node",
               const chpl::uast::Block*, return node->body())
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
  PLAIN_GETTER(Loop, header_location, "Get the Location of this Loop node's header",
               std::optional<chpl::Location>,
               auto loc = chpl::parsing::locateLoopHeaderWithAst(context, node);
               return getValidLocation(loc))
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
               Nilable<const chpl::uast::Decl*>, return node->index())
  PLAIN_GETTER(IndexableLoop, is_expression_level, "Check if this IndexableLoop node is expression level",
               bool, return node->isExpressionLevel())
  PLAIN_GETTER(IndexableLoop, iterand, "Get the iterand of this IndexableLoop node",
               const chpl::uast::AstNode*, return node->iterand())
  PLAIN_GETTER(IndexableLoop, with_clause, "Get the WithClause of this IndexableLoop node",
               Nilable<const chpl::uast::WithClause*>, return node->withClause())
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
               bool, return node->value())
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
  PLAIN_GETTER(Call, num_actuals, "Get the number of actuals for this Call node",
               int, return node->numActuals())
  METHOD(Call, actual, "Get the n'th actual of this Call node",
         const chpl::uast::AstNode*(int),
         return node->actual(std::get<0>(args)))
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
               Nilable<const chpl::uast::AstNode*>, return node->linkageName())
  PLAIN_GETTER(Decl, visibility, "Get the visibility of this Decl node",
               const char*, return Decl::visibilityToString(node->visibility()))
CLASS_END(Decl)

CLASS_BEGIN(TupleDecl)
  PLAIN_GETTER(TupleDecl, decls, "Get the declarations for this TupleDecl node",
               TypedIterAdapterBase<const chpl::uast::Decl*>*, return mkIterPair(node->decls()))
  PLAIN_GETTER(TupleDecl, init_expression, "Get the init expression of this TupleDecl node",
               Nilable<const chpl::uast::AstNode*>, return node->typeExpression())
  PLAIN_GETTER(TupleDecl, intent_or_kind, "Get the intent or kind of this TupleDecl node",
               const char*, return TupleDecl::intentOrKindToString(node->intentOrKind()))
  PLAIN_GETTER(TupleDecl, type_expression, "Get the type expression of this TupleDecl node",
               Nilable<const chpl::uast::AstNode*>, return node->initExpression())
CLASS_END(TupleDecl)

CLASS_BEGIN(ForwardingDecl)
  PLAIN_GETTER(ForwardingDecl, expr, "Get the expression of this ForwardingDecl node",
               Nilable<const chpl::uast::AstNode*>, return node->expr())
CLASS_END(ForwardingDecl)

CLASS_BEGIN(NamedDecl)
  PLAIN_GETTER(NamedDecl, name, "Get the name of this NamedDecl node",
               chpl::UniqueString, return node->name())
  PLAIN_GETTER(NamedDecl, name_location, "Get the textual location of the NamedDecl node's name",
               chpl::Location,
               auto loc = getValidLocation(chpl::parsing::locateDeclNameWithAst(context, node));
               return loc ? *loc : chpl::parsing::locateAst(context, node))
  PLAIN_GETTER(NamedDecl, header_location, "Get the Location of this NamedDecl node's header",
               std::optional<chpl::Location>,
               auto loc = chpl::parsing::locateDeclHeaderWithAst(context, node);
               return getValidLocation(loc))
CLASS_END(NamedDecl)

CLASS_BEGIN(EnumElement)
  PLAIN_GETTER(EnumElement, init_expression, "Get the init expression of this EnumElement node",
               Nilable<const chpl::uast::AstNode*>, return node->initExpression())
  PLAIN_GETTER(EnumElement, assigned_value, "Get the 'param' value assigned to this EnumElement node, either directly or via sequence increment",
               Nilable<const chpl::types::Param*>,
               auto parentId = node->id().parentSymbolId(context);
               auto& numericValues = chpl::resolution::computeNumericValuesOfEnumElements(context, parentId);

               if (auto it = numericValues.find(node->id());
                   it != numericValues.end()) {
                 return it->second.param();
               }

               return nullptr;
               )
CLASS_END(EnumElement)

CLASS_BEGIN(Function)
  PLAIN_GETTER(Function, formals, "Get the formals for this Function node",
               TypedIterAdapterBase<const chpl::uast::Decl*>*, return mkIterPair(node->formals()))
  PLAIN_GETTER(Function, body, "Get the body for this function",
               Nilable<const chpl::uast::Block*>, return node->body())
  METHOD(Function, formal, "Get the n'th Formal of this Function node",
         const chpl::uast::Decl*(int), return node->formal(std::get<0>(args)))
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
               Nilable<const chpl::uast::AstNode*>, return node->returnType())
  PLAIN_GETTER(Function, this_formal, "Get the this formal for this Function node",
               Nilable<const chpl::uast::Decl*>, return node->thisFormal())
  PLAIN_GETTER(Function, throws, "Check if this Function node is marked throws",
               bool, return node->throws())
  PLAIN_GETTER(Function, where_clause, "Get the where clause for this Function node",
               Nilable<const chpl::uast::AstNode*>, return node->whereClause())
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
  PLAIN_GETTER(Module, find_test_functions,
               "Get all of the UnitTest functions in this Module",
               std::vector<const chpl::uast::Function*>,
               return findTestFunctionsForModule(context, node))
  PLAIN_GETTER(Module, find_unittest_main,
               "Find the call to UnitTest.main in this module, if any",
               Nilable<const chpl::uast::FnCall*>,
               return findUnitTestMainForModule(context, node))
CLASS_END(Module)

CLASS_BEGIN(ReduceIntent)
  PLAIN_GETTER(ReduceIntent, op, "Get the op for this ReduceIntent node",
               const chpl::uast::AstNode*, return node->op())
CLASS_END(ReduceIntent)

CLASS_BEGIN(VarLikeDecl)
  PLAIN_GETTER(VarLikeDecl, init_expression, "Get the init expression of this VarLikeDecl node",
               Nilable<const chpl::uast::AstNode*>, return node->initExpression())
  PLAIN_GETTER(VarLikeDecl, storage_kind, "Get the storage kind of this VarLikeDecl node",
               const char*, return qualifierToString(node->storageKind()))
  PLAIN_GETTER(VarLikeDecl, type_expression, "Get the type expression of this VarLikeDecl node",
               Nilable<const chpl::uast::AstNode*>, return node->typeExpression())
  PLAIN_GETTER(VarLikeDecl, intent, "Get the intent for this VarLikeDecl node",
               const char*, return intentToString(node->storageKind()))
CLASS_END(VarLikeDecl)

CLASS_BEGIN(Formal)
  PLAIN_GETTER(Formal, is_this, "Check if this Formal node is a 'this' formal",
               bool, return node->name() == USTR("this"))
CLASS_END(Formal)

CLASS_BEGIN(VarArgFormal)
  PLAIN_GETTER(VarArgFormal, count, "Get the count expression of this VarArgFormal node",
               Nilable<const chpl::uast::AstNode*>, return node->count())
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
