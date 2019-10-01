# plisp

plisp is a scheme-like lisp interpreter (and hopefully soon compiler).

## useage

```
make
rlwrap ./plisp
```

## plisp objects

### integers

integers are signed 64bit integers.

only base 10, positive literals are currently supported.

integers are not `eq?` but are `eqv?`

literals:

```
0 1 1000 55
```

### booleans

booleans are either `#t` or `#f`.

booleans are `eq?`

literals:

```
#f #t
```

### symbols

symbols cannot contain the characters `'('` and `')'` and cannot start with a digit.

### cons cells

cons cells are a pair of `car` and `cdr`.

example cons cell literal:

```
> '(1 . 2)
(1 . 2)
```

they are used to represent lists:

```
> '(1 . (2 . (3 . (4 . ()))))
(1 2 3 4)
```
