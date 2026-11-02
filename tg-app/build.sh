#!/bin/sh
# Собирает game.cpp в web/pet.js (wasm лежит прямо внутри файла).
# Нужен Emscripten: https://emscripten.org/docs/getting_started/downloads.html
# Запускать из папки tg-app: sh build.sh

emcc game.cpp -O2 \
    -sSINGLE_FILE=1 \
    -sEXPORTED_RUNTIME_METHODS=ccall \
    -sEXPORTED_FUNCTIONS=_newGame,_setName,_loadGame,_getResult,_doAction,_getDefeatReason,_getStageName,_getHunger,_getHappiness,_getEnergy,_getHealth,_getCleanliness,_getAge,_getDaysToGrowUp \
    -o web/pet.js
