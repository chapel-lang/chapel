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
#ifndef CHPL_FRAMEWORK_ERRORWRITER_H
#define CHPL_FRAMEWORK_ERRORWRITER_H

#include <ostream>

#include "chpl/framework/ErrorBase.h"
#include "chpl/uast/all-uast.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/util/terminal.h"

namespace chpl {

class Context;

namespace errordetail {

/**
  Sometimes, locations reported using IDs are treated
  differently than locations reported using Location. In particular,
  the production compiler will print "In module: ..." if given an ID.

  This class is a shim for methods of ErrorWriterBase, which, when
  used as a location, results in a Location being produced. Thus,
  note(id, ...) will use an ID, whereas note(locationOnly(id),...) will
  use a Location.

  This class is used because error messages don't get access to
  the context themselves, and thus can't locate IDs themselves.
 */
template <typename T>
struct LocationOnly {
  T t; /* The thing whose location to compute */
};

template <typename T>
struct JustOneLine {
  T t; /* The thing whose location to compute */
};

inline Location locate(Context* context, const ID& id) {
  if (!context) return Location();
  return parsing::locateId(context, id);
}
inline Location locate(Context* context, const uast::AstNode* node) {
  return locate(context, node->id());
}
inline Location locate(Context* context, const Location& loc) {
  return loc;
}
inline Location locate(Context* context, const IdOrLocation& idOrLoc) {
  return idOrLoc.computeLocation(context);
}
template <typename T>
inline Location locate(Context* context, const JustOneLine<T>& jl) {
  auto loc = locate(context, jl.t);
  return Location(loc.path(),
                  loc.firstLine(), loc.firstColumn(),
                  loc.firstLine(), loc.firstColumn());
}

/// \cond DO_NOT_DOCUMENT
/**
  Wrapper class for any piece of data with a location that, when converted
  to a string, results in a printing the data's file and line number.
 */
template <typename T>
struct AsFileName {
  T t; /* The thing to locate */

  Location location(Context* context) const {
    return locate(context, t);
  }
};

/**
  Template to be partially specialized to provide custom to-string output
  specific to the ErrorWriter. Uses stringify by default.
 */
template <typename T>
struct Writer {
  void operator()(Context* context, std::ostream& oss, const T& t) {
    stringify<T> str;
    str(oss, CHPL_SYNTAX, t);
  }
};

template <>
struct Writer<const char*> {
  void operator()(Context* context, std::ostream& oss, const char* t) {
    oss << t;
  }
};

template <>
struct Writer<std::string> {
  void operator()(Context* context, std::ostream& oss, const std::string& t) {
    oss << t;
  }
};

template <>
struct Writer<UniqueString> {
  void operator()(Context* context, std::ostream& oss, UniqueString us) {
    oss << us.c_str();
  }
};

template <typename T>
struct Writer<errordetail::AsFileName<T>> {
  void operator()(Context* context, std::ostream& oss, const errordetail::AsFileName<T>& e) {
    auto loc = e.location(context);
    oss << loc.path().c_str() << ":" << loc.firstLine();
  }
};

template <>
struct Writer<const types::Type*> {
  void operator()(Context* context, std::ostream& oss, const types::Type* type) {
    if (type->isUnknownType()) {
      oss << "unknown type";
    } else {
      stringify<const types::Type*> str;
      str(oss, CHPL_SYNTAX, type);
    }
  }
};

template <>
struct Writer<types::QualifiedType> {
  void operator()(Context* context, std::ostream& oss, const types::QualifiedType& qt) {
    if (!qt.hasTypePtr()) {
      oss << "a value of unknown type" << std::endl;
    } else if (qt.kind() == types::QualifiedType::TYPE) {
      oss << "the type '";
      qt.type()->stringify(oss, CHPL_SYNTAX);
      oss << "'";
    } else if (qt.kind() == types::QualifiedType::PARAM) {
      oss << "a param of type '";
      qt.type()->stringify(oss, CHPL_SYNTAX);
      oss << "'";
    } else {
      oss << "a value of type '";
      qt.type()->stringify(oss, CHPL_SYNTAX);
      oss << "'";
    }
  }
};
/// \endcond

} // end namespace errordetail

/**
  Helper function to create an AsFileName class.
 */
template <typename T>
errordetail::AsFileName<T> fileNameOf(T t) {
  return errordetail::AsFileName<T> { std::move(t) };
}

/**
  Helper function to create a LocationOnly class.
 */
template <typename T>
errordetail::LocationOnly<T> locationOnly(T t) {
  return errordetail::LocationOnly<T> { std::move(t) };
}

/**
  Helper function to create a JustOneLine class.
*/
template <typename T>
errordetail::JustOneLine<T> justOneLine(T t) {
  return errordetail::JustOneLine<T> { std::move(t) };
}

/**
  ErrorWriterBase is the main way for error messages to output diagnostic
  information. It abstracts away writing code to output streams (in fact,
  some instances of ErrorWriterBase do not write to a stream at all),
  and provides functionality like printing and underlining code. The way
  data is formatted when fed to the various printing functions like
  ErrorWriterBase::heading and ErrorWriterBase::message is specified by
  specializations of the errordetail::Writer class.

  The ErrorWriterBase expects that the ErrorWriterBase::heading function be
  called first by every error message; this function serves the double purpose
  of printing out the error heading, as well as setting the error message's
  location.
 */
class ErrorWriterBase {
 public:
   /** The style of error reporting that the ErrorWriterBase should produce. */
  enum OutputFormat {
    /** Specify that all information about the error should be printed. */
    DETAILED,
    /** Specify that only key parts of the error should be printed. */
    BRIEF,
  };
 protected:
  Context* context;
  OutputFormat outputFormat_;

  ErrorWriterBase(Context* context, OutputFormat outputFormat)
    : context(context), outputFormat_(outputFormat) {}

  /**
    Makes tweaks to an error string depending on output format.
   */
  void tweakErrorString(std::string& message) const;

  /**
    Write the error heading, possibly with some color and text decoration.
    The location given to this function and its overloads is considered
    the error's main location.
   */
  virtual void writeHeading(ErrorBase::Kind kind, ErrorType type, IdOrLocation idOrLoc, const std::string& message) = 0;
  void writeHeading(ErrorBase::Kind kind, ErrorType type, const uast::AstNode* ast, const std::string& message);
  template <typename T>
  void writeHeading(ErrorBase::Kind kind, ErrorType type, errordetail::LocationOnly<T> t, const std::string& message) {
    writeHeading(kind, type, errordetail::locate(context, t.t), message);
  }

  /**
    Write additional information about the error. This information is only
    included when the error is reported in detailed mode; when printed
    in brief mode, information printed via writeMessage is omitted.
   */
  virtual void writeMessage(const std::string& message) = 0;

  /**
    Write a note about the error. Unlike messages, notes are printed
    even in brief mode. Thus, notes can be used to provide information
    that is useful "in all cases" (e.g., the location of a duplicate
    definition).
   */
  virtual void writeNote(IdOrLocation loc, const std::string& message) = 0;
  void writeNote(const uast::AstNode* ast, const std::string& message);
  template <typename T>
  void writeNote(errordetail::LocationOnly<T> t, const std::string& message) {
    writeNote(errordetail::locate(context, t.t), message);
  }

  /**
    Prints the lines of code associated with the given location. Additional
    locations provided via \p toHighlight parameter are underlined when the
    code is printed.
   */
  virtual void writeCode(const Location& place,
                         const std::vector<Location>& toHighlight = {}) = 0;

  template <typename ... Ts>
  std::string toString(Ts ... ts) {
    std::ostringstream oss;
    auto write = [&](auto t) {
      errordetail::Writer<decltype(t)> writer;
      writer(context, oss, t);
    };

    auto dummy = { (write(ts), 0)..., };
    (void) dummy;
    return oss.str();
  }
 public:
  /**
    Write the error heading, possibly with some color and text decoration.
    The location given to this function and its overloads is considered
    the error's main location.

    The variable arguments given to this function are automatically converted
    to strings.
   */
  template <typename LocationType, typename ... Ts>
  void heading(ErrorBase::Kind kind, ErrorType type, LocationType loc, Ts ... ts) {
    auto str = toString(std::forward<Ts>(ts)...);
    tweakErrorString(str);
    writeHeading(kind, type, loc, str);
  }

  /**
    Same as ErrorWriter::heading, but doesn't tweak the resulting error message
    to remove punctuation.
   */
  template <typename LocationType, typename ... Ts>
  void headingVerbatim(ErrorBase::Kind kind, ErrorType type, LocationType loc, Ts ... ts) {
    writeHeading(kind, type, loc, toString(std::forward<Ts>(ts)...));
  }

  /**
    Write a note about the error. Unlike messages, notes are printed
    even in brief mode. Thus, notes can be used to provide information
    that is useful "in all cases" (e.g., the location of a duplicate
    definition).

    The variable arguments given to this function are automatically converted
    to strings.
   */
  template <typename ... Ts>
  void message(Ts ... ts) {
    writeMessage(toString(std::forward<Ts>(ts)...));
  }

  /**
    Write a note about the error. Unlike messages, notes are printed
    even in brief mode. Thus, notes can be used to provide information
    that is useful "in all cases" (e.g., the location of a duplicate
    definition).

    The variable arguments given to this function are automatically converted
    to strings.
   */
  template <typename LocationType, typename ... Ts>
  void note(LocationType loc, Ts ... ts) {
    auto str = toString(std::forward<Ts>(ts)...);
    tweakErrorString(str);
    writeNote(loc, str);
  }

  /**
    Same as ErrorWriter::note, but doesn't tweak the resulting error message
    to remove punctuation.
   */
  template <typename LocationType, typename ... Ts>
  void noteVerbatim(ErrorBase::Kind kind, ErrorType type, LocationType loc, Ts ... ts) {
    writeNote(kind, type, loc, toString(std::forward<Ts>(ts)...));
  }

  /**
    Prints the lines of code associated with the given location. Additional
    locations provided via \p toHighlight field are underlined when the
    code is printed.

    This function accepts any type for which location can be inferred,
    for both the main location and the highlights.
   */
  template <typename LocHighlight = const uast::AstNode*, typename LocPlace>
  void code(const LocPlace& place,
            const std::vector<LocHighlight>& toHighlight = {}) {
    std::vector<Location> ids(toHighlight.size());
    std::transform(toHighlight.cbegin(), toHighlight.cend(), ids.begin(), [&](auto node) {
      return errordetail::locate(context, node);
    });
    writeCode(errordetail::locate(context, place), ids);
  }

  /**
    Specialization of ErrorWriter::code for printing definitions. Since
    definitions may be fairly length (e.g., a variable with a multi-line
    initializer), this will only print the first line of the definition.
   */
  template <typename LocPlace>
  void codeForDef(const LocPlace& place) {
    code<LocPlace>(justOneLine(place), { place });
  }

  /**
    Specialization of ErrorWriter::code for "bla bla defined here".
    Doesn't underline, but also doesn't print the whole thing. This
    is important, for instance, in the case that a record declaration
    is being printed. We don't want to dump all the fields / methods
    as part of the error.
   */
  template <typename LocPlace>
  void codeForLocation(const LocPlace& place) {
    code<LocPlace>(justOneLine(place));
  }
};

/**
  Implementation of ErrorWriterBase that prints error output to a stream.
  This class' output varies depending on if the error message is printed
  in brief or detailed mode, as well as if the useColor flag is set.
 */
class ErrorWriter : public ErrorWriterBase {
 protected:
  std::ostream& oss_;
  ErrorWriterBase::OutputFormat outputFormat_;
  bool useColor_;

  void setColor(TermColorName color);

  void writeHeading(ErrorBase::Kind kind, ErrorType type, IdOrLocation idOrLoc,
                    const std::string& message) override;
  void writeMessage(const std::string& message) override {
    if (outputFormat_ == DETAILED) {
      // In detailed mode, the body is indented.
      oss_  << "  ";
      oss_ << message << std::endl;
    }
  }
  void writeNote(IdOrLocation idOrLoc, const std::string& message) override;
  void writeCode(const Location& place,
                 const std::vector<Location>& toHighlight = {}) override;
 public:
  ErrorWriter(Context* context, std::ostream& oss,
              ErrorWriterBase::OutputFormat outputFormat, bool useColor) :
    ErrorWriterBase(context, outputFormat), oss_(oss),
    outputFormat_(outputFormat), useColor_(useColor) {}

  void writeNewline();
};

} // end namespace chpl

#endif
