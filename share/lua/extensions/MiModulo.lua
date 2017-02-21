function descriptor()
  return { title = 'StreamIt!',
           version = '0.1',
           author = 'Asier Santos',
           capabilities = {} 
         }
end

local txt = {
  int_search_devices = 'Search Devices',
  int_message = 'Message',
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
    deserunt mollit anim id est laborum.]],
  int_about = 'About', --TODO: redactar texto
  int_aboutText = [[
    <b><u>Lorem ipsum:</u></b> <br>
    Lorem ipsum dolor sit amet, consectetur adipiscing elit, 
    sed do eiusmod tempor incididunt ut labore et dolore magna 
    aliqua. Ut enim ad minim veniam, quis nostrud exercitation 
    ullamco laboris nisi ut aliquip ex ea commodo consequat. 
    Duis aute irure dolor in reprehenderit in voluptate velit 
    esse cillum dolore eu fugiat nulla pariatur. Excepteur sint 
    occaecat cupidatat non proident, sunt in culpa qui officia 
    deserunt mollit anim id est laborum.]], --TODO: redactar texto
  int_streamit = 'Stream It!',
  int_close = 'Close',
  int_renderer_name_dumb = 'render_name_default',
  int_fileInfo1 = '<b><u>Name:</u></b>',
  int_fileInfo2 = '<br><b><u>Duration:</u></b> ',
  int_volumeUp = 'V+',
  int_volumeDown =  'V-',
  int_rendererInfo = [[
    <b><u>Name:</u></b> Blah meh blah <br>
    <b><u>IP:</u></b> Blah meh blah <br>
    <b><u>Vendor:</u></b> Blah meh blah <br>
    <b><u>Others :</u></b> Blah meh blah ]],
  int_play = 'Play',
  int_pause = 'Pause',
  int_stop = 'Stop',
  int_status = 'Reproduction status',
  int_back = 'Go back',
  int_searching = 'Searching...',
  int_errorNoFile = [[
    <b><u>Error:</u></b> <br>
    There is no file in the playlist. Please, 
    select on file and then launch this extension. ]],
}

local dlg = nil
local input_table = {} -- General widget id reference

function activate()    --Initialization
  vlc.msg.dbg('StreamIt init') --Debug message
  if vlc.input.item() == nil then 
    vlc.msg.dbg('StreamIt error: no file playing') --Debug message
  	launch_error(txt['int_errorNoFile'])
  else 
    launch_main_menu()
  end
end


function close()
  vlc.msg.dbg('StreamIt closing') --Debug message
  dlg = nil
  collectgarbage() --important
  vlc.deactivate()
end


function close_dlg() --Close a dialog so another one can open
  vlc.msg.dbg('StreamIt closing dialog') --Debug message

  if dlg ~= nil then 
    dlg:hide() 
  end
  
  dlg = nil
  collectgarbage() --important
end


function launch_main_menu()
  close_dlg()
  dlg = vlc.dialog('Stream It!')
  input_table['list_devices'] = dlg:add_list(1, 1, 4, 4) 

  input_table['button_search'] = dlg:add_button(txt['int_search_devices'], search_devices, 5, 1, 2, 1)
  input_table['button_help'] = dlg:add_button(txt['int_help'], launch_help, 8, 1, 1, 1)

  input_table['spin'] = dlg:add_spin_icon(5, 2, 1, 1)
  input_table['label_search'] = dlg:add_label(txt['int_message'], 6, 2, 2, 1)
  input_table['button_about'] = dlg:add_button(txt['int_about'], launch_about, 8, 2, 1, 1)

  input_table['html_selectedinfo'] = dlg:add_html(txt['int_rendererInfo'], 5, 3, 2, 2) --TODO: extraerlo a un texto/funcion aparte para actualziarlo con facilidad

  input_table['button_streamit'] = dlg:add_button(txt['int_streamit'], launch_control_menu, 1, 5, 4, 1)
  input_table['button_close'] = dlg:add_button(txt['int_close'], close, 6, 5, 3, 1)

  dlg:show()
end    


function launch_control_menu()
  close_dlg()
  dlg = vlc.dialog('Streaming to :'..txt['int_renderer_name_dumb'])

  fileInfo = getFileInfo()
  
  input_table['html_fileInfo'] = dlg:add_html(fileInfo, 1, 1, 6, 4) --TODO: extraerlo a un texto/funcion aparte para actualziarlo con facilidad

  input_table['button_volumeUp'] = dlg:add_button(txt['int_volumeUp'], dumb, 7, 1, 1, 1)
  input_table['button_volumeDown'] = dlg:add_button(txt['int_volumeDown'], dumb, 7, 4, 1, 1)

  input_table['html_rendererInfo'] = dlg:add_html(txt['int_rendererInfo'], 8, 1, 3, 4) --TODO: extraerlo a un texto/funcion aparte para actualziarlo con facilidad
  
  input_table['button_play'] = dlg:add_button(txt['int_play'], dumb, 1, 5, 2, 1)
  input_table['button_pause'] = dlg:add_button(txt['int_pause'], dumb, 3, 5, 2, 1)
  input_table['button_stop'] = dlg:add_button(txt['int_stop'], dumb, 5, 5, 2, 1)
  input_table['button_back'] = dlg:add_button(txt['int_back'], launch_main_menu, 9, 5, 2, 1)

  input_table['label_search'] = dlg:add_label(txt['int_status'], 1, 6, 10, 1)

  dlg:show()
end 

function launch_about()
  close_dlg()
  dlg = vlc.dialog('Stream It! >> '..txt['int_help'])

  input_table['html_rendererInfo'] = dlg:add_html(txt['int_helpText'], 1, 1, 8, 8)
  input_table['button_back'] = dlg:add_button(txt['int_back'], launch_main_menu, 1, 9, 8, 1)
  
  dlg:show()
end


function launch_help()
  close_dlg()
  dlg = vlc.dialog('Stream It! >> '..txt['int_about'])

  input_table['html_rendererInfo'] = dlg:add_html(txt['int_aboutText'], 1, 1, 8, 8)   
  input_table['button_back'] = dlg:add_button(txt['int_back'], launch_main_menu, 1, 9, 8, 1)

  dlg:show()
end


function launch_error(text)
  close_dlg()
  dlg = vlc.dialog('Error!')

  input_table['html_rendererInfo'] = dlg:add_html(text, 1, 1, 8, 8)
  input_table['button_close'] = dlg:add_button(txt['int_close'], close, 1, 9, 8, 1)
  
  dlg:show()
end



function search_devices() --Search devices on the LAN   
    --TODO: develop this 
  services=vlc.sd.get_services_names()
  input_table['label_search']:set_text(txt['int_searching'])

  input_table['spin']:animate()

  input_table['list_devices']:clear()

  for i=1,5,1 --TODO: add real renderers' info
  do 
    input_table['list_devices']:add_value("Item"..i, 1) 
  end

end


function dumb()    
    --TODO: delete this function
end

function getFileInfo ()
  
  if vlc.input.item() == nil then 
    launch_error(txt['int_errorNoFile']) --TODO: this error doesn't work
  else
  	title = vlc.input.item():name()
  	duration = vlc.input.item():duration()/60 
  	duration_formatted = tonumber(string.format("%.2f", duration)) --this rounds the number
    return 
      txt['int_fileInfo1']..title..
      txt['int_fileInfo2']..duration_formatted
  end
end



--[[    
d:add_button( text, func, , posX, posY, lengthX, lengthY )
d:add_label( text, ... )
d:add_html( text, ... )
d:add_text_input( text, ... )
d:add_password( text, ... )
d:add_check_box( text, state, ... )
d:add_dropdown( ... )
d:add_list( ... ):        Allows multiple or empty selections.
d:add_image( path, ... )

Input
-----
input.is_playing(): Return true if input exists.
input.add_subtitle(url): Add a subtitle file (by path) to the current input
input.item(): Get the current input item. Input item methods are:
  :uri(): Get item's URI.
  :name(): Get item's name.
  :duration(): Get item's duration in seconds or negative value if unavailable.
  :is_preparsed(): Return true if meta data has been preparsed
  :metas(): Get meta data as a table.
  :set_meta(key, value): Set meta data.
  :info(): Get the current input's info. Return value is a table of tables. Keys of the top level table are info category labels.
  :stats(): Get statistics about the input. This is a table with the following fields:
    .read_packets
    .read_bytes
    .input_bitrate
    .average_input_bitrate
    .demux_read_packets
    .demux_read_bytes
    .demux_bitrate
    .average_demux_bitrate
    .demux_corrupted
    .demux_discontinuity
    .decoded_audio
    .decoded_video
    .displayed_pictures
    .lost_pictures
    .sent_packets
    .sent_bytes
    .send_bitrate
    .played_abuffers
    .lost_abuffers


    OSD
---
osd.icon( type, [id] ): Display an icon on the given OSD channel. Uses the
  default channel is none is given. Icon types are: 'pause', 'play',
  'speaker' and 'mute'.
osd.message( string, [id], [position], [duration] ): Display the text message on
  the given OSD channel. Position types are: 'center', 'left', 'right', 'top',
  'bottom', 'top-left', 'top-right', 'bottom-left' or 'bottom-right'. The
  duration is set in microseconds.
osd.slider( position, type, [id] ): Display slider. Position is an integer
  from 0 to 100. Type can be 'horizontal' or 'vertical'.
osd.channel_register(): Register a new OSD channel. Returns the channel id.
osd.channel_clear( id ): Clear OSD channel.

Services discovery
------------------

Interfaces and extensions can use the following SD functions:

sd.get_services_names(): Get a table of all available service discovery
  modules. The module name is used as key, the long name is used as value.
sd.add( name ): Add service discovery.
sd.remove( name ): Remove service discovery.
sd.is_loaded( name ): Check if service discovery is loaded.
...

--]]

--    var.libvlc_command( name, argument ): 
--    sd.get_services_names()



 
function deactivate() --TODO: Useful??
  vlc.msg.dbg('StreamIt closing') --Debug message
  if dlg then
    dlg:hide() 
  end
end