;; Test define
(define x 10)

;; Test basic lambda
(define plus (lambda (x y) (+ x y)))

;; Test basic recursion
(define countdown
  (lambda (n)
	(cond ((= n 0) n)
		  (t (countdown(- n 1))))))

;; Test lambda capture
(define count (lambda (n) (lambda () (set n (- n 1)))))
(define count-3 (count 3)) 
(define count-4 (count 4))
 





