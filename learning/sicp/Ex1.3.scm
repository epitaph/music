(define (writeln s) (write s) (newline))

(define (least2 x y) (if (< x y) x y))
(define (least3 x y z) (least2 x (least2 y z)))
(define (sq x) (* x x))
(define (sosq2-from-3 a b c) (- (+ (sq a) (sq b) (sq c)) (sq (least3 a b c))))

(writeln (least3 1 2 3))
(writeln (least3 6 5 4))
(writeln (least3 9 7 8))

(newline)

(writeln (sosq2-from-3 1 2 3))

