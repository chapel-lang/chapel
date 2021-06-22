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

// the following comment disables doxygen for these
/// \cond DO_NOT_DOCUMENT

// TODO: since everything inherits from Expression
// should we just remove it? Or rename ASTNode to Expression?
AST_BEGIN_SUBCLASSES(Expression)       // old AST: Expr

  AST_BEGIN_SUBCLASSES(SimpleBlockLike)
    AST_NODE(Begin)
    AST_NODE(Block)                    // old AST: BlockStmt
    AST_NODE(Defer)                    // old AST: DeferStmt
    AST_NODE(Local)                    //
    AST_NODE(On)                       //
    AST_NODE(Serial)                   //
  AST_END_SUBCLASSES(SimpleBlockLike)

  AST_NODE(As)                         //
  AST_NODE(Break)                    // old AST: GotoStmt
  AST_NODE(Cobegin)                  //
  AST_NODE(Conditional)              // old AST: IfExpr/CondStmt
  AST_LEAF(Comment)                    //
  AST_NODE(Continue)                 // old AST: GotoStmt
  AST_NODE(Delete)                     //
  AST_LEAF(ErroneousExpression)        //
  AST_LEAF(Identifier)                 // old AST: UnresolvedSymExpr
  AST_NODE(Label)                    //
  //AST_NODE(ExternBlock)                // old AST: ExternBlockStmt
  //AST_NODE(Implements)                 // old AST: ImplementsStmt
  //AST_NODE(Import)                     // old AST: ImportStmt
  AST_NODE(New)
  //AST_NODE(Require)                    //
  AST_NODE(Return)                   //
  //AST_NODE(Select)                   //
  AST_NODE(Sync)                     //
  //AST_NODE(TryCatch)                 // old AST: TryStmt/CatchStmt
  AST_NODE(Use)                        // old AST: UseStmt
  AST_NODE(UseClause)                  //
  AST_NODE(WithClause)
  AST_NODE(Yield)                    //

  AST_BEGIN_SUBCLASSES(Loop)         // old AST: LoopExpr / LoopStmt
      AST_NODE(DoWhile)                // old AST: DoWhileStmt
      AST_NODE(While)                  // old AST: WhileStmt

    AST_BEGIN_SUBCLASSES(IndexableLoop)
      AST_NODE(BracketLoop)
      AST_NODE(Coforall)
      AST_NODE(For)                    // old AST: ForLoop / LoopExpr
      AST_NODE(Forall)                 // old AST: ForallStmt / LoopExpr
      AST_NODE(Foreach)                //
    AST_END_SUBCLASSES(IndexableLoop)

  AST_END_SUBCLASSES(Loop)

  //AST_NODE(ArrayExpr) // TODO: figure out good names for these
  //AST_NODE(DomainExpr)
  //AST_NODE(TupleExpr)

  AST_BEGIN_SUBCLASSES(Literal)        // old AST: Immediate

    //AST_BEGIN_SUBCLASSES(NumericLiteral)  notional - currently a template
      AST_LEAF(ImagLiteral)
      AST_LEAF(IntLiteral)
      AST_LEAF(RealLiteral)
      AST_LEAF(UintLiteral)
    //AST_END_SUBCLASSES(NumericLiteral)

    AST_BEGIN_SUBCLASSES(StringLikeLiteral) 
      AST_LEAF(BytesLiteral)
      AST_LEAF(CStringLiteral)
      AST_LEAF(StringLiteral)
    AST_END_SUBCLASSES(StringLikeLiteral)

  AST_END_SUBCLASSES(Literal)

  AST_BEGIN_SUBCLASSES(Call)           // old AST:  CallExpr
    AST_NODE(Dot)                      //
    AST_NODE(FnCall)
    AST_NODE(OpCall)
    AST_NODE(PrimCall)                 // old AST: CallExpr/PrimitiveOp
    //AST_NODE(Try)                      //
    AST_NODE(Zip)
  AST_END_SUBCLASSES(Call)

  AST_BEGIN_SUBCLASSES(Decl)           // old AST: Symbol or DefExpr
    AST_NODE(MultiDecl)
    AST_NODE(TupleDecl)

    AST_BEGIN_SUBCLASSES(NamedDecl)
      //AST_NODE(ForwardingDecl)
      //AST_NODE(TypeDecl)

      AST_NODE(EnumElement)                // old AST: EnumSymbol
      AST_NODE(Function)                   // old AST: FnSymbol
      //AST_NODE(Interface)                  // old AST: InterfaceSymbol
      AST_NODE(Module)                     // old AST: ModuleSymbol

      AST_BEGIN_SUBCLASSES(VarLikeDecl)
        AST_LEAF(Formal)                   // old AST: ArgSymbol
        AST_LEAF(TaskVar)                  // old AST: ShadowVarSymbol
        AST_LEAF(Variable)                 // old AST: VarSymbol
      AST_END_SUBCLASSES(VarLikeDecl)

      AST_BEGIN_SUBCLASSES(TypeDecl)       // old AST: TypeSymbol/Type
        AST_NODE(Enum)                     // old AST: EnumType

        AST_BEGIN_SUBCLASSES(AggregateDecl)// old AST: AggregateType
          AST_NODE(Class)                  //
          AST_NODE(Record)                 //
          AST_NODE(Union)                  //
        AST_END_SUBCLASSES(AggregateDecl)

      AST_END_SUBCLASSES(TypeDecl)

    AST_END_SUBCLASSES(NamedDecl)

  AST_END_SUBCLASSES(Decl)

AST_END_SUBCLASSES(Expression)


/// \endcond

// this comment seems to be necessary for doxygen xml output to be well-formed
