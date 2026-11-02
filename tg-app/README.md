# Цифровой питомец: Telegram Mini App

Версия игры для Telegram. Правила в `game.cpp` (C++ → WebAssembly), интерфейс в `web/`.

Демо: https://olsh2zz.github.io/mypet/

## Сборка

Требуется [Emscripten](https://emscripten.org).

```bash
sh build.sh
```

Результат: `web/pet.js`. Публикация на GitHub Pages идёт автоматически через `.github/workflows/pages.yml`.

## Структура

```
tg-app/
├── game.cpp      логика игры
├── build.sh      сборка
└── web/
    ├── index.html
    ├── style.css
    ├── app.js
    └── img/
```

Анимации: [Icons8](https://icons8.com).
