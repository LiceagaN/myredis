#!/bin/bash

# Dirección y puerto del servidor
HOST="localhost"
PORT=6379

# Comando RESP que quieras enviar

COMMAND="*2\r\n\$4\r\nINCR\r\n\$3\r\nfoo\r\n"

# Enviar comando al servidor y mostrar la respuesta
echo -e "$COMMAND" | nc $HOST $PORT