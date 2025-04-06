    push 1
    pop BX

    in

    call FACT:

    push BX
    out
    hlt

FACT:
    pop AX
    push AX
    push 1

    jne NEXT:

    ret

NEXT:
    push AX
    push AX
    push 1
    sub

    call FACT:
    push BX
    mul
    pop BX
    ret
