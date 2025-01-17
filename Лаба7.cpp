#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <thread>
#include <chrono>
#include <stdexcept>
#include <memory>
#include <windows.h>

using namespace std;

// Абстрактный класс Person
class Person {
protected:
    string firstName;
    string lastName;

public:
    virtual string getLastName() const = 0;
    virtual void display() const = 0;

    virtual ~Person() = default; // Виртуальный деструктор
};

// Интерфейс Identifiable
class Identifiable {
public:
    virtual string getId() const = 0;
};

// Класс Appearance
class Appearance {
private:
    string hairColor;
    string clothes;

public:
    Appearance() : hairColor("Unknown"), clothes("Unknown") {}

    void setAppearance(const string& hair, const string& cloth) {
        hairColor = hair;
        clothes = cloth;
    }

    string getHairColor() const {
        return hairColor;
    }

    string getClothes() const {
        return clothes;
    }
};

// Класс Resident
class Resident : public Person, public Identifiable {
protected:
    string id; // Идентификационный номер из 2 цифр
    string phone; // Модификатор protected
    Appearance appearance;

public:
    Resident() : id("00"), phone("Unknown") {
        firstName = "Unknown";
        lastName = "Unknown";
    }

    void setResident(const string& first, const string& last, const string& idNum, const string& hair, const string& cloth, const string& phoneNum) {
        firstName = first;
        lastName = last;
        id = idNum;
        appearance.setAppearance(hair, cloth);
        phone = phoneNum;
    }

    string getFirstName() const { return firstName; }
    string getLastName() const override { return lastName; }
    string getId() const override { return id; }
    string getPhone() const { return phone; }
    Appearance getAppearance() const { return appearance; }

    string toString() const {
        return "Resident: " + firstName + " " + lastName + ", ID: " + id + ", Phone: " + phone;
    }

    void display() const override {
        cout << toString() << endl;
    }
};

// Производный класс VIPResident
class VIPResident : public Resident {
private:
    string vipPhone;

public:
    VIPResident(const string& first, const string& last, const string& idNum, const string& hair, const string& cloth, const string& phoneNum, const string& vipPhone) {
        setResident(first, last, idNum, hair, cloth, phoneNum);
        this->vipPhone = vipPhone;
    }

    string getVipPhone() const {
        return vipPhone;
    }

    string toString() const {
        return Resident::toString() + ", VIP Phone: " + vipPhone;
    }

    void display() const override {
        cout << toString() << endl;
    }
};

// Класс VisitorPass
class VisitorPass : public Person, public Identifiable {
private:
    string id;
    Appearance appearance;

public:
    VisitorPass() : id("00") {
        firstName = "Unknown";
        lastName = "Unknown";
    }

    void setVisitor(const string& first, const string& last, const string& idNum, const string& hair, const string& cloth) {
        firstName = first;
        lastName = last;
        id = idNum;
        appearance.setAppearance(hair, cloth);
    }

    string getFirstName() const { return firstName; }
    string getLastName() const override { return lastName; }
    string getId() const override { return id; }
    Appearance getAppearance() const { return appearance; }

    void display() const override {
        cout << "Visitor: " << firstName << " " << lastName << ", ID: " << id << endl;
    }
};

// Класс CheckResult
class CheckResult {
public:
    Resident resident;
    int isMatch;
    string discrepancyReason;

    CheckResult() : isMatch(0), discrepancyReason("") {}
};

// Шаблонный класс Container
template<typename T>
class Container {
private:
    vector<T> items;

public:
    void addItem(const T& item) {
        items.push_back(item);
    }

    T getItem(int index) const {
        return items.at(index);
    }

    const vector<T>& getItems() const {
        return items;
    }

    // Метод сортировки элементов контейнера по фамилии
    void sortByLastName() {
        std::sort(items.begin(), items.end(), [](const T& a, const T& b) {
            return a.getLastName() < b.getLastName(); // Сравнение фамилий для сортировки
            });
    }

    // Метод поиска элемента по ID
    T findById(const string& id) const {
        for (const auto& item : items) {
            if (item.getId() == id) {
                return item; // Возврат найденного элемента
            }
        }
        throw std::runtime_error("Не найден"); // Если элемент не найден, выбрасывается исключение
    }
};

// Класс Game
class Game {
private:
    static const int RESIDENTS_COUNT = 5;
    Container<Resident> residentContainer;

    void initializeResidents() {
        ifstream file("residents2.txt");
        if (!file.is_open()) {
            cerr << "Не удалось открыть файл жильцов." << endl;
            exit(1);
        }

        string line;
        while (getline(file, line) && residentContainer.getItems().size() < RESIDENTS_COUNT) {
            stringstream ss(line);
            string data[6];
            for (int i = 0; i < 6; ++i) {
                ss >> data[i]; // Чтение данных из строки
            }

            if (ss.fail()) {
                throw invalid_argument("Неверное количество данных для жильца.");
            }

            Resident resident;
            resident.setResident(data[0], data[1], data[2], data[3], data[4], data[5]);
            residentContainer.addItem(resident); // Добавление жильца в контейнер
        }

        if (residentContainer.getItems().empty()) {
            cout << "Ой. Похоже, что в этом доме ещё никто не живёт!" << endl;
            exit(1);
        }
    }

public:
    Game() {
        initializeResidents(); // Инициализация жильцов при создании игры
    }

    // Метод для ввода данных о посетителе
    static void inputVisitorPass(VisitorPass& visitor) {
        string first, last, id, hair, cloth;
        cout << "Введите имя посетителя: ";
        cin >> first;
        cout << "Введите фамилию посетителя: ";
        cin >> last;
        cout << "Введите ID посетителя: ";
        cin >> id;
        cout << "Введите цвет волос: ";
        cin >> hair;
        cout << "Введите одежду: ";
        cin >> cloth;

        visitor.setVisitor(first, last, id, hair, cloth); // Установка данных о посетителе
    }

    // Метод для проверки соответствия данных посетителя и жильца
    static CheckResult checkVisitor(const Container<Resident>& residents, const VisitorPass& visitor, int chel) {
        CheckResult result;
        Resident resident = residents.getItem(chel); // Получение жителя по индексу

        // Проверка на совпадение данных
        if (resident.getFirstName() != visitor.getFirstName()) {
            result.discrepancyReason = "Неправильное имя";
        }
        else if (resident.getLastName() != visitor.getLastName()) {
            result.discrepancyReason = "Неправильная фамилия";
        }
        else if (resident.getId() != visitor.getId()) {
            result.discrepancyReason = "Неправильный id";
        }
        else if (resident.getAppearance().getHairColor() != visitor.getAppearance().getHairColor()) {
            result.discrepancyReason = "Неправильный цвет волос";
        }
        else if (resident.getAppearance().getClothes() != visitor.getAppearance().getClothes()) {
            result.discrepancyReason = "Неправильная одежда";
        }
        else {
            result.isMatch = 1; // Если все данные совпадают, устанавливаем флаг совпадения
            return result;
        }
        return result; // Возвращаем результат проверки
    }

    void run() {
        ofstream resultFile("results2.txt", ios::app);
        if (!resultFile.is_open()) {
            cout << "Ошибка при работе с файлом." << endl;
            return;
        }

        int totalActualResidents = 0;
        int totalMonsters = 0;
        int totalActResNOT = 0;
        int totalMonsNOT = 0;
        int proh = 0;

        VisitorPass visitor;

        while (true) {
            srand(time(0));
            int chel = rand() % residentContainer.getItems().size(); // Генерация случайного индекса для жильца
            cout << chel << endl;

            // Ввод данных пришедшего жильца
            Game::inputVisitorPass(visitor);

            // Проверка данных посетителя с данными случайного жильца
            CheckResult result = checkVisitor(residentContainer, visitor, chel);
            if (result.isMatch == 1) {
                cout << "Данные совпадают. Допуск разрешён." << endl;
                char zvon;
                cout << "Вы хотите позвонить в квартиру жильца? (y/n): ";
                cin >> zvon;
                if (zvon == 'y' || zvon == 'Y') {
                    callResident(residentContainer.getItem(chel), 0); // Звонок жильцу
                }
                char prop;
                cout << "Вы хотите его пропустить? (y/n): ";
                cin >> prop;
                if (prop == 'y' || prop == 'Y') {
                    totalActualResidents++; // Увеличиваем счетчик пропущенных жильцов
                }
                else {
                    totalActResNOT++;
                }
            }
            else {
                cout << "Данные не совпадают. Возможно, это монстр-двойник! Причина: " << result.discrepancyReason << endl;
                char zvon;
                cout << "Вы хотите позвонить в квартиру жильца? (y/n): ";
                cin >> zvon;
                if (zvon == 'y' || zvon == 'Y') {
                    try {
                        Resident foundResident = residentContainer.findById(visitor.getId()); // Поиск жильца по ID
                        callResident(residentContainer.getItem(chel), 1); // Звонок жильцу
                    }
                    catch (const runtime_error& e) {
                        cout << "Не удалось найти жильца с ID " << visitor.getId() << "." << endl;
                    }
                }
                char prop;
                cout << "Вы хотите его пропустить? (y/n): ";
                cin >> prop;
                if (prop == 'y' || prop == 'Y') {
                    totalMonsters++; // Увеличиваем счетчик пропущенных монстров
                    cout << "Вы убиты! Game over!" << endl;
                    break;
                }
                else {
                    totalMonsNOT++;
                }
            }

            proh++;
            if (proh == 5) {
                cout << "Поздравляем! Вы не погибли сегодня!" << endl;
                break;
            }

            char continueChecking;
            cout << "Хотите продолжить проверку? (y/n): ";
            cin >> continueChecking;
            if (continueChecking != 'y' && continueChecking != 'Y') {
                break;
            }
        }

        // Вывод результатов
        cout << "\nРезультаты этой игры: \n";
        cout << "Настоящих жильцов пропущено: " << totalActualResidents << endl;
        cout << "Монстров-двойников пропущено: " << totalMonsters << endl;
        cout << "Настоящих жильцов не пропущено: " << totalActResNOT << endl;
        cout << "Монстров-двойников не пропущено: " << totalMonsNOT << endl;

        // Запись результатов
        resultFile << "Настоящих жильцов пропущено: " << totalActualResidents << endl;
        resultFile << "Монстров-двойников пропущено: " << totalMonsters << endl;
        resultFile << "Настоящих жильцов не пропущено: " << totalActResNOT << endl;
        resultFile << "Монстров-двойников не пропущено: " << totalMonsNOT << endl;
        cout << "Результаты проверки записаны в файл results2.txt" << endl;

        resultFile.close();

        // Чтение результатов
        char rek;
        cout << "\nХотите узнать рекорды за всё время?(y/n)\n";
        cin >> rek;
        if (rek == 'y' || rek == 'Y') {
            readResults();
        }
    }

    // Метод для звонка жильцу
    void callResident(const Resident& resident, int f) {
        cout << "Звонок на номер: " << resident.getPhone() << endl;
        // Симуляция звонка
        for (int i = 0; i < 3; ++i) {
            cout << "Звонок..." << endl;
            std::this_thread::sleep_for(std::chrono::seconds(1)); // Задержка 1 секунда
        }
        if (f == 1) {
            cout << "Ответ: Я дома!" << endl;
        }
        else {
            cout << "Ответа нет." << endl;
        }
    }

    // Метод для чтения результатов из файла
    void readResults() {
        int A[401];
        int i = 0;

        ifstream file("results2.txt");
        if (!file.is_open()) {
            cerr << "Не удалось открыть файл." << endl;
            exit(1);
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string key;
            int value;
            getline(ss, key, ':');
            ss >> value;
            A[i++] = value; // Сохранение значений в массив
        }

        int itog1 = A[0], itog2 = A[1], itog3 = A[2], itog4 = A[3];
        for (int j = 4; j < i; j++) {
            if (A[j] > itog1) itog1 = A[j]; // Поиск максимума для пропущенных жильцов
            j++;
            if (A[j] > itog2) itog2 = A[j]; // Поиск максимума для пропущенных монстров
            j++;
            if (A[j] > itog3) itog3 = A[j]; // Поиск максимума для непрошедших жильцов
            j++;
            if (A[j] > itog4) itog4 = A[j]; // Поиск максимума для непрошедших монстров
        }

        cout << "\nРекорд пропускания настоящих жильцов: " << itog1 << endl;
        cout << "Рекорд пропускания монстров-двойников: " << itog2 << endl;
        cout << "Больше всего не пропущенных настоящих жильцов: " << itog3 << endl;
        cout << "Больше всего не пропущенных монстров-двойников: " << itog4 << endl;

        file.close();
    }
};

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Game game;
    game.run();
    return 0;
}
