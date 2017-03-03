function descriptor()
  return { title = 'Test',
           version = '0.1',
           author = 'Asier Santos',
           capabilities = {} 
         }
end


local dlg = nil

function activate()    --Initialization
  launch_main_menu()
end


function close()
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


function launch_main_menu()
  close_dlg()
  vlc.msg.dbg('[StreamIt] launching main menu') --Debug message
  
  dlg = vlc.dialog('Prueba C + LUA')
  dlg:add_label('unimplemented', 1, 1, 2, 1)

  dlg:show()
end    
