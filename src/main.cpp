// Цифровой питомец, практическая работа №1.
// Тут собраны все шаги из методички и задания для самостоятельной работы.
// От себя добавил энергию, здоровье, чистоту, возраст, паузу с сохранением
// и две концовки.

#include <iostream>
#include <fstream> // файл сохранения
#include <string>
#include <cstdio>  // remove()
#include <cstdlib> // system()

using namespace std;

// границы для всех шкал
const int MIN_VALUE = 0;
const int MAX_VALUE = 100;

// столько дней надо продержаться, чтобы победить
const int DAYS_TO_GROW_UP = 30;

// сюда пишется игра, если нажать паузу
const string SAVE_FILE = "pet_save.txt";

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

// победа: собака с косточкой
void showVictory(string petName)
{
    cout << endl;
    cout << "*************************************" << endl;
    cout << "*             ГАВ-ГАВ!              *" << endl;
    cout << "*************************************" << endl;
    cout << "        / \\__" << endl;
    cout << "       (    ^\\___" << endl;
    cout << "       /         O     o\\_______/o" << endl;
    cout << "      /   (_____/      o/       \\o" << endl;
    cout << "     /_____/   U" << endl;
    cout << endl;
    cout << petName << " вырос и стал взрослым!" << endl;
    cout << "Вы отличный хозяин!" << endl;
}

// поражение, reason - почему проиграли
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
    // без этого в консоли Windows вместо русских букв кракозябры
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif

    string petName = "Бобик";
    int hunger = 50;       // 0 сыт, 100 очень голоден
    int happiness = 50;
    int energy = 80;
    int health = 100;
    int cleanliness = 80;
    int age = 0;           // в днях

    // если есть файл сохранения, предлагаем продолжить
    bool loaded = false;

    ifstream saveIn(SAVE_FILE);
    if (saveIn.is_open()) {
        cout << "Найдена сохранённая игра. Продолжить? (1 - да, 2 - нет): ";
        int answer;
        cin >> answer;
        // хвост строки после числа мешает getline, выкидываем его
        cin.clear();
        cin.ignore(10000, '\n');

        if (answer == 1) {
            // порядок тот же, что при записи
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
        cout << "Как вы назовёте своего питомца? ";
        string inputName;
        getline(cin, inputName);

        // пустое имя - остаётся Бобик
        if (inputName != "") {
            petName = inputName;
        }

        cout << endl;
        cout << "Поздравляем! У вас родился цифровой питомец!" << endl;
        cout << "Его зовут - " << petName << "!" << endl;
        cout << "Давайте позаботимся о нем." << endl;
        cout << "Цель: помогите ему дожить до " << DAYS_TO_GROW_UP << " дней и вырасти." << endl;
    }

    bool paused = false;

    // играем, пока питомец жив, не дожил до 30 дней и не нажата пауза
    while (hunger < 100 && happiness > 0 && health > 0
           && age < DAYS_TO_GROW_UP && paused == false)
    {
        showStatus(petName, hunger, happiness, energy, health, cleanliness, age);
        showMenu();

        int choice;
        cin >> choice;

        // ввели букву вместо числа
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Нужно ввести число из меню!" << endl;
            continue;
        }

        cout << endl;

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

        // прошёл день
        age += 1;
        hunger += 5;
        happiness -= 5;
        energy -= 5;
        cleanliness -= 5;

        // от плохого ухода он болеет
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

        hunger = limit(hunger);
        happiness = limit(happiness);
        energy = limit(energy);
        health = limit(health);
        cleanliness = limit(cleanliness);
    }

    // пауза: записываем всё в файл и выходим
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

    // игра кончилась, старое сохранение больше не нужно
    remove(SAVE_FILE.c_str());

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

    cout << endl;
    cout << "Итоги игры:" << endl;
    showStatus(petName, hunger, happiness, energy, health, cleanliness, age);

    return 0;
}
