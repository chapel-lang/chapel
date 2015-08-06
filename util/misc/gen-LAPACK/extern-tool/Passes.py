import xmltool
from xmltool import *
import codetools
from codetools import *
import copy

# Relative paths to LAPACK and its subdirectories.
lapack_root = "../LAPACK"
lapack_src = lapack_root + "/SRC"
lapack_matgen_src = lapack_root + "/TESTING/MATGEN"
lapack_install_src = lapack_root + "/INSTALL"
lapacke_include = lapack_root + "/lapacke/include"
blas_src = lapack_root + "/BLAS/SRC"

# Parses the documentation out from the text, stripping the comment tokens
# Captures: [1,text] raw documentation text, stripped of comment tokens
f_comment_regex = re.compile( r"(?:^|\n)\*>?(?P<text>.*)" )

# Parses the source out from the text
# Captures: [1,text] raw source code text
f_source_regex = re.compile( r"(?:^|\n)(?!\*>?)(?P<text>.*)" )

# Parses function declaration from the c header file
# Captures: [1,returns] return type, [2,name] function name, [3,arguments] full unparsed param list
c_func_decl_regex = re.compile( r"(?P<returns>\w+)\s+(?P<name>\w+)\s*\(\s*(?P<arguments>(?:\w+\s+)?\w+(?:\s*\*\s*|\s+)\w+(?:\s*,\s*(?:\w+\s+)?\w+(?:\s*\*\s*|\s+)\w+)*)?\s*\)\s*;" )

# Parses arguments to a function
# Captures: [1,modifier]? const modifier, [2,type] type, [3,refdepth] string containing whitespace and/or astierisk(s), [3,name] param name
c_args_regex = re.compile( r"(?:(?P<modifier>const)\s+)?(?P<type>\w+)(?P<refdepth>(?:\s*\*+\s*)|\s+)(?P<name>\w+)" )

# Parsers function declaration and argument documentation from the fortran code (Does not yet work with source. requires $ delimiting as well)
# Captures: [1,type]? return type, [2,name] function name, [3,arguments] full unparsed argument names, 
doc_func_regex = re.compile( r"(?:(?P<type>(?:[\w\*]+)(?: +\w+)*)\s+)?(?:(?:SUBROUTINE)|(?:FUNCTION))\s+(?P<name>\w+)\(\s*(?P<arguments>(?:\w+\s*(?:,\s*\w+\s*)*)?)\s*\)" )

# Parses the scalar arguments from the documentation (TODO determine if source works too.)
# Captures: [1,body] full unparsed text of scalar arguments documentation
doc_scalarargs_regex = re.compile( r"Scalar Arguments\s+\.\.\s+(?P<body>(?:.|\n)*?)\s+\.\." )
doc_scalarargs_decls_regex = re.compile( r"(?P<type>(?:[\w\*]+)(?: +\w+)*) +(?P<names>\w+(?:\s*,(?:\s*\$\s*)?\s*\w+)*)" )

# Parses the array arguments from the documentation (TODO determine if source works too.)
# Captures: [1,body] full unparsed text of array arguments documentation
doc_arrayargs_regex = re.compile( r"Array Arguments\s+\.\.\s+(?P<body>(?:.|\n)*?)\s+\.\." )
doc_arrayargs_decls_regex = re.compile( r"(?P<type>(?:[\w\*]+)(?: +\w+)*) +(?P<names>\w+(?:\([\s\S]*?\))?(?:\s*,(?:\s*\$\s*)?\s*\w+(?:\([\s\S]*?\))?)*)" )
doc_arrayargs_decls_names_dims_regex = re.compile( r"(?P<name>\w+)(?:\((?P<dimensions>.*?)\))?," )

doc_functionargs_regex = re.compile( r"Function Arguments\s+\.\.\s+(?P<body>(?:.|\n)*?)\s+\.\." )
doc_functionargs_decls_regex = doc_scalarargs_decls_regex

# Parses the argument information from the documentation
# Captures: [1,name] name, [2,intent] intent, [3,body] full unparsed body of argument document
doc_args_regex = re.compile( r"\s+\\param\[(?P<intent>\w+(?:,\w+)*)\]\s+(?P<name>\w+)\s+\\verbatim\s+(?P<body>(?:[\s\S])+?)\s*\\endverbatim" )

# Parses the typeinfo group of doc_args_regex
# Captures: [1,name] argument name, [2,type] type, [3,array]? captures array keyword if exists, [4,dimension] captures text describing dimensions
doc_args_typeinfo_regex = re.compile( r"(?P<name>\w+)\s+is\s+(?P<type>\w+)(?: (?P<array>array), dimension\s+(?P<dimensions>.*))?" )

# Parses the argument documentation and provides the matrix size of an array (if there is one)
# Captures: [1] 1st dimension, [2] 2nd dimension
doc_args_dimensions_regex = re.compile( r"(\w+)[- ]by[- ](\w+)(?: coefficient)? matrix" )

# Parses the human documentation of the fortran base of scalar ints to determine how (if at all) they relate to matrix arrays
# Captures: [1,what] the semantic information of relation (order, rows, columns, rank) [2,who] an unbroken sentence of names referring to matrices/arrays
scalar_matrix_relation_regex = re.compile( r"(?:number\s+of\s+)?(?P<what>\w+)\s+(?:(?:of)|(?:in))\s+(?:the\s+)?(?:input\s+)?(?:(?:matrix)|(?:matrices)|(?:submatrix))?(?:\s+)?(?P<who>(?:(?:(?:\w+\( \w+ \))|(?:\w+))\s*)+)" );

# Parses the function purpose documentation from the documentation
# Captures: [1,body] the human readable text documenting the purpose of the function
doc_purpose_regex = re.compile( r"\\par\s+Purpose:\s+=+\s+\\verbatim\s+(?P<body>(?:[\s\S]+?))\s+\\endverbatim" )

# Parses function names
# Captures: [1,type] literal type of matrix, [2,config] configuration type of matrix, [3,function] function group
#func_name_group_regex = re.compile( r"^(?P<type>[dszc]|(?:ds)|(?:zc))(?P<config>(?:bd)|(?:di)|(?:gb)|(?:ge)|(?:gg)|(?:gt)|(?:hb)|(?:he)|(?:hg)|(?:hp)|(?:hs)|(?:op)|(?:or)|(?:pb)|(?:po)|(?:pp)|(?:pt)|(?:sb)|(?:sp)|(?:st)|(?:sy)|(?:tb)|(?:tg)|(?:tp)|(?:tr)|(?:tz)|(?:un)|(?:up))(?P<function>.+)" )
func_name_group_regex = re.compile( r"^(?P<type>[dszc]|(?:ds)|(?:zc))(?P<config>\w\w)(?P<function>\w\w\w*)" )

'''
class ResolutionFailure ( Exception )

Purpose:
Exception for errors encountered during Pass.resolve( ) calls.

Member Functions:
__init__( self, value ):
constructor. Value is the Pass class who errored during resolution

__str__( self ):
returns string stating which Pass class had an error.

Member Variables:
value:
the Pass class that errored during resolution

'''
class ResolutionFailure ( Exception ):
  def __init__(self, value):
    self.value = value
  def __str__(self):
    return "Error applying " + repr(self.value) + " to tree"

'''
class GeneralPassFailure ( Exception )

Purpose:
Generic exception class that is thrown when passes encounter critical errors

Member Functions:
__init__( self, message ):
constructor. Message is the message from the Pass to the user

__str__( self ):
returns the message to from the Pass to the user

Member Variables:
message:
the message to from the Pass to the user

'''
class GeneralPassFailure ( Exception ):
  def __init__(self, message):
    self.message = message
  def __str__(self):
    return repr(self.message)

'''
class Pass

Purpose:
Parent of all other pass classes.
Container of input xml file.
Dependency resolver.

Member Functions:
resolve( staticclass, xml_tree ):
recursively resolves dependencies of pass staticclass onto xml_tree

apply( xml_tree ):
abstract static method(?).
raises NotImplementedError exception

Member Variables:
compete:
boolean has-complete-pass-somewhere-once.
if false, pass has never been performed or failed to perform
if true, pass has been performed once.

dependencies:
list of dependencies that must be completed before pass can be applied

input_xml:
the user input xml that give input from the user to the passes
especially for passes that could not do automated finding in the source text
'''
class Pass:
  complete = False
  dependencies = []
  input_xml = loadxml( "./input.xml" )
  
  @staticmethod
  def resolve( staticclass, xml_tree ):
    print "Resolving", staticclass, "Dependencies"
    for dep in staticclass.dependencies:
      if not dep.complete:
        dep.apply( xml_tree )
        if not dep.complete:
          raise ResolutionFailure( dep )
          
    print "Resolved", staticclass, "Dependencies"
  
  @staticmethod
  def apply( xml_tree ):
    raise NotImplementedError

'''
This is the pattern to be adopted by all Pass classes

class GenericPass ( Pass ):
  dependencies = []
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = GenericPass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    # Work
    raise NotImplementedError #REMOVE
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"
'''

'''
class CreateTreePas ( Pass )

Purpose:
takes in xmlelement with a root node, creates <LAPACK> and <LAPACKE> root nodes
'''
class CreateTreePass ( Pass ):
  dependencies = []
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = CreateTreePass
    Pass.resolve( selfname, xml_tree )
    print "[",selfname,"]"

    SubElement( xml_tree, "LAPACK" )
    SubElement( xml_tree, "LAPACKE" )
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"
 
'''
class DocumentSplitPass ( Pass )

Purpose:
Strip documentation (fortran comments) from code into seperate nodes 
under common file node under LAPACK node.
'''
class DocumentSplitPass ( Pass ):
  dependencies = [CreateTreePass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = DocumentSplitPass
    Pass.resolve( selfname, xml_tree )
    print "[",selfname,"]"
    
    text_node = SubElement( xml_tree.find( "./LAPACK" ), "text" )
    
    src_files = []
    
    source_dirs = [lapack_src, lapack_matgen_src, blas_src]
    
    for dir in source_dirs:
      for file in os.listdir( dir ):
        if fnmatch.fnmatch( file, '*.f' ):
         src_files.append( dir + "/" + file )
    
    file_count = 1
    for file in src_files:
      print file," (", file_count, "/", len(src_files),")"
      file_node = SubElement( text_node, "file" )
      file_node.set( "name", file )
      src_node = SubElement( file_node, "source" )
      doc_node = SubElement( file_node, "documentation" )
      
      src_node.text = str()
      doc_node.text = str()
      
      file_read = open( file ).read()
      
      for doc_match in f_comment_regex.finditer( file_read ):
        doc_node.text += doc_match.group( "text" ) + "\n"
      
       # Disabled the disabling# Disabled. Works. No use. Unnecessary load on tree.
      for src_match in f_source_regex.finditer( file_read ):
        src_node.text += src_match.group( "text" ) + "\n"
      
      file_count += 1
      
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class LAPACKFunctionDefinePass ( Pass )

Purpose:
find fortran functions in the documentation and put them in the 
<procedures> node under <LAPACK>
'''
class LAPACKFunctionDefinePass ( Pass ):
  dependencies = [DocumentSplitPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = LAPACKFunctionDefinePass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"
    
    lapack_node = xml_tree.find( "./LAPACK" )
    text_node = lapack_node.find( "./text" )
    procs_node = SubElement( lapack_node, "procedures" )
    
    file_nodes = text_node.findall( "./file" )
    file_count = 1
    for file_node in file_nodes:
      print file_node.get("name")," (", file_count, "/", len(file_nodes),")"
      
      file_doc = file_node.find( "./documentation" )
      for proc_decl in doc_func_regex.finditer( file_doc.text ):
        proc_node = SubElement( procs_node, "procedure" )
        proc_node.set( "name", proc_decl.group( "name" ) )
        proc_node.set( "file-name", file_node.get( "name" ) )
        
        if proc_decl.group( "type" ) != None:
          proc_node.set( "return-type", proc_decl.group( "type" ) )
        #print "\t", proc_decl.group("name")
        
        arguments = proc_decl.group( "arguments" ).split( "," );
        if len( arguments ) >= 1 and arguments[0] != "":
          args_node = SubElement( proc_node, "arguments-list" )
          arg_counter = 0
          for arg in arguments:
            #print "\t\t",arg.strip()
            
            arg_node = SubElement( args_node, "argument" )
            arg_node.set( "name", arg.strip() )
            arg_node.set( "position", str(arg_counter) )
            
            arg_counter += 1
        
        #SubElement( proc_node, "documentation" )
        
      file_count += 1   

    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class FuncPurposeDocPass ( Pass )

Purpose:
collect function purpose documentation from fortran text
'''
class FuncPurposeDocPass ( Pass ):
  dependencies = [LAPACKFunctionDefinePass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = FuncPurposeDocPass
    Pass.resolve( selfname, xml_tree )
    print "[",selfname,"]"
    
    lapack_node = xml_tree.find( "./LAPACK" )
    text_node = lapack_node.find( "./text" )
    procs_node = lapack_node.find( "./procedures" )

    for proc_node in procs_node.findall( "./procedure" ):
      proc_file_name = proc_node.get( "file-name" )
      doc_node = text_node.find( "./file/[@name='" + proc_file_name + "']/documentation" )
      
      purpose_match = doc_purpose_regex.search( doc_node.text )
      purpose = purpose_match.group( "body" ) if purpose_match != None else "Unspecified"
      SubElement( proc_node, "purpose" ).text = purpose
      
      
      
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class FuncArgsDocPass ( Pass ):

Purpose:
collect argument documentation from fortran text
'''
class FuncArgsDocPass ( Pass ):
  dependencies = [LAPACKFunctionDefinePass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = FuncArgsDocPass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    lapack_node = xml_tree.find( "./LAPACK" )
    text_node = lapack_node.find( "./text" )
    procs_node = lapack_node.find( "./procedures" )

    for proc_node in procs_node.findall( "./procedure" ):
      proc_file_name = proc_node.get( "file-name" )
      doc_node = text_node.find( "./file/[@name='" + proc_file_name + "']/documentation" )

      for arg_match in doc_args_regex.finditer( doc_node.text ):
        #print "\"",proc_file_name,"\"", arg_match.group()
        arg_name = arg_match.group( "name" ).strip()

        arg_node = proc_node.find( "./arguments-list/argument/[@name='" + arg_name + "']" )
        
        arg_node.set( "intent", arg_match.group( "intent" ) )
        dim_match = doc_args_dimensions_regex.search( arg_match.group( "body" ) )
        if dim_match != None:
          arg_node.set( "matrix-size", dim_match.group(1) +"," + dim_match.group(2) )
          
        SubElement( arg_node, "documentation" ).text = arg_match.group( "body" )
          
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class FuncArgsTypePass ( Pass )

NON FUNCTIONAL

Purpose:
collect argument names and types under the Scalar Arguments
and Array Arguments header and include in tree for semantic understanding
'''
class FuncArgsTypePass ( Pass ):
  dependencies = [LAPACKFunctionDefinePass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = FuncArgsTypePass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"
    
    lapack_node = xml_tree.find( "./LAPACK" )
    text_node = lapack_node.find( "./text" )
    procs_node = lapack_node.find( "./procedures" )
    
    for proc_node in procs_node.findall( "./procedure" ):
      proc_file_name = proc_node.get( "file-name" )
      doc_node = text_node.find( "./file/[@name='" + proc_file_name + "']/documentation" )
      
      # attribute scalar arguments
      scalars = doc_scalarargs_regex.search( doc_node.text )
      if scalars != None:
        for line in doc_scalarargs_decls_regex.finditer( scalars.group( "body" ) ):
          names_list = re.sub( r"[\s$]", "", line.group("names") ).split( "," )
          #print line.group( "type" ), ":", names_list
        
          type = line.group( "type" )
        
          #skip any "IMPLICIT" 'typed' arguments
          if type.lower() == "implicit":
            continue

          for name in names_list:
            arg_node = proc_node.find( "./arguments-list/argument/[@name='" + name + "']" )
          
            if arg_node == None:
              print "Non-match: argument", name, "of", proc_node.get( "name" ), "in", proc_file_name
              #prettyprintxml( proc_node.find("./arguments-list") )
              continue
            
            arg_node.set( "type", type )
            arg_node.set( "semantic", "scalar" )

      # attribute array arguments
      arrays = doc_arrayargs_regex.search( doc_node.text )
      if arrays != None:
        for line in doc_arrayargs_decls_regex.finditer( arrays.group( "body" ) ):
          name_list = re.sub( r"[\s$]", "", line.group("names") ) + ","
          type = line.group( "type" )
          
          for name_match in doc_arrayargs_decls_names_dims_regex.finditer( name_list ):
            name = name_match.group( "name" )

            arg_node = proc_node.find( "./arguments-list/argument/[@name='" + name + "']" )
          
            if arg_node == None:
              print "Non-match: argument", name, "of", proc_node.get( "name" ), "in", proc_file_name
              continue
            
            dimensions = name_match.group( "dimensions") if name_match.group( "dimensions") != None else ""
            
            arg_node.set( "type", type )
            arg_node.set( "semantic", "array" )
            arg_node.set( "dimensions", dimensions )
      
      # attribute function arguments
      functions = doc_functionargs_regex.search( doc_node.text )
      if functions != None:
        for line in doc_functionargs_decls_regex.finditer( functions.group( "body" ) ):
          names_list = re.sub( r"[\s$]", "", line.group("names") ).split( "," )
          #print line.group( "type" ), ":", names_list
        
          type = line.group( "type" )
        
          #skip any "IMPLICIT" 'typed' arguments
          if type.lower() == "external":
            continue

          for name in names_list:
            arg_node = proc_node.find( "./arguments-list/argument/[@name='" + name + "']" )
          
            if arg_node == None:
              print "Non-match: argument", name, "of", proc_node.get( "name" ), "in", proc_file_name
              #prettyprintxml( proc_node.find("./arguments-list") )
              continue
            
            arg_node.set( "type", type )
            arg_node.set( "semantic", "function" )
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class AssociateArgsToArrayPass ( Pass ):

Purpose:
Threshes out scalar-argument pairing for array concepts.
'''
class AssociateArgsToArrayPass ( Pass ):
  dependencies = [FuncArgsTypePass, FuncArgsDocPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = AssociateArgsToArrayPass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    lapack_node = xml_tree.find( "./LAPACK" )
    text_node = lapack_node.find( "./text" )
    procs_node = lapack_node.find( "./procedures" )

    #proc_info = {} # {func_name} => { arg name } => [ what, what, who ]

    for proc_node in procs_node.findall( "./procedure" ):
      proc_name = proc_node.get( "name" )
      
      '''
      if not proc_name in proc_info:
        proc_info[ proc_name ] = {}
      '''
      base_name = proc_name.lower()
      match = func_name_group_regex.search( base_name );
      if match == None:
        #print proc_name, "(", base_name, ") does not match regex"
        continue
    
      func = match.group( "function" )
      config = match.group( "config" )
      type = match.group( "type" )
    
      if not config.startswith( "ge" ):
        pass
      
      arg_names = [ arg.get("name") for arg in proc_node.findall( "./arguments-list/argument" ) ]
        
      for arg_node in proc_node.findall( "./arguments-list/argument" ):
        doc_node = arg_node.find( "documentation" )
        if doc_node == None or arg_node.get("semantic") != "scalar" or arg_node.get("type").lower() != "integer":
          continue
          
        what = []
        who = []
        string = []
        
        for m in scalar_matrix_relation_regex.finditer( doc_node.text ):
          if not m.group( "what" ) in ["rows", "columns", "order", "rank"] :
            continue
          
          names = m.group( "who" ).strip()
          names_list = []
          if " and " in names:
            names_list = [ name.strip() for name in names.split( "and" ) ]
          else:
            names_list = [ names ]

          nameHasSpace = False
          for name in names_list:
            if " " in name:
              nameHasSpace = True
              break
          
          if nameHasSpace:
            print names, " contains non names. Skipping."
            continue
          
          removes = []
          for name in names_list:
            if not name in arg_names:
              removes.append( name )

          for rm in removes:
            names_list.remove( rm )

          if len( names_list ) == 0:
            print "Names list had no argument names. Skipping" 
            continue
          
          what.append( m.group( "what" ) )
          who.append( names_list )
          string.append( re.sub( "\s+", " ", m.group(0) ) )
          
        if len( what ) == 0 and len( who ) == 0:
          continue
        
        #proc_info[ proc_name ][ arg_node.get( "name" ) ] =  [ what, who, string]
        
        associate_array = str()
        associate_field = str()

        first = True
        
        for i in range( len( who ) ):
          for array in who[i]:
            associate_array += ( "," if not first else "" ) + array
            associate_field += ( "," if not first else "" ) + what[i]
            first = False
          
        
        arg_node.set( "associate-array", associate_array )
        arg_node.set( "associate-field", associate_field )

    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class BaseLAPACKPass ( Pass )

Purpose:
Tie together all passes over the LAPACK fortran source code and 
resulting semantic analysis
'''
class BaseLAPACKPass ( Pass ):
  dependencies = [FuncArgsTypePass, FuncArgsDocPass, AssociateArgsToArrayPass, FuncPurposeDocPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = BaseLAPACKPass
    Pass.resolve( selfname, xml_tree )
    print "[",selfname,"]"
        
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class LAPACKEFunctionDefinePass ( Pass ):

Purpose:
from lapacke.h text define all C function decls
in under the <LAPACKE> tree.
'''
class LAPACKEFunctionDefinePass ( Pass ):
  dependencies = [CreateTreePass] # TODO include BaseLAPACKPass when the two need to meet
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = LAPACKEFunctionDefinePass
    Pass.resolve( selfname, xml_tree )
    print "[",selfname,"]"
    
    lapacke_root = xml_tree.find( "./LAPACKE" )
    procs_node = SubElement( lapacke_root, "procedures" )
    
    lapacke_header = open( lapacke_include + "/lapacke.h" ).read()

    for func_decl in c_func_decl_regex.finditer( lapacke_header ):
      
      #if func_decl.group( "name" ).lower().startswith( "lapacke_" ): continue
      
      if procs_node.find( "./procedure/[@name='" + func_decl.group( "name" ) + "']" ) != None:
        print "proc", func_decl.group( "name" ), "redefined. Skipping"
        continue
      
      proc_node = SubElement( procs_node, "procedure" )
      proc_node.set( "name", func_decl.group( "name" ) )
      proc_node.set( "return-type", func_decl.group( "returns" ) )
      args_node = SubElement( proc_node, "arguments-list" )

      arg_count = 0
      for arg in c_args_regex.finditer( func_decl.group( "arguments" ) ):
        arg_node = SubElement( args_node, "argument" )
        arg_node.set( "name", arg.group( "name" ) )
        arg_node.set( "type", arg.group( "type" ) )
        arg_node.set( "refdepth", str( arg.group( "refdepth" ).count("*") ) )
        if arg.group( "modifier" ) != None:
          arg_node.set( "modifier", arg.group( "modifier" ) )
        arg_node.set( "position", str(arg_count) )
        
        arg_count += 1
        
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class BaseLAPACKEPass ( Pass )

Purpose:
Ties together all passes over the lapacke.h text
and any basic analysis
'''
class BaseLAPACKEPass ( Pass ):
  dependencies = [LAPACKEFunctionDefinePass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = BaseLAPACKEPass
    Pass.resolve( selfname, xml_tree )
    print "[",selfname,"]"
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class CharacterArraySemanticsCorrectionPass ( Pass )

Purpose:
lapack fortran documentation defines character*1 (single characters) under 
array semantics. This corrects that to be a scalar.
'''
class CharacterArraySemanticsCorrectionPass ( Pass ):
  dependencies = [BaseLAPACKPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = CharacterArraySemanticsCorrectionPass
    Pass.resolve( selfname, xml_tree )
    print "[",selfname,"]"
    
    lapack_f_procs = xml_tree.find( "./LAPACK/procedures" )

    for f_proc in lapack_f_procs.findall( "./procedure" ):
      for f_arg in f_proc.findall( "./arguments-list/argument" ):
        if f_arg.get( "type" ) == None:
          continue
        #if f_arg.get( "name" ) == "JOBA":
        #  print f_proc.get( "name" ), f_arg.get( "name" ), f_arg.get( "type" ).lower()
          
        if f_arg.get( "type" ).lower() == "character*1":
        #  print f_proc.get( "name" ), f_arg.get( "name" ), f_arg.get( "type" ), f_arg.get( "semantic" ), f_arg.get( "intent" ), f_arg.get( "dimensions" )
          if f_arg.get( "semantic" ) == "array":
            f_arg.set( "semantic", "scalar" )
            if f_arg.get( "dimensions" ) != None:
              f_arg.unset( "dimensions" )
              
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class ArgumentSemanticsBucketingPass ( Pass )

Purpose:
Huristically assign argument semantics by bucketing all arguments.
Any argument who only has a None bucket and one other bucket can 
'safely' have those in the None assigned from as semantics of the other.

'''
class ArgumentSemanticsBucketingPass ( Pass ):
  dependencies = [CharacterArraySemanticsCorrectionPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = BucketArgumentsSemanticsPass
    Pass.resolve( selfname, xml_tree )
    print "[",selfname,"]"
    
    lapack_f_procs = xml_tree.find( "./LAPACK/procedures" )
    
    variables = {}
    
    for proc in lapack_f_procs.findall( "./procedure" ):
      proc_name = proc.get( "name" )
      for arg in proc.findall( "./arguments-list/argument" ):
        arg_name = arg.get( "name" )
        semantic = arg.get( "semantic" ) if arg.get( "semantic" ) != None else "none"
        
        if not arg_name in variables:
          variables[ arg_name ] = {}
        if not semantic in variables[ arg_name ]:
          variables[ arg_name ][ semantic ] = []
        
        variables[ arg_name ][ semantic ].append( proc_name )

    for arg in variables:
      if len( variables[ arg ] ) > 2:
        print arg
        for semantic in variables[ arg ]:
          print "  \"" + semantic + "\"", ":",variables[ arg ][ semantic ]

    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class AssociateLAPACKtoLAPACKEPASS ( Pass )

Purpose:
link functions and args from both the C and Fortran world
together with paths from root.
'''
class AssociateFunctionsLAPACKtoLAPACKEPass ( Pass ):
  dependencies = [BaseLAPACKEPass, BaseLAPACKPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = AssociateFunctionsLAPACKtoLAPACKEPass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    lapack_c_root = xml_tree.find( "./LAPACKE" )
    lapack_c_procs = lapack_c_root.find( "./procedures" )
    
    lapack_f_root = xml_tree.find( "./LAPACK" )
    lapack_f_procs = lapack_f_root.find( "./procedures" )
    
    for lapack_c_proc in lapack_c_procs.findall( "./procedure" ):

      proc_name = lapack_c_proc.get( "name" ).lower()

      base_name = str()
      if proc_name.startswith( "lapack_" ):
        base_name = proc_name.replace( "lapack_", "" )
      elif proc_name.startswith( "lapacke_" ):
        base_name = proc_name.replace( "lapacke_", "" )
      else:
        print "Unknown root of name:", lapack_c_proc.get( "name" )
        continue
      base_name = base_name.replace( "_work", "" )  
      base_name = base_name.upper()
      
      #print lapack_c_proc.get("name"), proc_name, base_name
      
      lapack_f_proc = lapack_f_procs.find( "./procedure/[@name='" + base_name + "']" )
      if lapack_f_proc == None:
        #print "Could not find the fortran analogue of C function", lapack_c_proc.get( "name" ), "from base-name", base_name
        continue

      SubElement( SubElementUnique( lapack_c_proc, "analogues" ), "analogue" ).text = "./LAPACK/procedures/procedure/[@name='" + lapack_f_proc.get( "name" ) + "']"
      SubElement( SubElementUnique( lapack_f_proc, "analogues" ), "analogue" ).text = "./LAPACKE/procedures/procedure/[@name='" + lapack_c_proc.get( "name" ) + "']" 
      
      '''
      misses = [] 
      for f_arg in lapack_f_proc.findall( "./arguments-list/argument" ):
        f_arg_name = f_arg.get( "name" );
        c_arg = lapack_c_proc.find( "./arguments-list/argument/[@name='" + f_arg_name.lower() + "']" )
        
        # skip non-analogous args.
        # TODO solve/mention matching failure somewhere? Maybe...
        if c_arg == None:
          #misses.append( f_arg_name )
          continue
        
        # ?_ana_node is the analogue record under ? language (ie c_ana_node notes the argument in the fortran tree, but lives in the C tree)
        # Note that it is totally possible to create the path string from the two atributes of the tag.
        # easier to create once here, instead of manytimes everywhere else.
        c_ana_node = SubElement( SubElementUnique( c_arg, "analogues" ), "analogue" )
        c_ana_node.text = "./LAPACK/procedures/procedure/[@name='" + lapack_f_proc.get( "name" ) + "']/arguments-list/argument/[@name='" + f_arg.get( "name" ) + "']"
        c_ana_node.set( "function", lapack_f_proc.get( "name" ) )
        c_ana_node.set( "name", f_arg.get( "name" ) )
        
        f_ana_node = SubElement( SubElementUnique( f_arg, "analogues" ), "analogue" )
        f_ana_node.text = "./LAPACKE/procedures/procedure/[@name='" + lapack_c_proc.get( "name" ) + "']/arguments-list/argument/[@name='" + c_arg.get( "name" ) + "']"
        f_ana_node.set( "function", lapack_c_proc.get( "name" ) )
        f_ana_node.set( "name", c_arg.get( "name" ) )
      '''    
      
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class DestroyUnassociatedCFunctionsTreePass ( Pass )

Purpose:
Remove procedures from LAPACKE subtree that do not have Fortran analogues

UNUSED
'''
class DestroyUnassociatedCFunctionsTreePass ( Pass ):
  dependencies = [AssociateFunctionsLAPACKtoLAPACKEPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = DestroyUnassociatedCFunctionsTreePass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    lapack_f_root = xml_tree.find( "./LAPACKE" )
    lapack_f_procs = lapack_f_root.find( "./procedures" )
    
    for f_proc in lapack_f_procs.findall( "./procedure" ):
      if f_proc.find( "./analogues" ) == None:
        lapack_f_procs.remove( f_proc )
        
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class DestroyUnassociatedFortranFunctionsTreePass ( Pass )

Purpose:
Remove procedures from LAPACK subtree that do not have C analogues
'''
class DestroyUnassociatedFortranFunctionsTreePass ( Pass ):
  dependencies = [AssociateFunctionsLAPACKtoLAPACKEPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = DestroyUnassociatedFortranFunctionsTreePass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    lapack_f_root = xml_tree.find( "./LAPACK" )
    lapack_f_procs = lapack_f_root.find( "./procedures" )
    
    for f_proc in lapack_f_procs.findall( "./procedure" ):
      if f_proc.find( "./analogues" ) == None:
        lapack_f_procs.remove( f_proc )
        
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class EasyAssociateArgsPass ( Pass )

Purpose:
Create association between C and Fortran analogue function arguments
when that association is easy (ie they have the same name)
'''
class EasyAssociateArgsPass ( Pass ):
  dependencies = [DestroyUnassociatedFortranFunctionsTreePass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = EasyAssociateArgsPass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    c_procs = xml_tree.find( "./LAPACKE/procedures" )
    for c_proc in c_procs.findall( "./procedure" ):

      proc_name = c_proc.get( "name" ).lower()
      
      supposed_f_ana_node = c_proc.find( "./analogues/analogue" )
      if supposed_f_ana_node == None:
        print "Proc", c_proc.get( "name" ), "has no Fortran analogues. Skipping"
        continue
        
      f_proc = xml_tree.find( supposed_f_ana_node.text )
      if f_proc == None:
        print "BAD! No analogue where analogue should exist"
        return
        #continue

      for f_arg in f_proc.findall( "./arguments-list/argument" ):
        f_arg_name = f_arg.get( "name" );
        c_arg = c_proc.find( "./arguments-list/argument/[@name='" + f_arg_name.lower() + "']" )
        
        # skip non-analogous args.
        if c_arg == None:
          continue
        
        # ?_ana_node is the analogue record under ? language (ie c_ana_node notes the argument in the fortran tree, but lives in the C tree)
        # Note that it is totally possible to create the path string from the two atributes of the tag.
        # easier to create once here, instead of manytimes everywhere else.
        c_ana_node = SubElement( SubElementUnique( c_arg, "analogues" ), "analogue" )
        c_ana_node.text = "./LAPACK/procedures/procedure/[@name='" + f_proc.get( "name" ) + "']/arguments-list/argument/[@name='" + f_arg.get( "name" ) + "']"
        c_ana_node.set( "function", f_proc.get( "name" ) )
        c_ana_node.set( "name", f_arg.get( "name" ) )
        
        f_ana_node = SubElement( SubElementUnique( f_arg, "analogues" ), "analogue" )
        f_ana_node.text = "./LAPACKE/procedures/procedure/[@name='" + c_proc.get( "name" ) + "']/arguments-list/argument/[@name='" + c_arg.get( "name" ) + "']"
        f_ana_node.set( "function", c_proc.get( "name" ) )
        f_ana_node.set( "name", c_arg.get( "name" ) )
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class ImportArgumentAnaloguesPass ( Pass )

Purpose:
Create argument associations of name per the input.xml
using the function association created automatically during runtime
'''
class ImportArgumentAssociationsPass ( Pass ):
  dependencies = [EasyAssociateArgsPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = ImportArgumentAssociationsPass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    c_procs = xml_tree.find( "./LAPACKE/procedures" )
    pass_input = Pass.input_xml.find( "./pass/[@name='ImportArgumentAnaloguesPass']" )
    
    for in_proc in pass_input.findall( "./procedure" ):
      c_proc = c_procs.find( "./procedure/[@name='" + in_proc.get( "name" ) + "']" )
      f_proc = xml_tree.find( c_proc.find( "./analogues/analogue" ).text )
      
      for in_arg in in_proc.findall( "./argument" ):
        c_arg = c_proc.find( "./arguments-list/argument/[@name='" + in_arg.get( "name" ) + "']" )
        f_arg = f_proc.find( "./arguments-list/argument/[@name='" + in_arg.get( "substitution" ) + "']" )
        #prettyprintxml( c_arg )
        if c_arg == None or f_arg == None:
          raise GeneralPassFailure( "Argument speficied in input not found in tree." + c_proc.get("name") +":"+ c_arg.get("name") )

        c_ana_node = SubElement( SubElementUnique( c_arg, "analogues" ), "analogue" )
        c_ana_node.text = "./LAPACK/procedures/procedure/[@name='" + f_proc.get( "name" ) + "']/arguments-list/argument/[@name='" + f_arg.get( "name" ) + "']"
        c_ana_node.set( "function", f_proc.get( "name" ) )
        c_ana_node.set( "name", f_arg.get( "name" ) )
      
        f_ana_node = SubElement( SubElementUnique( f_arg, "analogues" ), "analogue" )
        f_ana_node.text = "./LAPACKE/procedures/procedure/[@name='" + c_proc.get( "name" ) + "']/arguments-list/argument/[@name='" + c_arg.get( "name" ) + "']"
        f_ana_node.set( "function", c_proc.get( "name" ) )
        f_ana_node.set( "name", c_arg.get( "name" ) )
        
      #prettyprintxml( c_proc )
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class BaseAssociatePass ( Pass )

Purpose:
Ties together all association of analogues pass
'''
class BaseAssociatePass ( Pass ):
  dependencies = [EasyAssociateArgsPass, ImportArgumentAssociationsPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = BaseAssociatePass
    Pass.resolve( selfname, xml_tree )
    print "[",selfname,"]"
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class FoldLAPACKtoLAPACKEPass ( Pass ):

Purpose:
take the semantics derived from FuncArgsTypePass and 
FuncArgumentDocToSemanticsPass over the LAPACK information
and apply them to functions found in the lapacke.h code.
Especially important for the LAPACK_* C functions.
Also important for any LAPACKE_* C functions that take pointers
to scalars.
'''
class FoldLAPACKSemanticsIntentsToLAPACKEPass ( Pass ):
  dependencies = [CharacterArraySemanticsCorrectionPass, BaseAssociatePass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = FoldLAPACKSemanticsIntentsToLAPACKEPass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    lapack_c_root = xml_tree.find( "./LAPACKE" )
    lapack_c_procs = lapack_c_root.find( "./procedures" )
    
    for c_proc in lapack_c_procs.findall( "./procedure" ):
      analogues = c_proc.findall( "./analogues/analogue" )
      if len( analogues ) > 1:
        print "proc", c_proc.get( "name" ), "has", len( analogues ), "analogues. skipping"
      elif len( analogues ) == 0:
        print "skipping", c_proc.get( "name" )
        continue
            
      f_proc = xml_tree.find( analogues[0].text )
      
      for c_arg in c_proc.findall( "./arguments-list/argument" ):
        analogues = c_arg.findall( "./analogues/analogue" )
        if len( analogues ) > 1:
          print "arg", c_arg.get( "name" ), "has", len( analogues ), "analogues. skipping"
          prettyprintxml( c_proc )
        elif len( analogues ) == 0:
          continue
        
        f_arg = xml_tree.find( analogues[0].text )
        
        semantic = f_arg.get( "semantic" )
        if semantic != None:
          c_arg.set( "semantic", semantic )
          if semantic == "array":
            c_arg.set( "dimensions", f_arg.get( "dimensions" ) )
            
        intent = f_arg.get( "intent" )
        if intent != None:
          c_arg.set( "intent", intent )
        
        dimensions = f_arg.get( "dimensions" )
        if dimensions != None:
          c_arg.set( "dimensions", dimensions )
        
        matrix_size = f_arg.get( "matrix-size" )
        if matrix_size != None:
          c_arg.set( "matrix-size", matrix_size )
        
        associate_array = f_arg.get( "associate-array" )
        if associate_array != None:
          c_arg.set( "associate-array", associate_array )
        
        associate_field = f_arg.get( "associate-field" )
        if associate_field != None:
          c_arg.set( "associate-field", associate_field )
        
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class ImportLAPACKESemanticsIntentsPass ( Pass )

Purpose:
Import semantics and intents for LAPACKE arguments
that will may be unspecified after folding through 
associations.
Will over-write semantics and intents issued by 
FoldLAPACKSemanticsIntentsToLAPACKEPass
'''
class ImportLAPACKESemanticsIntentsPass ( Pass ):
  dependencies = [FoldLAPACKSemanticsIntentsToLAPACKEPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = ImportLAPACKESemanticsIntentsPass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    pass_input = Pass.input_xml.find( "./pass/[@name='ImportLAPACKESemanticsIntentsPass']" )
    for assign in pass_input.findall( "./assign" ):
      for arg in xml_tree.findall( assign.get( "path" ) ):
        semantic = assign.get( "semantic" )
        intent = assign.get( "intent" )
      
        if semantic == None and intent == None:
          raise GeneralPassFailure( "assignment contains no semantic or intent attributes" + assign.get( "path" ) )
      
        if semantic != None:
          arg.set( "semantic", semantic )
      
        if intent != None:
          arg.set( "intent", intent )
    
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class TypeSubstitutionPass ( Pass )

Purpose:
Token replacement pass of type tokens.
find-replace pairs are directly taken from input_xml file,
not inferred, detected, found, what-have-you.
No defining of types, purely text replacement.
applied to argument types and return-types of functions.
if replacement occurs, creates original-type and original-return-type 
attributes that take the original value of the type and return-type attributes

Developer Note:
May move placement of TypeSubstitutionPass
since it is more related to Chapelizing that semantic
transformations like folding.
'''
class TypeSubstitutionPass ( Pass ):
  dependencies = [BaseLAPACKEPass]
  complete = False

  
  @staticmethod
  def apply( xml_tree ):
    selfname = TypeSubstitutionPass
    Pass.resolve( selfname, xml_tree )
    print "[",selfname,"]"

    pass_input = Pass.input_xml.find( "./pass/[@name='TypeSubstitutionPass']" )
    procs = xml_tree.find( "./LAPACKE/procedures" )
    
    subs = {}
    for sub in pass_input.findall( "./substitution" ):
      subs[ sub.get( "find" ) ] = sub.get( "replace" )

    for proc in procs.findall( "./procedure" ):
      proc_type = proc.get( "return-type" )
      
      if proc_type in subs:
        proc.set( "original-return-type", proc_type )
        proc.set( "return-type", subs[ proc_type ] )
        
      for arg in proc.findall( "./arguments-list/argument" ):
        arg_type = arg.get( "type" )

        if arg_type in subs:
          arg.set( "original-type", arg_type )
          arg.set( "type",  subs[ arg_type ] )

    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class ImportTypeArgumentPass ( Pass )

Purpose:
take arguments from the input file and retype
all arguments of the same name within the LAPACKE 
tree to be of the type specified.
'''
class ImportArgumentTypePass ( Pass ):
  dependencies = [TypeSubstitutionPass]
  complete = False

  
  @staticmethod
  def apply( xml_tree ):
    selfname = ImportArgumentTypePass
    Pass.resolve( selfname, xml_tree )
    print "[",selfname,"]"

    pass_input = Pass.input_xml.find( "./pass/[@name='ImportArgumentTypePass']" )
    
    for proc in xml_tree.findall( "./LAPACKE/procedures/procedure" ):
      for arg in proc.findall( "./arguments-list/argument" ):
        find = pass_input.find( "./argument/[@name='" + arg.get("name") + "']" )
        if find == None:
          continue
        arg.set("type", find.get("type" ) )
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"
    
'''
class BaseTransformLAPACKEPass ( Pass )

Purpose:
Ties together any transformation passes on the LAPACKE tree
that are unrelated to Chapelizing

Developer Note:
May move placement of TypeSubstitutionPass
since it is more related to Chapelizing that semantic
transformations like folding.
'''
class BaseTransformLAPACKEPass( Pass ):
  dependencies = [BaseLAPACKEPass, TypeSubstitutionPass, FoldLAPACKSemanticsIntentsToLAPACKEPass, ImportLAPACKESemanticsIntentsPass, TypeSubstitutionPass, ImportArgumentTypePass ]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = BaseTransformLAPACKEPass
    Pass.resolve( selfname, xml_tree )
    print "[",selfname,"]"
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class CreateChapelModuleTreePass ( Pass )

Purpose:
Create chapel-module root, procedures, type-defines, const-defines subtrees
general setup for Chapelization and code generation
'''
class CreateChapelModuleTreePass ( Pass ):
  dependencies = [ CreateTreePass ]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = CreateChapelModuleTreePass
    Pass.resolve( selfname, xml_tree )
    print "[",selfname,"]"
    
    chpl_module = SubElement( xml_tree, "chapel-module" )
    procedures = SubElement( chpl_module, "procedures" )
    types = SubElement( chpl_module, "type-defines" )
    defines = SubElement( chpl_module, "const-defines" )
    enums = SubElement( chpl_module, "enum-defines")
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class ChapelizeLAPACKE_FunctionsPass ( Pass )

Purpose:
take all LAPACKE_* functions defined in <LAPACKE> tree,
bust them apart to provide most information for later passes on
Chapelizing the LAPACKE_functions
'''
class ChapelizeLAPACKEFunctionsPass ( Pass ):
  dependencies = [BaseTransformLAPACKEPass, CreateChapelModuleTreePass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = ChapelizeLAPACKEFunctionsPass
    Pass.resolve( selfname, xml_tree )
    print "[",selfname,"]"
    
    lapacke_root = xml_tree.find( "./LAPACKE" )
    lapacke_procs = lapacke_root.find( "./procedures" )
    
    module_root = xml_tree.find( "./chapel-module" )
    module_procs = module_root.find( "./procedures" )
    
    proc_count = 0
    for proc in lapacke_procs.findall( "./procedure" ):
      proc_name = proc.get( "name" )
      
      module_proc = SubElement( module_procs, "procedure" )
      module_proc.set("name", proc_name)
      module_proc.set("return-type", proc.get( "return-type" ) )
      
      module_proc_args = SubElement( module_proc, "arguments-list" )
           
      for arg in proc.findall( "./arguments-list/argument" ):
        #prettyprintxml( arg )
        module_arg = SubElement( module_proc_args, "argument" )
        module_arg.set( "name", arg.get("name") )
        module_arg.set( "position", arg.get( "position" ) )
        module_arg.set( "type", arg.get( "type" ) )
        arg_semantic = arg.get( "semantic" ) if arg.get( "semantic" ) != None else ""
        arg_intent = arg.get( "intent" ) if arg.get( "intent" ) != None else ""
        arg_refs = int( arg.get( "refdepth" ) )
        
        dimensions = arg.get( "dimensions" )
        if dimensions != None:
          module_arg.set( "dimensions", dimensions )
          
        matrix_size = arg.get( "matrix-size" )
        if matrix_size != None:
          module_arg.set( "matrix-size", matrix_size )
        
        associate_array = arg.get( "associate-array" )
        if associate_array != None:
          module_arg.set( "associate-array", associate_array )
        
        associate_field = arg.get( "associate-field" )
        if associate_field != None:
          module_arg.set( "associate-field", associate_field )
        
        intent = None #"BADSTATE " + arg_semantic + " " + arg_intent + " " + arg_refs
        semantic = None #"BADSTATE " + arg_semantic + " " + arg_intent + " " + arg_refs
        
        if arg_refs == 0:
          if arg_semantic == "array":
            raise GeneralPassFailure( "Attempted array semantic with 0 refdepth " + proc_name + " " +arg.get("name") )
            
          semantic = "scalar"
          intent = "none"
          
        if arg_refs == 1:
          if arg_semantic == "array":
            semantic = "array"
            intent = "none"
          else:
            semantic = "scalar"
            intent = "ref"

        module_arg.set( "intent", intent )
        module_arg.set( "semantic", semantic )
      
      #module_proc.set( "category", "direct" )
      proc_count += 1
    
    print "Chapelized", proc_count, "LAPACKE functions"
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class TranslateChapelKeywordsPass ( Pass ):

Purpose:
taking from the input xml file a list of chapel keywords
changes the text of argument names 

'''
class TranslateChapelKeywordsPass ( Pass ):
  dependencies = [ChapelizeLAPACKEFunctionsPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = TranslateChapelKeywordsPass
    Pass.resolve( selfname, xml_tree )
    print "[",selfname,"]"
    
    pass_input = Pass.input_xml.find( "./pass/[@name='TranslateChapelKeywordsPass']" )
    global_info = pass_input.find( "./global" )
    chpl_module_procs = xml_tree.find( "./chapel-module/procedures" )
    
    global_pre = "" if global_info == None \
                    or global_info.get( "prefix" ) == None \
                    else global_info.get( "prefix" )
                    
    global_suf = "" if global_info == None \
                    or global_info.get( "suffix" ) == None \
                    else global_info.get( "suffix" )
    keywords = {}
    for keyword in pass_input.findall( "./keyword" ):
      symbol = keyword.get( "symbol" )
      replacement = "" if keyword.get( "replacement" ) == None \
                    else keyword.get( "replacement" )
      
      if replacement == "" and global_pre == "" and global_suf == "":
        raise GeneralPassFailure( "If no global prefix or suffix is defined, a replacement for a symbol must be defined. (" + symbol + ")" )
      
      keywords[ symbol ] = replacement
      
    for proc in chpl_module_procs.findall( "./procedure" ):
      
      proc_name = proc.get( "name" )
      #print proc_name
      # Note: This will break includes if we go
      # about replacing their names.
      # arguments are fine because nobody cares about 
      # their names at the late stage of linking
      '''
      if proc_name in keywords:
        if keywords[ proc_name ] == "":
          proc_name = global_pre + proc_name + global_suf
        else:
          proc_name = keywords[ proc_name ]
          
        proc.set( "name", proc_name )
      '''  
      for arg in proc.findall( "./arguments-list/argument" ):
        arg_name = arg.get( "name" )
        #print "\t",arg_name
        if arg_name in keywords:
          if keywords[ arg_name ] == "":
            arg_name = global_pre + arg_name + global_suf
          else:
            arg_name = keywords[ arg_name ]
          #print "\t\t=>",arg_name
          arg.set( "name", arg_name )
        
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class ChapelerrificLAPACKEFunctionsPass ( Pass ):

Purpose:
Create Chapel-errific, LAPACKE Functions that take chapel arrays and abstract the 
dimensions of the arrays and matrices that are stored within.
'''
class ChapelerrificLAPACKEFunctionsPass ( Pass ):
  dependencies = [ChapelizeLAPACKEFunctionsPass, TranslateChapelKeywordsPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = ChapelerrificLAPACKEFunctionsPass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    chapel_module = xml_tree.find( "./chapel-module" );
    chapel_procedures = chapel_module.find( "./procedures" )
    
    for proc in chapel_procedures.findall( "./procedure" ):
      proc_name = proc.get( "name" )
      if proc_name.startswith( "LAPACK_" ) or proc_name.endswith( "_work" ):
        continue
      
      base_name = proc_name.replace( "LAPACKE_", "" )
      
      match = func_name_group_regex.search( base_name );
      
      if match == None:
        print proc_name, "(", base_name, ") does not match regex"
        continue
      
      func = match.group( "function" )
      config = match.group( "config" )
      type = match.group( "type" )
      
      if not config.startswith( "ge" ):
        pass
        
      #proc = copy.deepcopy( chpl_proc )
      args_node = proc.find( "./arguments-list" )
      args_list = [ ]
      args_names = []
      remove_list = set()
      pass_through = {}
      
      for arg in args_node.findall( "./argument" ):
        args_list.append( arg )
        args_names.append( arg.get("name") )
        pass_through[ arg.get("name") ] = arg.get( "name" )
        
      for arg in args_list:
        if arg.get( "semantic" ) != "array" :
          continue
          
        if arg.get( "dimensions" ) != None:
          dimensions = arg.get( "dimensions" ).lower().split(",")
        
          for i in range( len(dimensions) ):
            dimension = dimensions[i]
            if dimension == "*":
              continue
            
            removeVar = None
            for find in args_list:
              if find.get( "name" ) == dimension:
                removeVar = find
                break
                
            if removeVar != None:
              remove_list.add( removeVar.get("name") )
              pass_through[ dimension ] = "(" + arg.get("name") + ".domain.dim("+str(2-i)+").size) : c_int"
              
            else:
              print ( dimension + " is not described in the arguments of "+proc.get( "name" ) + " for argument " + arg.get("name") )
              
        if arg.get( "matrix-size" ) != None: 
          matrix_size = arg.get( "matrix-size" ).lower()
          rows = matrix_size.split(",")[0].strip()
          cols = matrix_size.split(",")[1].strip()
          
          removeRows = None
          removeCols = None
          for find in args_list:
            if find.get( "name" ) == rows:
              removeRows = find
            if find.get( "name" ) == cols:
              removeCols = find
                   
          if removeRows != None and removeCols != None:
            pass_through[ rows ] = "(if matrix_order == lapack_memory_order.row_major then " + arg.get("name") + ".domain.dim(1).size else " + arg.get("name") + ".domain.dim(2).size) : c_int"
            pass_through[ cols ] = "(if matrix_order == lapack_memory_order.row_major then " + arg.get("name") + ".domain.dim(2).size else " + arg.get("name") + ".domain.dim(1).size) : c_int"

            remove_list.add( removeRows.get("name") )
            remove_list.add( removeCols.get("name") )
          
          else:
            print ( rows + " and " + cols + " are not described in the arguments of "+proc.get( "name" ) )
      
      for arg in args_list:
        if arg.get( "semantic" ) != "scalar" :
          continue 
        
        if arg.get( "type" ) == "c_char":
          pass_through[ arg.get("name") ] = "ascii(" + arg.get( "name" ) + ") : c_char"
        
        
        associate_array_str = arg.get( "associate-array" )
        associate_field_str = arg.get( "associate-field" )
        if associate_array_str != None:
          array_field_map = {}
          arrays = associate_array_str.split(",")
          fields = associate_field_str.split(",")
          array = ""
          field = ""
          for i in range( len( arrays ) ) :
            arrays[i] = arrays[i].lower()
            fields[i] = fields[i].lower()
            array_field_map[ arrays[i] ] = fields[i]

          for associate_array in arrays:
            if associate_array in args_names:
              array = associate_array
              field = fields[ arrays.index( array ) ]
              break;
          
          if field == "rows":
            pass_through[ arg.get("name") ] = "(if matrix_order == lapack_memory_order.row_major then " + array + ".domain.dim(1).size else " + array + ".domain.dim(2).size) : c_int"
          elif field == "columns":
            pass_through[ arg.get("name") ] = "(if matrix_order == lapack_memory_order.row_major then " + array + ".domain.dim(2).size else " + array + ".domain.dim(1).size) : c_int"
          elif field == "order" or field == "rank":
            pass_through[ arg.get("name") ] = "(" + array + ".domain.dim(1).size) : c_int"
          else:
            raise GeneralPassFailure( field + " is not a recognized array association field" )
          
          remove_list.add( arg.get("name") )
          
      pass_through_node = SubElement( proc, "pass-through-arguments-list" )
      for arg in args_node.findall( "./argument" ):
        passing = copy.deepcopy( arg )
        passing.text = pass_through[ arg.get( "name" ) ]
        pass_through_node.append( passing )
      
      for arg in args_node:
        if arg.get("name") in remove_list:
          arg.set( "pass-up", "false" )
        else:
          arg.set( "pass-up", "true" )
      
      '''
      for arg in args_node:
        if arg.get( "name" ) == "matrix_order":
          arg.text = "LAPACK_ROW_MAJOR"
      '''    
      #proc.set( "category", "chapelerrific" )
      #proc.set( "call", proc.get("name") )
      #proc.set( "name", proc.get("name").replace( "LAPACKE_", "" ) )
      #chapel_procedures.append( proc )

    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class BaseChapelizePass ( Pass )

Purpose:
Tie together all chapelization passes.
After this point, no more transformations on the 
code should occur.
'''
class BaseChapelizePass ( Pass ):
  dependencies = [ CreateChapelModuleTreePass, ChapelizeLAPACKEFunctionsPass, TranslateChapelKeywordsPass, ChapelerrificLAPACKEFunctionsPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = BaseChapelizePass
    Pass.resolve( selfname, xml_tree )
    print "[",selfname,"]"
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class ImportTypeDefinesPass ( Pass )

Purpose: 
from input xml sets up tags that will be used
to generate typedefs in the module
'''
class ImportTypeDefinesPass ( Pass ):
  dependencies = [CreateChapelModuleTreePass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = ImportTypeDefinesPass
    Pass.resolve( selfname, xml_tree )
    print "[",selfname,"]"
    
    pass_input = Pass.input_xml.find( "./pass/[@name='ImportTypeDefinesPass']" )
    module_types = xml_tree.find( "./chapel-module/type-defines" )
    
    for define in pass_input.findall( "./define" ):
      module_types.append( define )
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class ImportExternConstDefinesPass ( Pass )

Purpose:
from input xml set up tags that will be used
to generate extern const definitions
'''
class ImportExternConstDefinesPass ( Pass ):
  dependencies = [CreateChapelModuleTreePass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = ImportExternConstDefinesPass
    Pass.resolve( selfname, xml_tree )
    print "[",selfname,"]"
    
    pass_input = Pass.input_xml.find( "./pass/[@name='ImportExternConstDefinesPass']" )
    module_defs = xml_tree.find( "./chapel-module/const-defines" )
    
    for define in pass_input.findall( "./define" ):
      module_defs.append( define )
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

''' 
class ImportEnumeratedTypeDefinesPass ( Pass )

Purpose:
from input xml set up tags that will be used
to generate local enum definitions
'''
class ImportEnumeratedTypeDefinesPass ( Pass ):
  dependencies = [CreateChapelModuleTreePass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = ImportEnumeratedTypeDefinesPass
    Pass.resolve( selfname, xml_tree )
    print "[",selfname,"]"
    
    pass_input = Pass.input_xml.find( "./pass/[@name='ImportEnumeratedTypeDefinesPass']" )
    module_defs = xml_tree.find( "./chapel-module/enum-defines" )
    
    for enumeration in pass_input.findall( "./enumeration" ):
      module_defs.append( enumeration )
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class BaseImportPass ( Pass )

Purpose:
Ties together all passes that import from input xml
'''
class BaseImportPass ( Pass ):
  dependencies = [ImportTypeDefinesPass, ImportExternConstDefinesPass, ImportEnumeratedTypeDefinesPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = BaseImportPass
    Pass.resolve( selfname, xml_tree )
    print "[",selfname,"]"
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class BaseCodegenReadyPass ( Pass )

Purpose:
Ties together all passes that must be completed before all codegen could be done
'''
class BaseCodegenReadyPass ( Pass ):
  dependencies = [BaseImportPass, BaseChapelizePass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = BaseCodegenReadyPass
    Pass.resolve( selfname, xml_tree )
    print "[",selfname,"]"
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class ChapelModuleExternProcPass ( Pass )

Purpose:
generate chapel code at each procedure in the <chapel-module>
from the details of each procedure.
these are raw, basic extern procs of these functions.
'''
class ChapelModuleExternProcPass ( Pass ):
  dependencies = [BaseChapelizePass, BaseImportPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = ChapelModuleExternProcPass
    Pass.resolve( selfname, xml_tree )
    print "[",selfname,"]"
    
    lapacke_root = xml_tree.find( "./LAPACKE" )
    lapacke_procs = lapacke_root.find( "./procedures" )
    
    module_root = xml_tree.find( "./chapel-module" )
    module_procs = module_root.find( "./procedures" )
    
    proc_count = 0;
    
    for proc in module_procs.findall( "./procedure" ):
          
      proc_name = proc.get( "name" )
      basename = proc_name.replace( "LAPACK_", "" ).replace( "LAPACKE_", "" ).upper()
      
      lapack_node = xml_tree.find( "./LAPACK/procedures/procedure/[@name='" + basename + "']" )
      
      purpose = "For more information, see the documentation for :proc:`" + proc_name + "`, or consult the Netlibs or Intel documentation.\n"
      ''' #TODO get legal approval for Documentation inclusion.
      if lapack_node == None or lapack_node.find( "./purpose" ) == None or lapack_node.find( "./purpose" ).text == None:
        purpose = ""
      else:
        purpose = re.sub( r"[ \t]+", " ", lapack_node.find( "./purpose" ).text )
      '''  
      proc_args = proc.findall( "./arguments-list/argument" )
      ordered_args = [None] * len( proc_args )
      
      for arg in proc_args:
        ordered_args[ int( arg.get( "position" ) ) ] = arg;

      def_code = SegmentProducer( "extern proc " + proc_name )
      args_code = ListProducer( ", ", "(", ")" )
      
      for arg in ordered_args:
        args_code.append( SegmentProducer(
                                      ("" if arg.get("intent") == "none" else arg.get("intent") + " ") + \
                                      arg.get("name") + " : " + \
                                      ("[] " if arg.get("semantic") == "array" else "") + \
                                      arg.get("type")
                                    )
                   )
      
      return_code = LineProducer( " : " + proc.get( "return-type" ) + ";" )
      
      
      #doc_comment = CommentProducer( "\nExternal Procedure to " + proc_name + "\n" + ("\nOriginal Fortran LAPACK documentation for " + basename + "::\n\n " + purpose + "\n\n" if purpose != "" else "") )
      doc_comment = CommentProducer( "\nExternal Procedure to " + proc_name + "\n" + purpose + "\n" )
      
      
      code = SequenceOfProducers()
      code.append( doc_comment )
      code.append( def_code )
      code.append( args_code )
      code.append( return_code )
      
      code_node = SubElement( proc, "code" )
      code_node.set( "category", "extern proc" )
      code_node.text = code.generate()
      
      proc_count += 1

    print "Generated code for", proc_count, "functions"
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class ChapelModuleStringToCharWraperProcPass ( Pass )
Purpose:
Create string wrappers to all of the generate external procs from
ChapelModuleExternProcPass
'''
class ChapelModuleStringToCharWraperProcPass ( Pass ):
  dependencies = [ChapelModuleExternProcPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = ChapelModuleStringToCharWraperProcPass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"
    
    module_root = xml_tree.find( "./chapel-module" )
    module_procs = module_root.find( "./procedures" )
    
    proc_count = 0
    for proc in module_procs.findall( "./procedure" ):

      proc_name = proc.get( "name" )
      
      if proc_name.startswith( "LAPACK_" ):
        continue
      
      proc_args = proc.findall( "./arguments-list/argument" )
      ordered_args = [None] * len( proc_args )
      
      char_flag = False
      for arg in proc_args:
        ordered_args[ int( arg.get( "position" ) ) ] = arg;
        char_flag = arg.get( "type" ) == "c_char" or char_flag
      
      # skip procedures that dont have char arguments
      if not char_flag:
        continue

      code = SequenceOfProducers()
      code.append( SegmentProducer( "inline proc " + proc_name ) )
      
      args_code = ListProducer( ", ", "(", ")" )
      for arg in ordered_args:
        args_code.append( SegmentProducer(
                                      ("" if arg.get("intent") == "none" else arg.get("intent") + " ") + \
                                      arg.get("name") + " : " + \
                                      ("[] " if arg.get("semantic") == "array" else "") + \
                                      ( arg.get("type") if arg.get("type") != "c_char" else "string" )
                                    )
                   )
      
      code.append( args_code )
      code.append( SegmentProducer( " : " + proc.get( "return-type" ) ) )
      
      func_body = ScopeProducer()

      call_args_producer = ListProducer( ", ", "(", ")" )
      for pass_arg in ordered_args:
        call_args_producer.append( SegmentProducer( ( pass_arg.get("name" ) if pass_arg.get("type") != "c_char" else "ascii(" + pass_arg.get( "name" ) + ") : c_char" ) ) )
      func_body.append( SegmentProducer( ( "return " if proc.get("return-type") != "void" else "" ) + proc.get("name") ) + call_args_producer + LineProducer( ";" ) )
      code.append( func_body )
      
      code.prepend( CommentProducer( "\nString wrapped procedure of " + proc_name + "\n" ) )
      
      code_node = SubElement( proc, "code" )
      code_node.set( "category", "string wrapped" )
      code_node.text = code.generate()
      
      proc_count += 1
    
    print "Generated code for", proc_count, "functions"
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class ChapelModuleChapelerrificProcPass ( Pass )

Purpose:
Generate code for Chapel-errific upward facing procedures
'''
class ChapelModuleChapelerrificProcPass ( Pass ):
  dependencies = [ChapelModuleExternProcPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = ChapelModuleChapelerrificProcPass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    module_root = xml_tree.find( "./chapel-module" )
    module_procs = module_root.find( "./procedures" )
    proc_count = 0
    no_repeat = set()
    for proc in module_procs.findall( "./procedure" ):
      if proc.find( "./pass-through-arguments-list" ) == None:
        continue
      
      base_name = proc.get("name").replace( "LAPACKE_", "" )
      
      match = func_name_group_regex.search( base_name );
      if match == None:
        print proc.get("name"), "(", base_name, ") does not match regex"
        continue
      
      func = match.group( "function" )
      config = match.group( "config" )
      type = match.group( "type" )
      
      typeToTypeString = { "s" : "c_float", 
                           "d" : "c_double",
                           "c" : "lapack_complex_float",
                           "z" : "lapack_complex_double"
                          }
      
      for name_category in [ (config+func, "untyped chapelerrific") ]: # (type+config+func, "chapelerrific")
      
        [proc_name, category_name] = name_category
        
        
        code = SequenceOfProducers()
        purpose = ""     
        lapack_node = xml_tree.find( "./LAPACK/procedures/procedure/[@name='" + base_name.upper() + "']" )
        purpose = "For more information, see the documentation for :proc:`" + proc_name + "`, or consult the Netlibs or Intel documentation.\n"
        
        ''' #TODO get legal approval for Documentation inclusion.
        if proc_name in no_repeat:
          purpose = "For more information, see the documentation for :proc:`" + proc_name + "`, or consult the Netlibs or Intel documentation.\n"
        elif lapack_node == None or lapack_node.find( "./purpose" ) == None or lapack_node.find( "./purpose" ).text == None:
          prupose = ""
        else:
          purpose = ("Original Fortran LAPACK purpose documentation for " + base_name.upper() + "::\n\n " + re.sub( r"[ \t]+", " ", lapack_node.find( "./purpose" ).text ) + "\n\n" )
        '''  
          
        proc_args = proc.findall( "./arguments-list/argument" )
        ordered_args = [None] * len( proc_args )
    
        for arg in proc_args:
          ordered_args[ int( arg.get( "position" ) ) ] = arg;
    
        code.append( SegmentProducer( "inline proc " + proc_name ) )
        
        args_doc = str()
        
        args_producer = ListProducer(", ", "(", ")")
        for arg in ordered_args: 
          if arg.get("pass-up") == "true":
            args_producer.append( SegmentProducer(
                                    ("" if arg.get("intent") == "none" else arg.get("intent") + " ") + \
                                    arg.get("name") + " : " + \
                                    ("[] " if arg.get("semantic") == "array" else "") + \
                                    ( arg.get("type") if arg.get("type") != "c_char" else "string" ) + \
                                    ( " = " + arg.text if arg.text != None and arg.text.strip() != "" else "" )
                                  )
                                )
                                
                                
            if lapack_node == None or arg.get("name") == "matrix_order":
              continue
            
            #print "./arguments-list/argument/[@name='" + arg.get("name").upper() + "']"
            lapack_arg_node = lapack_node.find( "./arguments-list/argument/[@name='" + arg.get("name").upper() + "']" )
            
            if lapack_arg_node == None:
              continue
              
            #prettyprintxml( lapack_arg_node )
            ''' #TODO get legal approval for Documentation inclusion.
            if (not proc_name in no_repeat) and lapack_arg_node.find( "./documentation" ) != None:
              #arg_doc = " " + arg.get(arg.get("name").upper() + " : " + arg.get("type") + ( "" if arg.get("intent") == "none" else arg.get("intent").strip() ) + "\n"
              text = re.sub( r"\n", "\n ", re.sub( r"[ \t]+", " ", lapack_node.find( "./arguments-list/argument/[@name='" + arg.get("name").upper() + "']/documentation" ).text ) )
              arg_doc =  " " + text + "\n\n"
              
              if args_doc == "":
                args_doc = "Original Fortran LAPACK argument documentation for " + base_name.upper() + "::\n\n"
              
              args_doc += arg_doc
            '''
        #args_doc += "\n\n"      
        code.prepend( CommentProducer( "\n" + ("Polymorphic " if category_name == "untyped chapelerrific" else "" ) + "Chapel idiomatic procedure of " + proc.get("name") + " for the type " + typeToTypeString[type] + ".\n\n" + purpose + args_doc ) )
        
        code.append( args_producer )
        code.append( SegmentProducer( ": " + proc.get( "return-type" ) ) )
        
        func_body = ScopeProducer()
    
        call_args_producer = ListProducer( ", ", "(", ")" )
        for pass_arg in proc.findall( "./pass-through-arguments-list/argument" ):
          call_args_producer.append( SegmentProducer( pass_arg.text ) )
    
        func_body.append( SegmentProducer( ( "return " if proc.get("return-type") != "void" else "" ) + proc.get("name") ) + call_args_producer + LineProducer( ";" ) )
        code.append( func_body )
        
        
        code_node = SubElement( proc, "code" )
        code_node.set( "category", category_name )
        code_node.text = code.generate()
        no_repeat.add( proc_name )
        proc_count += 1  

    print "Generated code for", proc_count, "functions"
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class ChapelModuleExternTypeDefinesPass ( Pass )

Purpose:
from the imported external type defines generate
external type code at each define tag
'''
class ChapelModuleExternTypeDefinesPass ( Pass ):
  dependencies = [BaseChapelizePass, BaseImportPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = ChapelModuleExternTypeDefinesPass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"
    
    module_defs = xml_tree.find( "./chapel-module/type-defines" )
    
    for define in module_defs.findall( "./define" ):
      def_str = ("/*"+ define.find("./description").text + "*/\n" if define.find("./description") != None else "")
      
      if define.get( "external" ) != None and define.get("external").lower() == "yes":
        def_str += "extern "
      
      def_str += "type " + define.get( "alias" ) + " "
      
      if define.get( "base-type" ) != None:
        def_str += "= " + define.get( "base-type" )
      
      def_str += ";"
      
      SubElement( define, "code" ).text = def_str
      
    #prettyprintxml( module_defs )
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class ChapelModuleExternConstDefinesPass ( Pass ):

Purpose:
from the imported external const defines generate
eternal const code at each define tag
'''
class ChapelModuleExternConstDefinesPass ( Pass ):
  dependencies = [BaseChapelizePass, BaseImportPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = ChapelModuleExternConstDefinesPass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    module_defs = xml_tree.find( "./chapel-module/const-defines" )
    
    for define in module_defs.findall( "./define" ):
      def_str = ("/*"+ define.find("./description").text + "*/\n" if define.find("./description") != None else "")
      
      if define.get( "external" ) != None and define.get( "external" ).lower() == "yes":
        def_str += "extern "
        
      
      def_str += "const " + define.get( "symbol" ) + " : " + define.get( "type" ) + " "
      
      if define.get( "value" ) != None:
        def_str += " = " + define.get( "value" )
        
      def_str += ";"
      SubElement( define, "code" ).text = def_str 
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class ChapelModuleEnumDefinesPass ( Pass ):

Purpose:
from the imported enumeration defines generate
enum code at each enumeration tag
'''
class ChapelModuleEnumDefinesPass ( Pass ):
  dependencies = [BaseChapelizePass, BaseImportPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = ChapelModuleEnumDefinesPass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    module_defs = xml_tree.find( "./chapel-module/enum-defines" )
    
    for define in module_defs.findall( "./enumeration" ):

      values = ListProducer(", ", "{", "}")
      for value in define.findall( "./value" ):
        values.append( SegmentProducer( value.get("name") + ( " = " + value.text if value.text != None and value.text.strip() != "" else "" ) ) )
      
      description_node = define.find("./description")
      if description_node != None:
        SubElement( define, "code" ).text = CommentProducer( description_node.text ).generate() + ( SegmentProducer( "enum " + define.get("name") ) + values + LineProducer(";") ).generate() 
      else:
        SubElement( define, "code" ).text = ( SegmentProducer( "enum " + define.get("name") ) + values + LineProducer(";") ).generate() 
        
      
    #prettyprintxml( module_defs )
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

''' 
class BaseCodeGenerationPass ( Pass )

Purpose:
Ties together all code generation passes before
any text dumping into a file.
'''
class BaseCodeGenerationPass ( Pass ):
  dependencies = [ChapelModuleExternProcPass, ChapelModuleStringToCharWraperProcPass, ChapelModuleChapelerrificProcPass, ChapelModuleExternTypeDefinesPass, ChapelModuleExternConstDefinesPass, ChapelModuleEnumDefinesPass ]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = BaseCodeGenerationPass
    Pass.resolve( selfname, xml_tree )
    print "[",selfname,"]"
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class DumpCodePass ( Pass )

Purpose:
traverses <chapel-module> tree, collecting generated code text
and gently places it into the file defined in the input xml 
pass information
'''
class DumpCodePass ( Pass ):
  dependencies = [BaseCodeGenerationPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = DumpCodePass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    pass_input = Pass.input_xml.find( "./pass/[@name='DumpCodePass']" )
    module_root = xml_tree.find( "./chapel-module" )

    ChaLAPACK_info = pass_input.find( "./main-module" )
    module_name = ChaLAPACK_info.get( "name" )
    module_file = open( ChaLAPACK_info.get( "file-name" ), "w" )
    
    
    module_file.write( "/*\n" + ChaLAPACK_info.find("./description").text + "\n*/\n" )
    module_file.write( "module " + module_name + " {\n" )
   
    for use in ChaLAPACK_info.findall( "./use" ):
      module_file.write( "use " + use.text + ";\n" )
    module_file.write( "\n" )
    
    module_file.write( "use LAPACK;\n" )

    for proc in module_root.findall( "./procedures/procedure" ):
      code = proc.find( "./code/[@category='untyped chapelerrific']" )
      if code != None:
        module_file.write( code.text + "\n" )
    
    module_file.write("} // ChaLAPACK\n")
    module_file.close();
    
    
    
    helper_info = pass_input.find( "./helper-module" )
    
    module_file = open( helper_info.get("file-name"), "w" )
    
    if helper_info.get("no-doc") == "all":
      module_file.write( "pragma \"no doc\"\n" )
    
    module_file.write( "/*\n" + helper_info.find( "./description" ).text + "\n*/\n" )
    module_file.write( "module LAPACK {\n" )
    
    for use in helper_info.findall( "./use" ):
      module_file.write( "use " + use.text + ";\n" )
    module_file.write( "\n" )
    
    nodoc_helper_types = helper_info.get("no-doc") == "internals" or helper_info.get("no-doc") == "types"
    for defn in module_root.findall( "./type-defines/define" ):
      module_file.write( defn.find("./code").text + "\n" )
    module_file.write( "\n\n" )
    
    for defn in module_root.findall( "./const-defines/define" ):
      module_file.write( defn.find("./code").text + "\n" )
    module_file.write( "\n\n" )
    
    for defn in module_root.findall( "./enum-defines/enumeration" ):
      module_file.write( defn.find("./code").text + "\n" )
    module_file.write( "\n" )
    
    nodoc_helper_procs = helper_info.get("no-doc") == "internals" or helper_info.get("no-doc") == "procedures"
    
    for proc in module_root.findall( "./procedures/procedure" ):
      code = proc.find( "./code/[@category='extern proc']")
      if code != None:
        if nodoc_helper_procs:
          module_file.write( "pragma\"no doc\"\n" )
        module_file.write( code.text + "\n" )
      
      code = proc.find( "./code/[@category='string wrapped']")
      if code != None:
        if nodoc_helper_procs:
          module_file.write( "pragma\"no doc\"\n" )
        module_file.write( code.text + "\n" )
    
    module_file.write( "} // LAPACK \n" )
    
    module_file.close()
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"




'''
Below are passes that were used to explore the LAPACK source, or are Passes that were removed from the main set.

EXTREME caution is advised if using them. They may (probably) not work with the current set of main passes
'''


class CountFunctions( Pass ):
  dependencies = [BaseLAPACKPass, BaseLAPACKEPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = CountFunctions
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"
    
    lapack = 0
    for proc in xml_tree.findall( "./LAPACK/procedures/procedure" ):
      lapack += 1
    
    lapacke = 0
    for proc in xml_tree.findall( "./LAPACKE/procedures/procedure" ):
      lapacke += 1
    
    print "LAPACK", lapack, "LAPACKE", lapacke
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class CreateAbstractLAPACKTreePass ( Pass )

Purpose:
Create Abstract-LAPACK tree
'''
class CreateAbstractLAPACKTreePass ( Pass ):
  dependencies = [BaseChapelizePass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = CreateAbstractLAPACKTreePass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"
    
    abstract_lapack = SubElement( xml_tree, "Abstract-LAPACK" )
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"
    
'''
class BucketLAPACKFunctionGroups ( Pass )

Purpose:
bucket LAPACK functions by their base function, type, and matrix type
'''
class BucketLAPACKFunctionGroupsPass ( Pass ):
  dependencies = [CreateAbstractLAPACKTreePass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = BucketLAPACKFunctionGroupsPass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"
    
    abstract_lapack = xml_tree.find( "./Abstract-LAPACK" )
    
    module_root = xml_tree.find( "./chapel-module" )
    module_procs = module_root.find( "./procedures" )
    
    funcs = set()
    
    for proc in module_procs.findall( "./procedure" ):
      proc_name = proc.get( "name" )
      #print proc_name
      if proc_name in funcs:
        raise GeneralPassFailure( "DOUBLE HIT " + proc_name )
      else:
        funcs.add( proc_name )
      
      # we only care about LAPACKE_ functions
      if proc_name.startswith( "LAPACK_" ) or proc_name.endswith( "_work" ):
        continue
      
      base_name = proc_name.replace( "LAPACKE_", "" ) #.replace( "_work", "" )
      
      match = func_name_group_regex.search( base_name );
      
      if match == None:
        print proc_name, "(", base_name, ") does not match regex"
        continue
      
      func = match.group( "function" )
      config = match.group( "config" )
      type = match.group( "type" )
      
      if config != "ge":
        continue
      
      group_node = abstract_lapack.find( "./group/[@name='" + func + "']" )
      if group_node == None:
        group_node = SubElement( abstract_lapack, "group" )
        group_node.set("name", func )
      
      config_node = group_node.find( "./matrix-configuration/[@name='" + config + "']" )
      if config_node == None:
        config_node = SubElement( group_node, "matrix-configuration" )
        config_node.set( "name", config )
      
      if config_node.find( "./types/type/[@name='" + type + "']" ) != None:
        print "Double declaration of abstract LAPACK function", type, config, func, base_name, proc_name
        continue
        #prettyprintxml( config_node.find( "./type/[@name='" + type + "']" ) )
      types_node = SubElementUnique( config_node, "types" )
      type_node = SubElement( types_node, "type" )
      type_node.set( "name", type )
      type_node.set( "analogue", "./chapel-module/procedures/procedure/[@name='" + proc_name + "']" )
      
    #prettyprintxml( abstract_lapack )      
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"


class ImportAbstractLAPACKFunctionsPass ( Pass ):
  dependencies = [BaseCodegenReadyPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = ImportAbstractLAPACKFunctionsPass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    module_defs = xml_tree.find( "./chapel-module/procedures" )
    group_input = Pass.input_xml.find( "./pass/[@name='ImportAbstractLAPACKFunctionsPass']" )
    
    proc_count = 0
    
    for group in group_input.findall( "./group" ):
      for config in group.findall( "./matrix-configuration" ):
        code = SequenceOfProducers()
        proc_name = config.get( "name" ) + group.get( "name" )
        code.append( SegmentProducer( "proc " + proc_name ) )

        args_producer = ListProducer(", ", "(", ")")
        for arg in config.findall( "./method-arguments/argument" ):
          args_producer.append( SegmentProducer(
                                  arg.get("intent") + " " + \
                                  arg.get("name") + " : " + \
                                  ("[] " if arg.get("semantic") == "array" else "") + \
                                  arg.get("type") + \
                                  ( " = " + arg.text if arg.text != None and arg.text.strip() != "" else "" )
                                )
                              )

        code.append( args_producer )
        code.append( LineProducer( ": lapack_int" ) )
        code.append( SegmentProducer( "where " ) )
        
        where_producer = ListProducer( " || ", "", "" )
        
        for type in config.findall("./types/type"):
          where_producer.append( SegmentProducer( "T == " + type.get( "type" ) ) )
        
        code.append( where_producer )
        
        info_var = config.get( "name" ) + group.get( "name" ) + "_return_info"
        
        func_body = ScopeProducer()
        func_body.append( LineProducer( "var " + info_var + " : lapack_int;" ) )

        #if_bodies = SequenceOfProducers()
        
        arg_relates = {}
        ana_args = []
        for arg in config.findall( "./analogue-arguments-list/argument" ):
          arg_name = arg.get("name")
          arg_relates[ arg_name ] = config.find( "./arguments-relationships/argument/[@name='" + arg_name + "']" )
          ana_args.append( arg );
        
      
        for type in config.findall("./types/type"):
          chpl_ana = xml_tree.find( type.get( "analogue" ) )
          if_condition = LineProducer( "if ( T == " + type.get("type") + " )" )
          func_body.append( if_condition )
          
          if_body = ScopeProducer()
          
          call_equals = SegmentProducer( info_var + " = " + chpl_ana.get( "name" ) )
          
          call_seq = ListProducer( ", ", "(", ")" )
          
          for ana_arg in ana_args:
            call_seq.append( SegmentProducer( 
                              "(" + arg_relates[ana_arg.get("name")].text.strip() + ")" + \
                              (" : " + ana_arg.get("type") if ana_arg.get("semantic") != "array" else "")
                             )
                           )
            
          if_body.append( call_equals + call_seq + LineProducer( ";" ) )
          func_body.append( if_body )
        
        func_body.append( LineProducer( "return " + info_var + ";" ) )
        
        code.append( func_body )
        
        module_proc = SubElement( module_defs, "procedure" )
        module_proc.set( "name", proc_name )
        code_node = SubElement( module_proc, "code" )
        code_node.set( "category", "upward facing" )
        code_node.text = code.generate()
        
        proc_count += 1
        
    print "Generated", proc_count, "procedures"
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class GroupsArgumentCollectionPass ( Pass )

Purpose:
collect common arguments into the function groups
'''
class CommonArgumentCollectionPass ( Pass ):
  dependencies = [BucketLAPACKFunctionGroupsPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = CommonArgumentCollectionPass
    Pass.resolve( selfname, xml_tree )
    print "[",selfname,"]"
    
    abstract_lapack = xml_tree.find( "./Abstract-LAPACK" )

    for group_node in abstract_lapack.findall( "./group" ):
      for config_node in group_node.findall( "./matrix-configuration" ):
        args_tree = ET.Element( "analogue-arguments-list" )
        ''''
        prefix_type_map = { 
                            "s" : "float",
                            "d" : "double", 
                            "c" : "complex_float",
                            "z" : "complex_double",
                            "float" : "s",
                            "double" : "d", 
                            "complex_float" : "c",
                            "complex_double" : "z"
                          }
        '''
        all_args = {} # dictionary to set [arg_name]=>set( function_names )
        all_funcs = set() # set of all functions encountered
        
        func_args_type = {} # {func_name} => {arg_name} => type_name
        for type_func in config_node.findall( "./types/type" ):
          type_name = type_func.get( "name" );
          all_funcs.add( type_name );
          func_args_type[ type_name ]  = {};
          
          chapel_func = xml_tree.find( type_func.get( "analogue" ) )
          
          if chapel_func == None:
            raise GeneralPassFailure( type_name + config_node.get( "name" ) + group_node.get( "name" ) + " does not have chapel analogue" )
          
          for arg in chapel_func.findall( "./arguments-list/argument" ):
            func_args_type[ type_name ][ arg.get("name") ] = arg.get("type") 
            
            args_type
            
            
            find = args_tree.find( "./argument/[@name='" + arg.get( "name" ) + "']" )
            if find == None:
              args_tree.append( arg )
            elif arg.get( "type" ) != find.get( "type" ):
              find.set( "type", "?T" )
              
            abstract_arg = ET.Element( "argument" )
            
            arg_name = arg.get( "name" )
            
            if not arg_name in all_args:
              all_args[arg_name] = set()
          
            all_args[arg_name].add(type_name)
        
        for arg_name in all_args:
          if all_args[ arg_name ] != all_funcs:
            arg = args_tree.find( "./argument/[@name='" + arg_name + "']" )
            args_tree.remove( arg )
            for type_func_name in all_args[ arg_name ]:
              #print "find", type_func_name
              #prettyprintxml( config_node )
              type_func = config_node.find( "./types/type/[@name='" + type_func_name + "']" )
              
              args_list = SubElementUnique( type_func, "arguments-list" )
              args_list.append( arg )
        
        config_node.append( args_tree )

        
    #prettyprintxml( abstract_lapack.find( "./group/[@name='sv']" ) )
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

''' 
class BaseLAPACKAbstractPass ( Pass )

Purpose:
Ties together all passes that populate the Abstract-LAPACK classes 
for upward facing LAPACK chapel procedures
'''
class BaseAbstractLAPACKPass ( Pass ):
  dependencies = [BucketLAPACKFunctionGroupsPass, CommonArgumentCollectionPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = BaseAbstractLAPACKPass
    Pass.resolve( selfname, xml_tree )
    print "[",selfname,"]"
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"


class DropAttemptedAssociations ( Pass ):
  dependencies = [BaseChapelizePass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = DropAttemptedAssociations
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"
    
    output_xml = ET.Element( "pass" );
    output_xml.set( "name", "DropAttemptedAssociations" )
    output_procs = SubElement( output_xml, "procedures" );
    
    for chpl_proc in xml_tree.findall( "./chapel-module/procedures/procedure" ):
      proc_name = chpl_proc.get( "name" )
      if proc_name.startswith( "LAPACK_" ) or proc_name.endswith( "_work" ):
        continue
      
      base_name = proc_name.replace( "LAPACKE_", "" )
      
      match = func_name_group_regex.search( base_name );
      
      if match == None:
        print proc_name, "(", base_name, ") does not match regex"
        continue
      
      func = match.group( "function" )
      config = match.group( "config" )
      type = match.group( "type" )
      
      if not config.startswith( "ge" ):
        continue
        
      proc = copy.deepcopy( chpl_proc )
      print proc.get( "name" )
      #prettyprintxml( proc )
      args_node = proc.find( "./arguments-list" )
      args_list = [ ]
      args_names = []
      remove_list = set()
      pass_through = {}
      
      for arg in args_node.findall( "./argument" ):
        args_list.append( arg )
        args_names.append( arg.get("name") )
        pass_through[ arg.get("name") ] = arg.get( "name" )
        
      for arg in args_list:
        if arg.get( "semantic" ) != "array" :
          continue
          
        if arg.get( "dimensions" ) != None:
          dimensions = arg.get( "dimensions" ).lower().split(",")
        
          for i in range( len(dimensions) ):
            dimension = dimensions[i]
            if dimension == "*":
              continue
            
            removeVar = None
            for find in args_list:
              if find.get( "name" ) == dimension:
                removeVar = find
                break
                
            if removeVar != None:
              remove_list.add( removeVar )
              pass_through[ dimension ] = arg.get("name") + ".domain.dim("+str(i+1)+").size"
              
            else:
              print ( dimension + " is not described in the arguments of "+proc.get( "name" ) )
              
        if arg.get( "matrix-size" ) != None: 
          matrix_size = arg.get( "matrix-size" ).lower()
          rows = matrix_size.split(",")[0].strip()
          cols = matrix_size.split(",")[1].strip()
          
          removeRows = None
          removeCols = None
          for find in args_list:
            if find.get( "name" ) == rows:
              removeRows = find
            if find.get( "name" ) == cols:
              removeCols = find
                   
          if removeRows != None and removeCols != None:
            pass_through[ rows ] = "if matrix_order == LAPACK_ROW_MAJOR then " + arg.get("name") + ".domain.dim(1).size else " + arg.get("name") + ".domain.dim(2).size "
            pass_through[ cols ] = "if matrix_order == LAPACK_ROW_MAJOR then " + arg.get("name") + ".domain.dim(2).size else " + arg.get("name") + ".domain.dim(1).size "

            remove_list.add( removeRows )
            remove_list.add( removeCols )
          
          else:
            print ( rows + " and " + cols + " are not described in the arguments of "+proc.get( "name" ) )
      
      for arg in args_list:
        if arg.get( "semantic" ) != "scalar" :
          continue 
        
        if arg.get( "type" ) == "c_char":
          arg.set( "type", "string" )
          pass_through[ arg.get("name") ] = "ascii(" + arg.get( "name" ) + ") : c_char"
        
        
        associate_array_str = arg.get( "associate-array" )
        associate_field_str = arg.get( "associate-field" )
        if associate_array_str != None:
          array_field_map = {}
          arrays = associate_array_str.split(",")
          fields = associate_field_str.split(",")
          array = ""
          field = ""
          for i in range( len( arrays ) ) :
            arrays[i] = arrays[i].lower()
            fields[i] = fields[i].lower()
            array_field_map[ arrays[i] ] = fields[i]

          for associate_array in arrays:
            if associate_array in args_names:
              array = associate_array
              field = fields[ arrays.index( array ) ]
              break;
          
          if field == "rows":
            pass_through[ arg.get("name") ] = "if matrix_order == LAPACK_ROW_MAJOR then " + array + ".domain.dim(1).size else " + array + ".domain.dim(2).size "
          elif field == "columns":
            pass_through[ arg.get("name") ] = "if matrix_order == LAPACK_ROW_MAJOR then " + array + ".domain.dim(2).size else " + array + ".domain.dim(1).size "
          elif field == "order" or field == "rank":
            pass_through[ arg.get("name") ] = array + ".domain.dim(1).size"
          else:
            raise GeneralPassFailure( field + " is not a recognized array association field" )
          
          remove_list.add( arg )
          
      pass_through_node = SubElement( proc, "pass-through" )
      for arg in args_node.findall( "./argument" ):
        passing = copy.deepcopy( arg )
        passing.text = pass_through[ arg.get( "name" ) ]
        pass_through_node.append( passing )
      
      for rm_arg in remove_list:
        args_node.remove( args_node.find( "./argument/[@name='" + rm_arg.get( "name" ) + "']" ) )
        
      count = 0
      for arg in args_node:
        arg.set("position", str( count ) )
        count += 1
        if arg.get( "name" ) == "matrix_order":
          arg.text = "LAPACK_ROW_MAJOR"
        
        
      #print pass_through
      #prettyprintxml( proc )
      #print pass_through, "\n", "==="*20, "\n"
      
      output_procs.append( proc )

    prettywritexml( output_xml, "DropAttemptedAssociations_output.xml" )
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"


class AbstractDropAttemptedAssociations ( Pass ):
  dependencies = [BaseAbstractLAPACKPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = DropAttemptedAssociations
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    abstract_lapack = xml_tree.find( "./Abstract-LAPACK" )
    
    for group_node in abstract_lapack.findall( "./group" ):
      for config_node in group_node.findall( "./matrix-configuration" ):
        if config_node.findall( "./types/type/arguments-list" ) != [] :
          print config_node.get("name") + group_node.get("name"), " has typed functions with non common arguments. Skipping."
          continue
        
        full_func_name = config_node.get("name") + group_node.get("name")
        all_args = []
        array_args = set()
        method_args = []
        pass_through = {} # str => str
        removed = {} # str => bool
        for arg in config_node.findall( "./analogue-arguments-list/argument" ):
          all_args.append( arg.get("name") )
          if arg.get( "semantic" ) == "array" :
            array_args.add( arg.get("name" ) )
          removed[ arg.get("name") ] = False
          method_args.append( arg ) 
          
        for arg in config_node.findall( "./analogue-arguments-list/argument" ):
          if removed[ arg.get("name") ] or arg.get( "semantic" ) != "array":
            continue
          
          pass_through[ arg.get("name") ] = arg.get( "name" )
          
          if arg.get( "dimensions" ) != None:
            dimensions = arg.get( "dimensions" ).split(",")
          
            for i in range( len(dimensions) ):
              dimension = dimensions[i].lower()
              if dimension == "*":
                continue
              
              pass_through[ dimension ] = arg.get("name") + ".domain.dim("+str(i+1)+").size"
              removed[ dimension ] = True
          
          matrix_size = arg.get( "matrix-size" )
          if matrix_size != None: 
            rows = matrix_size.split(",")[0].strip().lower()
            cols = matrix_size.split(",")[1].strip().lower()
            
            pass_through[ rows ] = "if matrix_order == LAPACK_ROW_MAJOR then " + arg.get("name") + ".domain.dim(1).size else " + arg.get("name") + ".domain.dim(2).size "
            pass_through[ cols ] = "if matrix_order == LAPACK_ROW_MAJOR then " + arg.get("name") + ".domain.dim(2).size else " + arg.get("name") + ".domain.dim(1).size "
            
            removed[ rows ] = True
            removed[ cols ] = True
    
        for arg in config_node.findall( "./analogue-arguments-list/argument" ):
          if removed[ arg.get("name") ] or arg.get( "semantic" ) != "scalar": 
            continue
          
          pass_through[ arg.get("name") ] = arg.get("name")
          
        for rm in removed:
          if not removed[rm]:
            continue
          for i in range( len( method_args ) ):
            if method_args[i].get("name") == rm:
              method_args.remove( method_args[i] )
              break;
          
        interface_node = SubElement( config_node, "method-arguments" )   
        for arg in method_args :
          argument = SubElement( interface_node, "argument" )
          argument.set( "name", arg.get("name") )
          argument.set( "intent" , arg.get("intent") )
          argument.set( "semantic", arg.get("semantic") )
          argument.set( "type", arg.get("type") )
          argument.text = " " if arg.get("name") != "matrix_order" else "LAPACK_ROW_MAJOR"
          
        pass_through_node = SubElement( config_node, "arguments-relationships" )
        for arg in  config_node.findall( "./analogue-arguments-list/argument" ):
          arg_name = arg.get( "name" )
          arg_relate = SubElement( pass_through_node, "argument" )
          arg_relate.set( "name", arg_name )
          arg_relate.text = pass_through[arg_name]
    
    prettywritexml( abstract_lapack, "AbstractDropAttemptedAssociations_output.xml" )
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class FindNeedsArgsDocPatchPass ( Pass )

Purpose:
Was used to find fortran files with incorrect argument documentation
( \param[intent] blah blah )
'''
class FindNeedsArgsDocPatchPass ( Pass ):
  dependencies = [BaseLAPACKPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = FindNeedsArgsDocPatchPass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    lapack_root_procs = xml_tree.find( "./LAPACK/procedures" )
    patch = []
    for proc in lapack_root_procs.findall( "./procedure" ):
      printed = False
      
      #prettyprintxml( proc )
      for arg in proc.findall( "./arguments-list/argument" ):
        if arg.find( "./documentation" ) == None:
          if not printed:
            print proc.get( "name" ), proc.get( "file-name" )
            printed = True
          print arg.get( "name" ), "MISSING"
          patch.append( (proc.get("name"), proc.get("file-name"), arg.get("name") ) )

    print patch

    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class FindNeedsFuncArgsTypePatchPass ( Pass )

Purpose:
Was used to find fortran files with incorrect argument type documentation
( ie ..Scalar Arguments.. blah blah )
'''
class FindNeedsFuncArgsTypePatchPass ( Pass ):
  dependencies = [LAPACKFunctionDefinePass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = FindNeedsFuncArgsTypePatchPass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    lapack_node = xml_tree.find( "./LAPACK" )
    text_node = lapack_node.find( "./text" )
    procs_node = lapack_node.find( "./procedures" )
    
    none_scalar = []
    none_array = []
    for proc_node in procs_node.findall( "./procedure" ):
      proc_file_name = proc_node.get( "file-name" )
      doc_node = text_node.find( "./file/[@name='" + proc_file_name + "']/documentation" )
      
      scalars = doc_scalarargs_regex.search( doc_node.text )
      arrays = doc_arrayargs_regex.search( doc_node.text )
      if scalars == None:
        none_scalar.append( (proc_node.get( "name"), proc_file_name) )
      if arrays == None:
        none_array.append( (proc_node.get( "name"), proc_file_name) )
      
    print "none_scalars", none_scalar,"\n\nnone_arrays", none_array
    print "="*100
    
    for i in none_scalar:
      sys.stdout.write( i[1] + "," )
    
    print "\n"*2
    
    for i in none_array:
      sys.stdout.write( i[1] + "," )
    
    print "\n"*2
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class FindAllLAPACKETypesPass ( Pass )

Purpose:
was used to collect all the types named in LAPACKE.h
to put into input xml type defines etc.
'''
class FindAllLAPACKETypesPass ( Pass ):
  dependencies = [BaseLAPACKEPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = FindAllLAPACKETypesPass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    lapacke_procs_root = xml_tree.find( "./LAPACKE/procedures" )
    
    types = set()
       
    for proc in lapacke_procs_root.findall( "./procedure" ):
      types.add( proc.get( "return-type" ) )
      for arg in proc.findall( "./arguments-list/argument" ):
        types.add( arg.get( "type" ) )
        
    print types
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class FindScalarOutIntentsPass ( Pass )

Purpose:
Find scalars in the fortran code with 'out' intents, that
are also not INFOs
Explore if there are LAPACKE scalars that are out intents
'''
class FindScalarOutIntentsPass ( Pass ):
  dependencies = [BaseLAPACKPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = FindScalarOutIntentsPass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    lapack_procs_root = xml_tree.find( "./LAPACK/procedures" )
    print lapack_procs_root
    outs = []
       
    for proc in lapack_procs_root.findall( "./procedure" ):
      for arg in proc.findall( "./arguments-list/argument" ):
        if arg.get( "semantic" ) == "scalar" \
           and "out" in arg.get( "intent" ) \
           and arg.get("name").lower() != "info":
          outs.append( (proc.get( "name" ), arg.get( "name" ), proc.get( "file-name" ) ) )
            
    print outs
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class FindIntentSetPass ( Pass ):

Purpose:
find the set of all intents that exist in LAPACKE fold
'''
class FindIntentSetPass ( Pass ):
  dependencies = [FoldLAPACKSemanticsIntentsToLAPACKEPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = FindIntentSetPass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    lapack_procs_root = xml_tree.find( "./LAPACKE/procedures" )
    
    intents = set()
       
    for proc in lapack_procs_root.findall( "./procedure" ):
      #print proc.get( "name" )
      #prettyprintxml( proc )
      for arg in proc.findall( "./arguments-list/argument" ):
        #print arg.get( "name" )
        if arg.get( "intent" ) != None:
          intents.add( arg.get( "intent" ) )
            
    print intents
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class FindIntentSemanticsRefsSetPass ( Pass ):

Purpose:
find the set of all combinations of intenst that exist in LAPACKE fold
'''
class FindIntentSemanticsRefsSetPass ( Pass ):
  dependencies = [FoldLAPACKSemanticsIntentsToLAPACKEPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = FindIntentSemanticsRefsSetPass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    lapack_procs_root = xml_tree.find( "./LAPACKE/procedures" )
    
    combos = {}
       
    for proc in lapack_procs_root.findall( "./procedure" ):
      for arg in proc.findall( "./arguments-list/argument" ):
        intent = arg.get( "intent" )
        semantic = arg.get( "semantic" )
        refdepth = arg.get( "refdepth" )
        
        combos[ (intent, semantic, refdepth ) ] = (proc, arg)
        
        
    for key in combos:
      print key, "(", combos[ key ][0].get( "name" ), combos[key][1].get( "name" ), ")"
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class FindBadIntentSemanticsCodePass ( Pass ):

Purpose:
find fortran code where intent or semantic are None
'''
class FindBadIntentSemanticsCodePass ( Pass ):
  dependencies = [FoldLAPACKSemanticsIntentsToLAPACKEPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = FindBadIntentSemanticsCodePass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    lapack_procs_root = xml_tree.find( "./LAPACKE/procedures" )
    
    list = {}
    
    for proc in lapack_procs_root.findall( "./procedure" ):
      proc_name = proc.get("name")
      for arg in proc.findall( "./arguments-list/argument" ):
        arg_name = arg.get( "name" )
        intent = arg.get( "intent" )
        semantic = arg.get( "semantic" )

        if arg_name != "matrix_order" and ( (intent == None) ^ (semantic == None) ):
          if not proc_name in list:
            list[proc_name] = []
          list[proc_name].append( (arg_name, intent, semantic, proc) )
          
    files_str = str()
    for key in list:
      proc = list[key][1][3]
      analogue_txt = proc.find( "./analogues/analogue" ).text
      analogue = xml_tree.find( analogue_txt )
      
      files_str += analogue.get( "file-name" ) + ","
      
      print key, analogue_txt, analogue.get( "file-name" )
      #prettyprintxml( proc )
      #prettyprintxml( analogue )
      for elem in list[key]:
        print "\t",elem
      print ""
      
    print files_str
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class FindNonAnalouges ( Pass ):

Purpose:
find all C lapack procedures with no fortran analogues
'''
class FindPassByRefNonAnalouges ( Pass ):
  dependencies = [BaseAssociatePass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = FindPassByRefNonAnalouges
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    lapack_procs_root = xml_tree.find( "./LAPACKE/procedures" )
    
    list = []
    
    for proc in lapack_procs_root.findall( "./procedure" ):
      proc_name = proc.get( "name" )
      if proc.find( "./analogues" ) == None:
        #list.add( proc.get( "name" ) )
        print "Function", proc_name, "has no fortran analogue"
        continue
      printed = False
      for arg in proc.findall( "./arguments-list/argument" ):
        if arg.find( "./analogues" ) == None and \
           int(arg.get( "refdepth" )) > 0  :
          if not printed:
            printed = True
            print "In function", proc_name, ":"
          print "\tArgument", arg.get( "name" ), "of refdepth", int(arg.get( "refdepth" )), "has no fortran analogue"
      if printed:
        print ""  
    
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class FindFortranNoTypes ( Pass )

Purpose:
find any fortran arguments with no associated type
'''
class FindFortranNoTypes ( Pass ):
  dependencies = [BaseLAPACKPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = FindFortranNoTypes
    Pass.resolve( selfname, xml_tree )
    print "[",selfname,"]"
    
    lapack_f_procs = xml_tree.find( "./LAPACK/procedures" )
    print lapack_f_procs
    for f_proc in lapack_f_procs.findall( "./procedure" ):
      for f_arg in f_proc.findall( "./arguments-list/argument" ):
        if f_arg.get( "type" ) == None:
          print f_proc.get( "name" ), f_proc.get( "file-name" ), f_arg.get( "name" )
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class BucketVariblesSemanticsPass ( Pass )

Purpose:
Find and bucket arguments by semantic
'''
class BucketArgumentsSemanticsPass ( Pass ):
  dependencies = [FoldLAPACKSemanticsIntentsToLAPACKEPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = BucketArgumentsSemanticsPass
    Pass.resolve( selfname, xml_tree )
    print "[",selfname,"]"
    
    lapack_f_procs = xml_tree.find( "./LAPACKE/procedures" )
    
    variables = {}
    
    for proc in lapack_f_procs.findall( "./procedure" ):
      proc_name = proc.get( "name" )
      for arg in proc.findall( "./arguments-list/argument" ):
        arg_name = arg.get( "name" )
        semantic = arg.get( "semantic" ) if arg.get( "semantic" ) != None else "none"
        
        if not arg_name in variables:
          variables[ arg_name ] = {}
        if not semantic in variables[ arg_name ]:
          variables[ arg_name ][ semantic ] = []
        
        variables[ arg_name ][ semantic ].append( proc_name )

    for arg in variables:
      if len( variables[ arg ] ) > 2:
        print arg
        for semantic in variables[ arg ]:
          print "  \"" + semantic + "\"", ":", len( variables[ arg ][ semantic ] )

    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class BucketFortranTypes ( Pass )

Purpose:
find all fortran types
'''
class BucketFortranTypes ( Pass ):
  dependencies = [DestroyUnassociatedFortranFunctionsTreePass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = BucketFortranTypes
    Pass.resolve( selfname, xml_tree )
    print "[",selfname,"]"
    
    
    lapack_f_procs = xml_tree.find( "./LAPACK/procedures" )
    lapack_c_procs = xml_tree.find( "./LAPACKE/procedures" )
    f_types = set()
    c_types = set()
    
    for f_proc in lapack_f_procs.findall( "./procedure" ):
      #if f_proc.get( "return-type" ) != None:
      #  f_types.add( f_proc.get( "return-type" ) )
      for f_arg in f_proc.findall( "./arguments-list/argument" ):
        if f_arg.get( "type" ) != None:
          f_types.add( f_arg.get( "type" ) )
        else:
          f_types.add( "~BAD. None for type~" )
          
    for c_proc in lapack_c_procs.findall( "./procedure" ):
      #if c_proc.get( "return-type" ) != None:
      #  c_types.add( c_proc.get( "return-type" ) )
      for c_arg in c_proc.findall( "./arguments-list/argument" ):
        if c_arg.get( "type" ) != None:
          c_types.add( c_arg.get( "type" ) )
        else:
          c_types.add( "~BAD. None for type~" )
    
    print "C types", c_types
    print "Fortran types", f_types,"\n"

    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class FindAFortranType ( Pass )

Purpose:
find a fortran type
'''
class FindAFortranType ( Pass ):
  dependencies = [ BaseAssociatePass ]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = FindAFortranType
    Pass.resolve( selfname, xml_tree )
    print "[",selfname,"]"
    
    
    lapack_f_procs = xml_tree.find( "./LAPACK/procedures" )
    #lapack_c_procs = xml_tree.find( "./LAPACKE/procedures" )
    f_types = set()
    #c_types = set()
    find = "RECURSIVE"
    for f_proc in lapack_f_procs.findall( "./procedure" ):
      if f_proc.get( "return-type" ) == find:
        print f_proc.get( "name" )
        #return
        
      for f_arg in f_proc.findall( "./arguments-list/argument" ):
        if f_arg.get( "type" ) == find:
          print f_proc.get( "name" ), f_arg.get( "name" )
          #return

    

    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class FindUnmatchedArgsPass ( Pass )

Purpose:
Find unresolved matches arising from misnames
'''
class FindUnmatchedArgsPass ( Pass ):
  dependencies = [BaseAssociatePass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = FindUnmatchedArgsPass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    lapack_c_root = xml_tree.find( "./LAPACKE" )
    lapack_c_procs = lapack_c_root.find( "./procedures" )
    
    lapack_f_root = xml_tree.find( "./LAPACK" )
    lapack_f_procs = lapack_f_root.find( "./procedures" )
    
    for c_proc in lapack_c_procs.findall( "./procedure" ):
      # Find the fortran analogue.
      # Note: there should only be one path from C -> Fortran.
      # though there may be many paths from Fortran -> C
      f_proc_ana = c_proc.find( "./analogues/analogue" )
      if f_proc_ana == None:
        continue
    
      f_proc = xml_tree.find( f_proc_ana.text )
      
      c_no_match = []
      f_no_match = []
      
      for c_arg in c_proc.findall( "./arguments-list/argument" ):
        #print c_arg.get( "name" )
        if c_arg.find( "./analogues/" ) == None \
           and c_arg.get( "name" ) != "matrix_order" :
          #print "has none"
          c_no_match.append( c_arg )
        #prettyprintxml( c_arg )
        
      for f_arg in f_proc.findall( "./arguments-list/argument" ):
        f_ana_node = f_arg.find( "./analogues" )
        # if zero analogues add no_match
        if f_ana_node == None:
          f_no_match.append( f_arg )
          continue
        
        # if no analogues to this function add no_match
        if f_ana_node.find( "./analogue/[@function='" + c_proc.get( "name" ) + "']" ) == None \
           and f_arg.get( "name" ) != "INFO":
          f_no_match.append( f_arg )
          
      if c_no_match == []: continue
        
      print c_proc.get( "name" ), ":", f_proc.get( "name" )
      print "+",c_proc.get( "name" )
      for m in c_no_match:
        #prettyprintxml( m )
        print "\t-", m.get( "name" )
        
      print "+",f_proc.get( "name" )
      for m in f_no_match:
        #prettyprintxml( m )
        print "\t-", m.get( "name" )
      
      print "\n"
        
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class FindNoneLAPACKESementicsPass ( Pass )

Purpose:
Find LAPACKE arguments of procedures with no semantics
this arises when the function or the arguments do not
have analogues or they have not been imported
'''
class FindNoneLAPACKESementicsPass ( Pass ):
  dependencies = [BaseTransformLAPACKEPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = FindNoneLAPACKESementicsPass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    lapack_c_root = xml_tree.find( "./LAPACKE" )
    lapack_c_procs = lapack_c_root.find( "./procedures" )
        
    for c_proc in lapack_c_procs.findall( "./procedure" ):
      #if c_proc.find( "./analogues/analogue" ) == None: continue
        
      printed = False
      for c_arg in c_proc.findall( "./arguments-list/argument" ):
        if c_arg.get( "semantic" ) == None:
          if not printed:
            print c_proc.get( "name" )
            printed = True
          print "Missing sementic on", c_arg.get( "name" )
      if printed:
        print ""
        
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

class FindHasNoFortranAnaloguePass ( Pass ):
  dependencies = [BaseAssociatePass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = FindHasNoFortranAnaloguePass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    lapack_c_root = xml_tree.find( "./LAPACKE" )
    lapack_c_procs = lapack_c_root.find( "./procedures" )
    list = set()    
    for c_proc in lapack_c_procs.findall( "./procedure" ):
      if c_proc.find( "./analogues/analogue" ) == None: 
        #list.add( c_proc.get("name").replace( "LAPACKE_", "" ).replace( "LAPACK_", "" ).replace( "_work", "" ) )
        print c_proc.get( "name" )
    for i in list:
      print i
      
        
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class DropFileOfCTreeUnmatchedArgsPass ( Pass )

Purpose:
export an xml file that has most skeleton of xml tree that could
be installed into the input xml for the ImportArgumentAnaloguesPass
'''
class DropFileOfCTreeUnmatchedArgsPass ( Pass ):
  dependencies = [BaseAssociatePass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = DropFileOfCTreeUnmatchedArgsPass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    lapack_c_root = xml_tree.find( "./LAPACKE" )
    lapack_c_procs = lapack_c_root.find( "./procedures" )
    
    output_tree = ET.Element( "pass-output" )
    pass_output = SubElement( output_tree, "pass" )
    pass_output.set( "name", "DropFileOfCTreeUnmatchedArgsPass" )
      
    for c_proc in lapack_c_procs.findall( "./procedure" ):
      # Find the fortran analogue.
      # Note: there should only be one path from C -> Fortran.
      # though there may be many paths from Fortran -> C
      f_proc_ana = c_proc.find( "./analogues/analogue" )
      if f_proc_ana == None:
        continue
    
      f_proc = xml_tree.find( f_proc_ana.text )
      c_no_match = []
      f_no_match = []
      
      for c_arg in c_proc.findall( "./arguments-list/argument" ):
        if c_arg.find( "./analogues/" ) == None \
           and c_arg.get( "name" ) != "matrix_order" :
          c_no_match.append( c_arg )
          
      for f_arg in f_proc.findall( "./arguments-list/argument" ):
        f_ana_node = f_arg.find( "./analogues" )
        # if zero analogues add no_match
        if f_ana_node == None:
          f_no_match.append( f_arg )
          continue
        
        # if no analogues to this function add no_match
        if f_ana_node.find( "./analogue/[@function='" + c_proc.get( "name" ) + "']" ) == None \
           and f_arg.get( "name" ) != "INFO":
          f_no_match.append( f_arg )
      
      if c_no_match == []: #and f_no_match == []:
        continue
        
      proc_node = SubElement( pass_output, "procedure" )
      proc_node.set("name", c_proc.get( "name" ) )
      proc_node.set( "analogue-path", f_proc_ana.text )
      
      for c_arg in c_no_match:
        arg_node = SubElement( proc_node, "argument" )
        arg_node.set( "name", c_arg.get("name") )
        possible = SubElement( arg_node, "possible_substitutions" )
        for f_arg in f_no_match :
          f_arg_node = SubElement( possible, "option" ) 
          f_arg_node.set( "name", f_arg.get( "name" ) )
          f_arg_node.set( "semantic", f_arg.get( "semantic" ) )
          f_arg_node.set( "intent", f_arg.get( "intent" ) )
          f_arg_node.set( "type", f_arg.get( "type" ) )
          f_arg_node.set( "substitution",  f_arg.get( "name" ) )
        
      prettywritexml( output_tree, "DropFileOfCTreeUnmatchedArgsPass_output.xml" )
      
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

'''
class DropFileOfCTreeUnmatchedArgsWithSuggestionsPass ( Pass )

Purpose:
export an xml file that has most skeleton of xml tree that could
be installed into the input xml for the ImportArgumentAnaloguesPass
and also include suggestions based on name-score and type union heuristics
that were used as an attempt to solve the issue automatically but were
found to be over-matchy
'''
class DropFileOfCTreeUnmatchedArgsWithSuggestionsPass ( Pass ):
  dependencies = [BaseAssociatePass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = DropFileOfCTreeUnmatchedArgsWithSuggestionsPass
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"
    
    output_tree = ET.Element( "pass-output" )
    pass_output = SubElement( output_tree, "pass" )
    pass_output.set( "name", "DropFileOfCTreeUnmatchedArgsPass" )
      
    lapack_c_root = xml_tree.find( "./LAPACKE" )
    lapack_c_procs = lapack_c_root.find( "./procedures" )
    
    lapack_f_root = xml_tree.find( "./LAPACK" )
    lapack_f_procs = lapack_f_root.find( "./procedures" )
    
    for c_proc in lapack_c_procs.findall( "./procedure" ):
    
      # Find the fortran analogue.
      # Note: there should only be one path from C -> Fortran.
      # though there may be many paths from Fortran -> C
      f_proc_ana = c_proc.find( "./analogues/analogue" )
      if f_proc_ana == None:
        continue
    
      f_proc = xml_tree.find( f_proc_ana.text )
      
      c_no_match = []
      f_no_match = []
      
      for c_arg in c_proc.findall( "./arguments-list/argument" ):
        if c_arg.find( "./analogues/" ) == None \
           and c_arg.get( "name" ) != "matrix_order" :
          c_no_match.append( c_arg )
          
      for f_arg in f_proc.findall( "./arguments-list/argument" ):
        f_ana_node = f_arg.find( "./analogues" )
        # if zero analogues add no_match
        if f_ana_node == None:
          f_no_match.append( f_arg )
          continue
        
        # if no analogues to this function add no_match
        if f_ana_node.find( "./analogue/[@function='" + c_proc.get( "name" ) + "']" ) == None \
           and f_arg.get( "name" ) != "INFO" :
          f_no_match.append( f_arg )
          
      if c_no_match == [] :
        continue
      
      proc_node = SubElement( output_tree, "procedure" )
      proc_node.set( "name", c_proc.get( "name" ) )
      proc_node.set( "path", "./LAPACKE/procedures/procedure/[@name='" + c_proc.get( "name" ) + "']")
      
      for c_arg in c_no_match:
        arg_node = SubElement( proc_node, "argument" )
        arg_info = SubElement( arg_node, "argument-info" )
        
        arg_node.set( "name", c_arg.get( "name" ) )
        arg_node.set( "substitution", "????" )
        arg_node.set( "substitution-path", "????")
        
        arg_info.set( "path", proc_node.get( "path" ) + "/arguments-list/argument/[@name='" + c_arg.get( "name" ) + "']" )
        arg_info.set( "type", c_arg.get( "type" ) )
        arg_info.set( "refdepth", c_arg.get("refdepth") )

        if f_no_match != None:
          possibles = SubElement( arg_node, "possible-analogues" )
          for f_arg in f_no_match:
            possible = SubElement( possibles, "possible" )
            possible.set( "name", f_arg.get( "name" ) )
            possible.set( "path", "./LAPACK/procedures/procedure/[@name='" + f_proc.get( "name" ) + "']/arguments-list/argument/[@name='" + f_arg.get( "name" ) + "']" )
            possible.set( "semantic", f_arg.get( "semantic" ) )
            possible.set( "intent", f_arg.get( "intent" ) )
            possible.set( "type", f_arg.get( "type" ) )
      
      
      print c_proc.get( "name" ), f_proc.get( "name" )
      
      type_map = { 
                  "LOGICAL" : "booelan",
                  "lapack_int" : "int32",
                  "INTEGER" : "int32",
                  "lapack_complex_double" : "complex128",
                  "lapack_complex_float" : "complex64",
                  "COMPLEX*16" : "complex128",
                  "COMPLEX" : "complex64",
                  "DOUBLE" : "real32",
                  "DOUBLE PRECISION" : "real64",
                  "REAL" : "real32",
                  "SINGLE PRECISION" : "real32",
                  "double" : "real64",
                  "float" : "real32",
                  "char" : "char",
                  "CHARACTER" : "char",
                  "CHARACTER*1" : "char"
                 }
      
      t_sets = {}
      
      for arg in c_no_match:
        type = type_map[ arg.get( "type" ) ]
        if not type in t_sets:
          t_sets[ type ] = set()
        t_sets[ type ].add( arg )
      
      for arg in f_no_match:
        #print f_proc.get("name"), arg.get("name")
        type = type_map[ arg.get( "type" ) ]
        if not type in t_sets:
          t_sets[ type ] = set()
        t_sets[ type ].add( arg )

      for type in t_sets:
        # when there only exists a pair of arguments in a type,
        # and those arguments are each in opposite code trees (fortran/C)
        # it can heuristically be assumed that those arguments can be associated
        if len( t_sets[ type ] ) == 2:

          arg_1 = t_sets[ type ].pop()
          arg_2 = t_sets[ type ].pop()
          
          if (arg_1 in c_no_match and arg_2 in f_no_match ) ^ \
             (arg_2 in c_no_match and arg_1 in f_no_match ):
            
            c_arg = arg_1 if arg_1 in c_no_match else arg_2
            f_arg = arg_2 if arg_2 in f_no_match else arg_1
            
            print "match", c_arg.get("name"), "to", f_arg.get("name"),"unique type union"

            # ?_ana_node is the analogue record under ? language (ie c_ana_node notes the argument in the fortran tree, but lives in the C tree)
            # Note that it is totally possible to create the path string from the two atributes of the tag.
            # easier to create once here, instead of manytimes everywhere else.
            arg_node = proc_node.find( "./argument/[@name='" + c_arg.get( "name" ) + "']" )
            arg_node.set( "substitution", f_arg.get( "name" ) )
            arg_node.set( "substitution-path", "./LAPACK/procedures/procedure/[@name='" + f_proc.get( "name" ) + "']/arguments-list/argument/[@name='" + f_arg.get( "name" ) + "']" )
            
            
            
            '''
            c_ana_node = SubElement( SubElementUnique( c_arg, "analogues" ), "analogue" )
            c_ana_node.text = "./LAPACK/procedures/procedure/[@name='" + f_proc.get( "name" ) + "']/arguments-list/argument/[@name='" + f_arg.get( "name" ) + "']"
            c_ana_node.set( "function", f_proc.get( "name" ) )
            c_ana_node.set( "name", f_arg.get( "name" ) )

            f_ana_node = SubElement( SubElementUnique( f_arg, "analogues" ), "analogue" )
            f_ana_node.text = "./LAPACKE/procedures/procedure/[@name='" + c_proc.get( "name" ) + "']/arguments-list/argument/[@name='" + c_arg.get( "name" ) + "']"
            f_ana_node.set( "function", c_proc.get( "name" ) )
            f_ana_node.set( "name", c_arg.get( "name" ) )
            '''
            c_no_match.remove( c_arg )
            f_no_match.remove( f_arg )
        
        # if there are more than two arguments in a type
        # we can try to match the strings from the 
        elif len( t_sets[ type ] ) > 2 :        
          change = True # True to emulate do-while
          iter = 1
          while change:
            print "Iteration:", iter
            change = False
            c_removes = []
            f_removes = []
        
            for c_arg in c_no_match:
              min_list = []
              min_score = 10**1000
  
              for f_arg in f_no_match:
                score = score_string( c_arg.get("name").lower(), f_arg.get("name" ).lower() )
                if score < min_score:
                  min_score = score
                  min_list = [ f_arg ]
                elif score == min_score:
                  min_list.append( f_arg )

            
              if len( min_list ) >1 :
                print "BOTCHED matching for", c_arg.get("name"),": args",
                for arg in min_list:
                  print arg.get("name"),",",
                print "have same score", min_score
                continue
              
              min = min_list[0]
              if min_score > 2:
                print "FAILED to match", c_arg.get("name"), "to", min.get("name"), "score", min_score, "was too bad"
                continue
              
              change = True
              
              print "match", c_arg.get("name"), "to", min.get("name"), "score", min_score 
              f_arg = min
              # ?_ana_node is the analogue record under ? language (ie c_ana_node notes the argument in the fortran tree, but lives in the C tree)
              # Note that it is totally possible to create the path string from the two atributes of the tag.
              # easier to create once here, instead of manytimes everywhere else.
              
              arg_node = proc_node.find( "./argument/[@name='" + c_arg.get( "name" ) + "']" )
              arg_node.set( "substitution", f_arg.get( "name" ) )
              arg_node.set( "substitution-path", "./LAPACK/procedures/procedure/[@name='" + f_proc.get( "name" ) + "']/arguments-list/argument/[@name='" + f_arg.get( "name" ) + "']" )
            
              
              '''
              c_ana_node = SubElement( SubElementUnique( c_arg, "analogues" ), "analogue" )
              c_ana_node.text = "./LAPACK/procedures/procedure/[@name='" + f_proc.get( "name" ) + "']/arguments-list/argument/[@name='" + f_arg.get( "name" ) + "']"
              c_ana_node.set( "function", f_proc.get( "name" ) )
              c_ana_node.set( "name", f_arg.get( "name" ) )
            
              f_ana_node = SubElement( SubElementUnique( f_arg, "analogues" ), "analogue" )
              f_ana_node.text = "./LAPACKE/procedures/procedure/[@name='" + c_proc.get( "name" ) + "']/arguments-list/argument/[@name='" + c_arg.get( "name" ) + "']"
              f_ana_node.set( "function", c_proc.get( "name" ) )
              f_ana_node.set( "name", c_arg.get( "name" ) )    
              '''
              
              c_removes.append( c_arg )
              f_removes.append( f_arg )
      
    
            for r in c_removes:
              c_no_match.remove( r )
              
            for r in f_removes:
              f_no_match.remove( r )
            
            iter += 1
          print "No changes"
      
        
      for c_arg in c_no_match:
        print "Could not match", c_arg.get( "name" )
      
      for f_arg in f_no_match:
        print "Could not match", f_arg.get( "name" )
        
      print ""
        
      prettywritexml( output_tree, "DropFileOfCTreeUnmatchedArgsPass_output.xml" )
      
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

class FindLAPACKFunctionGroups ( Pass ):
  dependencies = [ChapelizeLAPACKEFunctionsPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = FindLAPACKFunctionGroups
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    lapacke_root = xml_tree.find( "./LAPACKE" )
    lapacke_procs = lapacke_root.find( "./procedures" )
    
    module_root = xml_tree.find( "./chapel-module" )
    module_procs = module_root.find( "./procedures" )
    
    names = set()
    groups = {}
      
    for proc in lapacke_procs.findall( "./procedure" ):
      proc_name = proc.get( "name" )
      base_name = proc_name.replace( "LAPACK_", "" ).replace( "LAPACKE_", "" ).replace( "_work", "" )
      
      match = func_name_group_regex.search( base_name );
      
      if match == None:
        print proc_name, "ie", base_name, "does not match regex"
        continue
      
      #names.add( base_name );
      func = match.group( "function" )
      config = match.group( "mtype" )
      
      if not func in groups:
        groups[ func ] = {}
        
      if not config in groups[func] :
        groups[func][config] = []
        
      groups[func][config].append( proc_name )
      
    group_counts = 0
    config_count = 0
    type_counts = 0
    for func in groups:
      print func
      group_counts += 1
      for config in groups[func]:
        print "\t", config
        config_counts += 1
        for name in groups[func][config]:
          print "\t\t", name
          type_counts += 1
          
    print group_counts, config_counts, type_counts
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

class FindDuplicateLAPACKEFunctions ( Pass ):
  dependencies = [BaseLAPACKEPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = FindDuplicateLAPACKEFunctions
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    root_procs = xml_tree.find( "./LAPACKE/procedures" )
    proc_names = set()
    for proc in root_procs.findall( "./procedure" ):
      proc_name = proc.get( "name" )
      #print proc_name
      if proc_name in proc_names:
        raise GeneralPassFailure( proc_name )
      else:
        proc_names.add( proc_name )
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

class PrintArgDoc ( Pass ):
  dependencies = [FuncArgsDocPass, FuncArgsTypePass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = PrintArgDoc
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    lapack_node = xml_tree.find( "./LAPACK" )
    text_node = lapack_node.find( "./text" )
    procs_node = lapack_node.find( "./procedures" )
    
    doc_set = set();
    total = 0
    docked = 0
    arg="A"
    for proc_node in procs_node.findall( "./procedure" ):
      for arg_node in proc_node.findall( "./arguments-list/argument/[@name='"+ arg +"']" ):
        doc_node = arg_node.find( "documentation" )
        total += 1
        if doc_node == None:
          print proc_node.get( "name" ), "/", arg, "has no documentation"
          continue
          
        doc = doc_node.text.lower()
        doc = doc.replace( "-", "" )
        doc = doc.replace( "the", "" )
        doc = re.sub( "lda\s+is\s+integer", "", doc )
        doc = re.sub( "\s+", "", doc )
        doc_set.add( doc )
        docked += 1
    
    doc_list = sorted( list( doc_set ), key=len )
    for i in doc_list:
      print i
      
    print len( doc_list ), "/", docked, "/", total
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

class FindDifferenArgumentsPass ( Pass ):
  dependencies = [BucketLAPACKFunctionGroupsPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = FindDifferenArgumentsPass
    Pass.resolve( selfname, xml_tree )
    print "[",selfname,"]"
    
    abstract_lapack = xml_tree.find( "./Abstract-LAPACK" )
    
    for group_node in abstract_lapack.findall( "./group/[@name='sv']" ):
      for config_node in group_node.findall( "./matrix-configuration" ):
        name_to_args = {} # function_name => { order => args_node }
        name_to_counts = {} # function_name => number
        for type_node in config_node.findall( "./type" ):
          chap_func = xml_tree.find( type_node.get( "analogue" ) )
          name_to_args[ type_node.get( "name" ) ] = {}
          for arg in chap_func.findall( "./arguments-list/argument" ):
            name_to_args[ type_node.get( "name" ) ][ arg.get("position") ] = arg
          name_to_counts[ type_node.get( "name" ) ] = len( name_to_args[ type_node.get( "name" ) ] )
        
        all_same = True
        all_count = 0
        names = name_to_counts.keys()
        for i in range( len( names ) - 1 ):
          all_same = all_same and ( name_to_counts[ names[i] ] == name_to_counts[ names[i+1] ] )
        print all_same
        all_count = name_to_counts[ names[1] ] # grab arbitrary count if all the same
        
        for pos in range( all_count ):
          is_same = True
          for i in range( len(names)-1):
            is_same = is_same and ( name_to_args[names[i]].get("name")     == name_to_args[names[i+1]].get("name") ) \
                              and ( name_to_args[names[i]].get("semantic") == name_to_args[names[i+1]].get("semantic") ) \
                              and ( name_to_args[names[i]].get("intent")   == name_to_args[names[i+1]].get("intent") )
          print pos, is_same
          if not is_same:
            return
        
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"
    
class CountGroups ( Pass ):
  dependencies = [BucketLAPACKFunctionGroupsPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = FindDifferenArgumentsPass
    Pass.resolve( selfname, xml_tree )
    print "[",selfname,"]"
    
    abstract_lapack = xml_tree.find( "./Abstract-LAPACK" )
    groups = 0
    configs = 0
    types = 0
    
    for group_node in abstract_lapack.findall( "./group" ):
      groups += 1
      for config_node in group_node.findall( "./matrix-configuration" ):
        configs += 1
        for type_node in config_node.findall( "./type" ):
          types += 1
    
    print groups, configs, types
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

class DropFileOfGroups ( Pass ):
  dependencies = [BaseAbstractLAPACKPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = DropFileOfGroups
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    abstract_lapack = xml_tree.find( "./Abstract-LAPACK" )

    for group_node in abstract_lapack.findall( "./group/[@name='sv']" ):
      for config_node in group_node.findall( "./matrix-configuration" ):
        
        interface_tree = SubElement( config_node, "method-arguments" )
        argument = SubElement( interface_tree, "argument" )
        argument.set( "name", "?" )
        argument.set( "intent" , "?" )
        argument.set( "semantic", "?" )
        argument.set( "type", "?" )
        
        relation_tree = SubElement( config_node, "arguments-relationships" )
    
        for arg in config_node.findall( "./analogue-arguments-list/argument" ):
          arg_relate = SubElement( relation_tree, "argument" )
          arg_relate.set( "name", arg.get("name") )
          arg_relate.text = "RELATIONSHIP"
        
        
        
    prettyprintxml( abstract_lapack)
    prettywritexml( abstract_lapack, "DropFilesOfGroups.xml" )
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

class TryMatrixArgsUnion ( Pass ):
  dependencies = [BaseAbstractLAPACKPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = TryMatrixArgsUnion
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    abstract_lapack = xml_tree.find( "./Abstract-LAPACK" )
   
    for group_node in abstract_lapack.findall( "./group" ):
      for config_node in group_node.findall( "./matrix-configuration" ):
        for arg in config_node.findall( "./analogue-arguments-list/argument" ):
          pass
     
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

class SolveArgsUnionFor ( Pass ):
  dependencies = [BaseAbstractLAPACKPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = SolveArgsUnionFor
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    abstract_lapack = xml_tree.find( "./Abstract-LAPACK" )
    
    unique = set()
    co = set()
    non = set()
    unset = True;
    for group_node in abstract_lapack.findall( "./group" ):
      for config_node in group_node.findall( "./matrix-configuration" ):
        print config_node.get( "name" ) + group_node.get( "name" )
        config_args = set()
        array_args = set()
        array_dims = {}
        for arg in config_node.findall( "./analogue-arguments-list/argument" ):
          config_args.add( arg.get( "name" ).lower() )
          if arg.get( "semantic" ) == "array":
            array_args.add( arg.get("name") )
            #prettyprintxml( arg )
             
        if "m" in config_args:
          print array_args
          for elem in array_args:
            print elem
          co |= array_args
          if unset:
            unique |= array_args
            unset = False;
          else:
            unique &= array_args
          print unique, "\n"
    print "="*10        
    print unique, "\n"
    print co, "\n"
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

class FindDifferentLengthCalls ( Pass ):
  dependencies = [BaseLAPACKEPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = FindDifferentLengthCalls
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    procs_dict = {}
    fams = set()
    for proc in xml_tree.findall( "./LAPACKE/procedures/procedure" ):
      if proc.get("name").startswith("LAPACK_"):
        continue
      base_name = proc.get("name").replace( "LAPACKE_", "" )
      
      match = func_name_group_regex.search( base_name );
      if match == None:
        #print proc.get("name"), "(", base_name, ") does not match regex"
        continue
      
      func = match.group( "function" )
      config = match.group( "config" )
      type = match.group( "type" )
      
      name = config + func 
      
      if not name in procs_dict:
        procs_dict[ name ] = set()
      
      procs_dict[name].add( len( proc.findall( "./arguments-list/argument" ) ) )
      
      if len( procs_dict[name] ) > 1 :
        fams.add( name )
        #return
    #print procs_dict  
    for fam in fams:
      print fam
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

class IsNOrMEverTheSame ( Pass ):
  dependencies = [BaseLAPACKPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = IsNorMEverTheSame
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    lapack_node = xml_tree.find( "./LAPACK" )
    text_node = lapack_node.find( "./text" )
    procs_node = lapack_node.find( "./procedures" )

    
    for proc_node in procs_node.findall( "./procedure" ):
      proc_name = proc_node.get( "name" )
      '''
      base_name = proc_name.lower()
      match = func_name_group_regex.search( base_name );
      if match == None:
        #print proc_name, "(", base_name, ") does not match regex"
        continue
    
      func = match.group( "function" )
      config = match.group( "config" )
      type = match.group( "type" )
      
      if not config.startswith( "ge" ):
        continue
      '''
      arg_names = [ arg.get("name") for arg in proc_node.findall( "./arguments-list/argument" ) ]
        
      for arg_node in proc_node.findall( "./arguments-list/argument" ):
        doc_node = arg_node.find( "documentation" )
        if doc_node == None or arg_node.get("semantic") != "scalar" or arg_node.get("type").lower() != "integer":
          continue
          
        what = []
        who = []
        string = []
        
        for m in scalar_matrix_relation_regex.finditer( doc_node.text ):
          if not m.group( "what" ) in ["rows", "columns", "order", "rank"] :
            continue
          
          names = m.group( "who" ).strip()
          names_list = []
          if " and " in names:
            names_list = [ name.strip() for name in names.split( "and" ) ]
          else:
            names_list = [ names ]

          nameHasSpace = False
          for name in names_list:
            if " " in name:
              nameHasSpace = True
              break
          
          if nameHasSpace:
            print names, " contains non names. Skipping."
            continue
          
          removes = []
          for name in names_list:
            if not name in arg_names:
              removes.append( name )

          for rm in removes:
            names_list.remove( rm )

          if len( names_list ) == 0:
            print "Names list had no argument names. Skipping" 
            continue
          
          what.append( m.group( "what" ) )
          who.append( names_list )
          string.append( re.sub( "\s+", " ", m.group(0) ) )
          
        if len( what ) == 0 and len( who ) == 0:
          continue
        
        #proc_info[ proc_name ][ arg_node.get( "name" ) ] =  [ what, who, string]
        
        associate_array = str()
        associate_field = str()

        first = True
        
        for i in range( len( who ) ):
          for array in who[i]:
            associate_array += ( "," if not first else "" ) + array
            associate_field += ( "," if not first else "" ) + what[i]
            first = False
          
        
        arg_node.set( "associate-array", associate_array )
        arg_node.set( "associate-field", associate_field )
        
      prettyprintxml( proc_node )
    '''    
    for func in proc_info:
      if proc_info[func] == {}:
        continue
      print func
      for arg in proc_info[func]:
        print "\t", arg
        for elem in proc_info[func][arg]:
          print "\t\t", elem
    '''       
          
          
    
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

class FindGroupsWithUncommon ( Pass ):
  dependencies = [BaseAbstractLAPACKPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = PretendCreate
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    abstract_lapack = xml_tree.find( "./Abstract-LAPACK" )

    for group_node in abstract_lapack.findall( "./group" ):
      for config_node in group_node.findall( "./matrix-configuration" ):
        printed = False
        for type in config_node.findall( "./types/type" ):
          if type.find( "./arguments-list" ) != None:
            if not printed:
              print config_node.get("name") + group_node.get("name")
              printed = True
            print "\t", type.get("name")
          
         
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"

class TestInputGroupsGen ( Pass ):
  dependencies = [BaseCodegenReadyPass]
  complete = False
  
  @staticmethod
  def apply( xml_tree ):
    selfname = TestInputGroupsGen
    Pass.resolve( selfname, xml_tree ) 
    print "[",selfname,"]"

    #abstract_lapack = xml_tree.find( "./Abstract-LAPACK" )
    group_input = loadxml( "DropAttemptedAssociations_input.xml" )
    
    for group in group_input.findall( "./group" ):
      for config in group.findall( "./matrix-configuration" ):
        code = SequenceOfProducers()
        print config.get( "name" ) + group.get( "name" )
        code.append( SegmentProducer( "proc " + config.get( "name" ) + group.get( "name" ) ) )

        args_producer = ListProducer(", ", "(", ")")
        for arg in config.findall( "./method-arguments/argument" ):
          args_producer.append( SegmentProducer(
                                  arg.get("intent") + " " + \
                                  arg.get("name") + " : " + \
                                  ("[] " if arg.get("semantic") == "array" else "") + \
                                  arg.get("type") + \
                                  ( " = " + arg.text if arg.text != None and arg.text.strip() != "" else "" )
                                )
                              )

        code.append( args_producer )
        code.append( LineProducer( ": lapack_int" ) )
        code.append( SegmentProducer( "where " ) )
        
        where_producer = ListProducer( " || ", "", "" )
        
        for type in config.findall("./types/type"):
          where_producer.append( SegmentProducer( "T == " + type.get( "type" ) ) )
        
        code.append( where_producer )
        
        info_var = config.get( "name" ) + group.get( "name" ) + "_return_info"
        
        func_body = ScopeProducer()
        func_body.append( LineProducer( "var " + info_var + " : lapack_int;" ) )

        #if_bodies = SequenceOfProducers()
        
        arg_relates = {}
        ana_args = []
        for arg in config.findall( "./analogue-arguments-list/argument" ):
          arg_name = arg.get("name")
          arg_relates[ arg_name ] = config.find( "./arguments-relationships/argument/[@name='" + arg_name + "']" )
          ana_args.append( arg );
        
      
        for type in config.findall("./types/type"):
          chpl_ana = xml_tree.find( type.get( "analogue" ) )
          if_condition = LineProducer( "if ( T == " + type.get("type") + " )" )
          func_body.append( if_condition )
          
          if_body = ScopeProducer()
          
          call_equals = SegmentProducer( info_var + " = " + chpl_ana.get( "name" ) )
          
          call_seq = ListProducer( ", ", "(", ")" )
          
          for ana_arg in ana_args:
            call_seq.append( SegmentProducer( 
                              "(" + arg_relates[ana_arg.get("name")].text.strip() + ")" + \
                              (" : " + ana_arg.get("type") if ana_arg.get("semantic") != "array" else "")
                             )
                           )
            
          if_body.append( call_equals + call_seq + LineProducer( ";" ) )
          func_body.append( if_body )
        
        func_body.append( LineProducer( "return " + info_var + ";" ) )
        
        code.append( func_body )
        print code.generate()
        
                
    selfname.complete = True
    print "[",selfname,":", "Completed" if selfname.complete else "FAILED", "]\n"
