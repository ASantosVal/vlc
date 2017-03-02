function descriptor()
  return { title = 'fanTAGstic',
           version = '0.1',
           author = 'Asier Santos',
           capabilities = {} 
         }
end

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
}

local dlg = nil
local input_table = {} -- General widget id reference
local meta_image_path = nil --TODO: temporal, hay que repensarlo
local metas_table = nil --TODO: temporal, hay que repensarlo

function activate()    --Initialization
  vlc.msg.dbg('[StreamIt] init') --Debug message
  if vlc.input.item() == nil then 
    launch_error(txt['int_errorNoFile'])
  else 
    launch_main_menu()
  end
end


function close()
  vlc.msg.dbg('[StreamIt] closing') --Debug message
  dlg = nil
  collectgarbage() --important
  vlc.deactivate()
end


function close_dlg() --Close a dialog so another one can open
  vlc.msg.dbg('[StreamIt] closing dialog') --Debug message

  if dlg ~= nil then 
    dlg:hide() 
  end
  
  dlg = nil
  collectgarbage() --important
end


function launch_main_menu()
  close_dlg()
  vlc.msg.dbg('[StreamIt] launching main menu') --Debug message
  
  dlg = vlc.dialog(txt['int_extension_name'])

  fileInfo = getFileInfoHTML()
  input_table['html_fileInfo'] = dlg:add_html(fileInfo, 1, 1, 12, 12)


  input_table['button_help'] = dlg:add_button(txt['int_help'], launch_help, 13, 1, 1, 1)
  input_table['button_about'] = dlg:add_button(txt['int_about'], launch_about, 13, 2, 1, 1)

    input_table['img_artwork'] = dlg:add_image( meta_image_path, 14, 1, 8, 8)
  
  dlg:show()
end    


function launch_about()
  close_dlg()
  vlc.msg.dbg('[StreamIt] launching about') --Debug message
  dlg = vlc.dialog(txt['int_extension_name'].. ' >> '..txt['int_help'])

  input_table['html_rendererInfo'] = dlg:add_html(txt['int_helpText'], 1, 1, 8, 8)
  input_table['button_back'] = dlg:add_button(txt['int_back'], launch_main_menu, 1, 9, 8, 1)
  
    dlg:show()
end


function launch_help()
  close_dlg()
  vlc.msg.dbg('[StreamIt] launching help') --Debug message
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


function dumb()    
    --TODO: delete this function
    vlc.msg.dbg('[StreamIt] UNIMPLEMENTED') --Debug message  
end


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
  metas_table = vlc.input.item():metas()
  metas_html = ''
  for key,value in pairs(metas_table) do
    metas_html = metas_html..'<b><u>'..key..'</u></b> --> '..value..'<br>'
  end
  undecoded_uri = metas_table['artwork_url']
  decoded_uri = vlc.strings.url_parse(uri) --Parse URL. Returns a table with fields "protocol", "username", "password", "host", "port", path" and "option".
  meta_image_path = decoded_uri['path']

  return metas_html
end


function meta_changed()
  return false
end

