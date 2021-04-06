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

// the following comment disables doxygen for these
/// \cond DO_NOT_DOCUMENT

AST_BEGIN_SUBCLASSES(BaseAST)


  AST_BEGIN_SUBCLASSES(Expr)

    AST_LEAF(ErroneousExpr)
    AST_LEAF(Identifier)
    AST_LEAF(Literal)
    AST_LEAF(Comment)

    AST_NODE(ForExpr)
    AST_NODE(ForallExpr)
    AST_NODE(ForeachExpr)
    AST_NODE(IfExpr)
    AST_NODE(TryExpr)
    // Array literal expression?
    // Tuple literal expression?
    // new ClassOrRecord?
    // Where to draw the line on
    // new AST types vs. using a primitive?

    AST_BEGIN_SUBCLASSES(CallExpr)
      AST_NODE(PrimCallExpr)
      AST_NODE(FnCallExpr)
      AST_NODE(OpCallExpr)
    AST_END_SUBCLASSES(CallExpr)

    AST_BEGIN_SUBCLASSES(Decl)
      AST_NODE(FieldDecl)
      AST_NODE(FnDecl)
      AST_NODE(ForwardingDecl)
      AST_NODE(ModuleDecl)
      AST_NODE(TypeDecl)
      AST_NODE(VarDecl)
    AST_END_SUBCLASSES(Decl)

    AST_BEGIN_SUBCLASSES(Stmt)
      AST_NODE(BeginStmt)
      AST_NODE(BlockStmt)
      AST_NODE(BreakStmt)
      AST_NODE(CatchStmt)
      AST_NODE(CoforallStmt)
      AST_NODE(ContinueStmt)
      AST_NODE(DeferStmt)
      AST_NODE(DoWhileStmt)
      AST_NODE(ExternBlockStmt)
      AST_NODE(ForStmt)
      AST_NODE(ForallStmt)
      AST_NODE(ForeachStmt)
      AST_NODE(IfStmt)
      AST_NODE(ImplementsStmt)
      AST_NODE(ImportStmt)
      AST_NODE(LocalStmt)
      AST_NODE(OnStmt)
      AST_NODE(SerialStmt)
      AST_NODE(TryStmt)
      AST_NODE(UseStmt)
      AST_NODE(WhileStmt)
    AST_END_SUBCLASSES(Stmt)

  AST_END_SUBCLASSES(Expr)

  AST_BEGIN_SUBCLASSES(Symbol)
    AST_NODE(EnumElement)
    AST_NODE(Formal)
    AST_NODE(Function)
    AST_NODE(Interface)
    AST_NODE(Module)
    AST_NODE(Variable)

    AST_BEGIN_SUBCLASSES(Type)
      AST_NODE(Enum)
      AST_NODE(SimpleType)

      AST_BEGIN_SUBCLASSES(Aggregate)
        AST_NODE(Class)
        AST_NODE(Record)
        AST_NODE(Union)
      AST_END_SUBCLASSES(Aggregate)

    AST_END_SUBCLASSES(Type)

  AST_END_SUBCLASSES(Symbol)

AST_END_SUBCLASSES(BaseAST)

/// \endcond
