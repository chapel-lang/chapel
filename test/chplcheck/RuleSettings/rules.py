
from chapel import AstNode
from typing import Optional

def var_string(**kwargs):
    s = []
    for k, v in kwargs.items():
        val = v if v is None else f"'{v}'"
        s.append(f"{k}={val}")
    return ", ".join(s)

def rules(driver):

    @driver.basic_rule(AstNode, settings=[".Local", "Global"])
    def One(context, node, Local: Optional[str]=None, Global: str="default"):

        print(f"In One:", var_string(Local=Local, Global=Global))
        return True

    @driver.advanced_rule(settings=[".Local", "Global"])
    def Two(context, node, Local: Optional[str]=None, Global: str="different default"):

        print(f"In Two:", var_string(Local=Local, Global=Global))
        yield from []

