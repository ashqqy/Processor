    in
    push 100
    mul
    pop AX
    in
    push 100
    mul
    pop BX
    in 
    push 100
    mul
    pop CX

    call SQUARE:

    hlt

SQUARE:

    push AX
    push 0
    je aNULL:
    call DISCRIMINANT:

    push DX
    push 0
    jb NEGATIVEDISCR:

    push DX
    push 0
    je ZERODISCR:
    push 2
    out

    push -1

    push BX
    mul
    push 100
    mul
    push DX
    sqrt
    dump
    add
    push 2
    push AX
    mul
    div
    out

    push -1
    push BX
    mul
    push 100
    mul
    push -1
    push DX
    sqrt
    mul
    add
    push 2
    push AX
    mul
    div
    out

    hlt

aNULL:

    push BX
    push 0
    je bNULL:
    push 1
    out
    push -1
    push CX
    push 100
    mul
    mul
    push BX
    div
    out
    hlt

bNULL:

    push CX
    push 0
    je cNULL:
NEGATIVEDISCR:
    push 0
    out
    hlt

cNULL:

    push 3
    out
    hlt

ZERODISCR:
    push BX
    push -1
    mul
    push 100
    mul
    push 2
    push AX
    mul
    div
    push 1
    out
    out
    hlt

    ret


DISCRIMINANT:
    push BX
    push BX
    mul
    push 4
    push AX
    mul
    push CX
    mul
    sub
    pop DX
    ret
