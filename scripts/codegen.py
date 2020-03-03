inp = """program_struct: program_head T_SEMICOLON program_body T_DOT
program_head: T_PROGRAM T_ID T_OBRACKET idlist T_CBRACKET | T_PROGRAM T_ID
program_body: const_declarations var_declarations subprogram_declarations compound_statement
idlist: idlist T_COMMA T_ID | T_ID
const_declarations: T_CONST const_declaration T_SEMICOLON |
const_declaration: const_declaration T_SEMICOLON T_ID T_EQ const_value | T_ID T_EQ const_value
const_value: T_PLUS T_NUM | T_MINUS T_NUM | T_NUM
var_declarations: T_VAR var_declaration T_SEMICOLON |
var_declaration: var_declaration T_SEMICOLON idlist T_COLON type| idlist T_COLON type
type: T_BASIC_TYPE
subprogram_declarations: subprogram_declarations subprogram T_SEMICOLON |
subprogram: subprogram_head T_SEMICOLON subprogram_body
subprogram_head: T_PROCEDURE T_ID formal_parameter| T_FUNCTION T_ID formal_parameter T_COLON T_BASIC_TYPE
formal_parameter: T_OBRACKET parameter_list T_CBRACKET |
parameter_list:parameter_list T_SEMICOLON parameter| parameter
parameter: var_parameter | value_parameter
var_parameter: T_VAR value_parameter
value_parameter: idlist T_COLON T_BASIC_TYPE
subprogram_body: const_declarations var_declarations compound_statement
compound_statement:T_BEGIN statement_list T_END
statement_list:statement_list T_SEMICOLON statement | statement
statement: variable T_ASSIGNOP expression| procedure_call| compound_statement| T_IF expression T_THEN statement else_part| T_FOR T_ID T_ASSIGNOP expression T_TO expression T_DO statement| T_READ T_OBRACKET variable_list T_CBRACKET | T_WRITE T_OBRACKET expression_list T_CBRACKET |
variable_list: variable_list T_COMMA variable| variable
variable: T_ID
procedure_call: T_ID | T_ID T_OBRACKET expression_list T_CBRACKET
else_part: T_ELSE statement |
relop: T_CGT |T_CLT|T_CEQ|T_CNE|T_CGE|T_CLE
addop: T_PLUS | T_MINUS | T_OR
mulop: T_AND | T_MOD | T_DIV | T_MUL |
expression_list:expression_list T_COMMA expression | expression
expression:simple_expression relop simple_expression| simple_expression
simple_expression:simple_expression addop term | term
term:term mulop factor | factor
factor:T_NUM | variable| T_ID T_OBRACKET expression_list T_CBRACKET | T_OBRACKET expression T_CBRACKET | T_NOT factor"""

lines = inp.split("\n")
use_texts = ["T_BASIC_TYPE", "T_ID", "T_NUM"]


def tk_mapper(arg):
    (i, tk) = arg
    if tk.isupper():
        if tk in use_texts:
            return "        ast_node_set_attr_str(node,\"{}\",${});".format(tk[2:], i + 1)
        else:
            return ""
    else:
        return "        ast_node_attr_node_append(node,\"{}\",${});".format(tk.upper(), i + 1)


def gen_mapper(name, gen: str):
    gen = gen.strip()
    if len(gen) == 0:
        return gen + "{$$=NULL;}"
    set_attrs = "\n".join(map(tk_mapper, enumerate(gen.split(" "))))

    return gen + """
    {{
        ASTNode *node = ast_node_create_without_pos(\"{}\");
{}
        $$ = node;
    }}
    """.format(name.upper(), set_attrs)


def line_mapper(line):
    [name, gens] = line.split(":")
    return name + ":" + "|".join(map(lambda v: gen_mapper(name, v), gens.split("|")))


print("\n".join(map(line_mapper, lines)))
