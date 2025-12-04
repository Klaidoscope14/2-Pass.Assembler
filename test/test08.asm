; test01.asm - simple arithmetic, no branches
start:
    ldc 5          ; A := 5
    adc 7          ; A := A + 7  (A = 12)
    adc -2         ; A := 10
    HALT