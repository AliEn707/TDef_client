/*
buttons
{.(46)}{@(64)}{-(45)}{_(95)}{_(95)}{:(58)}{?(63)} {*(42)}{((40)}{)(41)} 
[1(49)][2(50)][3(51)][4(52)][5(53)][6(54)][7(55)][8(56)][9(57)][0(48)][bs(8)]

{Q(81)}{W(87)}{E(69)}{R(82)}{T(84)}{Y(89)}{U(85)} {I(73)}{O(79)}{P(80)} 
[q(113)][w(119)][e(101)][r(114)][t(116)][y(121)][u(117)][i(105)][o(111)][p(112)][enter(13)]

{A(65)}{S(83)}{D(68)}{F(70)}{G(71)}{H(72)}{J(74)}{K(75)}{L(76)} 
[a(97)][s(115)][d(100)][f(102)][g(103)][h(104)][j(106)][k(107)][l(108)][shift   (0)]

{Z(90)}{X(88)}{C(67)}{V(86)}{B(66)}{N(78)}{M(77)} 
[z(122)][x(120)][c(99)][v(118)][b(98)][n(110)][m(109)][<(60)][>(62)][space   (32)]

button size 46
between buttons 10
*/
void textShow();

void textAdd(char * text1,char * text2, int pos);

void textRemChr(char * dest, int pos);

void textHide(char * text);

void textSetCur(char* text, int cur);
void textRemCur(char* text);

void textStop();

void textMove(int i);
//actions

void actionKeyboard(void * arg);

void actionTextTest(void * arg);

void actionTextLogin(void * arg);

void actionTextPass(void * arg);
