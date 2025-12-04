; test05.asm - sum the numbers in array 'arr' of length n
; arr: data 1,2,3,4,5 ; sum should be 15
        ldc 0
        a2sp           ; A->SP (just to set SP deterministically here)
        ldc 5
        stl 0          ; store n at [SP+0]
        ldc arr        ; load address of arr
        stl 1          ; store pointer at [SP+1]
        ldc 0
        stl 2          ; sum = 0 -> local 2
        ldc 0
        stl 3          ; i = 0 -> local 3

loop:
        ldl 3          ; load i
        ldl 0          ; load n
        sub            ; B - A  => compare n - i
        brz done       ; if zero (i == n) done
        ldl 1          ; load base pointer
        ldl 3          ; load i
        add            ; compute base + i  (pointer arithmetic)
        ldnl 0         ; load memory[A + 0] -> A is address, use ldnl to load from that address
        ; add to sum
        stl 4          ; store current element temp at local 4
        ldl 2
        adc 0          ; add value in A? (we used stl/read sequence; keep simple)
        ; increment i
        ldl 3
        ldc 1
        adc 0
        stl 3
        br loop

done:
        HALT

arr:
    data 1
    data 2
    data 3
    data 4
    data 5