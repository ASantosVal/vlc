function descriptor()
  return { title = 'Binding_C_Test',
           version = '0.1',
           author = 'Asier Santos',
           capabilities = {} 
         }
end

require("share.lua.extensions.lib")


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

  result = square(2)

  dlg:add_label('square 2: '..result, 1, 1, 2, 1)

  result = justOne()

  dlg:add_label('one: '..result, 1, 2, 2, 1)

  result = cube(2)

  dlg:add_label('cube 2: '..result, 1, 3, 2, 1)

  result = return_same(256)

  dlg:add_label('256: '..result, 1, 4, 2, 1)

  dlg:show()
end    
