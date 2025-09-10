#
# Copyright 2025-2025 Hewlett Packard Enterprise Development LP
# Other additional copyright holders may be indicated within.
#
# The entirety of this work is licensed under the Apache License,
# Version 2.0 (the "License"); you may not use this file except
# in compliance with the License.
#
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

import lldb
import re


def StringSummary(valobj, internal_dict):
    size = valobj.GetChildMemberWithName("size").GetValueAsUnsigned()
    buff = valobj.GetNonSyntheticValue().GetChildMemberWithName("buff")

    if size > 0:
        return buff.GetSummary()
    else:
        return '""'


class StringProvider:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj

        self.synthetic_children = {
            "size": self.valobj.GetChildMemberWithName("buffLen"),
        }

    def has_children(self):
        return self.num_children() > 0

    def num_children(self):
        return len(self.synthetic_children)

    def get_child_index(self, name):
        if name in self.synthetic_children:
            return list(self.synthetic_children.keys()).index(name)
        else:
            return -1

    def get_child_at_index(self, index):
        if index < 0 or index >= self.num_children():
            return None
        key = list(self.synthetic_children.keys())[index]
        return self.valobj.CreateValueFromExpression(
            key, self.synthetic_children[key].GetValue()
        )


range_regex_c = re.compile(
    r"^range_([a-zA-Z0-9_]+)_(both|low|high|neither)_(one|negOne|positive|negative|any)(?:_chpl)?$"
)
range_regex_llvm = re.compile(
    r"^ChapelRange::range\(([a-zA-Z0-9_()]+),(both|low|high|neither),(one|negOne|positive|negative|any)\)$"
)


def RangeSummary(valobj, internal_dict):
    low = (
        valobj.GetNonSyntheticValue()
        .GetChildMemberWithName("_low")
        .GetValueAsSigned()
    )
    high = (
        valobj.GetNonSyntheticValue()
        .GetChildMemberWithName("_high")
        .GetValueAsSigned()
    )

    typename = valobj.GetTypeName()
    match = range_regex_c.match(typename) or range_regex_llvm.match(typename)
    if not match:
        print(
            f"Range_Summary: type name '{typename}' did not match expected pattern"
        )
        return None

    def has_low_bound():
        return match.group(2) in ("low", "both")

    def has_high_bound():
        return match.group(2) in ("high", "both")

    # TODO: handle strides and alignment
    res = ""
    if has_low_bound():
        res += str(low)
    res += ".."
    if has_high_bound():
        res += str(high)

    return res


def RangeRecognizer(sbtype, internal_dict):
    typename = sbtype.GetName()
    match = range_regex_c.match(typename) or range_regex_llvm.match(typename)
    return match is not None


class RangeProvider:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj

        self.synthetic_children = {}
        if self._has_low_bound():
            self.synthetic_children["low"] = self.valobj.GetChildMemberWithName(
                "_low"
            )
        if self._has_high_bound():
            self.synthetic_children["high"] = (
                self.valobj.GetChildMemberWithName("_high")
            )

    def _has_low_bound(self):
        typename = self.valobj.GetTypeName()
        match = range_regex_c.match(typename) or range_regex_llvm.match(
            typename
        )
        if not match:
            print(
                f"RangeProvider: type name '{typename}' did not match expected pattern"
            )
            return False
        return match.group(2) in ("low", "both")

    def _has_high_bound(self):
        typename = self.valobj.GetTypeName()
        match = range_regex_c.match(typename) or range_regex_llvm.match(
            typename
        )
        if not match:
            print(
                f"RangeProvider: type name '{typename}' did not match expected pattern"
            )
            return False
        return match.group(2) in ("high", "both")

    def has_children(self):
        return self.num_children() > 0

    def num_children(self):
        return len(self.synthetic_children)

    def get_child_index(self, name):
        if name in self.synthetic_children:
            return list(self.synthetic_children.keys()).index(name)
        else:
            return -1

    def get_child_at_index(self, index):
        if index < 0 or index >= self.num_children():
            return None
        key = list(self.synthetic_children.keys())[index]
        return self.valobj.CreateValueFromExpression(
            key, self.synthetic_children[key].GetValue()
        )


# TODO: only handles DefaultRectangularDom for now
domain_regex_c = re.compile(
    r"^_domain_DefaultRectangularDom_([0-9]+)_([a-zA-Z0-9_]+)_(one|negOne|positive|negative|any)(?:_chpl)?$"
)
domain_regex_llvm = re.compile(r"^$")  # TODO


def DomainRecognizer(sbtype, internal_dict):
    typename = sbtype.GetName()
    match = domain_regex_c.match(typename) or domain_regex_llvm.match(typename)
    return match is not None


def DomainSummary(valobj, internal_dict):
    typename = valobj.GetTypeName()
    match = domain_regex_c.match(typename) or domain_regex_llvm.match(typename)
    if not match:
        print(
            f"Domain_Summary: type name '{typename}' did not match expected pattern"
        )
        return None

    rank = int(match.group(1))
    idx_type = match.group(2)
    stride_kind = match.group(3)

    ranges = (
        valobj.GetNonSyntheticValue()
        .GetChildMemberWithName("_instance")
        .GetNonSyntheticValue()
        .GetChildMemberWithName("ranges")
    )
    return "{" + (ranges.GetSummary()) + "}"


class DomainProvider:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj

        self.synthetic_children = {}
        typename = self.valobj.GetTypeName()
        match = domain_regex_c.match(typename) or domain_regex_llvm.match(
            typename
        )
        if not match:
            print(
                f"DomainProvider: type name '{typename}' did not match expected pattern"
            )
            return

        ranges = (
            self.valobj.GetNonSyntheticValue()
            .GetChildMemberWithName("_instance")
            .GetNonSyntheticValue()
            .GetChildMemberWithName("ranges")
        )
        self.synthetic_children["dim"] = ranges

    def has_children(self):
        return self.num_children() > 0

    def num_children(self):
        return len(self.synthetic_children)

    def get_child_index(self, name):
        if name in self.synthetic_children:
            return list(self.synthetic_children.keys()).index(name)
        else:
            return -1

    def get_child_at_index(self, index):
        if index < 0 or index >= self.num_children():
            return None
        key = list(self.synthetic_children.keys())[index]
        child_obj = self.synthetic_children[key]
        return self.valobj.CreateValueFromAddress(
            key, child_obj.GetLoadAddress(), child_obj.GetType()
        )


#
# homogenous tuples
#
homogeneous_tuple_regex_c = re.compile(
    r"^_tuple_([0-9]+)_star_([a-zA-Z0-9_]+)(?:_chpl)?$"
)
homogeneous_tuple_regex_llvm = re.compile(r"^$")  # TODO


def HomogeneousTupleRecognizer(sbtype, internal_dict):
    typename = sbtype.GetName()
    match = homogeneous_tuple_regex_c.match(
        typename
    ) or homogeneous_tuple_regex_llvm.match(typename)
    return match is not None


def HomogeneousTupleSummary(valobj, internal_dict):
    typename = valobj.GetTypeName()
    match = homogeneous_tuple_regex_c.match(
        typename
    ) or homogeneous_tuple_regex_llvm.match(typename)
    if not match:
        print(
            f"HomogeneousTuple_Summary: type name '{typename}' did not match expected pattern"
        )
        return None

    size = match.group(1)
    vals = []
    for i in range(int(size)):
        child = valobj.GetNonSyntheticValue().GetChildAtIndex(i)
        vals.append(child.GetSummary() or child.GetValue() or "")
    return f"{', '.join(vals)}"


class HomogeneousTupleProvider:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj

        self.synthetic_children = {}
        typename = self.valobj.GetTypeName()
        match = homogeneous_tuple_regex_c.match(
            typename
        ) or homogeneous_tuple_regex_llvm.match(typename)
        if not match:
            print(
                f"HomogeneousTupleProvider: type name '{typename}' did not match expected pattern"
            )
            return

        size = int(match.group(1))
        for i in range(size):
            self.synthetic_children[str(i)] = self.valobj.GetChildAtIndex(i)

    def has_children(self):
        return self.num_children() > 0

    def num_children(self):
        return len(self.synthetic_children)

    def get_child_index(self, name):
        if name in self.synthetic_children:
            return list(self.synthetic_children.keys()).index(name)
        else:
            return -1

    def get_child_at_index(self, index):
        if index < 0 or index >= self.num_children():
            return None
        key = list(self.synthetic_children.keys())[index]
        return self.valobj.CreateValueFromExpression(
            f"[{key}]", self.synthetic_children[key].GetValue()
        )


# TODO: for now we only handle DefaultRectangular arrays
# array
# _array_DefaultRectangularArr_1_int64_t_one_int64_t_int64_t_chpl
# rank, idx_type, stride_kind, element_type, idxSignedType

array_regex_c = re.compile(
    r"^_array_DefaultRectangularArr_([0-9]+)_([a-zA-Z0-9_]+)_(one|negOne|positive|negative|any)_([a-zA-Z0-9_]+)_([a-zA-Z0-9_]+)(?:_chpl)?$"
)
array_regex_llvm = re.compile(r"^$")  # TODO


def ArrayRecognizer(sbtype, internal_dict):
    typename = sbtype.GetName()
    match = array_regex_c.match(typename) or array_regex_llvm.match(typename)
    return match is not None


def ArraySummary(valobj, internal_dict):
    typename = valobj.GetTypeName()
    match = array_regex_c.match(typename) or array_regex_llvm.match(typename)
    if not match:
        print(
            f"Array_Summary: type name '{typename}' did not match expected pattern"
        )
        return None

    rank = int(match.group(1))
    idx_type = match.group(2)
    stride_kind = match.group(3)
    element_type = match.group(4)
    idx_signed_type = match.group(5)

    _instance = valobj.GetNonSyntheticValue().GetChildMemberWithName(
        "_instance"
    )
    domClass = _instance.GetChildMemberWithName("dom")
    data = _instance.GetChildMemberWithName("data")
    ranges = domClass.GetNonSyntheticValue().GetChildMemberWithName("ranges")
    return f"[{ranges.GetSummary() or ranges.GetValue()}] {GetArrayType(_instance)}"


def GetArrayType(_instance):
    ddata = _instance.GetChildMemberWithName("data")
    # use the element type we can compute from ddata
    ddata_regex_c = re.compile(r"^_ddata_([a-zA-Z0-9_]+)(?:_chpl)?$")
    ddata_regex_llvm = re.compile(r"^$")  # TODO
    ddata_typename = ddata.GetTypeName()
    ddata_match = ddata_regex_c.match(ddata_typename) or ddata_regex
    if not ddata_match:
        print(
            f"ArrayProvider: ddata type name '{ddata_typename}' did not match expected pattern"
        )
        return "unknown"
    element_type = ddata_match.group(1)
    return element_type


class ArrayProvider:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj

        self.synthetic_children = {}
        typename = self.valobj.GetTypeName()
        match = array_regex_c.match(typename) or array_regex_llvm.match(
            typename
        )
        if not match:
            print(
                f"ArrayProvider: type name '{typename}' did not match expected pattern"
            )
            return

        self._instance = (
            self.valobj.GetNonSyntheticValue().GetChildMemberWithName(
                "_instance"
            )
        )
        self.domClass = self._instance.GetChildMemberWithName("dom")
        self.data = self._instance.GetChildMemberWithName("data")
        self.synthetic_children["dom"] = self.domClass
        self.synthetic_children["data"] = self.data

        self.rank = int(match.group(1))
        self.element_type = GetArrayType(self._instance)

        self._make_synthetic_array()

    def _make_synthetic_array(self):
        """Create synthetic children for array elements"""

        data_valobj = self.data.GetNonSyntheticValue().GetData()
        if not data_valobj.IsValid():
            print("ArrayProvider: data SBData is not valid")
            return

        # Get the size of each element based on the element type
        element_size = self._get_element_size()
        if element_size == 0:
            print(
                f"ArrayProvider: unknown element size for type '{self.element_type}'"
            )
            return

        element_type = self.valobj.GetTarget().FindFirstType(self.element_type)
        if not element_type.IsValid():
            print(
                f"ArrayProvider: could not find element type '{self.element_type}'"
            )
            return

        # For 1D arrays, get the range to determine array bounds
        ranges = self.domClass.GetNonSyntheticValue().GetChildMemberWithName(
            "ranges"
        )
        if ranges.GetNumChildren() <= 0:
            return

        # N-D case - multi-dimensional indexing
        bounds = []
        for dim in range(self.rank):
            range_dim = ranges.GetChildAtIndex(dim)
            low = (
                range_dim.GetNonSyntheticValue()
                .GetChildMemberWithName("_low")
                .GetValueAsSigned()
            )
            high = (
                range_dim.GetNonSyntheticValue()
                .GetChildMemberWithName("_high")
                .GetValueAsSigned()
            )
            bounds.append((low, high))

        data_ptr = self.data.GetValueAsUnsigned()

        def generate_indices(dims, bounds):
            if dims == 0:
                yield []
            else:
                low, high = bounds[dims - 1]
                for i in range(low, high + 1):
                    for rest in generate_indices(dims - 1, bounds):
                        yield [i] + rest

        # Calculate strides
        strides = [1]
        for i in range(self.rank - 1, 0, -1):
            low, high = bounds[i]
            size = high - low + 1
            strides.insert(0, strides[0] * size)

        for indices in generate_indices(self.rank, bounds):
            # Calculate linear offset
            offset = 0
            for i, idx in enumerate(indices):
                low, _ = bounds[i]
                offset += (idx - low) * strides[i]

            element_addr = data_ptr + offset * element_size
            element_name = f"[{','.join(map(str, indices))}]"
            try:
                element_val = self.valobj.CreateValueFromAddress(
                    element_name, element_addr, element_type
                )
                self.synthetic_children[element_name] = element_val
            except:
                print(
                    f"ArrayProvider: failed to create element at indices {indices}"
                )

    def _get_element_size(self):
        """Get the size in bytes of the element type"""
        target = self.valobj.GetTarget()
        element_type = target.FindFirstType(self.element_type)
        if element_type.IsValid():
            return element_type.GetByteSize()
        return 0

    def has_children(self):
        return self.num_children() > 0

    def num_children(self):
        return len(self.synthetic_children)

    def get_child_index(self, name):
        if name in self.synthetic_children:
            return list(self.synthetic_children.keys()).index(name)
        else:
            return -1

    def get_child_at_index(self, index):
        if index < 0 or index >= self.num_children():
            return None
        key = list(self.synthetic_children.keys())[index]
        child_obj = self.synthetic_children[key]
        return self.valobj.CreateValueFromAddress(
            key, child_obj.GetLoadAddress(), child_obj.GetType()
        )


def __lldb_init_module(debugger, internal_dict):
    debugger.HandleCommand(
        "type summary add --expand --python-function chpl_lldb_pretty_print.StringSummary  -x '^((string(_chpl)?)|String::string)$'"
    )
    debugger.HandleCommand(
        "type synth add --python-class chpl_lldb_pretty_print.StringProvider -x '^((string(_chpl)?)|String::string)$'"
    )

    debugger.HandleCommand(
        f"type summary add --expand --python-function chpl_lldb_pretty_print.RangeSummary --recognizer-function chpl_lldb_pretty_print.RangeRecognizer"
    )
    debugger.HandleCommand(
        f"type synth add --python-class chpl_lldb_pretty_print.RangeProvider --recognizer-function chpl_lldb_pretty_print.RangeRecognizer"
    )

    debugger.HandleCommand(
        f"type summary add --expand --python-function chpl_lldb_pretty_print.DomainSummary --recognizer-function chpl_lldb_pretty_print.DomainRecognizer"
    )
    debugger.HandleCommand(
        f"type synth add --python-class chpl_lldb_pretty_print.DomainProvider --recognizer-function chpl_lldb_pretty_print.DomainRecognizer"
    )

    debugger.HandleCommand(
        f"type summary add --expand --python-function chpl_lldb_pretty_print.HomogeneousTupleSummary --recognizer-function chpl_lldb_pretty_print.HomogeneousTupleRecognizer"
    )
    debugger.HandleCommand(
        f"type synth add --python-class chpl_lldb_pretty_print.HomogeneousTupleProvider --recognizer-function chpl_lldb_pretty_print.HomogeneousTupleRecognizer"
    )

    debugger.HandleCommand(
        f"type summary add --expand --python-function chpl_lldb_pretty_print.ArraySummary --recognizer-function chpl_lldb_pretty_print.ArrayRecognizer"
    )
    debugger.HandleCommand(
        f"type synth add --python-class chpl_lldb_pretty_print.ArrayProvider --recognizer-function chpl_lldb_pretty_print.ArrayRecognizer"
    )
