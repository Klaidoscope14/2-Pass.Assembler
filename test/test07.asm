; test03.asm - testing SET and forward references
val: SET 100       ; set label 'val' to 100
start:
    ldc val        ; load address/value stored in 'val'
    adc 23
    ldc val2       ; forward ref label used before defined
    HALT

val2: SET 66