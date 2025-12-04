; test02.asm - contains many errors to test diagnostics
label:
label:             ; duplicate label definition
    br nonesuch    ; undefined label
    ldc 08ge       ; malformed number ('08ge')
    ldc            ; missing operand
    add 5          ; unexpected operand to 'add'
    ldc 5, 6       ; extra tokens after operand
0bad:              ; illegal label (starts with digit)
fibble             ; unknown mnemonic
0def               ; bogus mnemonic (starts with digit)