#include "ErrorBase.h"
#include "ErrorWriter.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/framework/query-impl.h"

namespace chpl {

const owned<ParseError>&
ParseError::getParseError(Context* context,
                          ErrorBase::Kind kind,
                          ID id,
                          Location loc,
                          std::string message) {
  QUERY_BEGIN(getParseError, context, kind, id, loc, message);
  auto result = std::unique_ptr<ParseError>(new ParseError(kind, id, loc, message));
  return QUERY_END(result);
}

const ParseError* ParseError::get(Context* context, const ErrorMessage& error) {
  Kind kind;
  switch (error.kind()) {
    case ErrorMessage::ERROR: kind = ERROR; break;
    case ErrorMessage::WARNING: kind = WARNING; break;
    case ErrorMessage::NOTE: kind = NOTE; break;
    case ErrorMessage::SYNTAX: kind = SYNTAX; break;
  }
  return ParseError::getParseError(context, kind, error.id(), error.location(), error.message()).get();
}

Location ParseError::location(Context* context) const {
  // if the ID is set, determine the location from that
  if (!id_.isEmpty()) {
    Location loc = parsing::locateId(context, id_);
    return loc;
  }

  // otherwise, use the location stored here
  return loc_;
}

void ParseError::write(ErrorWriter& wr) const {
  // if the ID is set, determine the location from that
  if (!id_.isEmpty()) {
    wr.printBrief(kind_, id_, "parse error:", message_);
  } else {
    wr.printBrief(kind_, loc_, "parse error:", message_);
  }
}

void ParseError::mark(Context* context) const {
  id_.mark(context);
  loc_.mark(context);
}

#define ERROR_CLASS(name__, kind__, info__...)\
  const owned<Error##name__>& Error##name__::getError##name__(Context* context, std::tuple<info__> tuple) {\
    QUERY_BEGIN(getError##name__, context, tuple);\
    auto result = owned<Error##name__>(new Error##name__(tuple));\
    return QUERY_END(result);\
  }\
\
  const Error##name__* Error##name__::get(Context* context, std::tuple<info__> tuple) {\
    return Error##name__::getError##name__(context, std::move(tuple)).get();\
  }
#include "error-classes-list.h"
#undef ERROR_CLASS

void ErrorTupleExpansionNamedArgs::write(ErrorWriter& wr) const {
  auto fnCall = std::get<const uast::FnCall*>(info);
  auto tupleOp = std::get<const uast::OpCall*>(info);
  const uast::AstNode* namedActual = nullptr;
  for (int i = 0; i < fnCall->numActuals(); i++) {
    if(fnCall->isNamedActual(i)) {
      namedActual = fnCall->actual(i);
      break;
    }
  }

  wr.printBrief(kind_, fnCall, "Tuple expansion cannot be used with named arguments.");
  wr.printMessage("A tuple is being expanded here:");
  wr.printAst(fnCall, { tupleOp });
  wr.printMessage("The first named actual is here:");
  wr.printAst(fnCall, { namedActual });
}

void ErrorMemManagementRecords::write(ErrorWriter& wr) const {
  auto newCall = std::get<const uast::FnCall*>(info);
  auto record = std::get<const uast::Record*>(info);

  wr.printBrief(kind_, newCall, "Cannot use memory management strategies with records");
  wr.printAst(newCall, { record });
}

}
