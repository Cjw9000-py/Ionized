import re
import string

# constants
NUMBERS = string.digits
LETTERS = string.ascii_letters
IDENT_START_CHARS = LETTERS + '_'
IDENT_CHARS = LETTERS + NUMBERS + '_'
WHITESPACE = ' \t\r'
LINE_COMMENT_PREFIX = '#'


# utils
def literal(sub: str):
    return r'^' + re.escape(sub)


def regex(p: str):
    return r'^' + p

def keyword(k: str):
    return r'^' + re.escape(k) + r'[^\w]'

# regex helpers
def group(*args):
    return '|'.join(args)

def j(*args):
    return ''.join(args)

def p(x):
    return '(?:' + x + ')'

def escapes(*args):
    res = list()
    for v in args:
        res.append(p(v))
    return group(*res)


# regex patterns
p_number = r'[0-9](?:[0-9_]*[0-9])?'
p_expo = j(r'(?:[Ee][\\+-]?', p_number, r')?')
p_no_number = j('(', p_number, ')?')
p_string_prefix = r'(?:r|b)?'

p_string_escapes = escapes(
    r'\\\\',  # backslash
    r'\\n',  # newline
    r'\\t',  # tab
    r'\\r',  # carriage return
    r'\\b',  # backspace
    r'\\f',  # form feed
    r'\\v',  # vertical tab
    r'\\a',  # bell
    r'\\[xX][0-9a-fA-F]{2}',  # hex
    r'\\[uU][0-9a-fA-F]{4}',  # unicode
    r'\\[0-7]{1,3}',  # octal
)

p_string_single = j("'(?:", p_string_escapes, r"|(?:\\')|[^'\\])*'")
p_string_double = j('"(?:', p_string_escapes, r'|(?:\\")|[^"\\])*"')
p_string = j(p_string_prefix, group(p(p_string_single), p(p_string_double)))

HANDLERS = {
    # comments
    'TOK_LINE_COMMENT': regex(r'#[^\n]*'),
    'TOK_COMMENT': regex(r'\/\*(?:.|\n)*?\*\/'),

    # literals
    'TOK_HEX_NUM': regex(r'0[Xx][0-9A-Fa-f]([0-9A-Fa-f_]*[0-9A-Fa-f])?'),
    'TOK_BIN_NUM': regex(r'0[Bb][01]([01_]*[01])?'),
    'TOK_DECIMAL': regex(group(j('(', p_no_number, r'\.', p_number, p_expo, ')'), j(p_number, r'\.', p_expo))),
    'TOK_NUMBER': regex(j(p_number, p_expo)),
    'TOK_STRING': regex(p_string),
    'TOK_TRUE': keyword('true'),
    'TOK_FALSE': keyword('false'),
    'TOK_NULL': keyword('null'),

    # special
    'TOK_EOL': regex(r'\n'),
    'TOK_ELLIPSIS': literal('...'),
    'TOK_DDOT': literal('..'),
    'TOK_DOT': literal('.'),
    'TOK_COMMA': literal(','),
    'TOK_COLON': literal(':'),
    'TOK_SEMICOLON': literal(';'),
    'TOK_QUEST': literal('?'),
    'TOK_AT': literal('@'),

    # special assigns
    'TOK_POW_ASSIGN': literal('**='),
    'TOK_FLOORDIV_ASSIGN': literal('//='),
    'TOK_ADD_ASSIGN': literal('+='),
    'TOK_SUB_ASSIGN': literal('-='),
    'TOK_MUL_ASSIGN': literal('*='),
    'TOK_DIV_ASSIGN': literal('/='),
    'TOK_MOD_ASSIGN': literal('%='),
    'TOK_BOR_ASSIGN': literal('|='),
    'TOK_BAND_ASSIGN': literal('&='),
    'TOK_XOR_ASSIGN': literal('^='),
    'TOK_LSHIFT_ASSIGN': literal('<<='),
    'TOK_RSHIFT_ASSIGN': literal('>>='),

    # bin op
    'TOK_LSHIFT': literal('<<'),
    'TOK_RSHIFT': literal('>>'),
    'TOK_POW': literal('**'),
    'TOK_FLOORDIV': literal('//'),
    'TOK_INC': literal('++'),
    'TOK_DEC': literal('--'),
    'TOK_ADD': literal('+'),
    'TOK_SUB': literal('-'),
    'TOK_MUL': literal('*'),
    'TOK_DIV': literal('/'),
    'TOK_XOR': literal('^'),
    'TOK_NEG': literal('~'),
    'TOK_MOD': literal('%'),
    'TOK_BOR': literal('|'),
    'TOK_BAND': literal('&'),

    # bool op
    'TOK_EQ': literal('=='),
    'TOK_NE': literal('!='),
    'TOK_LE': literal('<='),
    'TOK_GE': literal('>='),
    'TOK_LT': literal('<'),
    'TOK_GT': literal('>'),
    'TOK_NOT': keyword('not'),
    'TOK_OR': keyword('or'),
    'TOK_AND': keyword('and'),

    # assign
    'TOK_ASSIGN': literal('='),

    # parens
    'TOK_LPAREN': literal('('),
    'TOK_RPAREN': literal(')'),
    'TOK_LBRACKET': literal('['),
    'TOK_RBRACKET': literal(']'),
    'TOK_LBRACE': literal('{'),
    'TOK_RBRACE': literal('}'),

    # keywords
    'TOK_IF': keyword('if'),
    'TOK_ELIF': keyword('elif'),
    'TOK_ELSE': keyword('else'),
    'TOK_WHILE': keyword('while'),
    'TOK_FOR': keyword('for'),
    'TOK_NEW': keyword('new'),
    'TOK_FROM': keyword('from'),
    'TOK_IN': keyword('in'),
    'TOK_IS': keyword('is'),
    'TOK_AS': keyword('as'),
    'TOK_WITH': keyword('with'),
    'TOK_BREAK': keyword('break'),
    'TOK_CONTINUE': keyword('continue'),
    'TOK_RETURN': keyword('return'),
    'TOK_DO': keyword('do'),
    'TOK_TRY': keyword('try'),
    'TOK_EXCEPT': keyword('except'),
    'TOK_FINALLY': keyword('finally'),
    'TOK_RAISE': keyword('raise'),
    'TOK_SWITCH': keyword('switch'),
    'TOK_CASE': keyword('case'),
    'TOK_DEFAULT': keyword('default'),
    'TOK_STRUCT': keyword('struct'),
    'TOK_CLASS': keyword('class'),
    'TOK_ENUM': keyword('enum'),
    'TOK_CONST': keyword('const'),
    'TOK_STATIC': keyword('static'),
    'TOK_ABSTRACT': keyword('abstract'),

    'TOK_IDENT': regex(r'[a-zA-Z_][a-zA-Z0-9_]*'),
}


# parse into a flat string

def parse_flat(tokens: dict):
    res = ''
    for i, (name, value) in enumerate(tokens.items()):
        res += str(i) + ':' + name + '=' + value + '\n'
    return res

open('regex.txt', 'w').write(parse_flat(HANDLERS))