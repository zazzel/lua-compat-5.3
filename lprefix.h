/*
** $Id: lprefix.h,v 1.2 2014/12/29 16:54:13 roberto Exp $
** Definitions for Lua code that must come before any other header file
** See Copyright Notice in lua.h
*/

#ifndef lprefix_h
#define lprefix_h


/*
** Allows POSIX/XSI stuff
*/
#if !defined(LUA_USE_C89)	/* { */

#if !defined(_XOPEN_SOURCE)
#define _XOPEN_SOURCE           600
#elif _XOPEN_SOURCE == 0
#undef _XOPEN_SOURCE  /* use -D_XOPEN_SOURCE=0 to undefine it */
#endif

/*
** Allows manipulation of large files in gcc and some other compilers
*/
#if !defined(LUA_32BITS) && !defined(_FILE_OFFSET_BITS)
#define _LARGEFILE_SOURCE       1
#define _FILE_OFFSET_BITS       64
#endif

#endif				/* } */


/*
** Windows stuff
*/
#if defined(_WIN32) 	/* { */

#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS  /* avoid warnings about ISO C functions */
#endif

#endif			/* } */


/* COMPAT53 adaptation */
#include "c-api/compat-5.3.h"

#undef LUAMOD_API
#define LUAMOD_API extern

#ifndef luaopen_utf8
#  define luaopen_utf8 luaopen_compat53_utf8
#endif

#ifdef ltablib_c
#  define luaopen_table luaopen_compat53_table
/* lua_rawgeti in compat53.h is implemented as a macro, so the
 * function signature doesn't match when you use a function pointer
 */
static int compat53_rawgeti (lua_State *L, int i, lua_Integer n) {
  return lua_rawgeti(L, i, n);
}
#  undef lua_rawgeti
#  define lua_rawgeti compat53_rawgeti
static void compat53_rawseti (lua_State *L, int i, lua_Integer n) {
  lua_rawseti(L, i, (int)n);
}
#  undef lua_rawseti
#  define lua_rawseti compat53_rawseti
/* we have lua_compare emulation for Lua 5.1, but it involves calling
 * Lua code, and the only use in the table library is for '<', so ...
 */
#  if LUA_VERSION_NUM == 501
#    undef lua_compare
#    define lua_compare(L, a, b, op) lua_lessthan(L, a, b)
#  endif
#endif /* ltablib_c */

#ifdef lstrlib_c
/* move the string library open function out of the way (we only take
 * the string packing functions)!
 */
#  define luaopen_string luaopen_string_XXX
/* used in string.format implementation, which we don't use: */
#  ifndef LUA_INTEGER_FRMLEN
#    define LUA_INTEGER_FRMLEN ""
#    define LUA_NUMBER_FRMLEN ""
#  endif
#  if LUA_VERSION_NUM < 503
/* lstrlib assumes that lua_Integer and lua_Unsigned have the same
 * size, so we use the unsigned equivalent of ptrdiff_t! */
#  define lua_Unsigned size_t
#  endif
static int str_pack (lua_State *L);
static int str_packsize (lua_State *L);
static int str_unpack (lua_State *L);
LUAMOD_API int luaopen_compat53_string (lua_State *L) {
  luaL_Reg const funcs[] = {
    { "pack", str_pack },
    { "packsize", str_packsize },
    { "unpack", str_unpack },
    { NULL, NULL }
  };
  luaL_newlib(L, funcs);
  return 1;
}
/* make luaopen_string(_XXX) static, so it (and all other referenced
 * string functions) won't be included in the resulting dll
 * (hopefully).
 */
#  undef LUAMOD_API
#  define LUAMOD_API static
#endif /* lstrlib.c */

#endif

