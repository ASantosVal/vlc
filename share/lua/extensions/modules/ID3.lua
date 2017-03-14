--[[ 
 
ID3 Lib v1.5
by Yongobongo
 
Description: A LUA library that allows users to easily grab ID3 data from MP3 files.
 
id3.getV1(mp3) - Returns ID3 V1 tags in an array.
id3.getV2(mp3,tag) - Returns the ID3 V2 tag specified.
id3.getArt(mp3, w, h) - Returns an image of album art, in PNG format.
 
]]--
 
id3 = {}

 
function id3.getV1(mp3)
	vlc.msg.dbg('[StreamIt] lib start') --TODO: delete 
	local name,artist,album,track,genre,year,comment = ""
	local fbuf = io.open(mp3,"r")
	local f = assert(io.open(mp3, "r"))
	if fbuf then
		fbuf:seek("end",-128)
		local tag = fbuf:read(3)
		if tag == "TAG" then
			fbuf:seek("end",-125)
			name = fbuf:read(30)
			print('*'..name)--TODO:delete
			fbuf:seek("end",-95)
			artist = fbuf:read(30)
			print('*'..artist)--TODO:delete
			fbuf:seek("end",-65)
			album = fbuf:read(30)
			fbuf:seek("end",-35)
			year = fbuf:read(4)
			fbuf:seek("end",-31)
			comment = fbuf:read(28)
			fbuf:seek("end",-2)
			track = fbuf:read(1)
			fbuf:seek("end",-1)
			genre = fbuf:read(1)
	 
			for i = 1, string.len(name) do
				if string.byte(string.sub(name,-1)) == 0 or string.byte(string.sub(name,-1)) == 32 then
					name = string.sub(name,1,-2)
				end
			end
			for i = 1, string.len(artist) do
				if string.byte(string.sub(artist,-1)) == 0 or string.byte(string.sub(artist,-1)) == 32 then
					artist = string.sub(artist,1,-2)
				end
			end
			for i = 1, string.len(album) do
				if string.byte(string.sub(album,-1)) == 0 or string.byte(string.sub(album,-1)) == 32 then
					album = string.sub(album,1,-2)
				end
			end
		end
		fbuf:close()
	end
	return { name = name, artist = artist, album = album, track = track, genre = genre, year = year, comment = comment }
end
 
function id3.getV2(mp3,tag)
	vlc.msg.dbg('[StreamIt] lib start') --TODO: delete 
	local fbuf = io.open(mp3,"r")
	local ttag = ""
	if fbuf then
		fbuf:seek("set")
		local fdata = fbuf:read(100000)
		fbuf:close()
	end
	if fdata then
		local fndid = string.find(fdata, tag)
	end
	 
	if fndid then
		ttag = string.sub(fdata, fndid + 11, fndid + 9 + string.byte(string.sub(fdata, fndid+7, fndid+7)))
		ttag = string.gsub(ttag, "\0", "")
	end
	 
	return ttag
end
 
function id3.getArt(mp3)
	local fbuf = io.open(mp3,"r")
	fbuf:seek("set")
	local fdata = fbuf:read(100000)
	local fndid = string.find(fdata, "APIC")
	local none_found = Image.createEmpty(128,128)
 
	if fndid then
		local beg = string.sub(fdata,fndid)
		local beg_pos = string.find(beg, "JFIF")-6
 
		if beg_pos then
			beg = string.sub(beg,beg_pos)
			local fin = string.find(beg, string.char(255)..string.char(217))
 
			if fin then
				fin = fin + 1
 
				img = string.sub(beg,1,fin)
 
				fbuf:close()
 
				file = io.open("buffer.jpg","w")
				file:write(img)
				file:close()
 
				image = Image.load("buffer.jpg")
				return image
			else
				return "none found"
			end
		else
			local beg_pos = string.find(beg, "‰PNG")
 
			if beg_pos then
				beg = string.sub(beg,beg_pos)
				local fin = string.find(beg, "IEND")+7
 
				img = string.sub(beg,1,fin)
 
				fbuf:close()
 
				image = Image.loadFromMemory(img)
				return image
			else
				return "none found"
			end
		end
	else
		return "none found"
	end
end


function id3.setV1(mp3, tags)
	local name,artist,album,track,genre,year,comment = ""
	local fbuf = io.open(mp3,"r+")
	local f = assert(io.open(mp3, "r+"))
	fbuf:seek("end",0)
	fbuf:write('TAG')
	if fbuf then		
		fbuf:seek("end",-128)
		local tag = fbuf:read(3)
		vlc.msg.dbg('[StreamIt] tag:'..tag) --TODO: delete 

		if tag == "TAG" then
			
			fbuf:seek("end",-125)
			fbuf:write(tags.title)

			vlc.msg.dbg('[StreamIt] writing:'..tags.title) --TODO: delete 
			
			fbuf:seek("end",-125)			
			name = fbuf:read(30)
			vlc.msg.dbg('[StreamIt] wrote:'..name) --TODO: delete 
			


			fbuf:seek("end",-95)
			--artist = fbuf:read(30)
			fbuf:write(tags.artist)			
	--vlc.msg.dbg('[StreamIt] wrote'..tags.artist) --TODO: delete 

			fbuf:seek("end",-65)
			album = fbuf:read(30)

			fbuf:seek("end",-35)
			year = fbuf:read(4)

			fbuf:seek("end",-31)
			--comment = fbuf:read(28)
			fbuf:write('comentario')	
			--vlc.msg.dbg('[StreamIt] wrote'..'comentario') --TODO: delete 


			fbuf:seek("end",-2)
			track = fbuf:read(1)

			fbuf:seek("end",-1)
			genre = fbuf:read(1)
	 
			-- for i = 1, string.len(name) do
			-- 	if string.byte(string.sub(name,-1)) == 0 or string.byte(string.sub(name,-1)) == 32 then
			-- 		name = string.sub(name,1,-2)
			-- 	end
			-- end
			-- for i = 1, string.len(artist) do
			-- 	if string.byte(string.sub(artist,-1)) == 0 or string.byte(string.sub(artist,-1)) == 32 then
			-- 		artist = string.sub(artist,1,-2)
			-- 	end
			-- end
			-- for i = 1, string.len(album) do
			-- 	if string.byte(string.sub(album,-1)) == 0 or string.byte(string.sub(album,-1)) == 32 then
			-- 		album = string.sub(album,1,-2)
			-- 	end
			-- end
		end
		fbuf:close()
	end
end

function id3.setV2(mp3,tag, value)
	vlc.msg.dbg('[StreamIt] lib set v2') --TODO: delete 
	local fbuf = io.open(mp3,"r+")
	local ttag = ""
	if fbuf then
		fbuf:seek("set")
		local fdata = fbuf:read(100000)
		vlc.msg.dbg('[StreamIt] fdata: '..fdata) --TODO: delete 
		fbuf:close()
	end
	if fdata then
		local fndid = string.find(fdata, tag)
		--TODO: this is not finished
		local header = string.sub(fndid,1, 10)--just the header bytes
		vlc.msg.dbg('[StreamIt] header: '..header) --TODO: delete 
	
	end
	 
	if fndid then
		ttag = string.sub(fdata, fndid + 11, fndid + 9 + string.byte(string.sub(fdata, fndid+7, fndid+7)))
		ttag = string.gsub(ttag, "\0", "")
	end
	vlc.msg.dbg('[StreamIt] found: '..ttag) --TODO: delete 
	 
	return ttag
end

function id3.edit ( path , tags , inherit )
	vlc.msg.dbg('[StreamIt] Edit tag special') --Debug message  

	local fd , err = io.open ( path , "r+" )
	--if not fd then return ferror ( err , 3 ) end
	
	-- if inherit then 
	-- 	local currenttags= info ( fd ) -- inherit from current data
	-- 	for k , v in pairs ( currenttags ) do
	-- 		for i , v in ipairs ( v ) do
	-- 			tags [ k ] [ #tags [ k ] + 1 ] = v
	-- 		end
	-- 	end
	-- end
	
	local id3 = generatetag ( tags )
	vlc.msg.dbg('[StreamIt] processed tag :'..id3) --Debug message 
	
	if #id3 ~= 128 then 
		vlc.msg.dbg('[StreamIt] tag creation error, #id3='..#id3) --Debug message  
	end
	
	-- Check if file already has an ID3 tag
	local starttag = find ( fd )
	if not starttag then -- If file has no id3v1 tag
		fd:seek ( "end" ) -- make tag at end of file
	end
	local ok , err = fd:write ( id3 )
	if not ok then return ok , err end
	fd:flush ( )
	
	return 
end

function generatetag ( tags )	
	vlc.msg.dbg('[StreamIt] Generate tag in lib') --Debug message  
	local title , artist , album, year , comment , track , genre
	
	-- Title
	if type ( tags.title ) == "table" then 
		title = tags.title
	else title = { "" }
	end

	title = makestring ( title [ 1 ] , 30 )
	vlc.msg.dbg('[StreamIt] -> '..title)
	
	do -- Artist
		local t
		if type ( tags.artist ) == "table" then 
			t= tags.artist
		else t = { "" }
		end
		artist = t [ 1 ]:sub ( 1 , 30 )
		for i=2 , #t-1 do
			if ( #artist + 3 + #t [ i ] ) > 30 then break end
			artist = artist .. " & " .. t [ i ]
		end
		artist = makestring ( artist , 30 )
		vlc.msg.dbg('[StreamIt] -> '..artist)
	end
	
	-- Album
	if type ( tags.album ) == "table" then 
		album = tags.album
	else album = { "" }
	end
	album = makestring ( album [ 1 ] , 30 )
	vlc.msg.dbg('[StreamIt] -> '..album)

	-- Year
	if type ( tags.date ) == "table" then 
		year = tags.date
	else year = { "" }
	end
	year = makestring ( ( guessyear ( year [ 1 ] ) or "" ) , 4 )
	
	-- Comment
	if tags.comment and #tags.comment [ 1 ] <= 28 then
		comment = tags.comment [ 1 ]
	else
		comment = ""
	end
	-- No one likes 28 character comments
	comment = makestring ( comment , 28 )

	-- Track
	if type ( tags.track ) == "table" then 
		track = tags.tracknumber
	else track = { "" }
	end
	--track = ( tonumber ( track [ 1 ] ) or 0 ):char ( )
	track = 0
	
	--Genre
	if type ( tags.genre ) == "table" then 
		--genre = toid3:iconv ( tags.genre [ 1 ] )
		genre = 'undeveloped'
	else 
		genre = ""
	end
	do
		local t = 12 -- "Other"
		-- for i , v in ipairs ( genreindex ) do
		-- 	if genre:lower ( ):find ( v:lower ( ):gsub ( "%W" , "." ) ) then
		-- 		t = i
		-- 		break
		-- 	end 
		-- end
		genre = t--:char ( )
	end
	
	-- local datadiscarded = false
	-- for k , v in pairs ( tags ) do
	-- 	if k == "title" or k == "artist" or k == "album" or k == "date" or k == "tracknumber" or k == "genre" then
	-- 		if v [ 2 ] then
	-- 			datadiscarded = true
	-- 			break
	-- 		end
	-- 	else
	-- 		datadiscarded = true
	-- 		break
	-- 	end
	-- end
	vlc.msg.dbg('[StreamIt] tag created ')
	--local tag = "TAG" .. title .. artist .. album .. year .. comment .. "\0" .. track .. genre -- String format doesn't like \0. --string.format ( "TAG%s%s%s%s%s\0%s%s" , title , artist , album , year , comment , track , genre)
	local tag = "TAG" .. 'titletitletitletitletitletitle' .. artist .. album .. year .. comment .. "\0" .. '1' .. '1' -- String format doesn't like \0. --string.format ( "TAG%s%s%s%s%s\0%s%s" , title , artist , album , year , comment , track , genre)
	return tag --, datadiscarded
end

--local iconv = require "iconv"
--local Locale = "ISO-8859-1"
--local toid3 = iconv.new ( Locale , "UTF-8" )

function makestring ( str , tolength )
	str = toid3:iconv ( str )
	str = str:sub ( 1 , tolength )
	return str .. ("\0"):rep ( tolength-#str )
end


function twodigityear ( capture )
	if tonumber ( capture ) < 30 then -- Break on 30s
		return "20"..capture
	else 
		return "19" .. capture
	end
end
	
function guessyear ( datestring )
	local patterns = {
		[".*%f[%d](%d%d%d%d)%f[%D].*"] = { matches = 1 , replace = "%1" }, -- MAGICAL FRONTIER PATTERN (undocumented)
		["^%W*(%d%d)%W*$"] = { matches = 1 , replace = twodigityear }, -- Eg: 70 or '70
		["^%s*%d%d?%s*[/-%s]%s*%d%d?%s*[/-%s]%W*(%d%d)%s*$"] = { matches = 1 , replace = twodigityear }, -- Eg: 20/4/87
		[".*Year%W*(%d%d)%W.-"] = { matches = 1 , replace = twodigityear }, -- Eg: Month: October, Year: 69
		[".*%W(%d%d)%W*$"] = { matches = 1 , replace = twodigityear }, -- Eg: Concert of '97.
	}
	for k , v in pairs ( patterns ) do
		local s , m = datestring:gsub ( k , v.replace )
		if m == v.matches then return s end
	end
	return false
end

function find ( fd )
	fd:seek ( "end" , -128 ) -- Look at end of file
	if fd:read ( 3 ) == "TAG" then
		return fd:seek ( "end" , -128 )
	else
		return false
	end
end