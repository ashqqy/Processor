push 0
pop AX
push 0
pop BX
push 0
pop CX

BEBRA:

push 1 + CX
pop CX

push 65 + BX
pop [AX]

push 1 + AX
pop AX

push 1 + BX
pop BX

push 99
push CX

jae BEBRA:

dump
draw

hlt