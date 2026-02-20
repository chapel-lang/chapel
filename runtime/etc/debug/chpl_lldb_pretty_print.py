#
# Copyright 2025-2026 Hewlett Packard Enterprise Development LP
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

from functools import cache
import lldb
import re


class Ptr:
    def __init__(self, target, ptr, owned=True):
        self.target = target
        assert self.target is not None
        self.ptr = ptr
        self.owned = owned

    def _cleanup(self):
        """
        This function should only be called when we are completely done with
        the pointer and want to free the memory allocated for it in the
        debugged program. Putting this in `__del__` will cause issues because
        `__del__` may fire too early and cause us to access freed memory.

        Currently this is unused since most values are stored by the LLDB API
        and its not actually clear when LLDB is done with them.
        """
        if self.owned and self.ptr is not None and self.ptr.IsValid():
            self.target.EvaluateExpression(
                f"chpl_debug_free((void*){self.ptr.GetValueAsUnsigned()})"
            )
            self.owned = False

    def get(self):
        if self.ptr is not None:
            return self.ptr
        else:
            # return an invalid SBValue
            return lldb.SBValue()


def debugp(*args, **kwargs):
    print("[chpl_lldb_pretty_print]", *args, **kwargs)


def errorp(*args, **kwargs):
    print("[chpl_lldb_pretty_print][ERROR]", *args, **kwargs)


def ResolveWidePointer(wideptr, size=None):
    locale = wideptr.GetNonSyntheticValue().GetChildMemberWithName("locale")
    addr = wideptr.GetNonSyntheticValue().GetChildMemberWithName("addr")
    addr_type = addr.GetType()

    target = wideptr.GetTarget()

    if size is None:
        size = addr_type.GetPointeeType().GetByteSize()
    ret_val = target.EvaluateExpression(
        f"chpl_debug_get((void*){locale.GetLoadAddress()}, (void*){addr.GetValueAsUnsigned()}, {size})"
    )
    if not ret_val.IsValid():
        return Ptr(target, None, owned=False)
    resolved_ptr = ret_val.Cast(addr_type)
    if not resolved_ptr.IsValid():
        return Ptr(target, None, owned=False)
    return Ptr(target, resolved_ptr)


def MaybeResolveWidePointer(ptr, size=None):
    if ptr.GetTypeName().startswith("wide("):
        return ResolveWidePointer(ptr, size=size)
    else:
        return Ptr(ptr.GetTarget(), ptr, owned=False)


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
    r"^range\((?P<eltType>[a-zA-Z0-9_()]+),(?P<boundsKind>both|low|high|neither),(?P<stride>one|negOne|positive|negative|any)\)$"
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
        errorp(
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
            errorp(
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
            errorp(
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
            errorp(
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
domain_regex_llvm = re.compile(
    r"^ChapelDomain::domain\((?P<rank>[0-9]+),(?P<eltType>[a-zA-Z0-9_\(\)]+),(?P<stride>one|negOne|positive|negative|any)\)$"
)
# TODO handle developer domain regex


def DomainRecognizer(sbtype, internal_dict):
    typename = sbtype.GetName()
    match = domain_regex_c.match(typename) or domain_regex_llvm.match(typename)
    return match is not None


def RangesToString(ranges):
    getSum = lambda r: r.GetSummary() or r.GetValue() or ""
    s = ", ".join(
        getSum(ranges.GetChildAtIndex(i))
        for i in range(ranges.GetNumChildren())
    )
    return s


def DomainSummary(valobj, internal_dict):
    _instance = MaybeResolveWidePointer(
        valobj.GetNonSyntheticValue().GetChildMemberWithName("_instance")
    )
    ranges = (
        _instance.get().GetNonSyntheticValue().GetChildMemberWithName("ranges")
    )
    return "{" + RangesToString(ranges) + "}"


class DomainProvider:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj

        self.synthetic_children = {}

        _instance = MaybeResolveWidePointer(
            valobj.GetNonSyntheticValue().GetChildMemberWithName("_instance")
        )
        ranges = (
            _instance.get()
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

    def get_type_name(self):
        name = self.valobj.GetTypeName()
        if name.startswith("ChapelDomain::"):
            name = name[len("ChapelDomain::") :]
        return name


# TODO: for now we only handle DefaultRectangular for C backend

array_regex_c = re.compile(
    r"^_array_DefaultRectangularArr_(?P<rank>[0-9]+)_(?P<idxType>[a-zA-Z0-9_]+)_(?P<stride>one|negOne|positive|negative|any)_(?P<eltType>[a-zA-Z0-9_]+)_(?P<idxSignedType>[a-zA-Z0-9_]+)(?:_chpl)?$"
)
array_regex_llvm = re.compile(
    r"^ChapelArray::\[domain\((?P<rank>[0-9]+),(?P<idxType>[a-zA-Z0-9_()]+),(?P<stride>one|negOne|positive|negative|any)\)\] (?P<eltType>[a-zA-Z0-9_()? ]+)$"
)
array_general_regex_llvm = re.compile(
    r"^ChapelArray::\[(?P<domainName>[a-zA-Z0-9_()]+)\((?P<rank>[0-9]+)[^]]+\] (?P<eltType>[a-zA-Z0-9_()? ]+)$"
)


def IsChapelArrayType(sbtype):
    typename = sbtype.GetName()
    if sbtype.IsReferenceType():
        typename = sbtype.GetPointeeType().GetName()
    match = (
        array_regex_c.match(typename)
        or array_regex_llvm.match(typename)
        or array_general_regex_llvm.match(typename)
    )
    return match is not None


def GetArrayRank(sbtype):
    typename = sbtype.GetName()
    if sbtype.IsReferenceType():
        typename = sbtype.GetPointeeType().GetName()
    match = (
        array_regex_c.match(typename)
        or array_regex_llvm.match(typename)
        or array_general_regex_llvm.match(typename)
    )
    return int(match.group("rank"))


def ArrayRecognizer(sbtype, internal_dict):
    return IsChapelArrayType(sbtype)


def ArraySummary(valobj, internal_dict):
    if not IsChapelArrayType(valobj.GetType()):
        errorp("ArraySummary: not a Chapel array type")
        return

    _instance = MaybeResolveWidePointer(
        valobj.GetNonSyntheticValue().GetChildMemberWithName("_instance")
    )

    domClass = MaybeResolveWidePointer(
        _instance.get().GetChildMemberWithName("dom")
    )
    ranges = (
        domClass.get().GetNonSyntheticValue().GetChildMemberWithName("ranges")
    )
    if not ranges.IsValid():
        # this may be a distributed array
        # TODO: in theory this should work but seems to always result in a
        # ranges with "incomplete type" - need to investigate further
        pass
        # whole = domClass.get().GetNonSyntheticValue().GetChildMemberWithName("whole")
        # whole_instance = whole.GetNonSyntheticValue().GetChildMemberWithName("_instance")
        # whole_instance_addr = MaybeResolveWidePointer(whole_instance)
        # ranges = whole_instance_addr.get().GetNonSyntheticValue().GetChildMemberWithName("ranges")
    if not ranges.IsValid():
        return f"[...] {GetArrayType(_instance.get()).GetName()}"
    else:
        return f"[{RangesToString(ranges)}] {GetArrayType(_instance.get()).GetName()}"


def GetArrayType(_instance):
    # returns SBType
    data = MaybeResolveWidePointer(_instance)
    ddata = data.get().GetNonSyntheticValue().GetChildMemberWithName("data")
    if not ddata.IsValid():
        # it may be a distributed array, try and figure out the array type
        # myLocArr->myElems._instance->data
        myLocArr = (
            data.get().GetNonSyntheticValue().GetChildMemberWithName("myLocArr")
        )
        if not myLocArr.IsValid():
            # TODO: ideally this returns some sort of unknown string
            nullptr = data.get().GetTarget().FindFirstType("nullptr_t")
            return nullptr
        myLocArr_addr = MaybeResolveWidePointer(myLocArr)
        myElems_instance = (
            myLocArr_addr.get()
            .GetNonSyntheticValue()
            .GetChildMemberWithName("myElems")
            .GetNonSyntheticValue()
            .GetChildMemberWithName("_instance")
        )
        if not myElems_instance.IsValid():
            # TODO: ideally this returns some sort of unknown string
            nullptr = data.get().GetTarget().FindFirstType("nullptr_t")
            return nullptr
        myElems_instance_addr = MaybeResolveWidePointer(myElems_instance)
        ddata = (
            myElems_instance_addr.get()
            .GetNonSyntheticValue()
            .GetChildMemberWithName("data")
        )

    if not ddata.IsValid():
        # TODO: ideally this returns some sort of unknown string
        nullptr = data.get().GetTarget().FindFirstType("nullptr_t")
        return nullptr
    ddata_addr = MaybeResolveWidePointer(ddata)
    return ddata_addr.get().GetType().GetPointeeType()


class ArrayProvider:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj

        self.synthetic_children = {}
        if not IsChapelArrayType(self.valobj.GetType()):
            errorp("ArrayProvider: not a Chapel array type")
            return

        self._instance = (
            self.valobj.GetNonSyntheticValue().GetChildMemberWithName(
                "_instance"
            )
        )
        self._instance_addr = MaybeResolveWidePointer(self._instance)
        self.domClass = self._instance_addr.get().GetChildMemberWithName("dom")
        self.data = self._instance_addr.get().GetChildMemberWithName("data")
        # TODO: itd be nice to have a domain as a valid child too
        # but the array printer currently gets confused if we do that
        # self.synthetic_children["domain"] = self.domClass

        self.rank = GetArrayRank(self.valobj.GetType())
        self.element_type = GetArrayType(self._instance)

        ty = self._instance_addr.get().GetType().GetPointeeType()
        self.dsiAccess = None
        for memberFunc in range(ty.GetNumberOfMemberFunctions()):
            if ty.GetMemberFunctionAtIndex(memberFunc).GetName() == "dsiAccess":
                self.dsiAccess = ty.GetMemberFunctionAtIndex(memberFunc)
                break
        if self.dsiAccess is not None:
            formalType = self.dsiAccess.GetArgumentTypeAtIndex(0)
            self.dsiAccessIndexType = (
                formalType.GetCanonicalType()
                .GetDereferencedType()
                .GetArrayElementType()
            )

        if self.data.IsValid():
            self._make_synthetic_array()
        elif self.dsiAccess is not None:
            self._make_synthetic_array_generic()

    def should_use_dsiAccess(self):
        return not self.data.IsValid() and self.dsiAccess is not None

    def _generate_indices(self, dims, bounds):
        if dims == 0:
            yield []
        else:
            low, high, stride = bounds[dims - 1]
            for i in range(low, high + 1, stride if stride else 1):
                for rest in self._generate_indices(dims - 1, bounds):
                    yield rest + [i]

    def _determine_bounds(self, ranges):
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
        return bounds

    def _make_synthetic_array(self):
        """Create synthetic children for array elements"""

        resolved_dom = MaybeResolveWidePointer(self.domClass)
        ranges = (
            resolved_dom.get()
            .GetNonSyntheticValue()
            .GetChildMemberWithName("ranges")
        )
        if ranges.GetNumChildren() <= 0:
            return

        bounds = self._determine_bounds(ranges)

        element_size = self.element_type.GetByteSize()
        num_elements = 1
        for low, high, stride in bounds:
            dim_size = (high - low) // (stride if stride else 1) + 1
            num_elements *= dim_size

        resolved_data = MaybeResolveWidePointer(
            self.data, num_elements * element_size
        )
        data_ptr = resolved_data.get().GetValueAsUnsigned()

        for indices in self._generate_indices(self.rank, bounds):
            # Calculate linear offset for multi-dimensional array
            offset = 0
            multiplier = 1
            for dim in reversed(range(self.rank)):
                low, high, stride = bounds[dim]
                dim_index = indices[dim]
                # Convert from Chapel index to 0-based index
                zero_based_index = (dim_index - low) // (
                    stride if stride else 1
                )
                offset += zero_based_index * multiplier
                # Update multiplier for next dimension
                dim_size = (high - low) // (stride if stride else 1) + 1
                multiplier *= dim_size

            element_addr = data_ptr + offset * element_size
            element_name = f"[{','.join(map(str, indices))}]"
            try:
                element_val = self.valobj.CreateValueFromAddress(
                    element_name, element_addr, self.element_type
                )
                self.synthetic_children[element_name] = element_val
            except:
                errorp(
                    f"ArrayProvider: failed to create element at indices {indices}"
                )

    def _make_synthetic_array_generic(self):
        """Create synthetic children for array elements using dsiAccess"""

        resolved_dom = MaybeResolveWidePointer(self.domClass)
        whole = (
            resolved_dom.get()
            .GetNonSyntheticValue()
            .GetChildMemberWithName("whole")
        )
        whole_instance = whole.GetNonSyntheticValue().GetChildMemberWithName(
            "_instance"
        )
        if not whole_instance.IsValid():
            return
        whole_instance_addr = MaybeResolveWidePointer(whole_instance)
        ranges = (
            whole_instance_addr.get()
            .GetNonSyntheticValue()
            .GetChildMemberWithName("ranges")
        )
        if ranges.GetNumChildren() <= 0:
            return

        bounds = self._determine_bounds(ranges)

        # TODO: this will not work with wide ptrs because _instance will be wide
        base_idx_expr = f"{self.valobj.GetName()}._instance"
        for indices in self._generate_indices(self.rank, bounds):
            indices_expr = ",".join(map(str, indices))
            element_name = f"[{indices_expr}]"
            try:
                index_expr = f"{self.dsiAccessIndexType} idx[] = {{{indices_expr}}}; ({base_idx_expr})->dsiAccess(idx)"
                element = self.valobj.CreateValueFromExpression(
                    element_name, index_expr
                )
                self.synthetic_children[element_name] = element
            except Exception as e:
                print(e)
                errorp(
                    f"ArrayProvider: failed to create element at indices {indices}"
                )

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
        if self.should_use_dsiAccess():
            return child_obj
        else:
            return self.valobj.CreateValueFromAddress(
                key, child_obj.GetLoadAddress(), child_obj.GetType()
            )

    def get_type_name(self):
        name = self.valobj.GetTypeName()
        if name.startswith("ChapelArray::"):
            name = name[len("ChapelArray::") :]
        return name


owned_regex = re.compile(r"^((_owned_.+(_chpl)?)|(owned .+))$")
shared_regex = re.compile(r"^((_shared_.+(_chpl)?)|(shared .+))$")


def ManagedObjectRecognizer(sbtype, internal_dict):
    typename = sbtype.GetName()
    match = owned_regex.match(typename) or shared_regex.match(typename)
    return match is not None and not sbtype.IsPointerType()


def ManagedObjectSummary(valobj, internal_dict):
    chpl_p = MaybeResolveWidePointer(
        valobj.GetNonSyntheticValue().GetChildMemberWithName("chpl_p")
    )

    if chpl_p.get().GetValueAsUnsigned() == 0:
        return "nil"
    else:
        return chpl_p.get().GetSummary() or chpl_p.get().GetValue()


class ManagedObjectProvider:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj

        chpl_p = MaybeResolveWidePointer(
            self.valobj.GetNonSyntheticValue().GetChildMemberWithName("chpl_p")
        )
        if chpl_p.get().GetValueAsUnsigned() == 0:
            self.synthetic_children = {}
        else:
            self.synthetic_children = {}
            deref_chpl_p = chpl_p.get().Dereference()
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


def WidePointerSummary(valobj, internal_dict):
    resolved_ptr = ResolveWidePointer(valobj)
    if not resolved_ptr.get().IsValid():
        return "nil"
    else:
        return resolved_ptr.get().GetSummary() or resolved_ptr.get().GetValue()


class WidePointerProvider:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj

        resolved_ptr = ResolveWidePointer(self.valobj)
        if not resolved_ptr.get().IsValid():
            self.synthetic_children = {}
        else:
            self.synthetic_children = {}
            deref_ptr = resolved_ptr.get().Dereference()
            if deref_ptr.IsValid():
                for i in range(deref_ptr.GetNumChildren()):
                    child = deref_ptr.GetChildAtIndex(i)
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


list_regex_llvm = re.compile(
    r"^(List::list\((?P<eltType>[a-zA-Z0-9_()? ]+)(,(?P<parSafe>(true|false)))?\))$"
)


def ListRecognizer(sbtype, internal_dict):
    typename = sbtype.GetName()
    match = list_regex_llvm.match(typename)
    return match is not None


def ListSummary(valobj, internal_dict):
    size = (
        valobj.GetNonSyntheticValue()
        .GetChildMemberWithName("_size")
        .GetValueAsUnsigned()
    )
    return f"size = {size}"


class ListProvider:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj

        self.synthetic_children = {}
        size = (
            self.valobj.GetNonSyntheticValue()
            .GetChildMemberWithName("_size")
            .GetValueAsUnsigned()
        )
        for i in range(size):
            element_name = f"[{i}]"
            try:
                element = self.valobj.CreateValueFromExpression(
                    element_name, f"({self.valobj.GetName()})._getRef({i})"
                )
            except:
                continue
            if element.IsValid():
                self.synthetic_children[f"[{i}]"] = element

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
        return child_obj


hashtable_entry_regex_llvm = re.compile(
    r"^(ChapelHashtable::chpl_TableEntry\((?P<keyType>[a-zA-Z0-9_()? ]+),(?P<valType>[a-zA-Z0-9_()? ]+)\))$"
)
hashtable_regex_llvm = re.compile(
    r"^(ChapelHashtable::chpl__hashtable\((?P<keyType>[a-zA-Z0-9_()? ]+),(?P<valType>[a-zA-Z0-9_()? ]+)\))$"
)


def HashTableEntryRecognizer(sbtype, internal_dict):
    typename = sbtype.GetName()
    match = hashtable_entry_regex_llvm.match(typename)
    return match is not None


def IsValidHashTableEntryType(valobj):
    target = valobj.GetTarget()
    full_enum = target.EvaluateExpression(
        "ChapelHashtable::chpl__hash_status::full"
    )
    status = valobj.GetNonSyntheticValue().GetChildMemberWithName("status")
    return status.GetValueAsUnsigned() == full_enum.GetValueAsUnsigned()


def HashTableEntrySummary(valobj, internal_dict):
    if not IsValidHashTableEntryType(valobj):
        return "<empty>"
    key = valobj.GetNonSyntheticValue().GetChildMemberWithName("key")
    val = valobj.GetNonSyntheticValue().GetChildMemberWithName("val")
    if val.IsValid():
        return f"{key.GetSummary() or key.GetValue() or ''} => {val.GetSummary() or val.GetValue() or ''}"
    else:
        return f"{key.GetSummary() or key.GetValue() or ''}"


class HashTableEntryProvider:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj

        self.synthetic_children = {}
        if IsValidHashTableEntryType(self.valobj):
            self.synthetic_children["key"] = (
                self.valobj.GetNonSyntheticValue().GetChildMemberWithName("key")
            )
            val_child = (
                self.valobj.GetNonSyntheticValue().GetChildMemberWithName("val")
            )
            if val_child.IsValid():
                self.synthetic_children["val"] = val_child

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
        return self.valobj.CreateValueFromData(
            f"{key}", child_obj.GetData(), child_obj.GetType()
        )


def GetHashTableElements(valobj):
    tablePtr = valobj.GetNonSyntheticValue().GetChildMemberWithName("table")
    tableSize = (
        valobj.GetNonSyntheticValue()
        .GetChildMemberWithName("tableSize")
        .GetValueAsUnsigned()
    )
    hashTableEntryType = (
        MaybeResolveWidePointer(tablePtr).get().GetType().GetPointeeType()
    )
    hashTableEntrySize = hashTableEntryType.GetByteSize()
    resolved_table = MaybeResolveWidePointer(
        tablePtr, tableSize * hashTableEntrySize
    )
    elements = []
    table_address = resolved_table.get().GetValueAsUnsigned()
    idx = 0
    for i in range(tableSize):
        offset = i * hashTableEntrySize
        element = valobj.CreateValueFromAddress(
            f"[{i}]", table_address + offset, hashTableEntryType
        )
        if IsValidHashTableEntryType(element):
            elements.append((idx, element))
            idx += 1
    return elements


def HashTableRecognizer(sbtype, internal_dict):
    typename = sbtype.GetName()
    match = hashtable_regex_llvm.match(typename)
    return match is not None


def HashTableSummary(valobj, internal_dict):
    size = (
        valobj.GetNonSyntheticValue()
        .GetChildMemberWithName("tableNumFullSlots")
        .GetValueAsUnsigned()
    )
    return f"size = {size}"


class HashTableProvider:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj
        self.tablePtr = valobj.GetNonSyntheticValue().GetChildMemberWithName(
            "table"
        )
        self.size = (
            valobj.GetNonSyntheticValue()
            .GetChildMemberWithName("tableNumFullSlots")
            .GetValueAsUnsigned()
        )
        self.capacity = (
            valobj.GetNonSyntheticValue()
            .GetChildMemberWithName("tableSize")
            .GetValueAsUnsigned()
        )

        self.synthetic_children = {}

        elements = GetHashTableElements(self.valobj)
        for index, element in elements:
            self.synthetic_children[f"[{index}]"] = element

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


set_regex_llvm = re.compile(
    r"^(Set::set\((?P<eltType>[a-zA-Z0-9_()? ]+)(,(?P<parSafe>(true|false)))?\))$"
)


def SetRecognizer(sbtype, internal_dict):
    typename = sbtype.GetName()
    match = set_regex_llvm.match(typename)
    return match is not None


def SetSummary(valobj, internal_dict):
    size = (
        valobj.GetNonSyntheticValue()
        .GetChildMemberWithName("_htb")
        .GetNonSyntheticValue()
        .GetChildMemberWithName("tableNumFullSlots")
        .GetValueAsUnsigned()
    )
    return f"size = {size}"


class SetProvider:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj
        self._htb = valobj.GetNonSyntheticValue().GetChildMemberWithName("_htb")

        self.synthetic_children = {}

        elements = GetHashTableElements(self._htb)
        for index, element in elements:
            self.synthetic_children[f"[{index}]"] = (
                element.GetNonSyntheticValue().GetChildMemberWithName("key")
            )

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


map_regex_llvm = re.compile(
    r"^(Map::map\((?P<keyType>[a-zA-Z0-9_()? ]+),(?P<valType>[a-zA-Z0-9_()? ]+)(,(?P<parSafe>(true|false)))?\))$"
)


def MapRecognizer(sbtype, internal_dict):
    typename = sbtype.GetName()
    if sbtype.IsReferenceType():
        typename = sbtype.GetPointeeType().GetName()
    match = map_regex_llvm.match(typename)
    return match is not None


def MapSummary(valobj, internal_dict):
    size = (
        valobj.GetNonSyntheticValue()
        .GetChildMemberWithName("table")
        .GetNonSyntheticValue()
        .GetChildMemberWithName("tableNumFullSlots")
        .GetValueAsUnsigned()
    )
    return f"size = {size}"


class MapProvider:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj
        self.table = valobj.GetNonSyntheticValue().GetChildMemberWithName(
            "table"
        )

        self.synthetic_children = {}

        elements = GetHashTableElements(self.table)
        for index, element in elements:
            self.synthetic_children[f"[{index}]"] = element

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
        element = self.synthetic_children[key]
        return self.valobj.CreateValueFromAddress(
            key, element.GetLoadAddress(), element.GetType()
        )


def DebugFunc_ResolveWidePointer(debugger, command, result, internal_dict):
    """Resolve a wide pointer manually.
    Usage: RWP <wide pointer variable> [size]
    Size is optional; if not provided, the size of the pointee type is used.
    Example: RWP myWidePtr
    """
    target = debugger.GetSelectedTarget()
    frame = target.GetProcess().GetSelectedThread().GetSelectedFrame()

    args = command.split()
    if len(args) == 0:
        result.PutCString("Error: No wide pointer expression provided.")
        result.SetStatus(lldb.eReturnStatusFailed)
        return
    if len(args) > 2:
        result.PutCString("Error: Too many arguments provided.")
        result.SetStatus(lldb.eReturnStatusFailed)
        return
    wideptr_expr = args[0]
    size = None
    if len(args) == 2:
        try:
            size = int(args[1])
        except ValueError:
            result.PutCString("Error: Size must be an integer.")
            result.SetStatus(lldb.eReturnStatusFailed)
            return
    wideptr = frame.EvaluateExpression(wideptr_expr)
    if not wideptr.IsValid():
        result.PutCString("Error: Invalid wide pointer expression.")
        result.SetStatus(lldb.eReturnStatusFailed)
        return

    resolved_ptr = ResolveWidePointer(wideptr, size)
    result.PutCString(
        f"Resolved Pointer: ({resolved_ptr.get().GetTypeName()}) {resolved_ptr.get().GetValue()}"
    )
    resolved_ptr._cleanup()
    result.SetStatus(lldb.eReturnStatusSuccessFinishResult)


def PrintType(debugger, command, result, internal_dict):
    target = debugger.GetSelectedTarget()
    frame = target.GetProcess().GetSelectedThread().GetSelectedFrame()
    evaled_expr = frame.EvaluateExpression(command)
    if not evaled_expr.IsValid():
        result.PutCString(str(evaled_expr.error))
        result.SetStatus(lldb.eReturnStatusFailed)
        return
    ty = evaled_expr.GetType().GetCanonicalType()
    s = str(ty)
    if ty.IsPointerType():
        ty = ty.GetPointeeType()
        s = str(ty) + " *"
    result.PutCString(s)


def __lldb_init_module(debugger, internal_dict):

    def register(summary_function, provider_class, recognizer_str):
        if summary_function is not None:
            debugger.HandleCommand(
                f"type summary add --skip-pointers --expand --python-function chpl_lldb_pretty_print.{summary_function} {recognizer_str}"
            )
        if provider_class is not None:
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
        regex("string(_chpl)?"),
    )
    register("RangeSummary", "RangeProvider", recognizer("RangeRecognizer"))
    register("DomainSummary", "DomainProvider", recognizer("DomainRecognizer"))
    register("ArraySummary", "ArrayProvider", recognizer("ArrayRecognizer"))
    register(
        "ManagedObjectSummary",
        "ManagedObjectProvider",
        recognizer("ManagedObjectRecognizer"),
    )

    register("ListSummary", "ListProvider", recognizer("ListRecognizer"))
    register(
        None, "HashTableEntryProvider", recognizer("HashTableEntryRecognizer")
    )
    register(
        "HashTableSummary",
        "HashTableProvider",
        recognizer("HashTableRecognizer"),
    )
    register("SetSummary", "SetProvider", recognizer("SetRecognizer"))
    register("MapSummary", "MapProvider", recognizer("MapRecognizer"))

    # TODO: I can't decide if having a wide pointer summary/provider is useful
    # or just confusing
    # register("WidePointerSummary", "WidePointerProvider", regex("wide\\(.+\\)"))
    debugger.HandleCommand(
        "command script add --overwrite -f chpl_lldb_pretty_print.DebugFunc_ResolveWidePointer RWP"
    )
    # emulate the gdb ptype command
    debugger.HandleCommand(
        "command script add --overwrite -f chpl_lldb_pretty_print.PrintType ptype"
    )

    # make sure int(8) and uint(8) show up as numbers, not characters
    # we have to handle char types as well because lldb doesn't cascade through
    # things like atomics properly
    debugger.HandleCommand("type format add -p -r --format u 'uint(8)'")
    debugger.HandleCommand("type format add -p -r --format d 'int(8)'")
    debugger.HandleCommand("type format add -p -r --format u 'uint8_t'")
    debugger.HandleCommand("type format add -p -r --format d 'int8_t'")
    debugger.HandleCommand("type format add -p -r --format u 'unsigned char'")
    debugger.HandleCommand("type format add -p -r --format d 'signed char'")
    debugger.HandleCommand("type format add -p -r --format d 'char'")
