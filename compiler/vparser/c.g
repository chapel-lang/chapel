c_declarator: '*' c_type_qualifier* c_declarator | c_direct_declarator ;
c_direct_declarator: identifier c_declarator_suffix* | '(' c_declarator ')' c_declarator_suffix* ;
c_declarator_suffix: '[' expression? ']' | '(' c_parameter_list? ')';
c_parameter_list: c_parameter_declaration_list ( ',' '...' )? ;
c_parameter_declaration_list : c_parameter_declaration ( ',' c_parameter_declaration )* ;
c_parameter_declaration : c_declaration_specifiers (c_declarator? | c_abstract_declarator) ;
c_abstract_declarator
  : '*' c_type_qualifier* c_abstract_declarator 
  | '(' c_abstract_declarator ')' c_abstract_declarator_suffix+
  | ('[' expression? ']')+
  | ;
c_abstract_declarator_suffix: '[' expression? ']' | '('  c_parameter_declaration_list? ')' ;
c_type_qualifier: 'const' | 'volatile';
c_declaration_specifiers : (c_storage_class_specifier | c_type_specifier | c_type_qualifier)+ ;
c_storage_class_specifier: 'auto' | 'register' | 'static' | 'extern' | 'typedef';
c_type_specifier: 'void' | 'char' | 'short' | 'int' | 'long' | 'float' 
  | 'double' | 'signed' | 'unsigned' | c_struct_or_union_specifier 
  | c_enum_specifier | c_type_identifier;
c_struct_or_union_specifier: ('struct' | 'union') 
  ( c_struct_ident | c_struct_ident? '{' c_struct_declaration+ '}') ;
c_struct_declaration: c_specifier_qualifier_list c_struct_declarator_list ';' ;
c_specifier_qualifier_list: (c_type_specifier | c_type_qualifier)+ ;
c_struct_declarator_list: c_struct_declarator (',' c_struct_declarator)* ;
c_struct_declarator: c_declarator | c_declarator? ':' constant;
c_enum_specifier: 'enum' 
  ( identifier ('{' c_enumerator_list '}')? 
  | '{' c_enumerator_list '}') ;
c_enumerator_list: c_enumerator (',' c_enumerator)* ;
c_enumerator: identifier ('=' expression)?;
c_struct_ident: identifier;
c_type_identifier: identifier;

