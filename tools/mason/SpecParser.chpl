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

  This parser expects a resolved spec, such that there
  are no version ranges.

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


/* Tokenize spec into list of tokens */
private proc readSpec(spec: string): list(string) {
  const pkgVersion = "([A-Za-z0-9\\-\\@\\.\\:]+)",
        compilerVersion = "(\\%[A-Za-z0-9\\_\\@\\.\\-]+)",
        variantInclude = "(\\+[A-Za-z0-9\\-\\_]+)",
        variantExclude = "(\\~[A-Za-z0-9\\-\\_]+)",
        dependency = "(\\^[a-zA-Z]*?[0-9]*?)",
        arch = "([A-Za-z0-9\\-\\_]+\\=[A-Za-z0-9\\-\\_]+)",
        emptyArch = "([A-Za-z0-9\\-\\_]+\\=)";

  var tokenList: list(string);
  const pattern = compile("|".join(pkgVersion,
                                   compilerVersion,
                                   variantInclude,
                                   variantExclude,
                                   dependency,
                                   emptyArch,
                                   arch));


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


private proc parseSpec(ref tokenList: list(string)): 4*string throws {

  const reCompilerVersion = compile("(\\%[A-Za-z0-9\\_\\@\\.\\-]+)");

  // required fields
  //   - package name
  //   - version
  var package: string;
  var packageVersion: string;
  var compiler: string;
  var compilerVersion: string;

  // This includes more than just variants
  // variants, arch, dependencies etc...
  var variants: list(string);

  if tokenList.size < 0 {
    throw new owned MasonError("Empty spec in Mason.toml");
  }
  while tokenList.size > 0 {
    var toke = tokenList.pop(1);

    // Package should be first token
    if package == '' {
      const pkgSplit = toke.split('@');
      package = pkgSplit[0];
      packageVersion = pkgSplit[1];
    } else if reCompilerVersion.match(toke).matched && compilerVersion == '' {
      const strippedToke = toke.strip('%');
      const compilerSplit = strippedToke.split('@');
      compiler = compilerSplit[0];
      if compilerSplit.size > 1 {
        // Note: This is currently unused
        compilerVersion = compilerSplit[1];
      }
    }
    else {
      variants.append(toke);
    }
  }
  return (package, packageVersion, compiler, " ".join(variants.these()).strip());
}


