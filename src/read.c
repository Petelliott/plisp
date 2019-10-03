#include <plisp/read.h>
#include <plisp/object.h>
#include <plisp/hashtable.h>
#include <plisp/builtin.h>
#include <plisp/gc.h>

#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>

plisp_t *intern_table = NULL;

plisp_t *fsym = NULL;
plisp_t *tsym = NULL;

void plisp_init_reader(void) {
    intern_table = plisp_make_hashtable(HT_EQUAL);
    fsym = make_interned_symbol("f");
    tsym = make_interned_symbol("t");
}

plisp_t *plisp_read_list(FILE *f) {
    int ch;
    do {
        ch = fgetc(f);
    } while (isspace(ch));

    if (ch == EOF) {
        fprintf(stderr, "error while parsing list: unexpected EOF\n");
        return NULL;
    }

    if (ch == ')') {
        return plisp_make_nil();
    } else if (ch == '.') {
        plisp_t *cdr = plisp_read(f);
        do {
            ch = fgetc(f);
        } while (isspace(ch));

        if (ch != ')') {
            if (ch == EOF) {
                fprintf(stderr, "error while parsing cons cell: expected ')', got EOF\n");
            } else {
                fprintf(stderr, "error while parsing cons cell: expected ')', got '%c'\n", ch);
            }
            return NULL;
        }

        return cdr;
    } else {
        ungetc(ch, f);
        plisp_t *car = plisp_read(f);
        plisp_t *cdr = plisp_read_list(f);

        return plisp_make_cons(car, cdr);
    }
}

static int hexdig(char dig) {
    if (dig >= 'A' && dig <= 'F') {
        return 0xa + (dig-'A');
    } else if (dig >= 'a' && dig <= 'f') {
        return 0xA + (dig-'a');
    } else if (dig >= '0' && dig <= '9') {
        return dig - '0';
    }
    return -1;
}
static char read_escape(FILE *f) {
    char ch = fgetc(f);
    if(ch == 'n') {
        return '\n';
    } else if (ch == 't') {
        return '\t';
    } else if (ch == 'r') {
        return '\r';
    } else if (ch == 'e') {
        return '\e';
    } else if (ch == '\\') {
        return '\\';
    } else if (isdigit(ch)) {
        return (ch-'0')*64 + (fgetc(f)-'0')*8 + (fgetc(f)-'0');
    } else if (isdigit(ch)) {
        return (ch-'0')*64 + (fgetc(f)-'0')*8 + (fgetc(f)-'0');
    } else if (ch=='x') {
        return hexdig(fgetc(f))*16 + hexdig(fgetc(f));
    } else {
        return ch;
    }
}

const size_t STR_INIT_CAP = 16;

plisp_t *plisp_read_string(FILE *f) {
    char ch;
    char *str = plisp_alloc_atomic(STR_INIT_CAP);
    size_t len = 0;
    size_t cap = STR_INIT_CAP;

    while ((ch = fgetc(f)) != '"') {
        if (ch == EOF) {
            fprintf(stderr, "error while parsing string: unexpected EOF\n");
            return NULL;
        }
        if (ch == '\\') {
            ch = read_escape(f);
        }
        str[len] = ch;
        len++;
        if (len+1 == cap) {
            cap = (cap * 5) / 4;
            str = plisp_realloc(str, cap);
        }
    }
    str[len] = 0;
    return plisp_make_string_owned(str, len);
}

bool symchar(int ch) {
    return !isspace(ch) && ch != EOF && ch != ')' &&
           ch != '(' && ch != '"';
}

plisp_t *make_interned_symbol(const char *text) {
    plisp_t *sym = plisp_make_symbol(text);

    plisp_t *table_sym = plisp_hashtable_find(intern_table, sym);
    if (table_sym == NULL) {
        plisp_hashtable_insert(intern_table, sym, sym);
        return sym;
    } else {
        return table_sym;
    }
}

plisp_t *plisp_read_symbol(FILE *f) {
    //TODO: unbounded symbols
    char text[128];
    size_t off = 0;

    char ch;
    while (symchar(ch = fgetc(f))) {
        text[off] = ch;
        off++;
    }
    ungetc(ch, f);

    text[off] = '\0';
    return make_interned_symbol(text);
}

plisp_t *plisp_read_number(FILE *f) {
    //TODO: floats
    long num = 0;

    int ch;
    while (true) {
        ch = fgetc(f);
        if (isdigit(ch)) {
            num = num * 10 + (ch - '0');
        } else {
            ungetc(ch, f);
            break;
        }
    }

    return plisp_make_int(num);
}

plisp_t *plisp_read_call(FILE *f, const char *sym) {
    plisp_t *body = plisp_read(f);
    return plisp_make_cons(
        make_interned_symbol(sym),
        plisp_make_cons(
            body,
            plisp_make_nil()));
}

plisp_t *plisp_read_hash(FILE *f) {
    plisp_t *sym = plisp_read_symbol(f);

    if (plisp_c_eq(sym, fsym)) {
        return plisp_make_bool(false);
    } else if (plisp_c_eq(sym, tsym)) {
        return plisp_make_bool(true);
    }

    return NULL;
}

plisp_t *plisp_read(FILE *f) {
    int ch;
    do {
        ch = fgetc(f);
    } while (isspace(ch));

    if (ch == EOF) {
        return NULL;
    }

    if (ch == '(') {
        return plisp_read_list(f);
    } else if (ch == '"') {
        return plisp_read_string(f);
    } else if (ch == '\'') {
        return plisp_read_call(f, "quote");
    } else if (ch == '`') {
        return plisp_read_call(f, "quasiquote");
    } else if (ch == ',') {
        return plisp_read_call(f, "unquote");
    } else if (ch == '#') {
        return plisp_read_hash(f);
    } else if (isdigit(ch)) {
        ungetc(ch, f);
        return plisp_read_number(f);
    } else {
        ungetc(ch, f);
        return plisp_read_symbol(f);
    }

    return NULL;
}
