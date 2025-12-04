; test08.asm - call and return example
; main calls add_one subroutine which increments A by 1 and returns

    ldc 5
    call do_inc     ; call subroutine (offset to label)
    HALT

do_inc:
    ; on entry: B := A ; A := PC ; PC := PC + offset (per spec)
    ; But as conventional subroutine, we simply add 1 and return
    ldc 1
    adc 0
    return