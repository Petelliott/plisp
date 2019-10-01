# plisp

plisp is a scheme-like lisp interpreter (and hopefully soon compiler), written in C.

It uses the [Boehm-Demers-Weiser](https://www.hboehm.info/gc/) garbage
collector and will use gnu lightning.

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

## special forms

### quote

quote returns it's argument unevaluated.

the reader will interpret `'<object>` and `(quote <object>)`

examples:

```
> 'a
a
> '(1 2 3)
(1 2 3)
> '''a
(quote (quote a))
```

### if

if evaluates and returns it's second argument if it's first argument is not
`#f` otherwise it will evaluate and return it's third argument.

examples:

```
> (if 1 2 3)
2
> (if #f 2 3)
3
> (if #f (write 'a) (write 'b))
b
```

### define

define creates a new variable in it's current scope, even if that variable already exists

examples:

```
> (define a 5)
5
> a
5
> (define b (lambda () a))
#<interpreted closure>
> (define a 8)
8
> a
8
> (b)
5
```

### set!

`set!` changes the value of a variable

```
> (define a 5)
5
> (define b (lambda () a))
#<interpreted closure>
> (define c (lambda () (set! a 19)))
#<interpreted closure>
> (define a 8)
8
> (b)
5
> (c)
19
> (b)
19
> a
5
```

### lambda

`lambda` creates a new closure.

examples

```
> (define curry+ (lambda (a) (lambda (b) (+ a b))))
#<interpreted closure>
> ((curry+ 5) 6)
11
```
