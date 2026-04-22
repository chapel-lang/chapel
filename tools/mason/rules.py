import chapel
from rule_types import BasicRuleResult
from fixits import Fixit, Edit


def rules(driver):

    @driver.basic_rule(chapel.StringLikeLiteral)
    def OnlyDoubleQuote(_, node: chapel.StringLikeLiteral):
        if node.quote_style() != '"' and node.quote_style() != '"""':
            # if the string contains a double quote, its ok
            # single quotes used to avoid escaping
            if node.value().find('"') != -1:
                return True
            return BasicRuleResult(node, ignorable=False)
        else:
            return True

    @driver.fixit(OnlyDoubleQuote)
    def OnlyDoubleQuoteFixit(context: chapel.Context, result: BasicRuleResult):
        if result.node.quote_style() == '"""':
            return None
        loc = result.node.location()
        s = result.node.value()
        fixit = Fixit.build(Edit.build(loc, '"' + s + '"'))
        return fixit
