# Copyright (c) 2003, 2004 Brian Sabbey
# contributions by Milosz Krajewski

import sys, types, os, md5, dparser_swigc, string 

class user_pyobjectsPtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __setattr__(self,name,value):
        if name == "t" :
            dparser_swigc.user_pyobjects_t_set(self.this,value)
            return
#        if name == "s" :
#            dparser_swigc.user_pyobjects_s_set(self.this,value)
            return
        self.__dict__[name] = value
    def __getattr__(self,name):
        if name == "t" : 
            return dparser_swigc.user_pyobjects_t_get(self.this)
#        if name == "s" : 
#            return dparser_swigc.user_pyobjects_s_get(self.this)
        raise AttributeError,name
    def __repr__(self):
        return "<C user_pyobjects instance>"
    
class user_pyobjects(user_pyobjectsPtr):
    def __init__(self,this):
        self.this = this

class d_loc_tPtr :
    def __init__(self, this, d_parser):
        self.this = this
        self.thisown = 0
        self.d_parser = d_parser
    def __setattr__(self, name, value):
        if name == "s" :
            dparser_swigc.my_d_loc_t_s_set(self.this,self.d_parser,value)
            return
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
    def __getattr__(self, name):
        if name == "s" : 
            return dparser_swigc.my_d_loc_t_s_get(self.this,self.d_parser)
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
    def __init__(self, this, d_parser, buf):
        self.this = this
        self.d_parser = d_parser
        self.buf = buf
        
class D_ParseNodePtr :
    def __init__(self, this):
        self.this = this
        self.thisown = 0
    def __setattr__(self, name, value):
        #        if name == "symbol" :
        #            dparser_swigc.D_ParseNode_symbol_set(self.this,value)
        #    return
        if name == "start_loc" :
            dparser_swigc.D_ParseNode_start_loc_set(self.this,value.this)
            return
        if name == "end" :
            dparser_swigc.my_D_ParseNode_end_set(self.this,self.d_parser,value)
            return
        if name == "end_skip" :
            dparser_swigc.my_D_ParseNode_end_skip_set(self.this,self.d_parser,value)
            return
        if name == "globals" :
            dparser_swigc.D_ParseNode_globals_set(self.this,value)
            return
        if name == "user" :
            dparser_swigc.D_ParseNode_user_set(self.this,value.this)
            return
        self.__dict__[name] = value
    def __getattr__(self, name):
        if name == "symbol" : 
            return dparser_swigc.my_D_ParseNode_symbol_get(self.this,self.d_parser)
        if name == "end" : 
            return dparser_swigc.my_D_ParseNode_end_get(self.this,self.d_parser)
        if name == "end_skip" : 
            return dparser_swigc.my_D_ParseNode_end_skip_get(self.this,self.d_parser)
        if name == "globals" : 
            return dparser_swigc.D_ParseNode_globals_get(self.this)
        if name == "user" : 
            return user_pyobjectsPtr(dparser_swigc.D_ParseNode_user_get(self.this))
        if name == "start_loc" :
            val = self.__dict__.get("start_loc")
            if not val:
                val = self.__dict__["start_loc"] = d_loc_t(dparser_swigc.D_ParseNode_start_loc_get(self.this), self.d_parser, self.buf)
            return val
        if name == "c":
            val = self.__dict__.get("c", None)
            if not val:
                nc = dparser_swigc.d_get_number_of_children(self.this)
                val = self.__dict__["c"] = [None]*nc
                for i in range(0, nc):
                    val[i] = D_ParseNode(dparser_swigc.d_get_child(self.this, i), self.d_parser, self.buf)
            return val
        raise AttributeError,name
    def __repr__(self):
        return "<C D_ParseNode instance>"
    
class D_ParseNode(D_ParseNodePtr):
    def __del__(self):
        if dparser_swigc:
            dparser_swigc.remove_parse_tree_viewer(self.d_parser)
    def __init__(self, this, d_parser, buf):
        self.this = this
        self.d_parser = d_parser
        self.buf = buf
        dparser_swigc.add_parse_tree_viewer(self.d_parser)

class Reject: pass

class SyntaxError(Exception):
    pass
class AmbiguityException(Exception):
    pass

def my_syntax_error_func(loc):
    ee = '...'
    be = '...'
    width = 25
    mn = loc.s - width
    if mn < 0:
        mn = 0
        be = ''
    mx = loc.s + 25
    if mx > len(loc.buf):
        mx = len(loc.buf)
        ee = ''
    begin = loc.buf[mn:loc.s]
    end = loc.buf[loc.s:mx]
    string = '\n\nsyntax error, line:' + str(loc.line) + '\n\n' + be + begin +  '[syntax error]' + end + ee + '\n'
    raise SyntaxError, string

def my_ambiguity_func(nodes):
    raise AmbiguityException, "\nunresolved ambiguity.  Symbols:\n" + string.join([node.symbol for node in nodes], "\n")

class Parser:
    def __init__(self, modules=None, file_prefix="d_parser_mach_gen",make_grammar_file=0):
        self.file_prefix = file_prefix
        self.actions = []
        sig = md5.new()
        sig.update('1.12')
        
        if not modules:
            try:
                raise RuntimeError
            except RuntimeError:
                e,b,t = sys.exc_info()
            
            dicts = [t.tb_frame.f_back.f_globals]
        else:
            if type(modules) == list:
                dicts = [module.__dict__ for module in modules]
            else:
                dicts = [modules.__dict__]
                
        functions = []
        for dict in dicts:
            f = [val for name, val in dict.items() 
                 if (isinstance(val, types.FunctionType)) and name[0:2] == 'd_']
            f.sort(lambda x, y: cmp(x.func_code.co_filename, y.func_code.co_filename) or cmp(x.func_code.co_firstlineno, y.func_code.co_firstlineno))
            functions.extend(f)
        if len(functions) == 0:
            raise "\nno actions found.  Action names must start with 'd_'"

        app_path = os.path.dirname(sys.argv[0])
        if len(app_path) == 0:
            app_path = os.getcwd()
        app_path = string.replace(app_path, '\\', '/')

        self.filename = app_path + '/' + self.file_prefix + ".g"
        grammar_str = []
        for f in functions:
            if f.__doc__:
                grammar_str.append(f.__doc__) 
                sig.update(f.__doc__)
            else:
                raise "\naction missing doc string:\n\t" + f.__name__
            grammar_str.append(" ${action};\n")
            if f.func_code.co_argcount == 0:
                raise "\naction " + f.__name__ + " must take at least one argument\n"
            speculative = 0
            takes_strings = 0
            arg_types = [0]
            for i in range(1, f.func_code.co_argcount):
                var = f.func_code.co_varnames[i]
                if var == 'spec':
                    arg_types.append(1)
                    speculative = 1
                elif var == 'g':
                    arg_types.append(2)
                elif var == 's':
                    arg_types.append(3)
                    takes_strings = 1
                elif var == 'nodes':
                    arg_types.append(4)
                elif var == 'this':
                    arg_types.append(5)
                elif var == 'spec_only':
                    arg_types.append(6)
                    speculative = -1
                elif var == 'parser':
                    arg_types.append(7)
                else:
                    raise "\nunknown argument name:\n\t" + var + "\nin function:\n\t" + f.__name__
            self.actions.append((f, arg_types, speculative, takes_strings))
        grammar_str = string.join(grammar_str, '')
        
        if make_grammar_file:
            g_file = open(self.filename, "wb") # 'binary' mode has been set to force \n on end of the line
            g_file.write(grammar_str)
            g_file.close()

        if self.sig_changed(sig):
            dparser_swigc.make_tables(grammar_str, self.filename)
            sig_file = open(self.filename + ".md5", "w")
            sig_file.write("%s\n" % repr(sig.digest()))
            sig_file.close()

        self.tables = dparser_swigc.load_parser_tables(self.filename + ".d_parser.dat")
    def sig_changed(self, sig):
        try:
            sig_file = open(self.filename + ".md5", "r")
            line = sig_file.readline()
            sig_file.close()
            if line and eval(line) == sig.digest():
                return 0
        except IOError, SyntaxError:
            pass
        return 1

    def parse(self, buf, buf_offset=0,
              initial_skip_space_fn=None,
              syntax_error_fn=my_syntax_error_func, ambiguity_fn=my_ambiguity_func,
              make_token=None,
              dont_fixup_internal_productions=0,
              dont_merge_epsilon_trees=0,
              commit_actions_interval=0,
              error_recovery=0,
              print_debug_info=0,
              partial_parses=0,
              dont_compare_stacks=0,
              dont_use_eagerness_for_disambiguation=0,
              dont_use_height_for_disambiguation=0,
              start_symbol=''):
        parser = dparser_swigc.make_parser(self.tables, self, Reject, make_token, d_loc_t, D_ParseNode,
                                           self.actions, initial_skip_space_fn, syntax_error_fn,
                                           ambiguity_fn, dont_fixup_internal_productions,
                                           dont_merge_epsilon_trees,
                                           commit_actions_interval,
                                           error_recovery,
                                           print_debug_info,
                                           partial_parses,
                                           dont_compare_stacks,
                                           dont_use_eagerness_for_disambiguation,
                                           dont_use_height_for_disambiguation,
                                           start_symbol)
        result = dparser_swigc.run_parser(parser, buf, buf_offset)
        if result == None:
            return None
        if len(result) == 3:
            self.s = result[2]
        self.top_node = result[1]
        return result[0]
