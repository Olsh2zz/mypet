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
// =============================================================

#include <iostream>
#include <string>
#include <cstdlib> // для system()

using namespace std;

// Минимальное и максимальное значение для всех шкал
const int MIN_VALUE = 0;
const int MAX_VALUE = 100;

// Сколько дней нужно прожить, чтобы питомец вырос (победа)
const int DAYS_TO_GROW_UP = 30;

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
    cout << "0 - Выйти из игры" << endl;
    cout << "Ваш выбор: ";
}

int main()
{
    // В консоли Windows включаем UTF-8, иначе русские буквы будут "кракозябрами"
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif

    // ---------- Задание "ввести имя" ----------
    string petName;
    cout << "Как вы назовёте своего питомца? ";
    getline(cin, petName);

    // Если пользователь ничего не ввёл — даём имя по умолчанию
    if (petName == "") {
        petName = "Бобик";
    }

    // ---------- Показатели питомца ----------
    int hunger = 50;       // 0 - сыт, 100 - очень голоден
    int happiness = 50;    // 0 - грустный, 100 - очень счастлив
    int energy = 80;       // 0 - без сил, 100 - полон сил
    int health = 100;      // 0 - тяжело болен, 100 - здоров
    int cleanliness = 80;  // 0 - очень грязный, 100 - чистый
    int age = 0;           // возраст в днях

    // ---------- Рождение питомца ----------
    cout << endl;
    cout << "Поздравляем! У вас родился цифровой питомец!" << endl;
    cout << "Его зовут - " << petName << "!" << endl;
    cout << "Давайте позаботимся о нем." << endl;
    cout << "Цель: помогите ему дожить до " << DAYS_TO_GROW_UP << " дней и вырасти." << endl;

    bool wantExit = false; // станет true, если игрок выберет "Выйти"

    // ---------- Главный цикл игры ----------
    // Игра идёт, пока питомец жив, ещё не вырос и игрок не вышел
    while (hunger < 100 && happiness > 0 && health > 0
           && age < DAYS_TO_GROW_UP && wantExit == false)
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
            wantExit = true;
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

    // ---------- Итог игры ----------
    cout << endl;
    cout << "===============================" << endl;
    if (wantExit == true) {
        cout << "Вы вышли из игры. " << petName << " будет ждать вас!" << endl;
    }
    else if (hunger >= 100) {
        cout << "Ой... " << petName << " слишком долго был голодным и убежал искать еду." << endl;
    }
    else if (happiness <= 0) {
        cout << "Ой... " << petName << " очень загрустил и ушёл к другому хозяину." << endl;
    }
    else if (health <= 0) {
        cout << "Ой... " << petName << " тяжело заболел. Ухаживайте лучше в следующий раз." << endl;
    }
    else {
        cout << "Ура! " << petName << " вырос и стал взрослым! Вы отличный хозяин!" << endl;
    }
    cout << "Питомец прожил дней: " << age << endl;
    cout << "===============================" << endl;

    return 0;
}
