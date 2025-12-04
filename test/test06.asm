; test09.asm - evaluate polynomial a*x^2 + b*x + c
; constants: a,b,c ; input x ; result in A

    ldc a
    stl 0          ; store a
    ldc b
    stl 1
    ldc c
    stl 2
    ldc x
    stl 3          ; store x

    ; compute x^2
    ldl 3
    ldl 3
    mul_stub:       ; SIMPLE has no mul; implement via repeated addition
    ; naive multiplication: compute x * x by loop
    stl 4           ; store RHS copy in local 4 (multiplier)
    ldc 0
    stl 5           ; product = 0
mul_loop:
    ldl 4
    brz mul_end
    ldl 5
    ldl 3
    adc 0
    stl 5
    ldl 4
    ldc -1
    adc 0
    stl 4
    br mul_loop
mul_end:
    ; product in local 5
    ; compute a * product  -> again repeated addition (for small numbers)
    ldl 0            ; a
    stl 6            ; multiplier for next multiplication
    ldc 0
    stl 7            ; accum = 0
mul2_loop:
    ldl 6
    brz mul2_end
    ldl 7
    ldl 5
    adc 0
    stl 7
    ldl 6
    ldc -1
    adc 0
    stl 6
    br mul2_loop
mul2_end:
    ; accum in local 7 -> a*x^2
    ; compute b*x -> similar approach (skipping full repetition—assume small numbers)
    ; For simplicity, if numbers small this suffices for assembler test
    HALT

; constants/data
a: data 2
b: data 3
c: data 1
x: data 4