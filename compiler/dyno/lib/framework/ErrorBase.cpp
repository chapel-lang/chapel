#include "ErrorBase.h"
#include "ErrorWriter.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/framework/query-impl.h"
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
    wr.printBrief(kind_, id_, message_);
  } else {
    wr.printBrief(kind_, loc_, message_);
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
    wr.printBrief(kind_, id_, message_);
  } else {
    wr.printBrief(kind_, loc_, message_);
  }
}

void GeneralError::mark(Context* context) const {
  id_.mark(context);
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

void ErrorPrivateToPublicInclude::write(ErrorWriter& wr) const {
  auto moduleInclude = std::get<const uast::Include*>(info);
  auto moduleDef = std::get<const uast::Module*>(info);
  wr.printBrief(kind_, moduleInclude,
                "cannot make a private module public through "
                "an include statement");
  wr.printAst(moduleInclude);
  wr.printMessage("module declared private here:");
  wr.printAst(moduleDef);
}

void ErrorPrototypeInclude::write(ErrorWriter& wr) const {
  auto moduleInclude = std::get<const uast::Include*>(info);
  auto moduleDef = std::get<const uast::Module*>(info);
  wr.printBrief(kind_, moduleInclude,
                "cannot apply prototype to module in include statement");
  wr.printAst(moduleInclude);
  wr.printMessage("put prototype keyword at module declaration here:");
  wr.printAst(moduleDef);
}

void ErrorMissingInclude::write(ErrorWriter& wr) const {
  auto moduleInclude = std::get<const uast::Include*>(info);
  auto& filePath = std::get<std::string>(info);
  wr.printBrief(kind_, moduleInclude, "cannot find included submodule");
  wr.printMessage("expected file at path '", filePath, "'");
}

void ErrorRedefinition::write(ErrorWriter& wr) const {
  auto decl = std::get<const uast::NamedDecl*>(info);
  auto& ids = std::get<std::vector<ID>>(info);
  wr.printBrief(kind_, decl, "'", decl->name().c_str(), "' has multiple definitions");
  wr.printAst(decl);
  wr.printMessage("redefined in these places:");
  for (const ID& id : ids) {
    if (id != decl->id()) {
      wr.printAst<ID, ID>(id);
    }
  }
}
}
