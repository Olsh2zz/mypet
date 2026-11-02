// =============================================================
//  Цифровой питомец — логика игры для Telegram Mini App
//
//  Правила те же, что в консольной версии (src/main.cpp).
//  Этот файл компилируется в WebAssembly (через Emscripten),
//  а кнопки и картинки рисует JavaScript в папке web/.
//
//  extern "C" означает: "оставь имена функций как есть",
//  чтобы JavaScript мог найти их по имени, например "doAction".
// =============================================================

#include <string>

using namespace std;

// Минимальное и максимальное значение для всех шкал
const int MIN_VALUE = 0;
const int MAX_VALUE = 100;

// Сколько дней нужно прожить, чтобы питомец вырос (победа)
const int DAYS_TO_GROW_UP = 30;

// Результат игры
const int GAME_GOING = 0;   // игра идёт
const int GAME_VICTORY = 1; // победа
const int GAME_DEFEAT = 2;  // поражение

// ---------- Показатели питомца ----------
string petName = "Бобик";
int hunger = 50;       // 0 - сыт, 100 - очень голоден
int happiness = 50;    // 0 - грустный, 100 - очень счастлив
int energy = 80;       // 0 - без сил, 100 - полон сил
int health = 100;      // 0 - тяжело болен, 100 - здоров
int cleanliness = 80;  // 0 - очень грязный, 100 - чистый
int age = 0;           // возраст в днях

// Последнее сообщение для игрока (его покажет JavaScript)
string message = "";

// Функция не даёт показателю выйти за границы 0..100
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

// Функция возвращает стадию роста питомца по его возрасту
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

// ---------- Функции, которые вызывает JavaScript ----------
extern "C" {

// Начать новую игру: все показатели как при рождении
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

// Задать имя питомца
void setName(const char* name)
{
    petName = name;
    if (petName == "") {
        petName = "Бобик";
    }
}

// Загрузить сохранённую игру
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

// Узнать, чем закончилась игра
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

// Сделать действие (1..6, как в меню консольной версии).
// Возвращает текст, который нужно показать игроку.
const char* doAction(int choice)
{
    // Если игра уже закончилась, ничего не делаем
    if (getResult() != GAME_GOING) {
        return message.c_str();
    }

    // ---------- Обрабатываем выбор ----------
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

    // ---------- Прошёл один день ----------
    age += 1;
    hunger += 5;       // питомец понемногу голодает
    happiness -= 5;    // и скучает
    energy -= 5;       // и устаёт
    cleanliness -= 5;  // и пачкается

    // На 10-й и 20-й день питомец переходит на новую стадию роста
    if (age == 10 || age == 20) {
        message += "\n🎉 " + petName + " подрос! Теперь он " + getStage(age) + ".";
    }

    // Если за питомцем плохо ухаживать, он начинает болеть
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

    // Показатели не выходят за границы 0..100
    hunger = limit(hunger);
    happiness = limit(happiness);
    energy = limit(energy);
    health = limit(health);
    cleanliness = limit(cleanliness);

    return message.c_str();
}

// Причина поражения (пустая строка, если поражения нет)
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

// Стадия роста: малыш, подросток или взрослый
const char* getStageName()
{
    // static - строка живёт и после выхода из функции
    static string stage;
    stage = getStage(age);
    return stage.c_str();
}

// Простые функции, чтобы JavaScript мог узнать показатели
int getHunger() { return hunger; }
int getHappiness() { return happiness; }
int getEnergy() { return energy; }
int getHealth() { return health; }
int getCleanliness() { return cleanliness; }
int getAge() { return age; }
int getDaysToGrowUp() { return DAYS_TO_GROW_UP; }

} // конец extern "C"
