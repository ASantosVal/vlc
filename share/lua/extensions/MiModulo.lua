function descriptor()
    return { title = "StreamThis",
             version = "0.1",
             author = "Asier Santos",
             capabilities = {} }
end

local translation = {
    int_search_devices = 'Buscar dispositivos'
}


function activate()    
    dlg = vlc.dialog("Stream It!")
--    button_play = dlg:add_button(translation["int_search_devices"], search_devices, 1, 4, 4, 1) 
    lis_devices = dlg:add_list(1, 1, 4, 4) 

    button_search = dlg:add_button("Search Devices", search_devices, 5, 1, 2, 1)
    label_search = dlg:add_label("Message", 5, 2, 2, 1)

    html_selectedinfo = dlg:add_html("Info about the selected renderer", 5, 3, 2, 2) --TODO: extraerlo a un texto/funcion aparte para actualziarlo con facilidad

    button_help = dlg:add_button("Help", dumb, 8, 1, 1, 1)
    button_about = dlg:add_button("About", dumb, 8, 2, 1, 1)

    button_streamit = dlg:add_button("Stream It!", dumb, 1, 5, 2, 1)
    button_close = dlg:add_button("Close", dumb, 7, 5, 1, 1)
    

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

   
    dlg:show()
end

function search_devices()    
    --TODO: develop this 
end

function dumb()    
    --TODO: delete this function
end

function deactivate()
end

function close()
    vlc.deactivate()
end


