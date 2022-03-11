/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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

module ChapelAutoAggregation {
  private use CopyAggregation;

  pragma "aggregator generator"
  proc chpl_srcAggregatorFor(arr: []) {
    return new SrcAggregator(arr.eltType);
  }

  // we can't do dom: domain here, it causes resolution issues
  pragma "aggregator generator"
  proc chpl_srcAggregatorFor(dom) where isDomain(dom) {
    return new SrcAggregator(dom.idxType);
  }

  pragma "aggregator generator"
  proc chpl_srcAggregatorFor(arr) {
    return nil;  // return type signals that we shouldn't aggregate
  }

  pragma "aggregator generator"
  proc chpl_dstAggregatorFor(arr: []) {
    return new DstAggregator(arr.eltType);
  }

  pragma "aggregator generator"
  proc chpl_dstAggregatorFor(dom) where isDomain(dom) {
    // this is only called if the user has:
    //
    // forall i in myDomain { i = foo(); }
    //
    // We want that code to fail with proper error message, so we have this
    // function but return nil from it.
    return nil;
  }

  pragma "aggregator generator"
  proc chpl_dstAggregatorFor(arr) {
    return nil;  // return type signals that we shouldn't aggregate
  }

  private proc elemTypeSupportsAggregation(type t) param {
    return isPODType(t);
  }

  proc chpl__arrayIteratorYieldsLocalElements(x) param {
    if isArray(x) {
      if elemTypeSupportsAggregation(x.eltType) { // I have no idea if we can do this for wide pointers
        return x.iteratorYieldsLocalElements();
      }
    }
    else if isDomain(x) {
      return x.iteratorYieldsLocalElements();
    }
    return false;
  }

  // make sure to resolve this, so that we can give a more meaningful error
  // message
  proc chpl__arrayIteratorYieldsLocalElements(type a) param {
    return false;
  }
}
