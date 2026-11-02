#!/bin/sh
# Сборка C++ логики питомца в WebAssembly.
# Нужен Emscripten: https://emscripten.org/docs/getting_started/downloads.html
#
# Результат - файл web/pet.js (WebAssembly встроен прямо в него).
# Запускать из папки tg-app:  sh build.sh

emcc game.cpp -O2 \
    -sSINGLE_FILE=1 \
    -sEXPORTED_RUNTIME_METHODS=ccall \
    -sEXPORTED_FUNCTIONS=_newGame,_setName,_loadGame,_getResult,_doAction,_getDefeatReason,_getStageName,_getHunger,_getHappiness,_getEnergy,_getHealth,_getCleanliness,_getAge,_getDaysToGrowUp \
    -o web/pet.js
