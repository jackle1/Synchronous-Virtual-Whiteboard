SERVER = 'http://192.168.137.1:8080'
function make_post(toPost)
    local idk = http.post("http://192.168.137.1:8080/test",
               'Content-Type: text/plain\r\n',
                toPost,
               function(code, data)
                  uart.write(0, data)
                end
            )
end
