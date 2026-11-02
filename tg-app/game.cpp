// Логика игры для версии в Telegram. Правила те же, что в src/main.cpp.
// Emscripten собирает этот файл в WebAssembly, а рисует всё app.js.
//
// extern "C" нужен, чтобы имена функций не искажались при компиляции
// и JavaScript мог вызвать их как "doAction", "getHunger" и т.д.

#include <string>

using namespace std;

// границы для всех шкал
const int MIN_VALUE = 0;
const int MAX_VALUE = 100;

// столько дней надо продержаться, чтобы победить
const int DAYS_TO_GROW_UP = 30;

// что возвращает getResult()
const int GAME_GOING = 0;
const int GAME_VICTORY = 1;
const int GAME_DEFEAT = 2;

string petName = "Бобик";
int hunger = 50;       // 0 сыт, 100 очень голоден
int happiness = 50;
int energy = 80;
int health = 100;
int cleanliness = 80;
int age = 0;           // в днях

// текст для игрока, его выводит app.js
string message = "";

// обрезает значение до 0..100
int limit(int value)
{
    if (value < MIN_VALUE) {
        return MIN_VALUE;
    }
    if (value > MAX_VALUE) {
        return MAX_VALUE;
    }
    return value;
}

// малыш, подросток или взрослый, смотря сколько дней прожил
string getStage(int days)
{
    if (days < 10) {
        return "малыш";
    }
    else if (days < 20) {
        return "подросток";
    }
    else {
        return "взрослый";
    }
}

// дальше функции, которые вызывает app.js
extern "C" {

// всё как при рождении
void newGame()
{
    hunger = 50;
    happiness = 50;
    energy = 80;
    health = 100;
    cleanliness = 80;
    age = 0;
    message = "Поздравляем! У вас родился цифровой питомец!";
}

void setName(const char* name)
{
    petName = name;
    if (petName == "") {
        petName = "Бобик";
    }
}

// продолжить сохранённую игру
void loadGame(int savedHunger, int savedHappiness, int savedEnergy,
              int savedHealth, int savedCleanliness, int savedAge)
{
    hunger = limit(savedHunger);
    happiness = limit(savedHappiness);
    energy = limit(savedEnergy);
    health = limit(savedHealth);
    cleanliness = limit(savedCleanliness);
    age = savedAge;
    message = "С возвращением! " + petName + " скучал по вам.";
}

int getResult()
{
    if (hunger >= 100 || happiness <= 0 || health <= 0) {
        return GAME_DEFEAT;
    }
    if (age >= DAYS_TO_GROW_UP) {
        return GAME_VICTORY;
    }
    return GAME_GOING;
}

// один ход, choice от 1 до 6 как в меню консольной версии.
// возвращает текст для игрока
const char* doAction(int choice)
{
    // игра уже кончилась
    if (getResult() != GAME_GOING) {
        return message.c_str();
    }

    if (choice == 1) {
        message = "Вы покормили " + petName + "! Он доволен.";
        hunger -= 30;
        happiness += 5;
    }
    else if (choice == 2) {
        if (energy < 20) {
            message = petName + " слишком устал, чтобы играть. Уложите его спать.";
        }
        else {
            message = "Вы поиграли с " + petName + "! Он виляет хвостом.";
            happiness += 30;
            hunger += 15;
            energy -= 20;
            cleanliness -= 10;
        }
    }
    else if (choice == 3) {
        message = petName + " сладко спит... Хр-р-р...";
        energy += 40;
        hunger += 10;
    }
    else if (choice == 4) {
        message = "Вы искупали " + petName + ". Он чистый, но немного обиделся на воду.";
        cleanliness += 40;
        happiness -= 5;
    }
    else if (choice == 5) {
        message = "Вы сводили " + petName + " к ветеринару. Укол - это неприятно, но полезно.";
        health += 30;
        happiness -= 10;
    }
    else if (choice == 6) {
        message = "Вы сидите и смотрите на " + petName + ". Он смотрит на вас.";
    }
    else {
        message = "Такой команды нет.";
        return message.c_str();
    }

    // прошёл день
    age += 1;
    hunger += 5;
    happiness -= 5;
    energy -= 5;
    cleanliness -= 5;

    // от плохого ухода он болеет
    if (hunger > 80) {
        message += "\n" + petName + " очень голоден, ему становится плохо!";
        health -= 10;
    }
    if (cleanliness < 20) {
        message += "\n" + petName + " слишком грязный, он может заболеть!";
        health -= 10;
    }
    if (energy < 10) {
        message += "\n" + petName + " валится с ног от усталости!";
        health -= 5;
    }

    hunger = limit(hunger);
    happiness = limit(happiness);
    energy = limit(energy);
    health = limit(health);
    cleanliness = limit(cleanliness);

    return message.c_str();
}

// почему проиграли (пусто, если не проиграли)
const char* getDefeatReason()
{
    if (hunger >= 100) {
        message = petName + " слишком долго был голодным и убежал искать еду.";
    }
    else if (happiness <= 0) {
        message = petName + " очень загрустил и ушёл к другому хозяину.";
    }
    else if (health <= 0) {
        message = petName + " тяжело заболел.";
    }
    else {
        message = "";
    }
    return message.c_str();
}

const char* getStageName()
{
    // static, чтобы строка не пропала после return
    static string stage;
    stage = getStage(age);
    return stage.c_str();
}

// геттеры для app.js
int getHunger() { return hunger; }
int getHappiness() { return happiness; }
int getEnergy() { return energy; }
int getHealth() { return health; }
int getCleanliness() { return cleanliness; }
int getAge() { return age; }
int getDaysToGrowUp() { return DAYS_TO_GROW_UP; }

} // extern "C"
