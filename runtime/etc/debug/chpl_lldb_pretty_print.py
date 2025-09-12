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
    r"^range_(?P<eltType>[a-zA-Z0-9_]+)_(?P<boundsKind>both|low|high|neither)_(?P<stride>one|negOne|positive|negative|any)(?:_chpl)?$"
)
range_regex_llvm = re.compile(
    r"^ChapelRange::range\((?P<eltType>[a-zA-Z0-9_()]+),(?P<boundsKind>both|low|high|neither),(?P<stride>one|negOne|positive|negative|any)\)$"
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
    stride = (
        valobj.GetNonSyntheticValue()
        .GetChildMemberWithName("_stride")
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
        return match.group("boundsKind") in ("low", "both")

    def has_high_bound():
        return match.group("boundsKind") in ("high", "both")

    def has_stride():
        return match.group("stride") != "one"

    # TODO: handle strides and alignment
    res = ""
    if has_low_bound():
        res += str(low)
    res += ".."
    if has_high_bound():
        res += str(high)
    if has_stride():
        res += f" by {stride}"


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
        if self._has_stride():
            self.synthetic_children["stride"] = (
                self.valobj.GetChildMemberWithName("_stride")
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
        return match.group("boundsKind") in ("low", "both")

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
        return match.group("boundsKind") in ("high", "both")

    def _has_stride(self):
        typename = self.valobj.GetTypeName()
        match = range_regex_c.match(typename) or range_regex_llvm.match(
            typename
        )
        if not match:
            print(
                f"RangeProvider: type name '{typename}' did not match expected pattern"
            )
            return False
        return match.group("stride") != "one"

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


# TODO: only handles DefaultRectangularDom for C for now
domain_regex_c = re.compile(
    r"^_domain_DefaultRectangularDom_(?P<rank>[0-9]+)_(?P<eltType>[a-zA-Z0-9_]+)_(?P<stride>one|negOne|positive|negative|any)(?:_chpl)?$"
)
domain_regex_llvm = re.compile(r"^ChapelDomain::domain\((?P<rank>[0-9]+),(?P<eltType>[a-zA-Z0-9_\(\)]+),(?P<stride>one|negOne|positive|negative|any)\)$")
# TODO handle developer domain regex


def DomainRecognizer(sbtype, internal_dict):
    typename = sbtype.GetName()
    match = domain_regex_c.match(typename) or domain_regex_llvm.match(typename)
    return match is not None


def RangesToString(ranges):
    getSum = lambda r: r.GetSummary() or r.GetValue() or ""
    s = ", ".join(getSum(ranges.GetChildAtIndex(i)) for i in range(ranges.GetNumChildren()))
    return s

def DomainSummary(valobj, internal_dict):
    typename = valobj.GetTypeName()
    match = domain_regex_c.match(typename) or domain_regex_llvm.match(typename)
    if not match:
        print(
            f"Domain_Summary: type name '{typename}' did not match expected pattern"
        )
        return None

    ranges = (
        valobj.GetNonSyntheticValue()
        .GetChildMemberWithName("_instance")
        .GetNonSyntheticValue()
        .GetChildMemberWithName("ranges")
    )
    return "{" + RangesToString(ranges) + "}"


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


# TODO: for now we only handle DefaultRectangular for C backend

array_regex_c = re.compile(
    r"^_array_DefaultRectangularArr_(?P<rank>[0-9]+)_(?P<idxType>[a-zA-Z0-9_]+)_(?P<stride>one|negOne|positive|negative|any)_(?P<eltType>[a-zA-Z0-9_]+)_(?P<idxSignedType>[a-zA-Z0-9_]+)(?:_chpl)?$"
)
array_regex_llvm = re.compile(r"^ChapelArray::\[domain\((?P<rank>[0-9]+),(?P<idxType>[a-zA-Z0-9_\(\)]+),(?P<stride>one|negOne|positive|negative|any)\)\] (?P<eltType>[a-zA-Z0-9_\(\)]+)$")


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

    _instance = valobj.GetNonSyntheticValue().GetChildMemberWithName(
        "_instance"
    )
    domClass = _instance.GetChildMemberWithName("dom")
    data = _instance.GetChildMemberWithName("data")
    ranges = domClass.GetNonSyntheticValue().GetChildMemberWithName("ranges")
    return f"[{RangesToString(ranges)}] {GetArrayType(_instance)}"


def GetArrayType(_instance):
    ddata = _instance.GetChildMemberWithName("data")
    # use the element type we can compute from ddata
    # TODO: handle wide pointers
    ddata_regex_c = re.compile(r"^_ddata_(?P<eltType>[a-zA-Z0-9_]+?)(?:_chpl)?$")
    ddata_regex_llvm = re.compile(r"^(?P<eltType>[a-zA-Z0-9_\(\)]+) \*$")
    ddata_typename = ddata.GetTypeName()
    ddata_match = ddata_regex_c.match(ddata_typename) or ddata_regex_llvm.match(ddata_typename)
    if not ddata_match:
        print(
            f"ArrayProvider: ddata type name '{ddata_typename}' did not match expected pattern"
        )
        return "unknown"
    element_type = ddata_match.group("eltType")
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

        self.rank = int(match.group("rank"))
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
            stride = (
                range_dim.GetNonSyntheticValue()
                .GetChildMemberWithName("_stride")
                .GetValueAsSigned()
            )
            bounds.append((low, high, stride))

        data_ptr = self.data.GetValueAsUnsigned()

        def generate_indices(dims, bounds):
            if dims == 0:
                yield []
            else:
                low, high, stride = bounds[dims - 1]
                for i in range(low, high + 1, stride if stride else 1):
                    for rest in generate_indices(dims - 1, bounds):
                        yield rest + [i]


        for indices in generate_indices(self.rank, bounds):
            # Calculate linear offset for multi-dimensional array
            offset = 0
            multiplier = 1
            for dim in reversed(range(self.rank)):
                low, high, stride = bounds[dim]
                dim_index = indices[dim]
                # Convert from Chapel index to 0-based index
                zero_based_index = (dim_index - low) // (stride if stride else 1)
                offset += zero_based_index * multiplier
                # Update multiplier for next dimension
                dim_size = (high - low) // (stride if stride else 1) + 1
                multiplier *= dim_size

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


owned_regex = re.compile(r"^((_owned_.+(_chpl)?)|(OwnedObject::owned .+))$")
shared_regex = re.compile(r"^((_shared_.+(_chpl)?)|(SharedObject::shared .+))$")


def ManagedObjectRecognizer(sbtype, internal_dict):
    typename = sbtype.GetName()
    match = owned_regex.match(typename) or shared_regex.match(typename)
    return match is not None and not sbtype.IsPointerType()


def ManagedObjectSummary(valobj, internal_dict):
    chpl_p = valobj.GetNonSyntheticValue().GetChildMemberWithName("chpl_p")

    if chpl_p.GetValueAsUnsigned() == 0:
        return "nil"
    else:
        return chpl_p.GetSummary() or chpl_p.GetValue()


class ManagedObjectProvider:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj

        chpl_p = self.valobj.GetNonSyntheticValue().GetChildMemberWithName(
            "chpl_p"
        )
        if chpl_p.GetValueAsUnsigned() == 0:
            self.synthetic_children = {}
        else:
            self.synthetic_children = {}
            deref_chpl_p = chpl_p.Dereference()
            if deref_chpl_p.IsValid():
                for i in range(deref_chpl_p.GetNumChildren()):
                    child = deref_chpl_p.GetChildAtIndex(i)
                    if child.IsValid():
                        self.synthetic_children[child.GetName()] = child

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
        element_data = self.synthetic_children[key].GetData()
        element_type = self.synthetic_children[key].GetType()
        return self.valobj.CreateValueFromData(
            f"{key}", element_data, element_type
        )


def __lldb_init_module(debugger, internal_dict):

    def register(summary_function, provider_class, recognizer_str):
        debugger.HandleCommand(
            f"type summary add --skip-pointers --expand --python-function chpl_lldb_pretty_print.{summary_function} {recognizer_str}"
        )
        debugger.HandleCommand(
            f"type synth add --skip-pointers --python-class chpl_lldb_pretty_print.{provider_class} {recognizer_str}"
        )

    def regex(type_regex):
        return f"-x '^({type_regex})$'"

    def recognizer(func_name):
        return f"--recognizer-function chpl_lldb_pretty_print.{func_name}"

    register(
        "StringSummary",
        "StringProvider",
        regex("(string(_chpl)?)|String::string"),
    )
    register("RangeSummary", "RangeProvider", recognizer("RangeRecognizer"))
    register("DomainSummary", "DomainProvider", recognizer("DomainRecognizer"))
    register("ArraySummary", "ArrayProvider", recognizer("ArrayRecognizer"))
    register(
        "ManagedObjectSummary",
        "ManagedObjectProvider",
        recognizer("ManagedObjectRecognizer"),
    )
