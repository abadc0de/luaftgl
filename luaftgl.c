#include <stdlib.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <FTGL/ftgl.h>

#define ustring const unsigned char *

/* work around typo in FTGL */
#ifndef ftglGetLayoutAlignment
  #define ftglGetLayoutAlignment ftglGetLayoutAlignement
#endif

#define LUA_Tlightuserdata LUA_TLIGHTUSERDATA
#define LUA_Tstring LUA_TSTRING
#define LUA_Tnumber LUA_TNUMBER
#define lua_tolightuserdata lua_touserdata
#define lua_pushvoid(L, x) x; lua_pushnil(L)

#define LUA_FUNC_0(name, tr) \
int lf_ ## name (lua_State *L) { \
    lua_push ## tr(L, name()); \
    return 1; \
}

#define LUA_FUNC_1(name, t1, tr) \
int lf_ ## name (lua_State *L) { \
    luaL_checktype(L, 1, LUA_T ## t1); \
    lua_push ## tr(L, name(lua_to ## t1(L, 1))); \
    return 1; \
}

#define LUA_FUNC_2(name, t1, t2, tr) \
int lf_ ## name (lua_State *L) { \
    luaL_checktype(L, 1, LUA_T ## t1); \
    luaL_checktype(L, 2, LUA_T ## t2); \
    lua_push ## tr(L, name(lua_to ## t1(L, 1), lua_to ## t2(L, 2))); \
    return 1; \
}

#define LUA_FUNC_3(name, t1, t2, t3, tr) \
int lf_ ## name (lua_State *L) { \
    luaL_checktype(L, 1, LUA_T ## t1); \
    luaL_checktype(L, 2, LUA_T ## t2); \
    luaL_checktype(L, 3, LUA_T ## t3); \
    lua_push ## tr(L, name(lua_to ## t1(L, 1), lua_to ## t2(L, 2), \
        lua_to ## t3(L, 3))); \
    return 1; \
}

#define PREFIXED_CONST(name, prefix) \
    lua_pushstring(L, #name); \
    lua_pushnumber(L, prefix ## _ ## name); \
    lua_rawset(L, -3);

#define FT_CONST(name) PREFIXED_CONST(name, FT)
#define FTGL_CONST(name) PREFIXED_CONST(name, FTGL)

LUA_FUNC_1(ftglDestroyFont, lightuserdata, void)
LUA_FUNC_2(ftglAttachFile, lightuserdata, string, number)

/*
int ftglAttachData(FTGLfont* font, const unsigned char * data,
                               size_t size);
*/
int lf_ftglAttachData(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    luaL_checktype(L, 2, LUA_TSTRING);
    size_t size;
    ustring data = (ustring)lua_tolstring(L, 2, &size);
    lua_pushnumber(L, ftglAttachData(
        lua_touserdata(L, 1), data, size));
    return 1;
}

LUA_FUNC_2(ftglSetFontCharMap, lightuserdata, number, number)
LUA_FUNC_1(ftglGetFontCharMapCount, lightuserdata, number)

/*
FT_Encoding* ftglGetFontCharMapList(FTGLfont* font)
*/
int lf_ftglGetFontCharMapList(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    FTGLfont *font;
    FT_Encoding *charMapList;
    font = lua_touserdata(L, 1);
    unsigned int charMapCount = ftglGetFontCharMapCount(font);
    charMapList = ftglGetFontCharMapList(font);
    lua_newtable(L);
    for (int i = 0; i < charMapCount; i++) {
        lua_pushnumber(L, charMapList[i]);
        lua_rawseti(L, -2, i + 1);
    }
    return 1;
}

LUA_FUNC_3(ftglSetFontFaceSize, lightuserdata, number, number, number)
LUA_FUNC_1(ftglGetFontFaceSize, lightuserdata, number)
LUA_FUNC_2(ftglSetFontDepth, lightuserdata, number, void)
LUA_FUNC_3(ftglSetFontOutset, lightuserdata, number, number, void)
LUA_FUNC_2(ftglSetFontDisplayList, lightuserdata, number, void)
LUA_FUNC_1(ftglGetFontAscender, lightuserdata, number)
LUA_FUNC_1(ftglGetFontDescender, lightuserdata, number)
LUA_FUNC_1(ftglGetFontLineHeight, lightuserdata, number)

/*
void ftglGetFontBBox(FTGLfont* font, const char *string,
                                 int len, float bounds[6]);
*/
int lf_ftglGetFontBBox(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    luaL_checktype(L, 2, LUA_TSTRING);
    size_t len;
    const char *a2 = lua_tolstring(L, 2, &len);
    float bounds[6];
    ftglGetFontBBox(lua_touserdata(L, 1), a2, len, bounds);
    lua_newtable(L);
    for (int i = 0; i < 6; i++) {
        lua_pushnumber(L, bounds[i]);
        lua_rawseti(L, -2, i + 1);
    }
    return 1;
}

LUA_FUNC_2(ftglGetFontAdvance, lightuserdata, string, number)
LUA_FUNC_3(ftglRenderFont, lightuserdata, string, number, void)
LUA_FUNC_1(ftglGetFontError, lightuserdata, number)
LUA_FUNC_1(ftglCreateBitmapFont, string, lightuserdata)
LUA_FUNC_1(ftglCreateBufferFont, string, lightuserdata)
LUA_FUNC_1(ftglCreateExtrudeFont, string, lightuserdata)
LUA_FUNC_1(ftglCreateOutlineFont, string, lightuserdata)
LUA_FUNC_1(ftglCreatePixmapFont, string, lightuserdata)
LUA_FUNC_1(ftglCreatePolygonFont, string, lightuserdata)
LUA_FUNC_1(ftglCreateTextureFont, string, lightuserdata)
LUA_FUNC_1(ftglDestroyLayout, lightuserdata, void)

/* 
void ftglGetLayoutBBox(FTGLlayout *layout, const char* string,
                                   float bounds[6]);
*/
int lf_ftglGetLayoutBBox(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    luaL_checktype(L, 2, LUA_TSTRING);
    FTGLlayout *a1 = lua_touserdata(L, 1);
    const char *a2 = lua_tostring(L, 2);
    float bounds[6];
    ftglGetLayoutBBox(a1, a2, bounds);
    lua_newtable(L);
    for (int i = 0; i < 6; i++) {
        lua_pushnumber(L, bounds[i]);
        lua_rawseti(L, -2, i + 1);
    }
    return 1;
}

LUA_FUNC_3(ftglRenderLayout, lightuserdata, string, number, void)
LUA_FUNC_1(ftglGetLayoutError, lightuserdata, number)
LUA_FUNC_0(ftglCreateSimpleLayout, lightuserdata)
LUA_FUNC_2(ftglSetLayoutFont, lightuserdata, lightuserdata, void)
LUA_FUNC_1(ftglGetLayoutFont, lightuserdata, lightuserdata)
LUA_FUNC_2(ftglSetLayoutLineLength, lightuserdata, number, void)
LUA_FUNC_1(ftglGetLayoutLineLength, lightuserdata, number)
LUA_FUNC_2(ftglSetLayoutAlignment, lightuserdata, number, void)
LUA_FUNC_1(ftglGetLayoutAlignment, lightuserdata, number)
LUA_FUNC_2(ftglSetLayoutLineSpacing, lightuserdata, number, void)

/* http://sourceforge.net/p/ftgl/bugs/35/
LUA_FUNC_1(ftglGetLayoutLineSpacing, lightuserdata, number) */


int luaopen_luaftgl(lua_State *L)
{
    const luaL_Reg api[] = {
        
        /* FTFont functions (FTGL/FTFont.h) */
        { "destroyFont", lf_ftglDestroyFont },
        { "attachFile", lf_ftglAttachFile },
        { "attachData", lf_ftglAttachData },
        { "setFontCharMap", lf_ftglSetFontCharMap },
        { "getFontCharMapCount", lf_ftglGetFontCharMapCount },
        { "getFontCharMapList", lf_ftglGetFontCharMapList },
        { "setFontFaceSize", lf_ftglSetFontFaceSize },
        { "getFontFaceSize", lf_ftglGetFontFaceSize },
        { "setFontDepth", lf_ftglSetFontDepth },
        { "setFontOutset", lf_ftglSetFontOutset },
        { "setFontDisplayList", lf_ftglSetFontDisplayList },
        { "getFontAscender", lf_ftglGetFontAscender },
        { "getFontDescender", lf_ftglGetFontDescender },
        { "getFontLineHeight", lf_ftglGetFontLineHeight },
        { "getFontBoundingBox", lf_ftglGetFontBBox }, /* added */
        { "getFontBBox", lf_ftglGetFontBBox }, /* original */
        { "getFontAdvance", lf_ftglGetFontAdvance },
        { "renderFont", lf_ftglRenderFont },
        { "getFontError", lf_ftglGetFontError },
        
        /* FTGL*Font functions (FTGL/FTGL*Font.h) */
        { "createBitmapFont", lf_ftglCreateBitmapFont },
        { "createBufferFont", lf_ftglCreateBufferFont },
        { "createExtrudeFont", lf_ftglCreateExtrudeFont },
        { "createOutlineFont", lf_ftglCreateOutlineFont },
        { "createPixmapFont", lf_ftglCreatePixmapFont },
        { "createPolygonFont", lf_ftglCreatePolygonFont },
        { "createTextureFont", lf_ftglCreateTextureFont },
        
        /* FTLayout functions (FTGL/FTLayout.h) */
        { "destroyLayout", lf_ftglDestroyLayout },
        { "getLayoutBoundingBox", lf_ftglGetLayoutBBox }, /* added */
        { "getLayoutBBox", lf_ftglGetLayoutBBox }, /* original */
        { "renderLayout", lf_ftglRenderLayout },
        { "getLayoutError", lf_ftglGetLayoutError },
        
        /* FTSimpleLayout functions (FTGL/FTSimpleLayout.h) */
        { "createSimpleLayout", lf_ftglCreateSimpleLayout },
        { "setLayoutFont", lf_ftglSetLayoutFont },
        { "getLayoutFont", lf_ftglGetLayoutFont },
        { "setLayoutLineLength", lf_ftglSetLayoutLineLength },
        { "getLayoutLineLength", lf_ftglGetLayoutLineLength },
        { "setLayoutAlignment", lf_ftglSetLayoutAlignment },
        { "getLayoutAlignment", lf_ftglGetLayoutAlignment }, /* added */
        { "getLayoutAlignement", lf_ftglGetLayoutAlignment }, /* original */
        { "setLayoutLineSpacing", lf_ftglSetLayoutLineSpacing },
        /* http://sourceforge.net/p/ftgl/bugs/35/
        { "getLayoutLineSpacing", lf_ftglGetLayoutLineSpacing }, */
        
        { NULL, NULL }
    };

#if LUA_VERSION_NUM == 501
    luaL_register (L, "luaftgl", api);
#else
    luaL_newlib (L, api);
#endif

    /*  FT_Encoding (freetype.h) */
    FT_CONST(ENCODING_NONE)
    FT_CONST(ENCODING_MS_SYMBOL)
    FT_CONST(ENCODING_UNICODE)
    FT_CONST(ENCODING_SJIS)
    FT_CONST(ENCODING_GB2312)
    FT_CONST(ENCODING_BIG5)
    FT_CONST(ENCODING_WANSUNG)
    FT_CONST(ENCODING_JOHAB)
    FT_CONST(ENCODING_MS_SJIS)
    FT_CONST(ENCODING_MS_GB2312)
    FT_CONST(ENCODING_MS_BIG5)
    FT_CONST(ENCODING_MS_WANSUNG)
    FT_CONST(ENCODING_MS_JOHAB)
    FT_CONST(ENCODING_ADOBE_STANDARD)
    FT_CONST(ENCODING_ADOBE_EXPERT)
    FT_CONST(ENCODING_ADOBE_CUSTOM)
    FT_CONST(ENCODING_ADOBE_LATIN_1)
    FT_CONST(ENCODING_OLD_LATIN_2)
    FT_CONST(ENCODING_APPLE_ROMAN)

    /*  FTGL constants */
    FTGL_CONST(RENDER_FRONT)
    FTGL_CONST(RENDER_BACK)
    FTGL_CONST(RENDER_SIDE)
    FTGL_CONST(RENDER_ALL)
    FTGL_CONST(ALIGN_LEFT)
    FTGL_CONST(ALIGN_CENTER)
    FTGL_CONST(ALIGN_RIGHT)
    FTGL_CONST(ALIGN_JUSTIFY)
    
    return 1;
}
