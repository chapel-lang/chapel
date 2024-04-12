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

import typing
from dataclasses import dataclass

import chapel
from fixits import Fixit


@dataclass
class BasicRuleResult:
    """
    Result type for basic rules. Rules can also return a plain boolean to
    represent a simple pass/fail result, with no fixit.
    """
    fixit: typing.Union[Fixit, typing.List[Fixit]]


_BasicRuleResult = typing.Union[bool, BasicRuleResult]
"""Internal type for basic rule results"""
BasicRule = typing.Callable[[chapel.Context, chapel.AstNode], _BasicRuleResult]
"""Function type for basic rules"""


@dataclass
class AdvancedRuleResult:
    """
    Result type for advanced rules. Advanced rules can also return a plain
    boolean to represent a simple pass/fail result, with no fixit.
    """
    node: chapel.AstNode
    anchor: typing.Optional[chapel.AstNode] = None
    fixit: typing.Optional[
        typing.Union[Fixit, typing.List[Fixit]]
    ] = None


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
Rule = typing.Union[BasicRule, AdvancedRule]
"""Union type for all rules"""
