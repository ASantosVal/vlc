//#include <id3/tag.h>    /* id3lib */
#include <id3v2lib.h>     /* id3 libraries */


#include <lua.h>        /* Lua libraries */
#include <lauxlib.h>                           
#include <lualib.h>                            


static int same_b(lua_State *L){            /* Internal name of func */
	float rtrn = lua_tonumber(L, -1);       /*Get the single number arg */
	lua_pushnumber(L,rtrn);     			/* Push the return */
	return 1;                              	/* One return value */
}

static int readTags_b(lua_State *L){     
	//*char path = lua_getstring(L, -1);       /*Get the single number arg */
	char path[255] = "/home/hal/Downloads/file.mp3";
	ID3v2_tag* tag = load_tag(path); // Load the full tag from the file
	if(tag == NULL)
	{
		tag = new_tag();
	}
	
	tag_set_title("Title", 0, tag);
	tag_set_artist("Artist", 0, tag);
	set_tag("/home/hal/Downloads/file.mp3", tag);

	// Load the fields from the tag
	ID3v2_frame* artist_frame = tag_get_artist(tag); // Get the full artist frame
	// We need to parse the frame content to make readable
	ID3v2_frame_text_content* artist_content = parse_text_frame_content(artist_frame); 
	printf("ARTIST: %s\n", artist_content->data); // Show the artist info
		
	ID3v2_frame* title_frame = tag_get_title(tag);
	ID3v2_frame_text_content* title_content = parse_text_frame_content(title_frame);
	printf("TITLE: %s\n", title_content->data);


	//lua_pushnumber(L,rtrn);     			/* Push the return */
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
	lua_register(L,"readTags",readTags_b);
	return 0;
}

