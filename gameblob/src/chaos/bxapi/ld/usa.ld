/* FIXME: When this gets a bit fat, it might be time to split into more ld files. */
/* Also, this is parsed by a very shoddy parser. Don't worry */

/* C library */
strlen = 0x0025bcf8;
memcpy = 0x0025a55c;
memset = 0x0025a60c;
sprintf = 0x0025b630;
vsprintf = 0x0025e858;

/* bx/main */

/* this symbol is used for hooking purposes */
bxInitREAL__Fv = 0x0018a648;
bxInitREAL_HookPoint = 0x0018a7e0;

/* cGame */
Update__5cGame = 0x00189c70;
Render__5cGame = 0x001bf350;
cGame_Render_NopPoint = 0x001bf948;

/* cApplication */
TheApp = 0x002852f8;


/* bx/render */

/* cGraphicsMan */
GrMan = 0x00295e90;
Submit__12cGraphicsMan = 0x001689a8;

/* cFont (this will be fun, theres a TON of overloads) */
Text__5cFontfffPCc = 0x001a1780;

/* REALMEM */
MEM_alloc = 0x0023a448;
MEM_free = 0x0023a998;
