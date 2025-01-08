/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#include "chpl/uast/BuilderResult.h"
#include "test-resolution.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/IntType.h"
#include "chpl/types/QualifiedType.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/FnCall.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Variable.h"

static const std::string INDENT = "  ";
static const bool NOT_A_TYPE_METHOD = false;
static const bool IS_A_TYPE_METHOD = true;

struct InterfaceSource;

static std::string intercalate(const std::vector<std::string>& lines, const std::string& separator) {
  std::string result;
  bool first = true;
  for (const auto& line : lines) {
    if (!first) {
      result += separator;
    }
    result += line;
    first = false;
  }
  return result;
}

struct RecordSource {
  std::string typeName;
  std::vector<std::tuple<bool, std::string>> methods;
  std::vector<InterfaceSource*> interfaces;

  RecordSource(std::string name) : typeName(std::move(name)) {}

  RecordSource& addMethod(bool isTypeMethod, std::string sig) {
    methods.push_back({isTypeMethod, std::move(sig)});
    return *this;
  }

  RecordSource& addInterfaceConstraint(InterfaceSource& interface) {
    interfaces.push_back(&interface);
    return *this;
  }

  std::string declLine(bool includeInterfaces) const;

  std::vector<std::string> getMethods(const std::string& indent,
                                      const std::string& prefix) const {
    std::vector<std::string> methodLines;
    for (const auto& line : methods) {
      auto isTypeMethod = std::get<0>(line);
      auto& sig = std::get<1>(line);
      methodLines.push_back(indent + "proc " + (isTypeMethod ? "type " : "") + prefix + sig);
    }
    return methodLines;
  }

  std::vector<std::string> primaryLines(bool includeInterfaces) const {
    std::vector<std::string> primary;
    primary.push_back(declLine(includeInterfaces) + " {");
    for (const auto& line : getMethods(INDENT, "")) {
      primary.push_back(line);
    }
    primary.push_back("}");
    return primary;
  }

  std::vector<std::string> definitionOnly(bool includeInterfaces) const {
    std::vector<std::string> defOnly;
    defOnly.push_back(declLine(includeInterfaces) + " {}");
    return defOnly;
  }

  std::vector<std::string> methodsOnly() const {
    return getMethods("", typeName + ".");
  }
};

struct InterfaceSource {
  std::string interfaceName;
  std::vector<std::string> linesInside;

  template <typename ... Args>
  InterfaceSource(std::string name, Args&& ... args) : interfaceName(std::move(name)) {
    (linesInside.push_back(std::move(args)), ...);
  }

  std::vector<std::string> allLines() const {
    std::vector<std::string> all;
    all.push_back("interface " + interfaceName + " {");
    for (const auto& line : linesInside) {
      all.push_back(INDENT + line);
    }
    all.push_back("}");
    return all;
  }

  std::string singleImplements(const RecordSource& record) const {
    return record.typeName + " implements " + interfaceName + ";";
  }

  template <typename ... Records>
  std::string generalImplements(Records&& ... records) const {
    std::string line = "implements " + interfaceName + "(";
    bool first = true;
    auto appendRecord = [&line, &first](const auto& record) {
      if (!first) {
        line += ", ";
      }
      line += record.typeName;
      first = false;
    };

    (appendRecord(records), ...);
    return line + ");";
  }
};

std::string RecordSource::declLine(bool includeInterfaces) const {
  std::string line = "record " + typeName;
  if (!includeInterfaces) {
    return line;
  }

  if (!interfaces.empty()) {
    line += " : ";
    bool first = true;
    for (const auto& interface : interfaces) {
      if (!first) {
        line += ", ";
      }
      line += interface->interfaceName;
      first = false;
    }
  }
  return line;
}

struct ModuleSource {
  std::string moduleName;
  std::vector<std::string> usesImports;
  std::vector<std::string> linesInside;
  std::vector<std::pair<std::string, std::string>> checks;

  enum MethodKind {
    /* when adding a record, don't add its methods */
    M_NONE,
    /* when adding a record, add its methods as primary methods */
    M_PRIMARY,
    /* when adding a record, add its methods outside of its body as secondary methods */
    M_SECONDARY,
  };

  enum ImplementationKind {
    /* when adding a record, don't add an interface 'implements' */
    I_NONE,
    /* when adding a record, include the interfaces it implements as part of its declaration */
    I_DECL,
    /* when adding a record, include the interfaces it implements as a statement in the form 'R implements I' */
    I_SINGLE,
    /* when adding a record, include the interfaces it implements as a statement in the form 'implements I(R)' */
    I_GENERAL,
  };

  ModuleSource(std::string name) : moduleName(std::move(name)) {}

  ModuleSource& addRecord(const RecordSource& record, MethodKind methodKind, ImplementationKind implKind) {
    bool includeInterfaces = implKind == I_DECL;
    bool includeMethods = methodKind == M_PRIMARY;
    for (const auto& line : includeMethods ? record.primaryLines(includeInterfaces) : record.definitionOnly(includeInterfaces)) {
      linesInside.push_back(line);
    }
    if (methodKind == M_SECONDARY) {
      for (const auto& line : record.methodsOnly()) {
        linesInside.push_back(line);
      }
    }

    if (implKind == I_SINGLE) {
      for (const auto& interface : record.interfaces) {
        linesInside.push_back(interface->singleImplements(record));
      }
    } else if (implKind == I_GENERAL) {
      for (const auto& interface : record.interfaces) {
        linesInside.push_back(interface->generalImplements(record));
      }
    }
    return *this;
  }

  ModuleSource& addRecordMethods(const RecordSource& record) {
    for (const auto& line : record.methodsOnly()) {
      linesInside.push_back(line);
    }
    return *this;
  }

  ModuleSource& addInterface(const InterfaceSource& interface) {
    for (const auto& line : interface.allLines()) {
      linesInside.push_back(line);
    }
    return *this;
  }

  ModuleSource& addUsesImport(std::string import) {
    usesImports.push_back(std::move(import));
    return *this;
  }

  ModuleSource& addSingleImplements(const InterfaceSource& interface, const RecordSource& record) {
    linesInside.push_back(interface.singleImplements(record));
    return *this;
  }

  template <typename ... Records>
  ModuleSource& addGeneralImplements(const InterfaceSource& interface, Records&& ... records) {
    linesInside.push_back(interface.generalImplements(records...));
    return *this;
  }

  ModuleSource& addLine(const std::string& line) {
    linesInside.push_back(line);
    return *this;
  }

  ModuleSource& addCheck(const std::string& check) {
    static int checkNum = 0;
    checks.push_back({"check" + std::to_string(checkNum++), check});
    return *this;
  }

  std::vector<std::string> allLines() const {
    std::vector<std::string> all;
    all.push_back("module " + moduleName + " {");
    for (const auto& line : usesImports) {
      all.push_back(INDENT + line);
    }
    all.push_back("");
    for (const auto& line : linesInside) {
      all.push_back(INDENT + line);
    }
    for (const auto& check : checks) {
      all.push_back(INDENT + "param " + check.first + " = " + check.second + ";");
    }
    all.push_back("}");
    return all;
  }

  void validateChecks(Context* context, const chpl::uast::Module* mod, bool expectError) const {
    std::vector<std::string> checkNames;
    for (const auto& check : checks) {
      checkNames.push_back(check.first);
    }

    auto types = resolveTypesOfVariables(context, mod, checkNames);
    for (auto& [name, type] : types) {
      std::cout << "checking " << name << std::endl;
      assert(expectError ? (type.isParamFalse() || type.isUnknownOrErroneous()) : type.isParamTrue());
    }
  }
};

static bool findError(const std::vector<owned<ErrorBase>>& errors, ErrorType type) {
  for (auto& err : errors) {
    if (err->type() == type) {
      return true;
    }
  }
  return false;
}

static void testSingleInterface(const InterfaceSource& interface,
                                const RecordSource& record,
                                chpl::optional<ErrorType> expectedError = chpl::empty) {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto validateAndAdvance = [context, &guard, expectedError](const ModuleSource& src, const Module* mod) {
    src.validateChecks(context, mod, (bool) expectedError);
    std::cout << std::endl;

    if (expectedError) {
      assert(findError(guard.errors(), *expectedError));
      guard.realizeErrors();
    } else {
      assert(guard.realizeErrors() == 0);
    }

    context->advanceToNextRevision(false);
  };

  // First, place the interface and the type in the same module.
  {
    for (auto methodKind : { ModuleSource::M_PRIMARY, ModuleSource::M_SECONDARY }) {
      for (auto implKind : { ModuleSource::I_DECL, ModuleSource::I_SINGLE, ModuleSource::I_GENERAL }) {
        auto module = ModuleSource("M")
          .addInterface(interface)
          .addRecord(record, methodKind, implKind)
          .addCheck("__primitive(\"implements interface\", " + record.typeName + ", " + interface.interfaceName + ") == 0");
        auto source = intercalate(module.allLines(), "\n");

        std::cout << "--- testing program ---" << std::endl;
        std::cout << source << std::endl << std::endl;

        auto filePath = UniqueString::get(context, "M.chpl");
        setFileText(context, filePath, source);
        auto& modVec = parseToplevel(context, filePath);
        assert(modVec.size() == 1);

        validateAndAdvance(module, modVec[0]);
      }
    }
  }

  // Then, split the interface, record definition, and checks into three modules.
  {
    for (auto methodKind : { ModuleSource::M_PRIMARY, ModuleSource::M_SECONDARY }) {
      for (auto implKind : { ModuleSource::I_DECL, ModuleSource::I_SINGLE, ModuleSource::I_GENERAL }) {
        for (auto importTechnique : { std::string("use MRec;"),
                                      std::string("import MRec.{") + record.typeName + "};" }) {
          auto moduleLib = ModuleSource("MLib")
            .addInterface(interface);
          auto moduleRec = ModuleSource("MRec")
            .addUsesImport("use MLib;")
            .addRecord(record, methodKind, implKind);
          auto moduleCheck = ModuleSource("MCheck")
            .addUsesImport("use MLib;")
            .addUsesImport(importTechnique)
            .addCheck("__primitive(\"implements interface\", " + record.typeName + ", " + interface.interfaceName + ") == 0");

          auto source =
              intercalate(moduleLib.allLines(), "\n") + "\n\n"
            + intercalate(moduleRec.allLines(), "\n") + "\n\n"
            + intercalate(moduleCheck.allLines(), "\n");

          std::cout << "--- testing program ---" << std::endl;
          std::cout << source << std::endl << std::endl;

          auto filePath = UniqueString::get(context, "file.chpl");
          setFileText(context, filePath, source);
          auto& modVec = parseToplevel(context, filePath);

          validateAndAdvance(moduleCheck, modVec[2]);
        }
      }
    }
  }

  // Then, define the record in one place, but provide the required methods in
  // another place.
  {
    for (auto implKind : { ModuleSource::I_SINGLE, ModuleSource::I_GENERAL }) {
      for (auto importTechnique : { std::string("use MRec;"),
                                    std::string("import MRec.{") + record.typeName + "};" }) {
        auto moduleLib = ModuleSource("MLib")
          .addInterface(interface);
        auto moduleRec = ModuleSource("MRec")
          .addUsesImport("use MLib;")
          .addRecord(record, ModuleSource::M_NONE, ModuleSource::I_NONE);
        auto moduleCheck = ModuleSource("MCheck")
          .addUsesImport("use MLib;")
          .addUsesImport(importTechnique)
          .addRecordMethods(record);

        implKind == ModuleSource::I_SINGLE ? moduleCheck.addSingleImplements(interface, record)
                                           : moduleCheck.addGeneralImplements(interface, record);
        moduleCheck.addCheck("__primitive(\"implements interface\", " + record.typeName + ", " + interface.interfaceName + ") == 0");

        auto source =
          intercalate(moduleLib.allLines(), "\n") + "\n\n"
          + intercalate(moduleRec.allLines(), "\n") + "\n\n"
          + intercalate(moduleCheck.allLines(), "\n");

        std::cout << "--- testing program ---" << std::endl;
        std::cout << source << std::endl << std::endl;

        auto filePath = UniqueString::get(context, "file.chpl");
        setFileText(context, filePath, source);
        auto& modVec = parseToplevel(context, filePath);

        validateAndAdvance(moduleCheck, modVec[2]);
      }
    }
  }
}

static void testRequiredMethodNoArgs() {
  auto i = InterfaceSource("myInterface", "proc Self.foo();");
  auto r1 = RecordSource("myRec")
    .addMethod(NOT_A_TYPE_METHOD, "foo() {}")
    .addInterfaceConstraint(i);
  testSingleInterface(i, r1);

  auto r2 = RecordSource("myRec")
    .addMethod(NOT_A_TYPE_METHOD, "foo(x: int) {}")
    .addInterfaceConstraint(i);
  testSingleInterface(i, r2, ErrorType::InterfaceMissingFn);
}

static void testRequiredMethodNoArgsDefault() {
  auto i = InterfaceSource("myInterface", "proc Self.foo();");
  auto r1 = RecordSource("myRec")
    .addMethod(NOT_A_TYPE_METHOD, "foo(x: int = 10) {}")
    .addInterfaceConstraint(i);
  testSingleInterface(i, r1);
}

static void testRequiredMethodOneArg() {
  auto i = InterfaceSource("myInterface", "proc Self.foo(x: int);");
  auto r1 = RecordSource("myRec")
    .addMethod(NOT_A_TYPE_METHOD, "foo(x: int) {}")
    .addInterfaceConstraint(i);
  testSingleInterface(i, r1);

  auto r2 = RecordSource("myRec")
    .addMethod(NOT_A_TYPE_METHOD, "foo() {}")
    .addInterfaceConstraint(i);
  testSingleInterface(i, r2, ErrorType::InterfaceMissingFn);

  auto r3 = RecordSource("myRec")
    .addMethod(NOT_A_TYPE_METHOD, "foo(x: bool) {}")
    .addInterfaceConstraint(i);
  testSingleInterface(i, r3, ErrorType::InterfaceMissingFn);
}

static void testRequiredMethodAmbiguous() {
  auto i = InterfaceSource("myInterface", "proc Self.foo(x: int);");
  auto r1 = RecordSource("myRec")
    .addMethod(NOT_A_TYPE_METHOD, "foo(x: int) {}")
    .addMethod(NOT_A_TYPE_METHOD, "foo(x: int) {}")
    .addInterfaceConstraint(i);
  testSingleInterface(i, r1, ErrorType::InterfaceAmbiguousFn);
}

static void testRequiredMethodWrongIntent() {
  auto i = InterfaceSource("myInterface", "proc Self.foo(x: int);");
  auto r1 = RecordSource("myRec")
    .addMethod(NOT_A_TYPE_METHOD, "foo(x: int) type do return int;")
    .addInterfaceConstraint(i);
  testSingleInterface(i, r1, ErrorType::InterfaceInvalidIntent);
}

static void testTwoRequiredMethods() {
  auto i = InterfaceSource("myInterface",
                           "proc Self.foo(x: int);",
                           "proc Self.bar(y: real);");

  auto r1 = RecordSource("myRec")
    .addMethod(NOT_A_TYPE_METHOD, "foo(x: int) {}")
    .addMethod(NOT_A_TYPE_METHOD, "bar(y: real) {}")
    .addInterfaceConstraint(i);
  testSingleInterface(i, r1);

  auto r2 = RecordSource("myRec")
    .addMethod(NOT_A_TYPE_METHOD, "foo(x: int) {}")
    .addMethod(NOT_A_TYPE_METHOD, "bar() {}")
    .addInterfaceConstraint(i);
  testSingleInterface(i, r2, ErrorType::InterfaceMissingFn);

  auto r3 = RecordSource("myRec")
    .addMethod(NOT_A_TYPE_METHOD, "foo(x: int) {}")
    .addMethod(NOT_A_TYPE_METHOD, "bar(y: int) {}")
    .addInterfaceConstraint(i);
  testSingleInterface(i, r3, ErrorType::InterfaceMissingFn);

  auto r4 = RecordSource("myRec")
    .addMethod(NOT_A_TYPE_METHOD, "foo(x: int) {}")
    .addInterfaceConstraint(i);
  testSingleInterface(i, r4, ErrorType::InterfaceMissingFn);

  auto r5 = RecordSource("myRec")
    .addMethod(NOT_A_TYPE_METHOD, "bar(y: real) {}")
    .addInterfaceConstraint(i);
  testSingleInterface(i, r5, ErrorType::InterfaceMissingFn);
}

static void testBasicGeneric() {
  auto i = InterfaceSource("myInterface",
                           "proc Self.foo(x);");
  auto r1 = RecordSource("myRec")
    .addMethod(NOT_A_TYPE_METHOD, "foo(x) {}")
    .addInterfaceConstraint(i);
  testSingleInterface(i, r1);

  // top-level type queries and missing types should be cross-compatible.
  auto r2 = RecordSource("myRec")
    .addMethod(NOT_A_TYPE_METHOD, "foo(x: ?t) {}")
    .addInterfaceConstraint(i);
  testSingleInterface(i, r2);

  // too specific: interface requires fully generic, we're giving it a specific type
  auto r3 = RecordSource("myRec")
    .addMethod(NOT_A_TYPE_METHOD, "foo(x: int) {}")
    .addInterfaceConstraint(i);
  testSingleInterface(i, r3, ErrorType::InterfaceMissingFn);

}

static void testBasicGenericTypeQuery() {
  auto i = InterfaceSource("myInterface",
                           "proc Self.foo(x: ?tq);");
  // top-level type queries and missing types should be cross-compatible.
  auto r1 = RecordSource("myRec")
    .addMethod(NOT_A_TYPE_METHOD, "foo(x) {}")
    .addInterfaceConstraint(i);
  testSingleInterface(i, r1);

  auto r2 = RecordSource("myRec")
    .addMethod(NOT_A_TYPE_METHOD, "foo(x: ?t) {}")
    .addInterfaceConstraint(i);
  testSingleInterface(i, r2);

  // too specific: interface requires fully generic, we're giving it a specific type
  auto r3 = RecordSource("myRec")
    .addMethod(NOT_A_TYPE_METHOD, "foo(x: int) {}")
    .addInterfaceConstraint(i);
  testSingleInterface(i, r3, ErrorType::InterfaceMissingFn);

}

static void testDependentGeneric() {
  auto i = InterfaceSource("myInterface",
                           "proc Self.foo(x: ?tq, y: tq);");
  auto r1 = RecordSource("myRec")
    .addMethod(NOT_A_TYPE_METHOD, "foo(x, y: x.type) {}")
    .addInterfaceConstraint(i);
  testSingleInterface(i, r1);

  auto r2 = RecordSource("myRec")
    .addMethod(NOT_A_TYPE_METHOD, "foo(x: ?t, y: t) {}")
    .addInterfaceConstraint(i);
  testSingleInterface(i, r2);
}

static void testAssociatedType() {
  auto i = InterfaceSource("myInterface",
                           "type someType;");
  auto r1 = RecordSource("myRec")
    .addMethod(IS_A_TYPE_METHOD, "someType type do return int;")
    .addInterfaceConstraint(i);
  testSingleInterface(i, r1);

  auto r2 = RecordSource("myRec")
    .addInterfaceConstraint(i);
  testSingleInterface(i, r2, ErrorType::InterfaceMissingAssociatedType);

  auto r3 = RecordSource("myRec")
    .addMethod(IS_A_TYPE_METHOD, "wrongName type do return int;")
    .addInterfaceConstraint(i);
  testSingleInterface(i, r2, ErrorType::InterfaceMissingAssociatedType);

  auto r4 = RecordSource("myRec")
    .addMethod(NOT_A_TYPE_METHOD, "someType type do return int;")
    .addInterfaceConstraint(i);
  testSingleInterface(i, r4, ErrorType::InterfaceMissingAssociatedType);

  auto r5 = RecordSource("myRec")
    .addMethod(IS_A_TYPE_METHOD, "someType do return 42;")
    .addInterfaceConstraint(i);
  testSingleInterface(i, r5, ErrorType::InterfaceMissingAssociatedType);
}

static void testAssociatedTypeInFn() {
  auto i = InterfaceSource("myInterface",
                           "type someType;"
                           "proc Self.foo(x: someType);");
  auto r1 = RecordSource("myRec")
    .addMethod(IS_A_TYPE_METHOD, "someType type do return int;")
    .addMethod(NOT_A_TYPE_METHOD, "foo(x: int) {}")
    .addInterfaceConstraint(i);
  testSingleInterface(i, r1);

  auto r2 = RecordSource("myRec")
    .addMethod(IS_A_TYPE_METHOD, "someType type do return int;")
    .addMethod(NOT_A_TYPE_METHOD, "foo(x: bool) {}")
    .addInterfaceConstraint(i);
  testSingleInterface(i, r2, ErrorType::InterfaceMissingFn);
}

static void testFormalNaming() {
  auto i = InterfaceSource("myInterface",
                           "proc Self.foo(x: int);");
  auto r1 = RecordSource("myRec")
    .addMethod(NOT_A_TYPE_METHOD, "foo(x: int) {}")
    .addInterfaceConstraint(i);
  testSingleInterface(i, r1);

  auto r2 = RecordSource("myRec")
    .addMethod(NOT_A_TYPE_METHOD, "foo(y: int) {}")
    .addInterfaceConstraint(i);
  testSingleInterface(i, r2, ErrorType::InterfaceMissingFn);
}

static void testFormalOrdering() {
  auto i = InterfaceSource("myInterface",
                           "proc Self.foo(x: int, y: int, z: int);");
  auto r1 = RecordSource("myRec")
    .addMethod(NOT_A_TYPE_METHOD, "foo(x: int, y: int, z: int) {}")
    .addInterfaceConstraint(i);
  testSingleInterface(i, r1);

  auto r2 = RecordSource("myRec")
    .addMethod(NOT_A_TYPE_METHOD, "foo(y: int, x: int, z: int) {}")
    .addInterfaceConstraint(i);
  testSingleInterface(i, r2, ErrorType::InterfaceReorderedFnFormals);
}

static void expectError(const std::string& program, ErrorType error) {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto filePath = UniqueString::get(context, "file.chpl");
  setFileText(context, filePath, program);
  auto& modVec = parseToplevel(context, filePath);
  assert(modVec.size() == 1);
  resolveModule(context, modVec[0]->id());

  assert(findError(guard.errors(), error));
  guard.realizeErrors();
}

static void testImplementsInvalidInterface() {
  expectError(
    R"""(
      record I {

      }
      int implements I;
    )""", ErrorType::InvalidImplementsInterface);
}

static void testImplementsInvalidActual() {
  expectError(
    R"""(
      interface I {

      }
      var x = 3;
      x implements I;
    )""", ErrorType::InvalidImplementsActual);

  expectError(
    R"""(
    interface I {

    }
    var x = 3;
    implements I(x);
    )""", ErrorType::InvalidImplementsActual);
};

static void testImplementsDuplicate() {
  expectError(
    R"""(
    interface I {

    }
    record R : I, I {

    }
    )""", ErrorType::InterfaceMultipleImplements);
};

static void testInvalidImplementsArity() {
  expectError(
    R"""(
    interface Unary {}
    implements Unary(int, bool);
    )""", ErrorType::InvalidImplementsArity);

  expectError(
    R"""(
    interface Unary(Self) {}
    implements Unary(int, bool);
    )""", ErrorType::InvalidImplementsArity);

  expectError(
    R"""(
    interface Binary(L, R) {}
    implements Binary(int);
    )""", ErrorType::InvalidImplementsArity);

  expectError(
    R"""(
    interface Binary(L, R) {}
    implements Binary(int, bool, real);
    )""", ErrorType::InvalidImplementsArity);

  expectError(
    R"""(
    interface Binary(L, R) {}
    record R : Binary {}
    )""", ErrorType::InterfaceNaryInInherits);
};

int main() {
  // tests for "basic" interface resolution (unary interfaces)
  testRequiredMethodNoArgs();
  testRequiredMethodNoArgsDefault();
  testRequiredMethodOneArg();
  testRequiredMethodAmbiguous();
  testRequiredMethodWrongIntent();
  testTwoRequiredMethods();
  testBasicGeneric();
  testBasicGenericTypeQuery();
  testDependentGeneric();
  testAssociatedType();
  testAssociatedTypeInFn();
  testFormalNaming();
  testFormalOrdering();

  // tests for the various error message cases
  testImplementsInvalidInterface();
  testImplementsInvalidActual();
  testImplementsDuplicate();
  testInvalidImplementsArity();
}
