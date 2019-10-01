#include <plisp/read.h>
#include <plisp/object.h>
#include <plisp/hashtable.h>
#include <plisp/builtin.h>

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

plisp_t *plisp_read_string(FILE *f) {
    return NULL;
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
