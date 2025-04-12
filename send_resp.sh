#!/bin/bash

# Dirección y puerto del servidor
HOST="localhost"
PORT=6379

# Comando RESP que quieras enviar
# Puedes modificar esta variable para probar otros comandos
COMMAND="*3\r\n\$3\r\nSET\r\n\$3\r\nfoo\r\n\$3\r\nbar\r\n"

# Enviar el comando al servidor y mostrar respuesta
echo -e "$COMMAND" | nc $HOST $PORT
