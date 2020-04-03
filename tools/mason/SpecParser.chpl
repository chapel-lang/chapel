/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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


private use List;
use MasonUtils;
use MasonExternal;
use Regexp;

config const debugSpecParser=false;

/*
  Interface for the Spack spec parser

  This module is used in the MasonExternal file to
  retrieve information from the spec listed in the
  Mason.toml file under the [external] table.

  This parser is not meant to retrieve all information
  from the spec as we only need the following to use
  packages from Spack
     - package name
     - version
     - compiler

  The compiler, if not listed, is inferred from the
  CHPL_TARGET_COMPILER. Everything else included in
  the spec is passed to spack to handle for us.

  Note: Spack has a different syntax than
  Chapel for version ranges. They use ':' as a
  separator instead of '..'
 */
proc getSpecFields(spec: string) {
  var specFields: 4*string;
  try! {
    var tokenList = readSpec(spec);
    const specInfo = parseSpec(tokenList);
    var compiler = specInfo[2];
    if compiler.size < 1 {
      compiler = inferCompiler();
    }
    specFields = (specInfo[0], specInfo[1], compiler,
                  specInfo[3]);
  }
  catch e: MasonError {
    stderr.writeln(e.message());
  }
  return specFields;
}


private proc inferCompiler() throws {
  var compiler = CHPL_TARGET_COMPILER;
  if compiler.size < 1 {
    throw new owned MasonError("Could not infer target compiler");
  }
  return compiler;
}


proc readSpec(spec: string) {
  const pkg = "([A-Za-z0-9\\-]+)",
        vers = "(\\@.[^%]+)",
        compiler = "(\\%[A-Za-z0-9\\-\\_]+[\\-a-zA-Z]*)",
        variantInclude = "(\\+[A-Za-z0-9\\-\\_]+)",
        variantExclude = "(\\~[A-Za-z0-9\\-\\_]+)",
        dependency = "(\\^[a-zA-Z]*?[0-9]*?)",
        arch = "([A-Za-z0-9\\-\\_]+\\=[A-Za-z0-9\\-\\_]+)",
        emptyArch = "([A-Za-z0-9\\-\\_]+\\=)",
        versRange = "(\\@.+\\:{1}\\.[^%]+)",
        minVers = "(\\@.+\\:)",
        maxVers = "(\\@\\:.[^%]+)";

  var tokenList: list(string);
  const pattern = compile("|".join(versRange,
                                   vers,
                                   minVers,
                                   maxVers,
                                   compiler,
                                   variantInclude,
                                   variantExclude,
                                   dependency,
                                   emptyArch,
                                   arch,
                                   pkg));


  if debugSpecParser then writeln(spec);
  for token in pattern.split(spec) {
    if token.strip().size != 0 {
      if debugSpecParser {
        writeln("Token: " + token);
        writeln();
      }
      tokenList.append(token);
    }
  }
  return tokenList;
}


proc parseSpec(ref tokenList: list(string)) throws {

  const rVers = compile("(\\@.+)");
  const rCompiler = compile("(\\%[A-Za-z0-9\\-\\_]+[\\-a-zA-Z]*)");
  const rMinVers = compile("(\\@.+\\:)");
  const rMaxVers = compile("(\\@\\:.[^%]+)");
  const rVersRange = compile("(\\@.+\\:{1}\\.[^%]+)");

  // required fields
  //   - package name
  //   - version
  var package: string;
  var pkgVersion: string;
  var compiler: string;
  var compVersion: string;
  // This includes more than just variants
  // variants, arch, dependencies etc...
  var variants: list(string);

  if tokenList.size < 0 {
    throw new owned MasonError("Empty spec in Mason.toml");
  }
  while tokenList.size > 0 {
    var toke = tokenList.pop(1);

    // get package name (should always be first token)
    if package.size < 1 {
      package = toke;
    }
    // Match a package, compiler or dep version
    // could be version, version range, min version or max version
    else if rVers.match(toke).matched == true
      || rVersRange.match(toke).matched == true
      || rMinVers.match(toke).matched == true
      || rMaxVers.match(toke).matched == true {

      if pkgVersion.size < 1 {
        pkgVersion = toke.strip("@");
      }
      else if compVersion.size < 1 {
        compVersion = toke;
        compiler = "".join(compiler, compVersion);
      }
      else {
        variants.append(toke);
      }
    }
    else if rCompiler.match(toke) {
      // throw an error if we reach a compiler without seeing
      // a package version.
      if pkgVersion.size < 1 {
        throw new owned MasonError("No package version found in spec");
      }
      // Match package compiler if one hasnt been matched
      if compiler.size < 1 {
        compiler = toke.strip("%");
      }
      else {
        variants.append(toke);
      }
    }
    else {
      // catch corner case where some compilers like "-"
      // include non-spec characters e.g. clang@9.0.0-apple
      if !toke.startsWith("-") {
        variants.append(toke);
      }
    }
  }
  return (package, pkgVersion, compiler, " ".join(variants.these()).strip());
}


