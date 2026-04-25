/* FIXME: When this gets a bit fat, it might be time to split into more ld files. */

/* C library */
strlen = 0x0025bcf8;
memcpy = 0x0025a55c;
memset = 0x0025a60c;

/* bx/main */

/* this symbol is used for hooking purposes */
bxInitREAL__Fv = 0x0018a648;

/* cGame */
Update__5cGame = 0x00189c70;
Render__5cGame = 0x001bf350;
/* FIXME: There's a good chance this could be an offset, and we just take
  reinterpret_cast<u32*>(bitcast<u8*>(&cGame::Update) + nopPoint) ? Would make it
  easier to port I suppose, if a bit unwieldy to use */
cGame_Render_NopPoint = 0x001bf948;


/* bx/render */

/* cGraphicsMan */
GrMan = 0x00295e90;
Submit__12cGraphicsMan = 0x001689a8;

/* REALMEM */
MEM_alloc = 0x0023a448;
MEM_free = 0x0023a998;
