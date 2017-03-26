//#include <id3/tag.h>    /* id3lib */
//#include <id3v2lib.h>     /* id3 libraries */


#include <lua.h>        /* Lua libraries */
#include <lauxlib.h>                           
#include <lualib.h>                            


static int same_b(lua_State *L){            /* Internal name of func */
	float rtrn = lua_tonumber(L, -1);       /*Get the single number arg */
	lua_pushnumber(L,rtrn);     			/* Push the return */
	return 1;                              	/* One return value */
}

/* Register this file's functions with the
 * luaopen_libraryname() function, where libraryname
 * is the name of the compiled .so output (and mybe the path). 
 * This function should contain lua_register() commands for
 * each function you want available from Lua.
 *
*/
int luaopen_share_lua_extensions_libs_id3libBinding (lua_State *L){ 
//	lua_register(
//			L,               /* Lua state variable */
//			"square",        /* func name as known in Lua */
//			isquare          /* func name in this file */
//			);  
	lua_register(L,"same",same_b);
	return 0;
}

