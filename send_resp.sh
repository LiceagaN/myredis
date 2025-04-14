#!/bin/bash

# Dirección y puerto del servidor
HOST="localhost"
PORT=6379

# Comando RESP que quieras enviar

COMMAND="*3\r\n\$3\r\nSET\r\n\$3\r\nfoo\r\n\$3\r\nbar\r\n"
echo -e "$COMMAND" | nc $HOST $PORT
COMMAND="*3\r\n\$6\r\nEXPIRE\r\n\$3\r\nfoo\r\n\$1\r\n5\r\n"
echo -e "$COMMAND" | nc $HOST $PORT
COMMAND="*2\r\n\$3\r\nTTL\r\n\$3\r\nfoo\r\n"
echo -e "$COMMAND" | nc $HOST $PORT