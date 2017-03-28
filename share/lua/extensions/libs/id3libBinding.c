#include <id3v2lib.h>     /* id3 libraries */

#include <lua.h>        /* Lua libraries */
#include <lauxlib.h>                           
#include <lualib.h>    
 

static int set_title_bind(lua_State *L){     
	char *text = (char*)lua_tostring(L, -1);       /*Get 2nd arg, string */
	char *path = (char*)lua_tostring(L, -2);       /*Get 1st arg, string */
	
	ID3v2_tag* tag = load_tag(path); // Load the full tag from the file //TODO: error handling if file not found
	if(tag == NULL)	{tag = new_tag();}
	
	tag_set_title(text, 0, tag);
	set_tag(path, tag);

	return 1;                              	/* One return value */
}

static int set_album_bind(lua_State *L){     
	char *text = (char*)lua_tostring(L, -1);       /*Get 2nd arg, string */
	char *path = (char*)lua_tostring(L, -2);       /*Get 1st arg, string */
	
	ID3v2_tag* tag = load_tag(path); // Load the full tag from the file //TODO: error handling if file not found
	if(tag == NULL)	{tag = new_tag();}
	
	tag_set_album(text, 0, tag);
	set_tag(path, tag);

	return 1;                              	/* One return value */
}

static int set_artist_bind(lua_State *L){     
	char *text = (char*)lua_tostring(L, -1);       /*Get 2nd arg, string */
	char *path = (char*)lua_tostring(L, -2);       /*Get 1st arg, string */

	ID3v2_tag* tag = load_tag(path); // Load the full tag from the file
	if(tag == NULL)	{tag = new_tag();}
	
	tag_set_artist(text, 0, tag);
	set_tag(path, tag);

	return 1;                              	/* One return value */
}

static int set_album_artist_bind(lua_State *L){     
	char *text = (char*)lua_tostring(L, -1);       /*Get 2nd arg, string */
	char *path = (char*)lua_tostring(L, -2);       /*Get 1st arg, string */

	ID3v2_tag* tag = load_tag(path); // Load the full tag from the file
	if(tag == NULL)	{tag = new_tag();}
	
	tag_set_album_artist(text, 0, tag);
	set_tag(path, tag);

	return 1;                              	/* One return value */
}

static int set_genre_bind(lua_State *L){     
	char *text = (char*)lua_tostring(L, -1);       /*Get 2nd arg, string */
	char *path = (char*)lua_tostring(L, -2);       /*Get 1st arg, string */

	ID3v2_tag* tag = load_tag(path); // Load the full tag from the file
	if(tag == NULL)	{tag = new_tag();}
	
	tag_set_genre(text, 0, tag);
	set_tag(path, tag);

	return 1;                              	/* One return value */
}

static int set_comment_bind(lua_State *L){     
	char *text = (char*)lua_tostring(L, -1);       /*Get 2nd arg, string */
	char *path = (char*)lua_tostring(L, -2);       /*Get 1st arg, string */

	ID3v2_tag* tag = load_tag(path); // Load the full tag from the file
	if(tag == NULL)	{tag = new_tag();}
	
	tag_set_comment(text, 0, tag);
	set_tag(path, tag);

	return 1;                              	/* One return value */
}

static int set_year_bind(lua_State *L){     //TODO: receive INT
	char *text = (char*)lua_tostring(L, -1);       /*Get 2nd arg, string */
	char *path = (char*)lua_tostring(L, -2);       /*Get 1st arg, string */

	ID3v2_tag* tag = load_tag(path); // Load the full tag from the file
	if(tag == NULL)	{tag = new_tag();}
	
	tag_set_year(text, 0, tag);
	set_tag(path, tag);

	return 1;                              	/* One return value */
}

static int set_track_bind(lua_State *L){     //TODO: receive INT
	char *text = (char*)lua_tostring(L, -1);       /*Get 2nd arg, string */
	char *path = (char*)lua_tostring(L, -2);       /*Get 1st arg, string */

	ID3v2_tag* tag = load_tag(path); // Load the full tag from the file
	if(tag == NULL)	{tag = new_tag();}
	
	tag_set_track(text, 0, tag);
	set_tag(path, tag);

	return 1;                              	/* One return value */
}

static int set_disc_number_bind(lua_State *L){   
	char *text = (char*)lua_tostring(L, -1);       /*Get 2nd arg, string */
	char *path = (char*)lua_tostring(L, -2);       /*Get 1st arg, string */

	ID3v2_tag* tag = load_tag(path); // Load the full tag from the file
	if(tag == NULL)	{tag = new_tag();}
	
	tag_set_disc_number(text, 0, tag);
	set_tag(path, tag);

	return 1;                              	/* One return value */
}

static int set_composer_bind(lua_State *L){     
	char *text = (char*)lua_tostring(L, -1);       /*Get 2nd arg, string */
	char *path = (char*)lua_tostring(L, -2);       /*Get 1st arg, string */

	ID3v2_tag* tag = load_tag(path); // Load the full tag from the file
	if(tag == NULL)	{tag = new_tag();}
	
	tag_set_composer(text, 0, tag);
	set_tag(path, tag);

	return 1;                              	/* One return value */
}

static int set_album_cover_bind(lua_State *L){     //gets the path to the file
	char *pathToCover = (char*)lua_tostring(L, -1);       /*Get 2nd arg, string */
	char *pathToAudio = (char*)lua_tostring(L, -2);       /*Get 1st arg, string */

	ID3v2_tag* tag = load_tag(pathToAudio); // Load the full tag from the file
	if(tag == NULL)	{tag = new_tag();}
	
	tag_set_album_cover(pathToCover, tag);
	set_tag(pathToAudio, tag);

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
	lua_register(
			L,               	/* Lua state variable */
			"set_title",        /* func name as known in Lua */
			set_title_bind         /* func name in this file */
			);  
	lua_register(L,"set_album",set_album_bind);
	lua_register(L,"set_artist",set_artist_bind);
	lua_register(L,"set_album_artist",set_album_artist_bind);
	lua_register(L,"set_genre",set_genre_bind);
	lua_register(L,"set_comment",set_comment_bind);
	lua_register(L,"set_year",set_year_bind);
	lua_register(L,"set_track",set_track_bind);
	lua_register(L,"set_disc_number",set_disc_number_bind);
	lua_register(L,"set_composer",set_composer_bind);
	lua_register(L,"set_album_cover",set_album_cover_bind);

	return 0;
}

