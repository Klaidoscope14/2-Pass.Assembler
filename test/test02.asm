; test06.asm - Bubble sort of a small array in-place
; Array at label 'arr', length at 'n'
; Simple implementation using nested loops and swap via temporary

        ; initialize SP (optional, to use locals)
        ldc 0
        a2sp

        ; store n and pointer
        ldc n
        stl 0          ; local 0 = n
        ldc arr
        stl 1          ; local 1 = base pointer

        ; outer index i = 0 to n-2
        ldc 0
        stl 2          ; i = local 2

outer_loop:
        ldl 2
        ldl 0
        sub             ; if i >= n-1 then stop -> compute n - i
        brlz outer_done ; if negative? use different logic: if n - i - 1 <= 0
        ; compute limit = n - i - 1
        ldl 0
        ldl 2
        sub
        ldc 1
        sub
        brz outer_done   ; if zero then done

        ; inner loop: j = 0 .. n-i-2
        ldc 0
        stl 3           ; j = local 3

inner_loop:
        ldl 3
        ldl 0
        ldl 2
        add
        sub              ; compute (n - i - j - 1) ; if zero -> end
        brz inner_end

        ; compute addr_j = base + j
        ldl 1
        ldl 3
        add
        ; load element j into A via ldnl 0 (A is address)
        ldnl 0
        stl 4            ; temp1 = A
        ; compute addr_j1 = base + j + 1
        ldl 1
        ldl 3
        ldc 1
        adc 0
        ldnl 0
        stl 5            ; temp2

        ; compare temp1 and temp2: want ascending -> if temp1 <= temp2, no swap
        ldl 4
        ldl 5
        sub              ; B - A  (temp2 - temp1)
        brz no_swap
        brlz no_swap     ; if temp2 - temp1 < 0 => temp1 > temp2 => swap needed
        ; swap: write temp2 to pos j and temp1 to pos j+1
        ; store temp2 at base+j
        ldl 1
        ldl 3
        add
        ldl 5
        stnl 0

        ; store temp1 at base+j+1
        ldl 1
        ldl 3
        ldc 1
        adc 0
        ldl 4
        stnl 0

no_swap:
        ; j = j + 1
        ldl 3
        ldc 1
        adc 0
        stl 3
        br inner_loop

inner_end:
        ; i = i + 1
        ldl 2
        ldc 1
        adc 0
        stl 2
        br outer_loop

outer_done:
        HALT

; data
n:  data 6
arr:
    data 9
    data 3
    data 7
    data 1
    data 5
    data 2