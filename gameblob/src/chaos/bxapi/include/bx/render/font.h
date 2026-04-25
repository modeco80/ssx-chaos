#ifndef BX_RENDER_FONT_H
#define BX_RENDER_FONT_H


class cFont {
	void Text(float x, float y, float size, const char* string);
	void Textf(float x, float y, char *format,...);
};

#endif
