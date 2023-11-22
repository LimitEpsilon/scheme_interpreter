(define (length l) (cond ((null? l) 0) (else (+ 1 (length (cdr l]
(define (fact n) (cond ((eq? n 0) 1) (else (* n (fact (- n 1]
(define (append a b) (cond ((null? a) b) (else (cons (car a) (append (cdr a) b]
(define (reverse l) (cond ((null? l) l) (else (append (reverse (cdr l)) (cons (car l) '()]
(define (map f x) (cond ((null? x) '()) (else (cons (f (car x)) (map f (cdr x]
(define (sum n total)(cond ((eq? n 0) total) (else (sum (- n 1) (+ n total]
(define (list-ref l k) (cond ((null? l) 'error) ((equal? k 1) (car l)) (else (list-ref (cdr l) (- k 1)))))
(map (lambda (l) (car l)) '((a b) (c d)))

