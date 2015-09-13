;; Helper to calculate length of list x
(define length-helper
  (lambda (x accu)
	(cond ((= nil x) accu)
		  (t (length-helper (cdr x) (+ 1 accu))))))

;; Returns length of list x
(define length
  (lambda (x)
	(length-helper x 0)))
				 
(define range
  (lambda (beg end)
	(cond ((>= beg end) end)
		  (t (cons beg (range (+ 1 beg) end))))))

(define reverse-helper
	(lambda (list acc)
	  (cond ((= nil list) acc)
			(t (reverse-helper (cdr list) (cons (car list) acc))))))

(define reverse (lambda (list)
  (reverse-helper list nil)))



;; Test length				
(length (range 15 20))

