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

// This file lists the AST classes to help with macros generating code for each
//
// Each line consists of one of
//   AST_NODE(NAME)
//   AST_LEAF(NAME)
//   AST_BEGIN_SUBCLASSES(NAME)
//   AST_END_SUBCLASSES(NAME)
//
// AST_NODE is for an AST node than can contain other AST nodes
// AST_LEAF is for an AST node that cannot contain other AST nodes
// AST_BEGIN_SUBCLASSES / AST_END_SUBCLASSES mark subclasses of abstract classes

// These AST nodes are documented in their respective header files
// and only listed here.

// This file should store the AST nodes in groups according to the
// class hierarchy and otherwise be in sorted order.

// A note about the philosophy:
// Where to draw the line on
// new AST types vs. using a primitive?
//  try to make AST nodes and use inheritance
//  to allow coarse-grain view

// Should there be a generic way to enumerate the
// children of a particular AST node besides the visitor?
//  yes, and allow getting "next" somehow
//  let's try making a vector of children

// the following comment disables doxygen for these
/// \cond DO_NOT_DOCUMENT

AST_BEGIN_SUBCLASSES(Expression)       // old AST: Expr

  AST_LEAF(Comment)                    //
  AST_LEAF(ErroneousExpression)        //
  AST_LEAF(Identifier)                 // old AST: UnresolvedSymExpr

  AST_NODE(Block)                      // old AST: BlockStmt
  AST_NODE(ExternBlock)                // old AST: ExternBlockStmt
  AST_NODE(Implements)                 // old AST: ImplementsStmt
  AST_NODE(Import)                     // old AST: ImportStmt
  AST_NODE(Local)                      //
  AST_NODE(Require)                    //
  AST_NODE(Serial)                     //
  AST_NODE(Use)                        // old AST: UseStmt

  AST_BEGIN_SUBCLASSES(ControlFlow)

    AST_NODE(Begin)                    //
    AST_NODE(Cobegin)                  //
    AST_NODE(Break)                    // old AST: GotoStmt
    AST_NODE(Conditional)              // old AST: IfExpr/CondStmt
    AST_NODE(Continue)                 // old AST: GotoStmt
    AST_NODE(Defer)                    // old AST: DeferStmt
    AST_NODE(On)                       //
    AST_NODE(Return)                   //
    AST_NODE(Select)                   //
    AST_NODE(Sync)                     //
    AST_NODE(TryCatch)                 // old AST: TryStmt/CatchStmt

    AST_BEGIN_SUBCLASSES(Loop)         // old AST: LoopExpr / LoopStmt

      AST_NODE(DoWhile)                // old AST: DoWhileStmt

      AST_BEGIN_SUBCLASSES(IndexableLoop)
        AST_NODE(Coforall)
        AST_NODE(For)                    // old AST: ForLoop / LoopExpr
        AST_NODE(Forall)                 // old AST: ForallStmt / LoopExpr
        AST_NODE(Foreach)                //
      AST_END_SUBCLASSES(IndexableLoop)

      AST_NODE(While)                  // old AST: WhileStmt

    AST_END_SUBCLASSES(Loop)

  AST_END_SUBCLASSES(ControlFlow)


  AST_BEGIN_SUBCLASSES(Literal)        // old AST: Immediate
    AST_LEAF(NumericLiteral)
    AST_LEAF(StringLiteral) // includes bytes
    AST_NODE(ArrayLiteral)
    AST_NODE(DomainLiteral)
    AST_NODE(TupleLiteral)
  AST_END_SUBCLASSES(Literal)

  AST_BEGIN_SUBCLASSES(Call)           // old AST:  CallExpr
    AST_NODE(Dot)                      //
    AST_NODE(FnCall)
    AST_NODE(New)
    AST_NODE(OpCall)
    AST_NODE(PrimCall)                 // old AST: CallExpr/PrimitiveOp
    AST_NODE(Try)                      //
  AST_END_SUBCLASSES(Call)


//  AST_BEGIN_SUBCLASSES(Decl)
    AST_NODE(MultiDecl)
    AST_NODE(TupleDecl)

    AST_BEGIN_SUBCLASSES(SymDecl)
      //AST_NODE(FieldDecl)
      AST_NODE(FormalDecl)
      //AST_NODE(ForwardingDecl)
      AST_NODE(FunctionDecl)
      AST_NODE(ModuleDecl)
      //AST_NODE(TypeDecl)
      AST_NODE(VariableDecl)
    AST_END_SUBCLASSES(SymDecl)
//  AST_END_SUBCLASSES(Decl)

AST_END_SUBCLASSES(Expression)

AST_BEGIN_SUBCLASSES(Sym)              // old AST: Symbol
  AST_NODE(EnumElement)                // old AST: EnumSymbol
  AST_LEAF(Formal)                     // old AST: ArgSymbol
  AST_NODE(Function)                   // old AST: FnSymbol
  AST_NODE(Interface)                  // old AST: InterfaceSymbol
  AST_NODE(Module)                     // old AST: ModuleSymbol
  AST_LEAF(Variable)                   // old AST: VarSymbol
                                       // old AST: ShadowVarSymbol

  AST_BEGIN_SUBCLASSES(TypeSym)        // old AST: TypeSymbol/Type
    AST_NODE(Enum)                     // old AST: EnumType
    //AST_NODE(SimpleType)             //

    AST_BEGIN_SUBCLASSES(AggregateTypeSym) // old AST: AggregateType
      AST_NODE(Class)                  //
      AST_NODE(Record)                 //
      AST_NODE(Union)                  //
    AST_END_SUBCLASSES(AggregateTypeSym)

  AST_END_SUBCLASSES(TypeSym)

AST_END_SUBCLASSES(Sym)

/// \endcond
