# Scheme Interpreter

A simple Scheme repl that was implemented as a class project for the **Introduction to Data Structures** course in the Department of Electrical and Computer Engineering in Seoul National University.

This repl, in retrospect, has many undesired features, such as:
- The global heap does not perform doubling.
- The scoping is not static. When a function application occurs, the bindings for the formal parameters are overwritten in the **global environment**, and the original bindings are put in a **global stack**. After the function returns, the bindings are restored.

Nevertheless, it still works for the [examples](examples/examples.l).
