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
    def One(c, n, Local: Optional[str] = None, Global: Optional[str] = None):
        print(f"In One:", var_string(Local=Local, Global=Global))
        return True

    @driver.advanced_rule(settings=[".Local", "Global"])
    def Two(c, n, Local: Optional[str] = None, Global: Optional[str] = None):
        print(f"In Two:", var_string(Local=Local, Global=Global))
        yield from []
