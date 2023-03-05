
--[[local s = net.createConnection(net.TCP, 0)
s:on("connection", function ( sck,c )
  print("connected to socket | ", c)
end)
s:connect(3001, "192.168.1.90")
print("Connection: ", s:getpeer())]]--

local wb = require("websocket")
print(wb)
local ws = wb.createClient()
ws:on("close", function(_, status)
  print('connection closed', status)
  ws = nil -- required to Lua gc the websocket client
end)
ws:connect("ws://192.168.137.1:8080")
print("CONNECTED TO ", SERVER)
if (ws) then
    ws:close()
end

print("Completed sockets")
