push 1
pop BX

in

call FACT:

push BX
mul
out
hlt

FACT:
dump
pop AX
push AX
push AX
push 1

jne NEXT:
ret

NEXT:
pop AX
push AX
push AX
push 1
sub

call FACT:

push BX
mul
pop BX
ret
