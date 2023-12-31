#N = 3
#Q = {0,cp1,cp2,shift,write,shift2,illegalShiftRight,illegalShiftLeft,i0,i1,i2,i3,i4,i5,i6,i7,i8,i9,i10,i11,i12,i13,unacceptHalt}
#F = {write}
#G = {a,b,c,_,I,l,e,g,i,n,p,u,t}
#S = {a,b}
#q0 = 0
#B = _

0 a__ *a_ rr* cp1
0 b__ *ab *** illegalShiftRight

cp1 a__ *a_ rr* cp1
cp1 b__ *_b r*r cp2
cp1 ___ _ab *** illegalShiftRight

cp2 b__ *_b r*r cp2
cp2 ___ *** lll shift
cp2 a__ *ab *** illegalShiftRight

shift *** _** l** shift
shift _** _** r** write

write _ab c** r*l write
write _a_ _*_ *lr shift2
shift2 _*b *** **r shift2
shift2 _*_ *** **l write



illegalShiftRight *** *** r** illegalShiftRight
illegalShiftRight _** _** l** illegalShiftLeft
illegalShiftLeft *** _** l** illegalShiftLeft
illegalShiftLeft _** _** r** i0
i0 _** I** r** i1
i1 _** l** r** i2
i2 _** l** r** i3
i3 _** e** r** i5
i5 _** g** r** i6
i6 _** a** r** i7
i7 _** l** r** i8
i8 _** _** r** i9
i9 _** i** r** i10
i10 _** n** r** i11
i11 _** p** r** i12
i12 _** u** r** i13
i13 _** t** r** unacceptHalt
