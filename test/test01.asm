; test07.asm - branch behavior tests
start:
    ldc 0
    brz is_zero      ; if A == 0
    br not_zero
is_zero:
    ldc 10
    br done
not_zero:
    ldc 20
    br done
done:
    ldc -5
    brlz is_negative
    HALT
is_negative:
    ldc 99
    HALT