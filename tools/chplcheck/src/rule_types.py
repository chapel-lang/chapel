#
# Copyright 2024 Hewlett Packard Enterprise Development LP
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

from dataclasses import dataclass, field
import typing

import chapel
from fixits import Fixit, Edit


def _build_ignore_fixit(
    anchor: chapel.AstNode, lines: typing.List[str], rule_name: str
) -> Fixit:
    # TODO: how should this handle multiple ignores?
    loc = anchor.location()
    text = chapel.range_to_text(loc, lines)
    indent_amount = max(loc.start()[1] - 1, 0)
    indent = " " * indent_amount
    text = f'@chplcheck.ignore("{rule_name}")\n' + indent + text
    ignore = Fixit.build(Edit.build(loc, text))
    ignore.description = "Ignore this warning"
    return ignore


class BasicRuleResult:
    """
    Result type for basic rules. Rules can also return a plain boolean to
    represent a simple pass/fail result, with no fixit.
    """

    def __init__(
        self,
        node: chapel.AstNode,
        ignorable: bool = False,
        fixits: typing.Optional[typing.Union[Fixit, typing.List[Fixit]]] = None,
        data: typing.Optional[typing.Any] = None,
    ):
        self.node = node
        self.ignorable = ignorable
        if fixits is None:
            self._fixits = []
        elif isinstance(fixits, Fixit):
            self._fixits = [fixits]
        else:
            self._fixits = fixits
        self.data = data

    def add_fixits(self, *fixits: Fixit):
        """
        Add fixits to the result.
        """
        self._fixits.extend(fixits)

    def fixits(self, context: chapel.Context, name: str) -> typing.List[Fixit]:
        """
        Get the fixits associated with this result. Also builds an ignore fixit, if necessary.
        """
        to_return = self._fixits
        if self.ignorable:
            lines = chapel.get_file_lines(context, self.node)
            ignore = _build_ignore_fixit(self.node, lines, name)
            to_return.append(ignore)
        return to_return


_BasicRuleResult = typing.Union[bool, BasicRuleResult]
"""Internal type for basic rule results"""
BasicRuleCheck = typing.Callable[
    [chapel.Context, chapel.AstNode], _BasicRuleResult
]
"""Function type for basic rules; (context, node) -> bool or BasicRuleResult"""


class AdvancedRuleResult:
    """
    Result type for advanced rules. Advanced rules can also return a plain
    boolean to represent a simple pass/fail result, with no fixit. Having an anchor implies that it is ignorable
    """

    def __init__(
        self,
        node: chapel.AstNode,
        anchor: typing.Optional[chapel.AstNode] = None,
        fixits: typing.Optional[typing.Union[Fixit, typing.List[Fixit]]] = None,
    ):
        self.node = node
        self.anchor = anchor
        if fixits is None:
            self._fixits = []
        elif isinstance(fixits, Fixit):
            self._fixits = [fixits]
        else:
            self._fixits = fixits

    def fixits(self, context: chapel.Context, name: str) -> typing.List[Fixit]:
        """
        Get the fixits associated with this result. Also builds an ignore fixit, if necessary.
        """
        to_return = self._fixits
        if self.anchor is not None:
            lines = chapel.get_file_lines(context, self.anchor)
            ignore = _build_ignore_fixit(self.anchor, lines, name)
            to_return.append(ignore)
        return to_return


_AdvancedRuleResult = typing.Iterator[
    typing.Union[chapel.AstNode, AdvancedRuleResult]
]
"""Internal type for advanced rule results"""
AdvancedRule = typing.Callable[
    [chapel.Context, chapel.AstNode], _AdvancedRuleResult
]
"""Function type for advanced rules"""

RuleResult = typing.Union[_BasicRuleResult, _AdvancedRuleResult]
"""Union type for all rule results"""
Rule = typing.Union[BasicRuleCheck, AdvancedRule]
"""Union type for all rules"""



FixitHook = typing.Callable[
    [chapel.Context, RuleResult],
    typing.Optional[typing.Union[Fixit, typing.List[Fixit]]],
]
"""
Function type for fixits; (context, data) -> None or Fixit or List[Fixit]
"""


@dataclass
class BasicRule:
    """
    Class containing all information for the driver about rules"""
    name: str
    pattern: typing.Any
    check_func: BasicRuleCheck
    fixit_funcs: typing.List[FixitHook] = field(default_factory=list)
