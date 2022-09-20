#include "chpl/framework/ErrorBase.h"
#include "chpl/framework/ErrorWriter.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/framework/query-impl.h"
#include "chpl/types/all-types.h"
#include <sstream>

namespace chpl {

std::string ErrorBase::message() const {
  std::ostringstream oss;
  ErrorWriter ew(/* context */ nullptr, oss, ErrorWriter::MESSAGE_ONLY);
  write(ew);
  return oss.str();
}

Location ErrorBase::location(Context* context) const {
  std::ostringstream oss;
  ErrorWriter ew(context, oss, ErrorWriter::MESSAGE_ONLY);
  write(ew);
  return ew.lastLocation();
}

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

void ParseError::write(ErrorWriter& wr) const {
  // if the ID is set, determine the location from that
  if (!id_.isEmpty()) {
    wr.writeHeading(kind_, id_, message_);
  } else {
    wr.writeHeading(kind_, loc_, message_);
  }
}

void ParseError::mark(Context* context) const {
  id_.mark(context);
  loc_.mark(context);
}

const owned<GeneralError>&
GeneralError::getGeneralErrorID(Context* context, Kind kind, ID id, std::string message) {
  QUERY_BEGIN(getGeneralErrorID, context, kind, id, message);
  auto result = owned<GeneralError>(new GeneralError(kind, id, std::move(message)));
  return QUERY_END(result);
}

const owned<GeneralError>&
GeneralError::getGeneralErrorLocation(Context* context, Kind kind, Location loc, std::string message) {
  QUERY_BEGIN(getGeneralErrorLocation, context, kind, loc, message);
  auto result = owned<GeneralError>(new GeneralError(kind, loc, std::move(message)));
  return QUERY_END(result);
}

const GeneralError* GeneralError::vbuild(Context* context, Kind kind, ID id, const char* fmt, va_list vl) {
  auto message = vprintToString(fmt, vl);
  return getGeneralErrorID(context, kind, id, message).get();
}

const GeneralError* GeneralError::vbuild(Context* context, Kind kind, Location loc, const char* fmt, va_list vl) {
  auto message = vprintToString(fmt, vl);
  return getGeneralErrorLocation(context, kind, loc, message).get();
}

void GeneralError::write(ErrorWriter& wr) const {
  if (!id_.isEmpty()) {
    wr.writeHeading(kind_, id_, message_);
  } else {
    wr.writeHeading(kind_, loc_, message_);
  }
}

void GeneralError::mark(Context* context) const {
  id_.mark(context);
}

#define DIAGNOSTIC_CLASS(NAME, KIND, EINFO...)\
  const owned<Error##NAME>& Error##NAME::getError##NAME(Context* context, std::tuple<EINFO> tuple) {\
    QUERY_BEGIN(getError##NAME, context, tuple);\
    auto result = owned<Error##NAME>(new Error##NAME(tuple));\
    return QUERY_END(result);\
  }\
\
  const Error##NAME* Error##NAME::get(Context* context, std::tuple<EINFO> tuple) {\
    return Error##NAME::getError##NAME(context, std::move(tuple)).get();\
  }
#include "chpl/framework/error-classes-list.h"
#undef DIAGNOSTIC_CLASS

void ErrorIncompatibleIfBranches::write(ErrorWriter& wr) const {
  auto ifExpr = std::get<const uast::Conditional*>(info);
  auto qt1 = std::get<1>(info);
  auto qt2 = std::get<2>(info);

  wr.writeHeading(kind_, ifExpr, "branches of if-expression have incompatible types.");
  wr.writeMessage("in the following if-expression:");
  wr.writeCode(ifExpr, { ifExpr->thenBlock(), ifExpr->elseBlock() });
  wr.writeMessage("the first branch has type ", qt1,
                   ", while the second has type ", qt2);
}

void ErrorTupleExpansionNamedArgs::write(ErrorWriter& wr) const {
  auto fnCall = std::get<const uast::FnCall*>(info);
  auto tupleOp = std::get<const uast::OpCall*>(info);

  wr.writeHeading(kind_, fnCall, "tuple expansion cannot be used with named arguments.");
  wr.writeMessage("a tuple is being expanded here:");
  wr.writeCode(fnCall, { tupleOp });
}

void ErrorMemManagementRecords::write(ErrorWriter& wr) const {
  auto newCall = std::get<const uast::New*>(info);
  auto record = std::get<const types::RecordType*>(info);

  wr.writeHeading(kind_, newCall,
      "cannot use memory management strategy ",
      uast::New::managementToString(newCall->management()),
      " with record ",
      record->name());
  wr.writeCode(newCall, { newCall->typeExpression() });
  wr.writeNote("declared as record at ", fileNameOf(record->id()));
}

void ErrorPrivateToPublicInclude::write(ErrorWriter& wr) const {
  auto moduleInclude = std::get<const uast::Include*>(info);
  auto moduleDef = std::get<const uast::Module*>(info);
  wr.writeHeading(kind_, moduleInclude,
                "cannot make a private module public through "
                "an include statement");
  wr.writeCode(moduleInclude);
  wr.writeMessage("module declared private here:");
  wr.writeCode(moduleDef);
}

void ErrorPrototypeInclude::write(ErrorWriter& wr) const {
  auto moduleInclude = std::get<const uast::Include*>(info);
  auto moduleDef = std::get<const uast::Module*>(info);
  wr.writeHeading(kind_, moduleInclude,
                "cannot apply prototype to module in include statement");
  wr.writeCode(moduleInclude);
  wr.writeMessage("put prototype keyword at module declaration here:");
  wr.writeCode(moduleDef);
}

void ErrorMissingInclude::write(ErrorWriter& wr) const {
  auto moduleInclude = std::get<const uast::Include*>(info);
  auto& filePath = std::get<std::string>(info);
  wr.writeHeading(kind_, moduleInclude, "cannot find included submodule");
  wr.writeMessage("expected file at path '", filePath, "'");
}

void ErrorRedefinition::write(ErrorWriter& wr) const {
  auto decl = std::get<const uast::NamedDecl*>(info);
  auto& ids = std::get<std::vector<ID>>(info);
  wr.writeHeading(kind_, decl, "'", decl->name().c_str(), "' has multiple definitions");
  wr.writeCode(decl);
  for (const ID& id : ids) {
    if (id != decl->id()) {
      wr.writeNote("redefined at ", fileNameOf(id));
      wr.writeCode<ID, ID>(id);
    }
  }
}
}
