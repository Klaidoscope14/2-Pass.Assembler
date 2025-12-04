; test10.asm - compute mean of 5 numbers, and give rough median via simple selection
; This file focuses on assembler features (arrays, loops, comparisons)

    ldc n
    stl 0        ; n
    ldc arr
    stl 1        ; base pointer
    ldc 0
    stl 2        ; sum = 0
    ldc 0
    stl 3        ; i = 0

sum_loop:
    ldl 3
    ldl 0
    sub
    brz sum_done
    ldl 1
    ldl 3
    add
    ldnl 0
    stl 4        ; temp
    ldl 2
    ldl 4
    adc 0
    stl 2
    ldl 3
    ldc 1
    adc 0
    stl 3
    br sum_loop

sum_done:
    ; compute mean = sum / n  -- no divide, use repeated subtraction to approximate
    ldl 2
    stl 5        ; dividend
    ldc 0
    stl 6        ; quotient
div_loop:
    ldl 5
    ldl 0
    sub
    brlz div_done
    ldl 5
    ldl 0
    sub
    stl 5
    ldl 6
    ldc 1
    adc 0
    stl 6
    br div_loop
div_done:
    ; mean in local 6

    ; median (simple selection: find middle by scanning—approximate)
    ; For 5 elements, pick element at index 2 after a naive selection (not full sort)
    ldc 2
    stl 7        ; target index
    ldc 0
    stl 8        ; i
median_loop:
    ldl 8
    ldl 1
    ldl 7
    add
    ldnl 0
    ; (selector omitted) -> this is a placeholder to ensure assembler tests indexing
    ldl 8
    ldc 1
    adc 0
    stl 8
    ldl 8
    ldl 0
    sub
    brz median_done
    br median_loop

median_done:
    HALT

n: data 5
arr:
 data 7
 data 2
 data 9
 data 1
 data 4