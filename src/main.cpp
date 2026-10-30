// =============================================================
//  Цифровой питомец — итоговая версия
//  Выполнены все задания практической работы:
//    1. Рождение питомца (cout, string)
//    2. Показатели питомца (int)
//    3. Диалог с пользователем (cin, if / else if)
//  Самостоятельная работа:
//    - показатели не выходят за границы 0..100
//    - третье действие "Уложить спать" (и ещё несколько)
//    - пользователь сам вводит имя питомца
//    - цикл while: игра идёт, пока питомец "жив"
//  Свои дополнительные характеристики:
//    - энергия, здоровье, чистота, возраст (в днях)
//  Ещё дополнительно:
//    - две концовки: победа и поражение (с причиной)
//    - пауза: игра сохраняется в файл и продолжается с того же места
// =============================================================

#include <iostream>
#include <fstream> // для работы с файлом сохранения
#include <string>
#include <cstdio>  // для remove() - удаление файла
#include <cstdlib> // для system()

using namespace std;

// Минимальное и максимальное значение для всех шкал
const int MIN_VALUE = 0;
const int MAX_VALUE = 100;

// Сколько дней нужно прожить, чтобы питомец вырос (победа)
const int DAYS_TO_GROW_UP = 30;

// Имя файла, в который сохраняется игра на паузе
const string SAVE_FILE = "pet_save.txt";

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
string getStage(int age)
{
    if (age < 10) {
        return "малыш";
    }
    else if (age < 20) {
        return "подросток";
    }
    else {
        return "взрослый";
    }
}

// Функция выводит все показатели питомца
void showStatus(string petName, int hunger, int happiness, int energy,
                int health, int cleanliness, int age)
{
    cout << "-------------------------------" << endl;
    cout << "Питомец:    " << petName << " (" << getStage(age) << ")" << endl;
    cout << "Возраст:    " << age << " дн." << endl;
    cout << "Голод:      " << hunger << "/100" << endl;
    cout << "Настроение: " << happiness << "/100" << endl;
    cout << "Энергия:    " << energy << "/100" << endl;
    cout << "Здоровье:   " << health << "/100" << endl;
    cout << "Чистота:    " << cleanliness << "/100" << endl;
    cout << "-------------------------------" << endl;
}

// Функция выводит меню действий
void showMenu()
{
    cout << "Что вы хотите сделать?" << endl;
    cout << "1 - Покормить" << endl;
    cout << "2 - Поиграть" << endl;
    cout << "3 - Уложить спать" << endl;
    cout << "4 - Искупать" << endl;
    cout << "5 - Полечить" << endl;
    cout << "6 - Просто посидеть рядом" << endl;
    cout << "0 - Пауза (сохранить и выйти)" << endl;
    cout << "Ваш выбор: ";
}

// Концовка 1: ПОБЕДА (собачка радуется косточке)
void showVictory(string petName)
{
    cout << endl;
    cout << "*************************************" << endl;
    cout << "*             ГАВ-ГАВ!              *" << endl;
    cout << "*************************************" << endl;
    cout << "        / \\__" << endl;
    cout << "       (    ^\\___" << endl;
    cout << "       /         O     o\\_______/o" << endl;
    cout << "      /   (_____/      o/_______\\o" << endl;
    cout << "     /_____/   U" << endl;
    cout << endl;
    cout << petName << " вырос и стал взрослым!" << endl;
    cout << "Вы отличный хозяин!" << endl;
}

// Концовка 2: ПОРАЖЕНИЕ (reason - причина поражения)
void showDefeat(string petName, string reason)
{
    cout << endl;
    cout << "*************************************" << endl;
    cout << "*            ПОРАЖЕНИЕ...           *" << endl;
    cout << "*************************************" << endl;
    cout << "        / \\__" << endl;
    cout << "       (    T\\___" << endl;
    cout << "       /         O     Ску-ку..." << endl;
    cout << "      /   (_____/" << endl;
    cout << "     /_____/" << endl;
    cout << endl;
    cout << "Причина: " << petName << " " << reason << endl;
    cout << "Попробуйте ещё раз и ухаживайте лучше!" << endl;
}

int main()
{
    // В консоли Windows включаем UTF-8, иначе русские буквы будут "кракозябрами"
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif

    // ---------- Показатели питомца ----------
    string petName = "Бобик";
    int hunger = 50;       // 0 - сыт, 100 - очень голоден
    int happiness = 50;    // 0 - грустный, 100 - очень счастлив
    int energy = 80;       // 0 - без сил, 100 - полон сил
    int health = 100;      // 0 - тяжело болен, 100 - здоров
    int cleanliness = 80;  // 0 - очень грязный, 100 - чистый
    int age = 0;           // возраст в днях

    // ---------- Есть ли сохранённая игра? ----------
    bool loaded = false; // станет true, если игру загрузили из файла

    ifstream saveIn(SAVE_FILE);
    if (saveIn.is_open()) {
        cout << "Найдена сохранённая игра. Продолжить? (1 - да, 2 - нет): ";
        int answer;
        cin >> answer;
        // Убираем из ввода всё до конца строки, чтобы потом сработал getline
        cin.clear();
        cin.ignore(10000, '\n');

        if (answer == 1) {
            // Читаем данные в том же порядке, в каком сохраняли
            getline(saveIn, petName);
            saveIn >> hunger >> happiness >> energy >> health >> cleanliness >> age;

            if (saveIn.fail()) {
                cout << "Сохранение повреждено, начинаем новую игру." << endl;
                petName = "Бобик";
                hunger = 50;
                happiness = 50;
                energy = 80;
                health = 100;
                cleanliness = 80;
                age = 0;
            }
            else {
                loaded = true;
            }
        }
    }
    saveIn.close();

    if (loaded == true) {
        cout << endl;
        cout << "С возвращением! " << petName << " скучал по вам." << endl;
    }
    else {
        // ---------- Задание "ввести имя" ----------
        cout << "Как вы назовёте своего питомца? ";
        string inputName;
        getline(cin, inputName);

        // Если пользователь ничего не ввёл - остаётся имя по умолчанию
        if (inputName != "") {
            petName = inputName;
        }

        // ---------- Рождение питомца ----------
        cout << endl;
        cout << "Поздравляем! У вас родился цифровой питомец!" << endl;
        cout << "Его зовут - " << petName << "!" << endl;
        cout << "Давайте позаботимся о нем." << endl;
        cout << "Цель: помогите ему дожить до " << DAYS_TO_GROW_UP << " дней и вырасти." << endl;
    }

    bool paused = false; // станет true, если игрок поставит игру на паузу

    // ---------- Главный цикл игры ----------
    // Игра идёт, пока питомец жив, ещё не вырос и игрок не нажал паузу
    while (hunger < 100 && happiness > 0 && health > 0
           && age < DAYS_TO_GROW_UP && paused == false)
    {
        showStatus(petName, hunger, happiness, energy, health, cleanliness, age);
        showMenu();

        int choice;
        cin >> choice;

        // Если ввели не число (например, букву) — просим повторить
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Нужно ввести число из меню!" << endl;
            continue;
        }

        cout << endl;

        // ---------- Обрабатываем выбор ----------
        if (choice == 1) {
            cout << "Вы покормили " << petName << "! Он доволен." << endl;
            hunger -= 30;
            happiness += 5;
        }
        else if (choice == 2) {
            if (energy < 20) {
                cout << petName << " слишком устал, чтобы играть. Уложите его спать." << endl;
            }
            else {
                cout << "Вы поиграли с " << petName << "! Он виляет хвостом." << endl;
                happiness += 30;
                hunger += 15;
                energy -= 20;
                cleanliness -= 10;
            }
        }
        else if (choice == 3) {
            cout << petName << " сладко спит... Хр-р-р..." << endl;
            energy += 40;
            hunger += 10;
        }
        else if (choice == 4) {
            cout << "Вы искупали " << petName << ". Он чистый, но немного обиделся на воду." << endl;
            cleanliness += 40;
            happiness -= 5;
        }
        else if (choice == 5) {
            cout << "Вы сводили " << petName << " к ветеринару. Укол - это неприятно, но полезно." << endl;
            health += 30;
            happiness -= 10;
        }
        else if (choice == 6) {
            cout << "Вы сидите и смотрите на " << petName << ". Он смотрит на вас." << endl;
        }
        else if (choice == 0) {
            paused = true;
            continue;
        }
        else {
            cout << "Такой команды нет. Попробуйте ещё раз." << endl;
            continue;
        }

        // ---------- Прошёл один день ----------
        age += 1;
        hunger += 5;       // питомец понемногу голодает
        happiness -= 5;    // и скучает
        energy -= 5;       // и устаёт
        cleanliness -= 5;  // и пачкается

        // Если за питомцем плохо ухаживать, он начинает болеть
        if (hunger > 80) {
            cout << petName << " очень голоден, ему становится плохо!" << endl;
            health -= 10;
        }
        if (cleanliness < 20) {
            cout << petName << " слишком грязный, он может заболеть!" << endl;
            health -= 10;
        }
        if (energy < 10) {
            cout << petName << " валится с ног от усталости!" << endl;
            health -= 5;
        }

        // ---------- Задание "границы 0..100" ----------
        hunger = limit(hunger);
        happiness = limit(happiness);
        energy = limit(energy);
        health = limit(health);
        cleanliness = limit(cleanliness);
    }

    // ---------- Пауза: сохраняем игру в файл ----------
    if (paused == true) {
        ofstream saveOut(SAVE_FILE);
        saveOut << petName << endl;
        saveOut << hunger << " " << happiness << " " << energy << " "
                << health << " " << cleanliness << " " << age << endl;
        saveOut.close();

        cout << "Игра на паузе и сохранена." << endl;
        cout << petName << " будет ждать вас! Запустите программу снова, чтобы продолжить." << endl;
        return 0;
    }

    // ---------- Игра закончилась: сохранение больше не нужно ----------
    remove(SAVE_FILE.c_str());

    // ---------- Две концовки: поражение или победа ----------
    if (hunger >= 100) {
        showDefeat(petName, "слишком долго был голодным и убежал искать еду.");
    }
    else if (happiness <= 0) {
        showDefeat(petName, "очень загрустил и ушёл к другому хозяину.");
    }
    else if (health <= 0) {
        showDefeat(petName, "тяжело заболел.");
    }
    else {
        showVictory(petName);
    }

    // Итоговые показатели
    cout << endl;
    cout << "Итоги игры:" << endl;
    showStatus(petName, hunger, happiness, energy, health, cleanliness, age);

    return 0;
}
