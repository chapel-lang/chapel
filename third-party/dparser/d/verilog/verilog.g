
/*
 * Verilog Grammar for dparser.
 *
 * Created from various BNF descriptions found on the web.
 * Thomas Skibo.
 * May 23-29, 2003
 *
 */


{ #include "vparse.h" }

/* Source Text */
source_text : description* ;
description : module | UDP ;

module :
        'module' name_of_module list_of_ports? ';' module_item* 'endmodule' |
        'macromodule' name_of_module list_of_ports? ';' module_item*
            'endmodule' ;

name_of_module : IDENTIFIER ;
list_of_ports : '(' port ( ',' port )* ')' ;
port :
        port_expression? |
        '.' name_of_port '(' port_expression? ')' ;
port_expression :
        port_reference |
        '{' port_reference ( ',' port_reference )* '}' ;
port_reference :
        name_of_variable |
        name_of_variable '[' constant_expression ']' |
        name_of_variable '[' constant_expression ':' constant_expression ']' ;
name_of_port : IDENTIFIER ;

module_item :
        parameter_declaration |
        input_declaration |
        output_declaration |
        inout_declaration |
        net_declaration |
        reg_declaration |
        time_declaration |
        integer_declaration |
        real_declaration |
        event_declaration |
        gate_declaration |
        UDP_instantiation |
        module_instantiation |
        parameter_override |
        continuous_assign |
        specify_block |
        initial_statement |
        always_statement |
        task |
        function ;

UDP : 'primitive' name_of_UDP
        '(' name_of_variable ( ',' name_of_variable )* ')' ';'
        UDP_declaration+ UDP_initial_statement? table_definition
        'endprimitive' ;

UDP_declaration :
        output_declaration |
        reg_declaration |
        input_declaration ;

UDP_initial_statement : 'initial' output_terminal_name '=' init_val ';' ;
init_val : '1\'b0' | '1\'b1' | '1\'bx' | '1' | '0' ;
output_terminal_name : IDENTIFIER ;

table_definition : 'table' table_entries 'endtable' ;
table_entries :
        combinational_entry+ |
        sequential_entry+ ;
combinational_entry : level_input_list ':' OUTPUT_SYMBOL ';' ;
sequential_entry : input_list ':' state ':' next_state ';' ;
input_list : level_input_list | edge_input_list ;
level_input_list : LEVEL_SYMBOL+ ;
edge_input_list : LEVEL_SYMBOL* edge LEVEL_SYMBOL* ;
edge : '(' LEVEL_SYMBOL LEVEL_SYMBOL ')' | EDGE_SYMBOL ;
state : LEVEL_SYMBOL ;
next_state : OUTPUT_SYMBOL | '-' ;
OUTPUT_SYMBOL : "[01xX]" ;
LEVEL_SYMBOL : "[01xX\?bB]" ;
EDGE_SYMBOL : "[rRfFpPnN\*]" ;

task : 'task' name_of_decld_task ';'
            tf_declaration* statement_or_null 'endtask' ;
name_of_decld_task : IDENTIFIER ;

function : 'function' range_or_type? name_of_function ';'
        tf_declaration+ statement 'endfunction' ;
range_or_type : range | 'integer' | 'real' ;
tf_declaration :
        parameter_declaration |
        input_declaration |
        output_declaration |
        inout_declaration |
        reg_declaration |
        time_declaration |
        integer_declaration |
        real_declaration |
        event_declaration ;

/* Declarartions */
parameter_declaration :
        'parameter' range_or_type? list_of_param_assignments ';' ;
list_of_param_assignments : param_assignment ( ',' param_assignment )* ;
param_assignment : identifier '=' constant_expression ;
input_declaration : 'input' range? list_of_variables ';' ;
output_declaration : 'output' range? list_of_variables ';' ;
inout_declaration : 'inout' range? list_of_variables ';' ;
net_declaration :
        NETTYPE expandrange? delay? list_of_variables ';' |
        'trireg' charge_strength? expandrange? delay? list_of_variables ';' ;
NETTYPE :
        'wire' |
        'tri' |
        'tri1' |
        'supply0' |
        'wand' |
        'triand' |
        'tri0' |
        'supply1' |
        'wor' |
        'trior' |
        'trireg' ;
expandrange :
        range |
        'scalared' range |
        'vectored' range ;
reg_declaration : 'reg' range? list_of_register_variables ';' ;
time_declaration : 'time' list_of_register_variables ';' ;
integer_declaration : 'integer' list_of_register_variables ';' ;
real_declaration : 'real' list_of_register_variables ';' ;
event_declaration : 'event' name_of_event ( ',' name_of_event )* ';' ;
continuous_assign :
        'assign' drive_strength? delay? list_of_assignments ';' |
        NETTYPE drive_strength? expandrange? delay? list_of_assignments ';' ;
parameter_override : 'defparam' list_of_param_assignments ';' ;
list_of_variables : name_of_variable ( ',' name_of_variable )* ;
name_of_variable : IDENTIFIER ;
list_of_register_variables : register_decl ( ',' register_decl )* ;
register_decl : /* XXX Register initializers? */
        name_of_register ( '=' constant_expression )? |
        name_of_memory '[' constant_expression ':' constant_expression ']' ;
name_of_register : IDENTIFIER ;
name_of_memory : IDENTIFIER ;
name_of_event : IDENTIFIER ;
charge_strength :
        '(' 'small' ')' |
        '(' 'medium' ')' |
        '(' 'large' ')' ;
drive_strength :
        '(' STRENGTH0 ',' STRENGTH1 ')' |
        '(' STRENGTH1 ',' STRENGTH0 ')' ;
STRENGTH0:
        'supply0' |
        'strong0' |
        'pull0' |
        'weak0' |
        'highz0' ;
STRENGTH1:
        'supply1' |
        'strong1' |
        'pull1' |
        'weak1' |
        'highz1' ;
range : '[' constant_expression ':' constant_expression ']' ;
list_of_assignments : assignment ( ',' assignment )* ;

/* Primitive Instances */

gate_declaration : GATETYPE drive_strength? delay?
        gate_instance ( ',' gate_instance )* ';' ;
GATETYPE :
        'and' |
        'nand' |
        'or' |
        'nor' |
        'xor' |
        'xnor' |
        'buf' |
        'bufif0' |
        'bufif1' |
        'not' |
        'notif0' |
        'notif1' |
        'pulldown' |
        'pullup' |
        'nmos' |
        'rnmos' |
        'pmos' |
        'rpmos' |
        'cmos' |
        'rcmos' |
        'tran' |
        'rtran' |
        'tranif0' |
        'rtranif0' |
        'tranif1' |
        'rtranif1' ;

gate_instance : name_of_gate_instance? range? /* XXX */
        '(' terminal ( ',' terminal )* ')' ;
name_of_gate_instance : IDENTIFIER ;
UDP_instantiation : name_of_UDP drive_strength? delay?
        UDP_instance ( ',' UDP_instance )* ;
name_of_UDP : IDENTIFIER ;
UDP_instance : name_of_UDP_instance? '(' terminal ( ',' terminal )* ')' ;
name_of_UDP_instance : identifier ;
terminal : expression /* | IDENTIFIER */ ;

/* Module Instantiations */
module_instantiation : name_of_module parameter_value_assignment?
        module_instance ( ',' module_instance )* ';' ;
parameter_value_assignment :
        '#' '(' expression ( ',' expression )+ ')' |
        '#' expression ; /* XXX */
/* XXX? module arrays? */
module_instance : name_of_instance range?
        '(' list_of_module_connections? ')' ;
name_of_instance : IDENTIFIER ;
list_of_module_connections :
        module_port_connection ( ',' module_port_connection )* |
        named_port_connection ( ',' named_port_connection )* ;
module_port_connection : expression | /* NULL */ ;
named_port_connection : '.' IDENTIFIER '(' expression? ')' ;

/* Behavioral Statements */
initial_statement : 'initial' statement ;
always_statement : 'always' statement ;
statement_or_null : statement | ';' ;

statement :
        blocking_assignment ';' |
        non_blocking_assignment ';' |
        'if' '(' expression ')' statement_or_null $right 1 |
        'if' '(' expression ')' statement_or_null
            'else' statement_or_null $right 2 |
        'case' '(' expression ')' case_item+ 'endcase' |
        'casex' '(' expression ')' case_item+ 'endcase' |
        'casez' '(' expression ')' case_item+ 'endcase' |
        'forever' statement |
        'repeat' '(' expression ')' statement |
        'while' '(' expression ')' statement |
        'for' '(' assignment ';' expression ';' assignment ')' statement |
        delay_control statement_or_null |
        event_control statement_or_null |
        'wait' '(' expression ')' statement_or_null |
        '->' name_of_event ';' |
        seq_block |
        par_block |
        task_enable |
        system_task_enable |
        'disable' name_of_task_or_block ';' |
        'force' assignment ';' |
        'release' lvalue ';' |
        'assign' assignment ';' |
        'deassign' lvalue ';' ;

name_of_task_or_block : identifier ;
name_of_task : identifier ;

assignment : lvalue '=' expression ;
blocking_assignment :
        lvalue '=' expression |
        lvalue '=' delay_control expression |
        lvalue '=' event_control expression ;
non_blocking_assignment :
        lvalue '<=' expression |
        lvalue '<=' delay_control expression |
        lvalue '<=' event_control expression ;

case_item :
        expression ( ',' expression )* ':' statement_or_null |
        'default' ':'? statement_or_null ;
seq_block :
        'begin' statement* 'end' |
        'begin' ':' name_of_block block_declaration* statement* 'end' ;
par_block :
        'fork' statement* 'join' |
        'fork' ':' name_of_block block_declaration* statement* 'join' ;
name_of_block : IDENTIFIER ;
block_declaration :
        parameter_declaration |
        reg_declaration |
        integer_declaration |
        real_declaration |
        time_declaration |
        event_declaration ;
task_enable :
        name_of_task ';' |
        name_of_task '(' expression ( ',' expression )* ')' ';' ;
system_task_enable :
        system_name_of_task ';' |
        system_name_of_task '(' expression? ( ',' expression? )* ')' ';' ;
system_name_of_task : "\$[a-zA-Z_][a-zA-Z_0-9\$]*";

/* Specify Section. */  
specify_block : 'specify' specify_item* 'endspecify' ;
specify_item :
        specparam_declaration |
        path_declaration |
        level_sensitive_path_declaration |
        edge_sensitive_path_declaration |
        system_timing_check |
        sdpd ;

specparam_declaration : 'specparam' list_of_param_assignments ';' ;
path_declaration : path_description '=' path_delay_value ';' ;
path_description :
        '(' specify_input_terminal_descriptor '=>'
            specify_output_terminal_descriptor ')' |
        '(' list_of_path_inputs '*>' list_of_path_outputs ')' ;
list_of_path_inputs : specify_input_terminal_descriptor
        ( ',' specify_input_terminal_descriptor )* ;
list_of_path_outputs : specify_output_terminal_descriptor
        ( ',' specify_output_terminal_descriptor )* ;
specify_input_terminal_descriptor :
        input_identifier |
        input_identifier '[' constant_expression ']' |
        input_identifier '[' constant_expression ':' constant_expression ']' ;
specify_output_terminal_descriptor :
        output_identifier |
        output_identifier '[' constant_expression ']' |
        output_identifier '[' constant_expression ':' constant_expression ']' ;
input_identifier : IDENTIFIER ;
output_identifier : IDENTIFIER ;
path_delay_value :
        path_delay_expression |
        '(' path_delay_expression ',' path_delay_expression ')' |
        '(' path_delay_expression ',' path_delay_expression ','
            path_delay_expression ')' |
        '(' path_delay_expression ',' path_delay_expression ','
            path_delay_expression ',' path_delay_expression ','
            path_delay_expression ',' path_delay_expression ')' ;
path_delay_expression : constant_mintypmax_expression ;
constant_mintypmax_expression : mintypmax_expression ;
system_timing_check :
        '$setup' '(' timing_check_event ',' timing_check_event ','
                    timing_check_limit ( ',' notify_register )? ')' ';' |
        '$hold' '(' timing_check_event ',' timing_check_event ','
                    timing_check_limit ( ',' notify_register )? ')' ';' |
        '$period' '(' controlled_timing_check_event ',' timing_check_limit
                    (',' notify_register )? ')' ';' |
        '$width' '(' controlled_timing_check_event ',' timing_check_limit ','
                    (',' constant_expression ',' notify_register)? ')' ';' |
        '$skew' '(' timing_check_event ',' timing_check_event ','
                    timing_check_limit (',' notify_register)? ')' ';' |
        '$recovery' '(' controlled_timing_check_event ','
                        timing_check_event ',' timing_check_limit
                        ( ',' notify_register )? ')' ';' |
        '$setuphold' '(' timing_check_event ',' timing_check_event ','
                        timing_check_limit ',' timing_check_limit ','
                        ( ',' notify_register )? ')' ';' ;
timing_check_event :
        timing_check_event_control? specify_terminal_descriptor
        ( '&&&' timing_check_condition )? ;
specify_terminal_descriptor :
        specify_input_terminal_descriptor |
        specify_output_terminal_descriptor ;
controlled_timing_check_event :
        timing_check_event_control specify_terminal_descriptor
        ( '&&&' timing_check_condition )? ;
timing_check_event_control :
        'posedge' |
        'negedge' |
        edge_control_specifier ;
edge_control_specifier :
        'edge' '[' edge_descriptor ( ',' edge_descriptor )* ']' ;
edge_descriptor : '01' | '10' | '0x' | 'x1' | '1x' | 'x0' ;
timing_check_condition :
        SCALAR_EXPRESSION |
        '~' SCALAR_EXPRESSION |
        SCALAR_EXPRESSION '==' scalar_constant |
        SCALAR_EXPRESSION '===' scalar_constant |
        SCALAR_EXPRESSION '!=' scalar_constant |
        SCALAR_EXPRESSION '!==' scalar_constant ;
SCALAR_EXPRESSION : expression /* one bit */ ;
timing_check_limit : expression ;
scalar_constant : "1'[bB][01]" ;
notify_register : identifier ;
level_sensitive_path_declaration :
        'if' '(' conditional_port_expression ')'
        '(' specify_terminal_descriptor polarity_operator? '=>'
            specify_terminal_descriptor ')' '=' path_delay_value ';' |
        'if' '(' conditional_port_expression ')'
        '(' list_of_path_inputs polarity_operator? '*>'
            list_of_path_outputs ')' '=' path_delay_value ';' ;
conditional_port_expression :
        port_reference |
        UNARY_OPERATOR port_reference |
        port_reference BINARY_OPERATOR port_reference ;
polarity_operator : '+' | '-' ;
edge_sensitive_path_declaration :
        ( 'if' '(' expression ')' )?
        '(' specify_terminal_descriptor '=>'
            '(' specify_terminal_descriptor polarity_operator? ':'
                data_source_expression ')' ')' '=' path_delay_value ';' |
        ( 'if' '(' expression ')' )?
        '(' specify_terminal_descriptor '*>'
            '(' list_of_path_outputs polarity_operator? ':'
                data_source_expression ')' ')' '=' path_delay_value ';' ;
data_source_expression : expression /* XXX ? */ ;
edge_identifier : 'posedge' | 'negedge' ;
sdpd :
        'if' '(' sdpd_conditional_expression ')'
            path_description '=' path_delay_value ';' ;
sdpd_conditional_expression :
        expression BINARY_OPERATOR expression |
        UNARY_OPERATOR expression ;

    

/* Expressions */
lvalue :
        identifier |
        identifier '[' expression ']' |
        identifier '[' constant_expression ':' constant_expression ']' |
        concatenation ;
constant_expression : expression ;
mintypmax_expression : expression | expression ':' expression ':' expression ;

expression :
        primary |
        UNARY_OPERATOR primary |
        expression BINARY_OPERATOR expression |
        expression '?' expression ':' expression $right 10 |
        STRING ;

UNARY_OPERATOR :
        '+'		$unary_op_right 40 |
        '-'		$unary_op_right 40 |
        '!'		$unary_op_right 40 |
        '~'		$unary_op_right 40 |
        '&'		$unary_op_right 40 |
        '~&'	$unary_op_right 40 |
        '|'		$unary_op_right 40 |
        '~|'	$unary_op_right 40 |
        '^'		$unary_op_right 40 |
        '~^' 	$unary_op_right 40 |
        '^~' 	$unary_op_right 40 ;

BINARY_OPERATOR :
        '*'		$binary_op_left 20 |
        '/'		$binary_op_left 20 |
        '%'		$binary_op_left 20 |
        '+'		$binary_op_left 19 |
        '-'		$binary_op_left 19 |
        '>>'	$binary_op_left 18 |
        '<<'	$binary_op_left 18 |
        '<'		$binary_op_left 17 |
        '<='	$binary_op_left 17 |
        '>'		$binary_op_left 17 |
        '>='	$binary_op_left 17 |
        '=='	$binary_op_left 16 |
        '!='	$binary_op_left 16 |
        '==='	$binary_op_left 16 |
        '!=='	$binary_op_left 16 |
        '&'		$binary_op_left 15 |
        '^'		$binary_op_left 14 |
        '^~'	$binary_op_left 14 |
        '~^'	$binary_op_left 14 |
        '|'		$binary_op_left 13 |
        '&&'	$binary_op_left 12 |
        '||'	$binary_op_left 11 ;

STRING : "\"[^\"]*\"" ; /* can't handle quoted quote */

primary :
        number |
        identifier |
        identifier '[' expression ']' |
        identifier '[' constant_expression ':' constant_expression ']' |
        concatenation |
        multiple_concatenation |
        function_call |
        '(' mintypmax_expression ')' ;

number : DECIMAL_NUMBER |
         SIZE_BASE NUMBER |
         REAL_NUMBER ;

SIZE_BASE : "[0-9_]*'[BbOoDdHh]" ;
NUMBER : "[\+\-]?[0-9a-fA-FxXzZ_\?]+" ;
DECIMAL_NUMBER : "[\+\-]?[0-9_]+" ;
REAL_NUMBER :	"[\+\-]?[0-9_]+\.[0-9_]+" |
         		"[\+\-]?[0-9_]+(\.[0-9_]+)?[Ee][\+\-][0-9_]+" ;

concatenation : '{' expression ( ',' expression )* '}' ;
multiple_concatenation :
        '{' expression '{' expression ( ',' expression )* '}' '}' ;
function_call :
        name_of_function '(' (expression ( ',' expression )*)? ')' |
        name_of_system_function '(' (expression ( ',' expression )*)? ')' |
        name_of_system_function ;
name_of_function : identifier ;
name_of_system_function : "\$[a-zA-Z_][a-zA-Z0-9_\$]*" ;

/* General */
identifier : IDENTIFIER ( '.' IDENTIFIER ) * ;
IDENTIFIER :
        "\\[^ ]+ " |
        "[a-zA-Z_][a-zA-Z_0-9\$]*" $term -3	[
            if ( v_iskeyword($n0.start_loc.s, $n0.end) )
                ${reject};
         ] ;
delay : '#' number |
        '#' IDENTIFIER |
        '#' '(' mintypmax_expression ( ',' mintypmax_expression
            ( ',' mintypmax_expression ) ? ) ? ')' ;

delay_control :
        '#' number |
        '#' IDENTIFIER |
        '#' '(' mintypmax_expression ')' ;

event_control :
        '@' IDENTIFIER |
        '@' '(' event_expression ')' ;

event_expression :
        expression |
        'posedge' SCALAR_EVENT_EXPRESSION |
        'negedge' SCALAR_EVENT_EXPRESSION |
        event_expression 'or' event_expression $left -1 ;

SCALAR_EVENT_EXPRESSION :
        expression ; /* must resolve to a one bit value */

