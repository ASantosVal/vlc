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
    dlg = vlc.dialog("Stream this!!!")
    button_play = dlg:add_button(translation["int_search_devices"], search_devices, 1, 4, 4, 1) 
    list = dlg:add_list(1, 3, 4, 1)    

    var.libvlc_command( name, argument ): 
    --[[sd.get_services_names()--]]
   
    dlg:show()
end

function search_devices()    
    if (not selection) then return 1 end   
end

function deactivate()
end

function close()
    vlc.deactivate()
end


