// Шаг 2: у питомца появляются голод и настроение (переменные int).

#include <iostream>
#include <string>
using namespace std;

int main()
{
    string petName = "Бобик";
    int hunger = 50;    // Шкала от 0 (сыт) до 100 (очень голоден)
    int happiness = 50; // Шкала от 0 (грустный) до 100 (очень счастлив)

    cout << "Поздравляем! У вас родился цифровой питомец!" << endl;
    cout << "Его зовут - " << petName << "!" << endl;
    cout << "-------------------------------" << endl;
    // Выводим состояние питомца
    cout << "Голод: " << hunger << "/100" << endl;
    cout << "Настроение: " << happiness << "/100" << endl;
    cout << "-------------------------------" << endl;

    return 0;
}
