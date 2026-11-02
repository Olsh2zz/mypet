// Интерфейс для Telegram. Сама игра считается в game.cpp,
// тут только кнопки и анимации. C++ вызывается через Module.ccall.

// вне Telegram этого объекта может не быть
var tg = null;
if (window.Telegram && window.Telegram.WebApp) {
    tg = window.Telegram.WebApp;
}

var SAVE_KEY = "pet_save";

// имя храним тут, чтобы класть его в сохранение
var petName = "Бобик";

// сколько длятся анимации, мс
var ACTION_TIME = 1600;
var LONG_ACTION_TIME = 3000; // сон и купание

// пока идёт анимация, новые нажатия игнорируем
var busy = false;

var moodClass = "";
var actionClass = "";

// pet.js вызовет это, когда C++ загрузится
var Module = {
    onRuntimeInitialized: function () {
        onCppReady();
    }
};

// функции C++ без параметров: одна возвращает число, другая строку
function cppNumber(name) {
    return Module.ccall(name, "number", [], []);
}

function cppText(name) {
    return Module.ccall(name, "string", [], []);
}

// в браузере initData пустая
function inTelegram() {
    return tg != null && tg.initData != "";
}

function vibrate(type) {
    if (inTelegram() && tg.isVersionAtLeast("6.1")) {
        if (type == "success" || type == "error") {
            tg.HapticFeedback.notificationOccurred(type);
        }
        else {
            tg.HapticFeedback.impactOccurred("light");
        }
    }
}

// CloudStorage появился в Telegram 6.9
function canUseCloud() {
    return inTelegram() && tg.isVersionAtLeast("6.9");
}

// В Telegram сохраняем в облако, в браузере в localStorage.

function saveGame() {
    var data = {
        name: petName,
        hunger: cppNumber("getHunger"),
        happiness: cppNumber("getHappiness"),
        energy: cppNumber("getEnergy"),
        health: cppNumber("getHealth"),
        cleanliness: cppNumber("getCleanliness"),
        age: cppNumber("getAge")
    };
    var text = JSON.stringify(data);

    if (canUseCloud()) {
        tg.CloudStorage.setItem(SAVE_KEY, text);
    }
    else {
        try {
            localStorage.setItem(SAVE_KEY, text);
        } catch (e) {
            // нет хранилища, играем без сохранения
        }
    }
}

// облако отвечает не сразу, поэтому через callback. Пусто, если сохранения нет
function loadSave(callback) {
    if (canUseCloud()) {
        tg.CloudStorage.getItem(SAVE_KEY, function (error, value) {
            if (error || !value) {
                callback("");
            }
            else {
                callback(value);
            }
        });
    }
    else {
        var text = "";
        try {
            text = localStorage.getItem(SAVE_KEY) || "";
        } catch (e) {
            text = "";
        }
        callback(text);
    }
}

function deleteSave() {
    if (canUseCloud()) {
        tg.CloudStorage.removeItem(SAVE_KEY);
    }
    else {
        try {
            localStorage.removeItem(SAVE_KEY);
        } catch (e) {
        }
    }
}


function showScreen(id) {
    document.getElementById("screen-start").classList.add("hidden");
    document.getElementById("screen-game").classList.add("hidden");
    document.getElementById("screen-end").classList.add("hidden");
    document.getElementById(id).classList.remove("hidden");
}


function onCppReady() {
    if (tg != null) {
        tg.ready();
        tg.expand();
    }

    document.getElementById("days-total").textContent = cppNumber("getDaysToGrowUp");

    document.getElementById("btn-start").onclick = startNewGame;
    document.getElementById("btn-continue").onclick = continueGame;
    document.getElementById("btn-new").onclick = function () {
        document.getElementById("continue-box").classList.add("hidden");
        document.getElementById("new-box").classList.remove("hidden");
    };
    document.getElementById("btn-again").onclick = function () {
        document.getElementById("name-input").value = petName;
        document.getElementById("continue-box").classList.add("hidden");
        document.getElementById("new-box").classList.remove("hidden");
        showScreen("screen-start");
    };

    // номер действия лежит в data-action
    var buttons = document.querySelectorAll(".action");
    for (var i = 0; i < buttons.length; i++) {
        buttons[i].onclick = function () {
            doAction(Number(this.getAttribute("data-action")));
        };
    }

    loadSave(function (text) {
        if (text == "") {
            return;
        }
        var data = JSON.parse(text);
        document.getElementById("continue-text").textContent =
            data.name + " ждёт вас! День " + data.age + ".";
        document.getElementById("continue-box").classList.remove("hidden");
        document.getElementById("new-box").classList.add("hidden");
        // пригодится, когда нажмут "Продолжить"
        document.getElementById("btn-continue").saveData = data;
    });
}

function startNewGame() {
    petName = document.getElementById("name-input").value.trim();
    if (petName == "") {
        petName = "Бобик";
    }

    Module.ccall("setName", null, ["string"], [petName]);
    Module.ccall("newGame", null, [], []);

    vibrate("light");
    showScreen("screen-game");
    document.getElementById("message").textContent =
        "Поздравляем! У вас родился цифровой питомец! Его зовут - " + petName + "!";
    updateScreen();
    saveGame();
}

function continueGame() {
    var data = document.getElementById("btn-continue").saveData;
    petName = data.name;

    Module.ccall("setName", null, ["string"], [petName]);
    Module.ccall("loadGame", null,
        ["number", "number", "number", "number", "number", "number"],
        [data.hunger, data.happiness, data.energy, data.health, data.cleanliness, data.age]);

    vibrate("light");
    showScreen("screen-game");
    document.getElementById("message").textContent = "С возвращением! " + petName + " скучал по вам.";
    updateScreen();
}


function doAction(choice) {
    if (busy) {
        return;
    }
    busy = true;
    vibrate("light");

    // при энергии меньше 20 C++ не даст играть, нужна другая анимация
    var tooTired = (choice == 2 && cppNumber("getEnergy") < 20);

    var text = Module.ccall("doAction", "string", ["number"], [choice]);
    document.getElementById("message").textContent = text;

    if (tooTired) {
        playAnimation(0);
    }
    else {
        playAnimation(choice);
    }
    updateScreen();

    var animationTime = ACTION_TIME;
    if (!tooTired && (choice == 3 || choice == 4)) {
        animationTime = LONG_ACTION_TIME;
    }

    // после анимации проверяем, не конец ли игры
    setTimeout(function () {
        busy = false;
        setActionClass("");
        setDog("img/puppy.gif");

        var result = cppNumber("getResult");
        if (result == 0) {
            saveGame();
        }
        else {
            deleteSave();
            showEnding(result);
        }
    }, animationTime);
}


function updateScreen() {
    document.getElementById("pet-name").textContent = petName;
    document.getElementById("pet-stage").textContent = cppText("getStageName");
    document.getElementById("pet-day").textContent = cppNumber("getAge");

    // у голода плохо, когда много, у остальных, когда мало
    updateBar("bar-hunger", cppNumber("getHunger"), true);
    updateBar("bar-happiness", cppNumber("getHappiness"), false);
    updateBar("bar-energy", cppNumber("getEnergy"), false);
    updateBar("bar-health", cppNumber("getHealth"), false);
    updateBar("bar-cleanliness", cppNumber("getCleanliness"), false);

    updateMood();
}

function updateBar(id, value, highIsBad) {
    var bar = document.getElementById(id);
    bar.querySelector(".bar-fill").style.width = value + "%";
    bar.querySelector(".bar-value").textContent = value;

    var danger = 100 - value;
    if (highIsBad) {
        danger = value;
    }

    bar.classList.remove("warn");
    bar.classList.remove("bad");
    if (danger >= 70) {
        bar.classList.add("bad");
    }
    else if (danger >= 50) {
        bar.classList.add("warn");
    }
}

// значок в углу сцены и как щенок двигается
function updateMood() {
    var icon = "";
    moodClass = "";

    if (cppNumber("getHealth") < 30) {
        icon = "🤒";
        moodClass = "sick";
    }
    else if (cppNumber("getHunger") > 70) {
        icon = "🍖❓";
        moodClass = "sad";
    }
    else if (cppNumber("getHappiness") < 30) {
        icon = "😢";
        moodClass = "sad";
    }
    else if (cppNumber("getEnergy") < 20) {
        icon = "🥱";
        moodClass = "tired";
    }
    else if (cppNumber("getCleanliness") < 30) {
        icon = "🪰";
    }
    else {
        icon = "💛";
    }

    document.getElementById("mood").textContent = icon;
    applySceneClass();
}

function setActionClass(name) {
    actionClass = name;
    applySceneClass();
}

function applySceneClass() {
    document.getElementById("scene").className = "scene " + moodClass + " " + actionClass;
}

function setDog(src) {
    var dog = document.getElementById("dog");
    // если подставить тот же src, GIF начнётся сначала
    if (dog.getAttribute("src") != src) {
        dog.setAttribute("src", src);
    }
}

// летающий эмодзи на сцене
function spawn(container, emoji, animation, left, bottom, delay, duration) {
    var p = document.createElement("div");
    p.className = "particle";
    p.textContent = emoji;
    p.style.left = left + "%";
    p.style.bottom = bottom + "px";
    p.style.animation = animation + " " + duration + "s ease-out " + delay + "s both";
    container.appendChild(p);

    setTimeout(function () {
        p.remove();
    }, (delay + duration) * 1000 + 100);
}

// ванна нарисована в CSS, пена из белых кружков
function makeBath(container) {
    var tub = document.createElement("div");
    tub.className = "tub";

    for (var i = 0; i < 9; i++) {
        var foam = document.createElement("div");
        foam.className = "foam";
        foam.style.left = (i * 11 - 4) + "%";
        foam.style.animationDelay = (i * 0.1) + "s";
        tub.appendChild(foam);
    }

    container.appendChild(tub);

    setTimeout(function () {
        tub.remove();
    }, LONG_ACTION_TIME);
}

function makeBed(container) {
    var bed = document.createElement("div");
    bed.className = "bed";
    container.appendChild(bed);

    setTimeout(function () {
        bed.remove();
    }, LONG_ACTION_TIME);
}

function playAnimation(choice) {
    var fx = document.getElementById("effects");
    var i;

    if (choice == 1) {
        // покормить
        setActionClass("act-feed");
        spawn(fx, "🥣", "slide-in-right", 72, 10, 0, 1.5);
        spawn(fx, "🍖", "rise", 74, 40, 0.3, 1.2);
    }
    else if (choice == 2) {
        // поиграть
        setActionClass("act-play");
        spawn(fx, "🎾", "bounce-ball", 45, 40, 0, 1.4);
        spawn(fx, "💛", "rise", 70, 120, 0.4, 1.1);
    }
    else if (choice == 3) {
        // спать: подъезжает лежанка, щенок ложится, через полсекунды засыпает
        makeBed(fx);
        setActionClass("lying-down");
        setTimeout(function () {
            setDog("img/sleep.gif");
            setActionClass("sleeping");
            for (var j = 0; j < 4; j++) {
                spawn(fx, "💤", "rise", 62 + j * 6, 130, j * 0.4, 1.2);
            }
        }, 500);
    }
    else if (choice == 4) {
        // купание: ванна успевает подъехать, потом щенок в пене
        makeBath(fx);
        setTimeout(function () {
            setActionClass("act-bath");
            for (var j = 0; j < 12; j++) {
                spawn(fx, "🫧", "rise", 25 + (j % 6) * 9, 70, j * 0.15, 1.2);
            }
        }, 500);
    }
    else if (choice == 5) {
        // полечить
        setActionClass("act-heal");
        spawn(fx, "💉", "slide-in-right", 70, 90, 0, 1.4);
        for (i = 0; i < 4; i++) {
            spawn(fx, "❤️", "rise", 55 + i * 8, 100, 0.3 + i * 0.2, 1.1);
        }
    }
    else if (choice == 6) {
        setActionClass("act-sit");
        spawn(fx, "🙂", "rise", 65, 140, 0, 1.4);
    }
    else {
        // устал и не хочет играть
        setActionClass("act-refuse");
        spawn(fx, "🥱", "rise", 62, 140, 0, 1.4);
    }
}


function showEnding(result) {
    var screen = document.getElementById("screen-end");
    var fx = document.getElementById("end-effects");
    var i;

    fx.innerHTML = "";
    document.getElementById("end-days").textContent = "Питомец прожил дней: " + cppNumber("getAge");

    if (result == 1) {
        screen.className = "screen victory";
        document.getElementById("end-title").textContent = "ГАВ-ГАВ!";
        document.getElementById("end-dog").setAttribute("src", "img/puppy.gif");
        document.getElementById("end-text").textContent =
            petName + " вырос и стал взрослым! Вы отличный хозяин!";
        var bone = document.createElement("div");
        bone.className = "particle bone";
        bone.textContent = "🦴";
        fx.appendChild(bone);
        for (i = 0; i < 16; i++) {
            spawn(fx, "🎉", "fall", (i * 37) % 95, 220, i * 0.15, 2);
        }
        vibrate("success");
    }
    else {
        screen.className = "screen defeat";
        document.getElementById("end-title").textContent = "ПОРАЖЕНИЕ...";
        document.getElementById("end-dog").setAttribute("src", "img/sleep.gif");
        document.getElementById("end-text").textContent = "Причина: " + cppText("getDefeatReason");
        vibrate("error");
    }

    showScreen("screen-end");
}
