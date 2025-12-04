; test04.asm - local variables and stack pointer adjustments
; We'll use SP-relative locals. Assume SP initially set by runner; this file just tests assembly.
main:
    ldc 0
    a2sp            ; SP := A ; set SP from A (testing a2sp)
    ldc 10
    stl 0           ; store 10 at [SP + 0]
    ldc 20
    stl 1           ; store 20 at [SP + 1]
    ldl 0           ; load local 0 into A
    adc 5           ; add 5 -> 15
    stl 2           ; store result into local 2
    HALT

; data section
pad: data 0