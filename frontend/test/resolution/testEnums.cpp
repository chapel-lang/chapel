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

#include "test-resolution.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Record.h"
#include "chpl/uast/Variable.h"

static void test1() {
  auto context = buildStdContext();
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                         enum color {
                           red, green, blue
                         }

                         var x = color.red;
                         )"""");
  assert(qt.kind() == QualifiedType::PARAM);
  assert(qt.type() && qt.type()->isEnumType());
  assert(qt.param() && qt.param()->isEnumParam());

  auto et = qt.type()->toEnumType();
  auto ep = qt.param()->toEnumParam();
  assert(et->id().contains(ep->value().id));
  auto enumAst = parsing::idToAst(context, et->id());
  assert(enumAst && enumAst->isEnum());
  auto elemAst = parsing::idToAst(context, ep->value().id);
  assert(elemAst && elemAst->isEnumElement());
}

static void test2() {
  auto context = buildStdContext();
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                         enum color {
                           red, red, blue
                         }

                         var x = color.red;
                         )"""");
  assert(qt.kind() == QualifiedType::CONST_VAR);
  assert(qt.type() && qt.type()->isEnumType());
}

static void test3() {
  auto context = buildStdContext();
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                         enum color {
                           green, blue
                         }

                         var x = color.red;
                         )"""");
  assert(qt.kind() == QualifiedType::UNKNOWN);
  assert(qt.type() && qt.type()->isErroneousType());
}

// Test numeric conversions of enums
static void test4() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context,
      R"""(
      enum color {
        red = 1,
        green,
        blue = 42,
        gold
      }
      param a = color.red : int;
      param b = color.green : int;
      param c = color.blue : int;
      param d = color.gold : int;
      )""", {"a", "b", "c", "d"});

  ensureParamInt(vars.at("a"), 1);
  ensureParamInt(vars.at("b"), 2);
  ensureParamInt(vars.at("c"), 42);
  ensureParamInt(vars.at("d"), 43);
}

static const std::map<std::string, QualifiedType>
enumConstantValues(Context* context, const QualifiedType& qt) {
  assert(qt.isType() && qt.type());
  auto enumType = qt.type()->toEnumType();
  assert(enumType);

  auto id = enumType->id();
  auto enumAst = parsing::idToAst(context, id)->toEnum();
  assert(enumAst);

  std::map<std::string, QualifiedType> result;
  auto& computedMap = computeNumericValuesOfEnumElements(context, id);
  for (auto elem : enumAst->enumElements()) {
    auto it = computedMap.find(elem->id());
    if (it == computedMap.end()) {
      continue;
    }
    result[std::string(elem->name().c_str())] = it->second;
  }

  return result;
}

static void test5() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context,
      R"""(
      enum color {
        red = "hello",
        green,
        blue = 42,
        gold
      }
      type t = color;
      param a = color.red : int;
      param b = color.green : int;
      param c = color.blue : int;
      param d = color.gold : int;
      )""", {"t", "a", "b", "c", "d"});


  // First, ensure that the actual computation marks 'red' as erroneous

  auto qtT = vars.at("t");
  auto enumValuesByName = enumConstantValues(context, qtT);

  assert(enumValuesByName.at("red").isErroneousType());
  assert(enumValuesByName.at("green").isUnknown());
  ensureParamInt(enumValuesByName.at("blue"), 42);
  ensureParamInt(enumValuesByName.at("gold"), 43);

  assert(vars.at("a").isUnknown());
  assert(vars.at("b").isUnknown());
  ensureParamInt(vars.at("c"), 42);
  ensureParamInt(vars.at("d"), 43);

  // Expect an error from "hello".
  assert(guard.numErrors() == 1);
  assert(guard.error(0)->type() == ErrorType::EnumInitializerNotInteger);
  guard.realizeErrors();
}

static void test6() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context,
      R"""(
      enum color {
        negative = __primitive("-", 0, 1),
        huge = 0x8000000000000000,
      }
      type t = color;
      param a = color.negative : int;
      param b = color.huge : uint;
      )""", {"t", "a", "b"});


  // First, ensure that the actual computation marks 'red' as erroneous

  auto qtT = vars.at("t");
  auto enumValuesByName = enumConstantValues(context, qtT);

  assert(enumValuesByName.at("negative").isErroneousType());
  ensureParamUint(enumValuesByName.at("huge"), 0x8000000000000000);

  assert(vars.at("a").isUnknown());
  ensureParamUint(vars.at("b"), 0x8000000000000000);

  // Expect an error from unfitable types.
  assert(guard.numErrors() == 1);
  assert(guard.error(0)->type() == ErrorType::NoTypeForEnumElem);
  guard.realizeErrors();
}

static void test7() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context,
      R"""(
      enum color {
        red,
        green,
        blue
      }
      type t = color;
      param a = color.red : int;
      param b = color.green : int;
      param c = color.blue : int;
      )""", {"t", "a", "b", "c"});


  auto qtT = vars.at("t");
  auto enumValuesByName = enumConstantValues(context, qtT);
  assert(enumValuesByName.empty());

  assert(vars.at("a").isErroneousType());
  assert(vars.at("b").isErroneousType());
  assert(vars.at("c").isErroneousType());

  assert(guard.numErrors() == 3);
  for (auto& err : guard.errors()) {
    assert(err->type() == ErrorType::EnumAbstract);
  }
  guard.realizeErrors();
}

static void test8() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context,
      R"""(
      var x = 1;
      enum color {
        red = x,
        green,
        blue
      }
      type t = color;
      param a = color.red : int;
      param b = color.green : int;
      param c = color.blue : int;
      )""", {"t", "a", "b", "c"});

  auto qtT = vars.at("t");
  auto enumValuesByName = enumConstantValues(context, qtT);
  assert(enumValuesByName.at("red").isErroneousType());
  assert(enumValuesByName.at("green").isUnknown());
  assert(enumValuesByName.at("blue").isUnknown());

  assert(vars.at("a").isUnknown());
  assert(vars.at("b").isUnknown());
  assert(vars.at("c").isUnknown());

  assert(guard.numErrors() == 1);
  assert(guard.error(0)->type() == ErrorType::EnumInitializerNotParam);
  guard.realizeErrors();
}

static void test9() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context,
      R"""(
      var x = 1;
      enum color {
        red,
        green = 0,
        blue
      }
      type t = color;
      param a = color.red : int;
      param b = color.green : int;
      param c = color.blue : int;
      )""", {"t", "a", "b", "c"});

  auto qtT = vars.at("t");
  auto enumValuesByName = enumConstantValues(context, qtT);
  assert(enumValuesByName.find("red") == enumValuesByName.end());
  ensureParamInt(enumValuesByName.at("green"), 0);
  ensureParamInt(enumValuesByName.at("blue"), 1);

  assert(vars.at("a").isErroneousType());
  ensureParamInt(vars.at("b"), 0);
  ensureParamInt(vars.at("c"), 1);

  assert(guard.numErrors() == 1);
  assert(guard.error(0)->type() == ErrorType::EnumValueAbstract);
  guard.realizeErrors();
}

static void test10() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context,
      R"""(
      enum color {
        red = 0,
        green,
        blue
      }
      type t = color;
      param a = 3 : color;
      )""", {"t", "a"});

  auto qtT = vars.at("t");
  auto enumValuesByName = enumConstantValues(context, qtT);
  ensureParamInt(enumValuesByName.at("red"), 0);
  ensureParamInt(enumValuesByName.at("green"), 1);
  ensureParamInt(enumValuesByName.at("blue"), 2);

  assert(vars.at("a").isErroneousType());
  assert(guard.numErrors() == 1);
  assert(guard.error(0)->type() == ErrorType::NoMatchingEnumValue);
  guard.realizeErrors();
}

static void test11() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  // Production allows multiple constants to have the same numeric value.
  // When casting backwards, the first matching constant is picked.
  auto vars = resolveTypesOfVariables(context,
      R"""(
      enum color {
        red = 0,
        green = 0,
        blue = 1,
        gold = 1,
      }
      type t = color;
      param a = 0 : color;
      param b = 1 : color;
      )""", {"t", "a", "b"});

  auto qtT = vars.at("t");
  auto enumValuesByName = enumConstantValues(context, qtT);
  ensureParamInt(enumValuesByName.at("red"), 0);
  ensureParamInt(enumValuesByName.at("green"), 0);
  ensureParamInt(enumValuesByName.at("blue"), 1);
  ensureParamInt(enumValuesByName.at("gold"), 1);

  auto param0 = vars.at("a").param();
  assert(param0 && param0->isEnumParam());
  assert(param0->toEnumParam()->value().id.postOrderId() == 1);

  auto param1 = vars.at("b").param();
  assert(param1 && param1->isEnumParam());
  assert(param1->toEnumParam()->value().id.postOrderId() == 5);
}

static void test12() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  // Production allows multiple constants to have the same numeric value.
  // When casting backwards, the first matching constant is picked.
  auto vars = resolveTypesOfVariables(context,
      R"""(
      enum color {
        red = 0,
        green = 1,
        blue = 2,
        gold = 3,
      }
      var redI: int = 0;
      var greenU: uint = 1;
      var blueI8: int(8) = 2;
      var goldU8: uint(8) = 3;
      var a = redI : color;
      var b = greenU : color;
      var c = blueI8 : color;
      var d = goldU8 : color;
      )""", {"a", "b", "c", "d"});

  assert(vars.at("a").type()->isEnumType() && vars.at("a").type()->toEnumType()->name() == "color");
  assert(vars.at("b").type()->isEnumType() && vars.at("b").type()->toEnumType()->name() == "color");
  assert(vars.at("c").type()->isEnumType() && vars.at("c").type()->toEnumType()->name() == "color");
  assert(vars.at("d").type()->isEnumType() && vars.at("d").type()->toEnumType()->name() == "color");
}

static void test13() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  // Production allows multiple constants to have the same numeric value.
  // When casting backwards, the first matching constant is picked.
  auto vars = resolveTypesOfVariables(context,
      R"""(
      enum color {
        red = 0,
        green = 1,
        blue = 2,
        gold = 3,
      }
      var red = color.red;
      var a = red : int;
      var b = red : uint;
      var c = red : int(8);
      var d = red : uint(8);
      )""", {"red", "a", "b", "c", "d"});

  assert(vars.at("red").type()->isEnumType());
  assert(vars.at("a").type()->isIntType() && vars.at("a").type()->toIntType()->bitwidth() == IntType::defaultBitwidth());
  assert(vars.at("b").type()->toUintType() && vars.at("b").type()->toUintType()->bitwidth() == UintType::defaultBitwidth());
  assert(vars.at("c").type()->isIntType() && vars.at("c").type()->toIntType()->bitwidth() == 8);
  assert(vars.at("d").type()->isUintType() && vars.at("d").type()->toUintType()->bitwidth() == 8);
}

static void test14() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  // Production allows multiple constants to have the same numeric value.
  // When casting backwards, the first matching constant is picked.
  auto vars = resolveTypesOfVariables(context,
      R"""(
      enum color {
        red,
        green,
        blue,
        gold,
      }
      var redI: int = 0;
      var greenU: uint = 1;
      var blueI8: int(8) = 2;
      var goldU8: uint(8) = 3;
      var a = redI : color;
      var b = greenU : color;
      var c = blueI8 : color;
      var d = goldU8 : color;
      )""", {"a", "b", "c", "d"});

  for (auto pair : vars) {
    assert(pair.second.isErroneousType());
  }

  assert(guard.numErrors() == 4);
  for (int i = 0; i < 4; i ++) {
    assert(guard.error(i)->type() == ErrorType::NoMatchingCandidates);
  }
  guard.realizeErrors();
}


static void test15() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  // Production allows multiple constants to have the same numeric value.
  // When casting backwards, the first matching constant is picked.
  auto vars = resolveTypesOfVariables(context,
      R"""(
      enum color {
        red,
        green,
        blue,
        gold,
      }
      var red = color.red;
      var a = red : int;
      var b = red : uint;
      var c = red : int(8);
      var d = red : uint(8);
      )""", {"a", "b", "c", "d"});

  for (auto pair : vars) {
    assert(pair.second.isErroneousType());
  }

  assert(guard.numErrors() == 4);
  for (int i = 0; i < 4; i ++) {
    assert(guard.error(i)->type() == ErrorType::NoMatchingCandidates);
  }
  guard.realizeErrors();
}

static void test16() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context,
      R"""(
      enum property {
        shape, color, size
      }
      enum color {
        red, green, blue
      }
      enum colorDifferent {
        magenta, cyan, yellow
      }
      proc type colorDifferent.size param do return 2;

      var a = property.size;
      param b = property.size;
      var c = color.size;
      param d = color.size;
      var e = colorDifferent.size;
      param f = colorDifferent.size;
      )""", {"a", "b", "c", "d", "e", "f"});

  assert(vars.at("a").type());
  assert(vars.at("a").type()->isEnumType());
  assert(vars.at("b").type());
  assert(vars.at("b").type()->isEnumType());
  assert(vars.at("b").param());
  assert(vars.at("b").param()->isEnumParam());
  assert(vars.at("c").type());
  assert(vars.at("c").type()->isIntType());
  assert(vars.at("d").type());
  assert(vars.at("d").type()->isIntType());
  assert(vars.at("d").param());
  assert(vars.at("d").param()->isIntParam());
  assert(vars.at("d").param()->toIntParam()->value() == 3);
  assert(vars.at("e").type());
  assert(vars.at("e").type()->isIntType());
  assert(vars.at("f").type());
  assert(vars.at("f").type()->isIntType());
  assert(vars.at("f").param());
  assert(vars.at("f").param()->isIntParam());
  assert(vars.at("f").param()->toIntParam()->value() == 2);
}

static void test17() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context,
      R"""(
      enum color {
        red, green, blue
      }
      var tmp = 0;
      param tmpParam = 0;
      var a = chpl__orderToEnum(tmp, color);
      param b = chpl__orderToEnum(tmp, color); // error
      param c = chpl__orderToEnum(tmpParam, color);
      param d = chpl__orderToEnum(0, color);
      param e = chpl__orderToEnum(1, color);
      param f = chpl__orderToEnum(2, color);
      )""", {"a", "b", "c", "d", "e", "f"});

  assert(vars.at("a").type());
  assert(vars.at("a").type()->isEnumType());
  assert(!vars.at("a").param());
  assert(vars.at("b").isErroneousType());
  assert(vars.at("c").type());
  assert(vars.at("c").type()->isEnumType());
  assert(vars.at("c").param());
  assert(vars.at("c").param()->isEnumParam());
  assert(vars.at("c").param()->toEnumParam()->value().id.postOrderId() == 0);
  assert(vars.at("d").type());
  assert(vars.at("d").type()->isEnumType());
  assert(vars.at("d").param());
  assert(vars.at("d").param()->isEnumParam());
  assert(vars.at("d").param()->toEnumParam()->value().id.postOrderId() == 0);
  assert(vars.at("e").type());
  assert(vars.at("e").type()->isEnumType());
  assert(vars.at("e").param());
  assert(vars.at("e").param()->isEnumParam());
  assert(vars.at("e").param()->toEnumParam()->value().id.postOrderId() == 1);
  assert(vars.at("f").type());
  assert(vars.at("f").type()->isEnumType());
  assert(vars.at("f").param());
  assert(vars.at("f").param()->isEnumParam());
  assert(vars.at("f").param()->toEnumParam()->value().id.postOrderId() == 2);

  assert(guard.realizeErrors() == 1);
}

static void test18() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context,
      R"""(
      enum color {
        red, green, blue
      }
      var tmp = color.red;
      param tmpParam = color.red;
      var a = chpl__enumToOrder(tmp);
      param b = chpl__enumToOrder(tmp); // error
      param c = chpl__enumToOrder(tmpParam);
      param d = chpl__enumToOrder(color.red);
      param e = chpl__enumToOrder(color.green);
      param f = chpl__enumToOrder(color.blue);
      )""", {"a", "b", "c", "d", "e", "f"});

  assert(vars.at("a").type());
  assert(vars.at("a").type()->isIntType());
  assert(!vars.at("a").param());
  assert(vars.at("b").isErroneousType());
  assert(vars.at("c").type());
  assert(vars.at("c").type()->isIntType());
  assert(vars.at("c").param());
  assert(vars.at("c").param()->isIntParam());
  assert(vars.at("c").param()->toIntParam()->value() == 0);
  assert(vars.at("d").type());
  assert(vars.at("d").type()->isIntType());
  assert(vars.at("d").param());
  assert(vars.at("d").param()->isIntParam());
  assert(vars.at("d").param()->toIntParam()->value() == 0);
  assert(vars.at("e").type());
  assert(vars.at("e").type()->isIntType());
  assert(vars.at("e").param());
  assert(vars.at("e").param()->isIntParam());
  assert(vars.at("e").param()->toIntParam()->value() == 1);
  assert(vars.at("f").type());
  assert(vars.at("f").type()->isIntType());
  assert(vars.at("f").param());
  assert(vars.at("f").param()->isIntParam());
  assert(vars.at("f").param()->toIntParam()->value() == 2);

  assert(guard.realizeErrors() == 1);
}

// same as test18, except places the code in an internal module to ensure
// it still works. This is a regression test; internal modules don't auto-use
// `ChapelBase`, and thus didn't have `==` available.
static void test18internal() {
  // duplicated from buildStdContext
  std::string chpl_home;
  if (const char* chpl_home_env = getenv("CHPL_HOME")) {
    chpl_home = chpl_home_env;
  } else {
    printf("CHPL_HOME must be set");
    exit(1);
  }
  Context::Configuration config;
  config.chplHome = chpl_home;
  Context ctx(config);
  // end duplicate

  auto context = &ctx;
  ErrorGuard guard(context);

  // duplicate/inline/fuse from buildStdContext + setupModuleSearchPaths
  // unlike those functions, configures `myint` to be an internal path
  {
    auto& chplHomeStr = context->chplHome();
    CHPL_ASSERT(chplHomeStr != "");
    auto chplEnv = context->getChplEnv();
    CHPL_ASSERT(!chplEnv.getError() && "printchplenv error handling not implemented");

    // CHPL_MODULE_PATH isn't always in the output; check if it's there.
    auto it = chplEnv->find("CHPL_MODULE_PATH");
    auto chplModulePath = (it != chplEnv->end()) ? it->second : "";
    setupModuleSearchPaths(context,
                           chplHomeStr,
                           "",
                           false,
                           chplEnv->at("CHPL_LOCALE_MODEL"),
                           false,
                           chplEnv->at("CHPL_TASKS"),
                           chplEnv->at("CHPL_COMM"),
                           chplEnv->at("CHPL_SYS_MODULES_SUBDIR"),
                           chplModulePath,
                           {"myint"},  // prependInternalModulePaths
                           {},  // prependStandardModulePaths
                           {}, // cmdLinePaths
                           {});
  }
  // end duplicate

  auto path = UniqueString::get(context, "myint/input.chpl");
  setFileText(context, path,
      R"""(
      // internal modules don't get auto-generated assignment operators
      use ChapelBase only =;

      enum color {
        red, green, blue
      }
      var tmp = color.red;
      param tmpParam = color.red;
      var a = chpl__enumToOrder(tmp);
      param b = chpl__enumToOrder(tmp); // error
      param c = chpl__enumToOrder(tmpParam);
      param d = chpl__enumToOrder(color.red);
      param e = chpl__enumToOrder(color.green);
      param f = chpl__enumToOrder(color.blue);
      )""");

  auto& br = parseFileToBuilderResultAndCheck(context, path, {});
  assert(br.numTopLevelExpressions() == 1);
  auto mod = br.topLevelExpression(0)->toModule();
  auto vars = resolveTypesOfVariables(context, mod,
      {"a", "b", "c", "d", "e", "f"});

  assert(vars.at("a").type());
  assert(vars.at("a").type()->isIntType());
  assert(!vars.at("a").param());
  assert(vars.at("b").isErroneousType());
  assert(vars.at("c").type());
  assert(vars.at("c").type()->isIntType());
  assert(vars.at("c").param());
  assert(vars.at("c").param()->isIntParam());
  assert(vars.at("c").param()->toIntParam()->value() == 0);
  assert(vars.at("d").type());
  assert(vars.at("d").type()->isIntType());
  assert(vars.at("d").param());
  assert(vars.at("d").param()->isIntParam());
  assert(vars.at("d").param()->toIntParam()->value() == 0);
  assert(vars.at("e").type());
  assert(vars.at("e").type()->isIntType());
  assert(vars.at("e").param());
  assert(vars.at("e").param()->isIntParam());
  assert(vars.at("e").param()->toIntParam()->value() == 1);
  assert(vars.at("f").type());
  assert(vars.at("f").type()->isIntType());
  assert(vars.at("f").param());
  assert(vars.at("f").param()->isIntParam());
  assert(vars.at("f").param()->toIntParam()->value() == 2);

  assert(guard.realizeErrors() == 1);
}

static void test19() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context,
      R"""(
      enum color {
        red, green, blue
      }
      for c in color {
        var res = c;
      }
      )""", {"res"});

  assert(guard.realizeErrors() == 0);
  assert(vars.at("res").type());
  assert(vars.at("res").type()->isEnumType());
}

// Param cast to string
static void test20() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context,
      R"""(
        enum colors {red, green, blue};

        param c = colors.red;
        param s = c:string;

        param x = colors.red:string;
        param y = colors.green:string;
        param z = colors.blue:string;

        record R {
          param p : colors;
        }

        var r = new R(colors.green);
      )""", {"s", "x", "y", "z", "r"});

  assert(guard.realizeErrors() == 0);

  auto check = [] (QualifiedType qt, std::string text) {
    assert(qt.type()->isStringType());
    assert(qt.param()->toStringParam()->value() == text);
  };

  check(vars.at("s"), "red");
  check(vars.at("x"), "red");
  check(vars.at("y"), "green");
  check(vars.at("z"), "blue");

  std::ostringstream oss;
  vars.at("r").type()->stringify(oss, StringifyKind::CHPL_SYNTAX);
  assert(oss.str() == "R(green)");
}

// Non-param cast to string
static void test21() {
  auto context = buildStdContext();
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context,
      R"""(
        enum colors {red, green, blue};

        param c = colors.red;
        var s = c:string;

        var x = colors.red:string;
        var y = colors.green:string;
        var z = colors.blue:string;
      )""", {"s", "x", "y", "z"});

  assert(guard.realizeErrors() == 0);

  auto check = [] (QualifiedType qt, std::string text) {
    assert(qt.type()->isStringType());
    assert(!qt.isParam());
  };

  check(vars.at("s"), "red");
  check(vars.at("x"), "red");
  check(vars.at("y"), "green");
  check(vars.at("z"), "blue");
}

static void test22() {
  auto context = buildStdContext();
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                         proc id(param x) param do return x;
                         proc foo() param {
                           enum color {
                             red, green, blue
                           }

                           return id(color.red);
                         }

                         var x = foo();
                         )"""");
  assert(qt.kind() == QualifiedType::PARAM);
  assert(qt.type() && qt.type()->isEnumType());
  assert(qt.param() && qt.param()->isEnumParam());

  ensureParamEnumStr(qt, "red");
}

int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();
  test8();
  test9();
  test10();
  test11();
  test12();
  test13();
  test14();
  test15();
  test16();
  test17();
  test18();
  test18internal();
  test19();
  test20();
  test21();
  test22();

  return 0;
}
