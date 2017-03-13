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
	--mp3='/home/hal/Downloads/04-%20Poligrafo%20Bakarra.mp3'
	--mp3 = string.gsub(mp3, '%20+', "%%s")
	mp3 = unescape(mp3)
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


--[[
	LOMP ( Lua Open Music Player )
	Copyright (C) 2007- daurnimator
	This program is free software: you can redistribute it and/or modify it under the terms of version 3 of the GNU General Public License as published by the Free Software Foundation.
	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
	You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.
]]

--[[
require "general"

local ipairs , pairs , pcall , select , require , tonumber , tostring , type = ipairs , pairs , pcall , select , require , tonumber , tostring , type
local ioopen = io.open

module ( "lomp.fileinfo.id3v1" , package.see ( lomp ) )

local iconv = require "iconv"
local genreindex = require ( select ( 1 , ... ):match ( "(.*%.)[^.]+$" ) .. "genrelist" )

local Locale = "ISO-8859-1"
local toid3 = iconv.new ( Locale , "UTF-8" )

_NAME = "ID3v1 tag utils"

local speedindex = {
	[1] = "slow" ,
	[2] = "medium" ,
	[3] = "fast" ,
	[4] = "hardcore"
}

local function readstring ( str )
	str = str:gsub ( "[%s%z]*$" , "" )
	if #str == 0 then return nil end
	return str:utf8 ( Locale )
end

function find ( fd )
	fd:seek ( "end" , -128 ) -- Look at end of file
	if fd:read ( 3 ) == "TAG" then
		return fd:seek ( "end" , -128 )
	else
		return false
	end
end

function info ( fd , offset )
	--  offset should always be (size of file-128)
	if offset then fd:seek ( "set" , offset ) end
	
	if fd:read ( 3 ) ==  "TAG" then 
		local tags = { }
		tags.title = { readstring ( fd:read( 30 ) ) } 
		tags.artist = { readstring ( fd:read ( 30 ) ) } 
		tags.album = { readstring ( fd:read ( 30 ) ) } 
		tags.date = { readstring ( fd:read ( 4 ) ) }
		
		do -- ID3v1 vs ID3v1.1
			local a = fd:read ( 28 )
			local b = fd:read ( 1 )
			if b == "\0" then -- Check if comment is 28 or 30 characters
				-- Get up to 28 character comment
				fd:seek ( "cur" , -29 )
				tags.comment = { readstring ( a , 28 ) }
				
				-- Get track number
				local track = fd:read ( 1 ):byte ( )
				if track ~= 0 then tags.tracknumber = { track } end
			else -- Is ID3v1, could have a 30 character comment tag
				tags.comment = { readstring ( a .. b .. fd:read ( 1 ) ) } 
			end
		end
		
		tags.genre = { genreindex [ tonumber ( fd:read ( 1 ):byte( ) ) ] }
		
		-- Check for extended tags (Note: these are damn rare, worthwhile supporting them??)
		fd:seek ( "end" , -355 )
		if fd:read ( 4 ) == "TAG+" then
			tags.title [ 1 ] = tags.title[1] .. readstring ( fd:read ( 60 ) )
			tags.artist [ 1 ] = tags.artist[1] .. readstring ( fd:read ( 60 ) )
			tags.album [ 1 ] = tags.album[1] .. readstring ( fd:read ( 60 ) )
			tags.speed = { speedindex [ tonumber ( fd:read ( 1 ):byte ( ) ) ] }
			tags.genre [ 2 ] = readstring ( fd:read ( 30 ) )
			do
				local start = readstring ( fd:read ( 30 ) )
				if #start == 6 and start:sub ( 4 , 4 ) == ":" then
					tags["start-time"] = { tostring ( tonumber ( start:sub ( 1 , 3 ) ) * 60 + start:sub ( 5 , 6 ) ) }
				end
				local fin = readstring ( fd:read ( 30 ) )
				if #fin == 6 and fin:sub ( 4 , 4 ) == ":" then
					tags["end-time"] = { tostring ( tonumber ( fin:sub ( 1 , 3 ) ) * 60 + fin:sub ( 5 , 6 ) ) }
				end
			end
			
		end
		return tags , { }
	else
		-- File doesn't have an ID3v1 Tag
		return false
	end
end

local function twodigityear ( capture )
	if tonumber ( capture ) < 30 then -- Break on 30s
		return "20"..capture
	else 
		return "19" .. capture
	end
end
	
local function guessyear ( datestring )
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

local function makestring ( str , tolength )
	str = toid3:iconv ( str )
	str = str:sub ( 1 , tolength )
	return str .. ("\0"):rep ( tolength-#str )
end

function generatetag ( tags )	
	local title , artist , album, year , comment , track , genre
	
	-- Title
	if type ( tags.title ) == "table" then 
		title = tags.title
	else title = { "" }
	end
	title = makestring ( title [ 1 ] , 30 )
	
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
	end
	
	-- Album
	if type ( tags.album ) == "table" then 
		album = tags.album
	else album = { "" }
	end
	album = makestring ( album [ 1 ] , 30 )

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
	track = ( tonumber ( track [ 1 ] ) or 0 ):char ( )
	
	-- Genre
	if type ( tags.genre ) == "table" then 
		genre = toid3:iconv ( tags.genre [ 1 ] )
	else 
		genre = ""
	end
	do
		local t = 12 -- "Other"
		for i , v in ipairs ( genreindex ) do
			if genre:lower ( ):find ( v:lower ( ):gsub ( "%W" , "." ) ) then
				t = i
				break
			end 
		end
		genre = t:char ( )
	end
	
	local datadiscarded = false
	for k , v in pairs ( tags ) do
		if k == "title" or k == "artist" or k == "album" or k == "date" or k == "tracknumber" or k == "genre" then
			if v [ 2 ] then
				datadiscarded = true
				break
			end
		else
			datadiscarded = true
			break
		end
	end
	local tag = "TAG" .. title .. artist .. album .. year .. comment .. "\0" .. track .. genre -- String format doesn't like \0. --string.format ( "TAG%s%s%s%s%s\0%s%s" , title , artist , album , year , comment , track , genre)
	return tag , datadiscarded
end

function edit ( path , tags , inherit )
	local fd , err = ioopen ( path , "rb+" )
	if not fd then return ferror ( err , 3 ) end
	
	if inherit then 
		local currenttags= info ( fd ) -- inherit from current data
		for k , v in pairs ( currenttags ) do
			for i , v in ipairs ( v ) do
				tags [ k ] [ #tags [ k ] + 1 ] = v
			end
		end
	end
	
	local id3 = generatetag ( tags )
	
	if #id3 ~= 128 then return ferror ( "Unknown error" , 3 ) end
	
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
]]