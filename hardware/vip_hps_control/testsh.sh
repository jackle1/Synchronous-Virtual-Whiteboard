curl -i -N  \
    -H "Connection: Upgrade"  \
        -H "Upgrade: websocket"  \
            -H "Host: 192.168.137.1:8080"  \
                -H "Origin: 192.168.137.1:8080" \
                    http://echo.websocket.org
