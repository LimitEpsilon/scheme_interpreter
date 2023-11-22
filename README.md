# Scheme Interpreter

A simple Scheme repl that was implemented as a class project for the **Introduction to Data Structures** course in the Department of Electrical and Computer Engineering in Seoul National University.

This repl, in retrospect, has many undesired features, such as:
- Only a value that evaluates to a non-primitive lambda expression can be in function position. That is,
```
(define (map f l) (cond ((null? l) l) (cons (f (car l)) (map f (cdr l)))))
(map cons '((a b) (c d))) ;; error
(map (lambda (l) (cons l)) '((a b) (c d))) ;; evaluates to '(a c)
```
- The scoping is not static. When a function application occurs, the bindings for the formal parameters are overwritten in the **global environment**, and the original bindings are put in a **global stack**. After the function returns, the bindings are restored.

Nevertheless, it still works for the [examples](examples/examples.l).
