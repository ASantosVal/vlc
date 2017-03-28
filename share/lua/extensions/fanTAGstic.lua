function descriptor()
  return { title = 'fanTAGstic',
           version = '0.1',
           author = 'Asier Santos',
           capabilities = {} 
         }
end

require("share.lua.extensions.libs.id3libBinding") --My own C program for the LUA and C binding


-----------------------------------------------------------TEXT AND TRANSLATIONS--------------------------------------------------------------
local txt = {
  int_extension_name= 'fanTAGstic',
  int_help = 'Help',
  int_helpText = [[
    <b><u>Lorem ipsum:</u></b> <br>
    Lorem ipsum dolor sit amet, consectetur adipiscing elit, 
    sed do eiusmod tempor incididunt ut labore et dolore magna 
    aliqua. Ut enim ad minim veniam, quis nostrud exercitation 
    ullamco laboris nisi ut aliquip ex ea commodo consequat. 
    Duis aute irure dolor in reprehenderit in voluptate velit 
    esse cillum dolore eu fugiat nulla pariatur. Excepteur sint 
    occaecat cupidatat non proident, sunt in culpa qui officia 
    deserunt mollit anim id est laborum.]], --TODO: redactar texto Help
  int_about = 'About', 
  int_aboutText = [[
    <b><u>Lorem ipsum:</u></b> <br>
    Lorem ipsum dolor sit amet, consectetur adipiscing elit, 
    sed do eiusmod tempor incididunt ut labore et dolore magna 
    aliqua. Ut enim ad minim veniam, quis nostrud exercitation 
    ullamco laboris nisi ut aliquip ex ea commodo consequat. 
    Duis aute irure dolor in reprehenderit in voluptate velit 
    esse cillum dolore eu fugiat nulla pariatur. Excepteur sint 
    occaecat cupidatat non proident, sunt in culpa qui officia 
    deserunt mollit anim id est laborum.]], --TODO: redactar texto About
  int_streamit = 'Stream It!',
  int_close = 'Close',
  int_renderer_name_dumb = 'render_name_default',
  int_fileInfo1 = '<b><u>Name:</u></b>',
  int_fileInfo2 = '<br><b><u>Duration:</u></b> ',
  int_fileInfo3 = '<br><b><u>Path:</u></b> ',
  int_fileInfo4 = '<br><b><u>Metas:</u></b> ',
  int_back = 'Go back',
  int_errorNoFile = [[
    <b><u>Error:</u></b> <br>
    There is no file in the playlist. Please, 
    select on file and then launch this extension. ]],
  int_filename_analysis = 'Run filename analysis',
  int_posible_names_found = 'These are the possible artist and song names found:',
  int_this_is_artist = 'This is the artist',
  int_this_is_name = 'This is the song',
  int_use_this_artist = 'No, better use this as artist',
  int_use_this_name = 'No, better use this as song',
}

-----------------------------------------------------------NEEDED VARIABLES--------------------------------------------------------------
local dlg = nil
local input_table = {} -- General widget id reference
local meta_image_path = nil --TODO: temporal, hay que repensarlo
local metas_table = nil --TODO: temporal, hay que repensarlo

-----------------------------------------------------------BASIC FUNCTIONS FOR VLC--------------------------------------------------------------

function activate()    --Initialization
  vlc.msg.dbg('[fanTAGstic] initialization') --Debug message
  initDebbugShortcut()
  if vlc.input.item() == nil then 
    launch_error(txt['int_errorNoFile'])
  else 
    launch_main_menu()
  end
end


function initDebbugShortcut() --TODO: delete this
  --this is used to test certain functions during load as a development shortcut
  return false
end

function deactivate() --Closing [triggered by problems]
  close()
end


function close() --Closing [triggered by clicking 'X']
  vlc.msg.dbg('[fanTAGstic] closing') --Debug message
  dlg = nil
  collectgarbage() --important
  vlc.deactivate()
end


function close_dlg() --Close a dialog so another one can open
  if dlg ~= nil then 
    dlg:hide() 
  end
  
  dlg = nil
  collectgarbage() --important
end


function meta_changed()
  return false --TODO: this is foor when playing track changes
end

-----------------------------------------------------------GUI CREATION FUNCTIONS--------------------------------------------------------------
function launch_main_menu()
  close_dlg()
  
  dlg = vlc.dialog(txt['int_extension_name'])

  fileInfo = getFileInfoHTML()
  input_table['html_fileInfo'] = dlg:add_html(fileInfo, 1, 1, 12, 12)

  input_table['button_filename_analysis'] = dlg:add_button(txt['int_filename_analysis'], launch_filename_analysis, 14, 1, 1, 1)
  
  
  input_table['button_help'] = dlg:add_button(txt['int_help'], launch_help, 13, 1, 1, 1)
  input_table['button_about'] = dlg:add_button(txt['int_about'], launch_about, 13, 2, 1, 1)

  input_table['img_artwork'] = dlg:add_image( meta_image_path, 15, 1, 8, 8)
  
  dlg:show()
end    


function launch_filename_analysis() --TODO: añadir inputs a input_table[]
  close_dlg()
  
  dlg = vlc.dialog(txt['int_extension_name'])

  candidates=filename_analysis()

  dlg:add_label(txt['int_posible_names_found'], 1, 1, 1, 1)
  for key,value in pairs(table_sections) do
    dlg:add_label(value, key, 2, 1, 1)
    dlg:add_button(txt['int_this_is_artist'], set_artist, key, 3, 1, 1)
    dlg:add_button(txt['int_this_is_name'], set_title, key, 4, 1, 1)
  end
  dlg:add_label('------------------------------------------------------------', 1, 5, 1, 1)
  dlg:add_text_input( artist, 1, 6, 1, 1 )
  dlg:add_button(txt['int_use_this_artist'], set_artist, 2, 6, 1, 1)
  dlg:add_text_input( name, 1, 7, 1, 1 )
  dlg:add_button(txt['int_use_this_name'], set_title, 2, 7, 1, 1)

  dlg:show()
end


function launch_about()
  close_dlg()
  dlg = vlc.dialog(txt['int_extension_name'].. ' >> '..txt['int_help'])

  input_table['html_rendererInfo'] = dlg:add_html(txt['int_helpText'], 1, 1, 8, 8)
  input_table['button_back'] = dlg:add_button(txt['int_back'], launch_main_menu, 1, 9, 8, 1)
  
  dlg:show()
end


function launch_help()
  close_dlg()
  dlg = vlc.dialog(txt['int_extension_name'].. ' >> '..txt['int_about'])

  input_table['html_rendererInfo'] = dlg:add_html(txt['int_aboutText'], 1, 1, 8, 8)   
  input_table['button_back'] = dlg:add_button(txt['int_back'], launch_main_menu, 1, 9, 8, 1)

  dlg:show()
end


function launch_error(text)
  close_dlg()
  vlc.msg.err('[StreamIt] ERROR: '..text) --Debug message  
  dlg = vlc.dialog('Error!')

  input_table['html_rendererInfo'] = dlg:add_html(text, 1, 1, 8, 8)
  input_table['button_close'] = dlg:add_button(txt['int_close'], close, 1, 9, 8, 1)
  
  dlg:show()
end

-----------------------------------------------------------OTHER UTILITIES-------------------------------------------------------------
function filename_analysis()
  fileName = GetFileName()
  bol_spaceFound = false
  int_spaceCount = 0
  int_foundSections = 1
  table_sections = {}
  table_sections[int_foundSections] = ''

  --now it will look for posible artist and song-name analysing the filename
  --a while is needed to be able to modify the index (i) and jump the space (represented as '%20')'
  for i=1,fileName:len() do
    current_char = fileName:sub(i,i)
    if current_char == " " then--found space, posible multiple space separator
      bol_spaceFound = true
      int_spaceCount = int_spaceCount + 1 --start counting spaces
      if int_spaceCount == 3 then --triple space found, create new section
        bol_spaceFound = false
        int_spaceCount = 0
        int_foundSections = int_foundSections + 1
        table_sections[int_foundSections] = ''
      end
    else
      --the follofing means it found a space, but was not a triple-space separator, 
      --so it's added to the section
      if bol_spaceFound == true then 
        table_sections[int_foundSections] = table_sections[int_foundSections]..' '      
        bol_spaceFound = false
        int_spaceCount = 0
      end

      if current_char == "-" then --separator found (-), new section inititialized
        int_foundSections = int_foundSections + 1
        table_sections[int_foundSections] = ''
      
      elseif current_char == "." then --extension starts, ignore the rest
        break
      
      else --normal character, add it to the section
        table_sections[int_foundSections] = table_sections[int_foundSections]..current_char
      end 

    end
  end
  return table_sections
end


function hex_to_char (x)
  return string.char(tonumber(x, 16))
end


function unescape (url) --unescape special chars
  return url:gsub("%%(%x%x)", hex_to_char)
end


function dumb()    
    --TODO: delete this function
    vlc.msg.dbg('[fanTAGstic] UNIMPLEMENTED') --Debug message  
end

-----------------------------------------------------------GETTERS--------------------------------------------------------------
function GetFileName() --gets only the filename of the current playing file
  uri = vlc.input.item():uri()  
  path = uri:match("^.+/(.+)$")
  return unescape(path) --return unescaped filename
end

function getPath ()  
  uri = vlc.input.item():uri()  
  parsed_uri = vlc.strings.url_parse(uri)
  path = parsed_uri['path']
  return unescape(path) --return unescaped path to file
end

-----------------------------------------------------------SETTERS--------------------------------------------------------------
--[[
All the setters are done in C and binded to LUA due to VLC limitations. These are de available functions:
	set_album(path, text)
	set_artist(path, text)
	set_album_artist(path, text)
	set_genre(path, text)
	set_comment(path, text)
	set_year(path, text)
	set_track(path, text)
	set_disc_number(path, text)
	set_composer(path, text)
	set_album_cover(pathToAudio, pathToCover)
--]]

-----------------------------------------------------------HTML CREATORS--------------------------------------------------------------
function getFileInfoHTML ()  
  title = vlc.input.item():name()
  
  duration = vlc.input.item():duration() --this info is in seconds
  duration_hour = string.format("%.0f", duration / 3600) --this gets the hours
  duration_min = string.format("%.0f", (duration - duration_hour*60)  / 60) --this gets the minutes
  duration_secs = string.format("%.0f", duration - (duration_hour*3600) - (duration_min*60)) --this gets the seconds
  
  uri = vlc.input.item():uri()
  decoded_uri = vlc.strings.url_parse(uri) --Parse URL. Returns a table with fields "protocol", "username", "password", "host", "port", path" and "option".
    
  metas_html = getMetasHTML()

  return 
    txt['int_fileInfo1']..title..
    txt['int_fileInfo2']..duration_hour..':'..duration_min..':'..duration_secs..
    txt['int_fileInfo3']..decoded_uri['path']..
    txt['int_fileInfo4']..metas_html
end

function getMetasHTML ()  
  metas_table = vlc.input.item():metas() -- via vlc
  
  metas_html = ''
  for key,value in pairs(metas_table) do
    metas_html = metas_html..'<b><u>'..key..'</u></b> --> '..value..'<br>'
  end
  undecoded_uri = metas_table['artwork_url']
  decoded_uri = vlc.strings.url_parse(uri) --Parse URL. Returns a table with fields "protocol", "username", "password", "host", "port", path" and "option".
  meta_image_path = decoded_uri['path']

  return metas_html
end
