function descriptor()
  return { title = "StreamIt!",
           version = "0.1",
           author = "Asier Santos",
           capabilities = {} 
         }
end

local translation = {
  int_search_devices = 'Search Devices',
  int_help = 'Help',
  int_message = 'Message',
  int_about = 'About',
  int_selectedinfo = [[
    <b><u>Name:</u></b>
    <br>
    <b><u>IP:</u></b>
    <br>
    <b><u>Vendor:</u></b>
    <br>
    <b><u>Others :</u></b> Info about the selected renderer
    ]],
  int_streamit = 'Stream It!',
  int_close = 'Close',
  int_renderer_name_dumb = 'render_name_default',
  int_fileInfo = [[
    <b><u>Name:</u></b> Blah meh blah
    <br>
    <b><u>File location:</u></b> Blah meh blah
    <br>
    <b><u>Bibrarte:</u></b> Blah meh blah
    <br>
    <b><u>Resolution :</u></b> Blah meh blah
    ]] ,
  int_volumeUp = 'V+',
  int_volumeDown =  'V-',
  int_rendererInfo = [[
    <b><u>Name:</u></b>
    <br>
    <b><u>IP:</u></b>
    <br>
    <b><u>Vendor:</u></b>
    <br>
    <b><u>Others :</u></b> Info about the selected renderer
    ]],
  int_play = 'Play',
  int_pause = 'Pause',
  int_stop = 'Stop',
  int_status = 'Reproduction status',
}

local dlg = nil

function activate()    --Initialization
    vlc.msg.dbg("StreamIt init") --Debug message
    launch_main_menu()
end


function close()
    vlc.msg.dbg("StreamIt closing") --Debug message
    vlc.deactivate()
end


function close_dlg() --Close a dialog so another one can open
  vlc.msg.dbg("StreamIt closing dialog") --Debug message

  if dlg ~= nil then 
    dlg:hide() 
  end
  
  dlg = nil
  collectgarbage() --important 
end


function launch_main_menu()
  close_dlg()
  dlg = vlc.dialog("Stream It!")
  list_devices = dlg:add_list(1, 1, 4, 4) 

  button_search = dlg:add_button(translation["int_search_devices"], search_devices, 5, 1, 2, 1)
  button_help = dlg:add_button(translation["int_help"], dumb, 8, 1, 1, 1)

  label_search = dlg:add_label(translation["int_message"], 5, 2, 2, 1)
  button_about = dlg:add_button(translation["int_about"], dumb, 8, 2, 1, 1)

  html_selectedinfo = dlg:add_html(translation["int_selectedinfo"], 5, 3, 2, 2) --TODO: extraerlo a un texto/funcion aparte para actualziarlo con facilidad

  button_streamit = dlg:add_button(translation["int_streamit"], launch_control_menu, 1, 5, 4, 1)
  button_close = dlg:add_button(translation["int_close"], close, 6, 5, 3, 1)

  dlg:show()
end    


function launch_control_menu()
  close_dlg()
  dlg = vlc.dialog("Streaming to :"..translation["int_renderer_name_dumb"])
  
  html_fileInfo = dlg:add_html(translation["int_fileInfo"], 1, 1, 6, 4) --TODO: extraerlo a un texto/funcion aparte para actualziarlo con facilidad

  button_volumeUp = dlg:add_button(translation["int_volumeUp"], dumb, 7, 1, 1, 1)
  button_volumeDown = dlg:add_button(translation["int_volumeDown"], dumb, 7, 4, 1, 1)

  html_rendererInfo = dlg:add_html(translation["int_rendererInfo"], 8, 1, 3, 4) --TODO: extraerlo a un texto/funcion aparte para actualziarlo con facilidad
  
  button_play = dlg:add_button(translation["int_play"], dumb, 1, 5, 2, 1)
  button_pause = dlg:add_button(translation["int_pause"], dumb, 3, 5, 2, 1)
  button_stopp = dlg:add_button(translation["int_stop"], dumb, 5, 5, 2, 1)
  button_close = dlg:add_button(translation["int_close"], close, 9, 5, 2, 1)

  label_search = dlg:add_label(translation["int_status"], 1, 6, 10, 1)

  dlg:show()
end 


function search_devices() --Search devices on the LAN   
    --TODO: develop this 
end


function dumb()    
    --TODO: delete this function
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
--]]

--    var.libvlc_command( name, argument ): 
--    sd.get_services_names()



 --[[
function deactivate() --TODO: Useful??
  vlc.msg.dbg("StreamIt closing") --Debug message
  if dlg then
    dlg:hide() 
  end
end]]