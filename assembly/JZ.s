.org 0

    ld a,5
    ld b,1

loop:
    sub a,b
    jp nz,loop
    halt
