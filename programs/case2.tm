#N = 2
#Q = {0,copy,shift,cmp,acc,acc1,acc2,acc3,accHalt,rej,rej1,rej2,rej3,rej4,rejHalt}
#F = {accHalt}
#G = {a,b,c,_,t,r,u,e,f,a,l,s}
#S = {a,b,c}
#q0 = 0
#B = _

0 a_ _a rr copy
0 b_ _b rr copy
0 c_ *_ *l shift

copy a_ _a rr copy
copy b_ _b rr copy
copy c_ *_ *l shift
copy __ __ ** rej

shift ca ** *l shift
shift cb ** *l shift
shift c_ __ rr cmp

cmp aa __ rr cmp
cmp bb __ rr cmp
cmp ab __ rr rej
cmp ba __ rr rej
cmp a_ __ rr rej
cmp b_ __ rr rej
cmp _a __ ** rej
cmp _b __ ** rej
cmp c* __ rr rej
cmp c_ __ rr rej
cmp __ __ ** acc

acc __ t_ r* acc1
acc1 __ r_ r* acc2
acc2 __ u_ r* acc3
acc3 __ e_ r* accHalt

rej *_ __ r* rej
rej ** __ r* rej
rej __ f_ r* rej1
rej1 __ a_ r* rej2
rej2 __ l_ r* rej3
rej3 __ s_ r* rej4
rej4 __ e_ r* rejHalt