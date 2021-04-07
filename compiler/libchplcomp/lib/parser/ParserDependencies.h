// This is included at the right time in the generated bison .h/.cpp
// It is split out into this separate file for easier maintenance.
// It contains any #includes necessary for the parser.

#include "chpl/AST/BaseAST.h"
#include "chpl/AST/Builder.h"
#include "chpl/AST/Comment.h"
#include "chpl/AST/Context.h"
#include "chpl/AST/Decl.h"
#include "chpl/AST/ErroneousExpr.h"
#include "chpl/AST/Expr.h"
#include "chpl/AST/Identifier.h"
#include "chpl/AST/UniqueString.h"

#include "parser.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <utility>
#include <vector>

using namespace chpl;
using namespace ast;

struct ParserError;
struct ParserComment;
