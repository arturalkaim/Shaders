#lang racket/base
(require ffi/unsafe
         ffi/unsafe/define)

(define-ffi-definer define-test (ffi-lib "/Users/arturalkaim/Documents/Shaders/libShaders"))

(define-test init (_fun -> _int))
(define-test createPoints (_fun _int -> _int))
(define-test box (_fun _float _float _float _float _float _float -> _int))
(define-test cylinder (_fun _float _float _float _float _float -> _int))
(define-test start (_fun _int -> _int))
(define-test city (_fun _int -> _void))


#;
(define (building x y z w l h)
  (begin
    (box x y z w l (* 0.7 h))
    (cylinder x y h (* 0.2 w) h)
    )
  )

(start 1000)

;(let ([grid-size 5.0])
;  (for* ([xi  (in-range (- grid-size) grid-size 0.3)]
;         [yi  (in-range (- grid-size) grid-size 0.3)])
;    (building xi yi 0.0 0.1 0.1 0.4)))

(city 200)
;(cylinder 0.0 0.0 0.2 0.2)
;(box -0.4 0.4 0.2 0.2)
;(box 1.0 0.0 0.2 0.2)
;(box 0.2 0.4 0.2 0.2)
;(box -0.4 0.4 0.2 0.2)
(init)