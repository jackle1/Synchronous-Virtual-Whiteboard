SERVER = 'https://test-wifi.fly.dev'
function make_post(toPost)
    local idk = http.post("http://test-wifi.fly.dev/test",
               'Content-Type: text/plain\r\n',
                toPost,
               function(code, data)
                  uart.write(0, data)
                end
            )
end
