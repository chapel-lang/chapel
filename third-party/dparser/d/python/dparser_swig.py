# This file was created automatically by SWIG.
import dparser_swigc
class user_pyobjectsPtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __setattr__(self,name,value):
        if name == "t" :
            dparser_swigc.user_pyobjects_t_set(self.this,value)
            return
        if name == "s" :
            dparser_swigc.user_pyobjects_s_set(self.this,value)
            return
        if name == "inced_global_state" :
            dparser_swigc.user_pyobjects_inced_global_state_set(self.this,value)
            return
        self.__dict__[name] = value
    def __getattr__(self,name):
        if name == "t" : 
            return dparser_swigc.user_pyobjects_t_get(self.this)
        if name == "s" : 
            return dparser_swigc.user_pyobjects_s_get(self.this)
        if name == "inced_global_state" : 
            return dparser_swigc.user_pyobjects_inced_global_state_get(self.this)
        raise AttributeError,name
    def __repr__(self):
        return "<C user_pyobjects instance>"
class user_pyobjects(user_pyobjectsPtr):
    def __init__(self,this):
        self.this = this




class d_loc_tPtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __setattr__(self,name,value):
        if name == "pathname" :
            dparser_swigc.d_loc_t_pathname_set(self.this,value)
            return
        if name == "previous_col" :
            dparser_swigc.d_loc_t_previous_col_set(self.this,value)
            return
        if name == "col" :
            dparser_swigc.d_loc_t_col_set(self.this,value)
            return
        if name == "line" :
            dparser_swigc.d_loc_t_line_set(self.this,value)
            return
        self.__dict__[name] = value
    def __getattr__(self,name):
        if name == "pathname" : 
            return dparser_swigc.d_loc_t_pathname_get(self.this)
        if name == "previous_col" : 
            return dparser_swigc.d_loc_t_previous_col_get(self.this)
        if name == "col" : 
            return dparser_swigc.d_loc_t_col_get(self.this)
        if name == "line" : 
            return dparser_swigc.d_loc_t_line_get(self.this)
        raise AttributeError,name
    def __repr__(self):
        return "<C d_loc_t instance>"
class d_loc_t(d_loc_tPtr):
    def __init__(self,this):
        self.this = this




class D_ParseNodePtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __setattr__(self,name,value):
        if name == "symbol" :
            dparser_swigc.D_ParseNode_symbol_set(self.this,value)
            return
        if name == "start_loc" :
            dparser_swigc.D_ParseNode_start_loc_set(self.this,value.this)
            return
        if name == "globals" :
            dparser_swigc.D_ParseNode_globals_set(self.this,value)
            return
        if name == "user" :
            dparser_swigc.D_ParseNode_user_set(self.this,value.this)
            return
        self.__dict__[name] = value
    def __getattr__(self,name):
        if name == "symbol" : 
            return dparser_swigc.D_ParseNode_symbol_get(self.this)
        if name == "start_loc" : 
            return d_loc_tPtr(dparser_swigc.D_ParseNode_start_loc_get(self.this))
        if name == "globals" : 
            return dparser_swigc.D_ParseNode_globals_get(self.this)
        if name == "user" : 
            return user_pyobjectsPtr(dparser_swigc.D_ParseNode_user_get(self.this))
        raise AttributeError,name
    def __repr__(self):
        return "<C D_ParseNode instance>"
class D_ParseNode(D_ParseNodePtr):
    def __init__(self,this):
        self.this = this






#-------------- FUNCTION WRAPPERS ------------------

my_d_loc_t_s_set = dparser_swigc.my_d_loc_t_s_set

my_d_loc_t_s_get = dparser_swigc.my_d_loc_t_s_get

my_D_ParseNode_end_set = dparser_swigc.my_D_ParseNode_end_set

my_D_ParseNode_end_skip_set = dparser_swigc.my_D_ParseNode_end_skip_set

my_D_ParseNode_end_get = dparser_swigc.my_D_ParseNode_end_get

my_D_ParseNode_end_skip_get = dparser_swigc.my_D_ParseNode_end_skip_get

my_D_ParseNode_symbol_get = dparser_swigc.my_D_ParseNode_symbol_get

remove_parse_tree_viewer = dparser_swigc.remove_parse_tree_viewer

add_parse_tree_viewer = dparser_swigc.add_parse_tree_viewer

del_parser = dparser_swigc.del_parser

make_parser = dparser_swigc.make_parser

run_parser = dparser_swigc.run_parser

set_parser_functions = dparser_swigc.set_parser_functions

def d_get_child(arg0,arg1):
    val = dparser_swigc.d_get_child(arg0.this,arg1)
    val = D_ParseNodePtr(val)
    return val

def d_find_in_tree(arg0,arg1):
    val = dparser_swigc.d_find_in_tree(arg0.this,arg1)
    val = D_ParseNodePtr(val)
    return val

def d_get_number_of_children(arg0):
    val = dparser_swigc.d_get_number_of_children(arg0.this)
    return val



#-------------- VARIABLE WRAPPERS ------------------

