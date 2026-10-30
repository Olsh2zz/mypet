// Шаг 1. "Рождение" питомца
// Задача: вывести на экран приветствие и имя питомца.
// Концепции: #include <iostream>, using namespace std;, int main(), cout <<, string.

// Это магия, которая позволяет нам печатать текст
#include <iostream>
#include <string>
// Чтобы не писать везде std::
using namespace std;

// Главный "дом" для нашей программы
int main()
{
    // Объявляем имя питомца
    string petName = "Бобик";

    // Рождение питомца!
    cout << "Поздравляем! У вас родился цифровой питомец!" << endl;
    cout << "Его зовут - " << petName << "!" << endl;
    cout << "Давайте позаботимся о нем." << endl;

    return 0;
}
