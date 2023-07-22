/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

record chpl_PrivatizedDistHelper {
  type instanceType;
  var _pid:int;  // only used when privatized
  pragma "owned"
  var _instance: instanceType;
  var _unowned:bool; // 'true' for the result of 'getDistribution',
                     // in which case, the record destructor should
                     // not attempt to delete the _instance.

  inline proc _value {
    if _isPrivatized(_instance) {
      return chpl_getPrivatizedCopy(_instance.type, _pid);
    } else {
      return _instance;
    }
  }

  forwarding _value except targetLocales;

  inline proc _do_destroy() {
    if ! _unowned && ! _instance.singleton() {
      on _instance {
        // Count the number of domains that refer to this distribution.
        // and mark the distribution to be freed when that number reaches 0.
        // If the number is 0, .remove() returns the distribution
        // that should be freed.
        var distToFree = _instance.remove();
        if distToFree != nil {
          _delete_dist(distToFree!, _isPrivatized(_instance));
        }
      }
    }
  }

  proc deinit() {
    _do_destroy();
  }

    /* This is a workaround for an internal failure I experienced when
       this code was part of newRectangularDom() and relied on split init:
       var x;
       if __prim(...) then x = ...;
       else if __prim(...) then x = ...;
       else compilerError(...); */
    proc chpl_dsiNRDhelp(param rank, type idxType, param strides, ranges) {

      // Due to a bug, see library/standard/Reflection/primitives/ResolvesDmap
      // we use "method call resolves" instead of just "resolves".
      if __primitive("method call resolves", _value, "dsiNewRectangularDom",
                     rank, idxType, strides, ranges) {
        return _value.dsiNewRectangularDom(rank, idxType, strides, ranges);
      }

      // The following supports deprecation by Vass in 1.31 to implement #17131
      // Once range.stridable is removed, replace chpl_dsiNRDhelp() with
      //   var x = _value.dsiNewRectangularDom(..., strides, ranges);
      // and uncomment proc dsiNewRectangularDom() in ChapelDistribution.chpl

      param stridable = strides.toStridable();
      const ranges2 = chpl_convertRangeTuple(ranges, stridable);
      if __primitive("method call resolves", _value, "dsiNewRectangularDom",
                     rank, idxType, stridable, ranges2) {

        compilerWarning("the domain map '", _value.type:string,
          "' needs to be updated from 'stridable: bool' to",
          " 'strides: strideKind' because 'stridable' is deprecated");

        return _value.dsiNewRectangularDom(rank, idxType, stridable, ranges2);
      }

      compilerError("rectangular domains are not supported by",
                    " the distribution ", this.type:string);
    }

    proc newRectangularDom(param rank: int, type idxType,
                           param strides: strideKind,
                           ranges: rank*range(idxType, boundKind.both, strides),
                           definedConst: bool = false) {
      var x = chpl_dsiNRDhelp(rank, idxType, strides, ranges);

      x.definedConst = definedConst;

      if x.linksDistribution() {
        _value.add_dom(x);
      }
      return x;
    }

    proc newRectangularDom(param rank: int, type idxType,
                           param strides: strideKind,
                           definedConst: bool = false) {
      var ranges: rank*range(idxType, boundKind.both, strides);
      return newRectangularDom(rank, idxType, strides, ranges, definedConst);
    }

    proc idxToLocale(ind) do return _value.dsiIndexToLocale(ind);

    proc writeThis(f) throws {
      f.write(_value);
    }

    @chpldoc.nodoc
    proc serialize(writer, ref serializer) throws {
      writer.write(_value);
    }

    proc displayRepresentation() { _value.dsiDisplayRepresentation(); }

    /*
       Return an array of locales over which this distribution was declared.
    */
    proc targetLocales() const ref {
      return _value.dsiTargetLocales();
    }


}