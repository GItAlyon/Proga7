#define _CRT_SECURE_NO_WARNINGS 
#include <iostream> 
#include <fstream> 
#include <string> 
#include <vector> 
#include <stdexcept> 
#include <cstdlib> 
#include <ctime> 
#include <windows.h> 

const int RESIDENTS_COUNT = 5;
const char* FILENAME = "residents2.txt";
const char* RESULT_FILENAME = "results2.txt";

class Appearance {
private:
    std::string hairColor;
    std::string clothes;

public:
    Appearance() : hairColor("Unknown"), clothes("Unknown") {}

    void setAppearance(const std::string& hair, const std::string& cloth) {
        hairColor = hair;
        clothes = cloth;
    }

    const std::string& getHairColor() const {
        return hairColor;
    }

    const std::string& getClothes() const {
        return clothes;
    }
};

class Resident {
protected: // Используем protected для доступа в производных классах 
    std::string firstName;
    std::string lastName;
    std::string id; // Идентификационный номер 
    Appearance appearance;
    std::string phone;

public:
    Resident() : firstName("Unknown"), lastName("Unknown"), id("00"), phone("Unknown") {}

    void setResident(const std::string& first, const std::string& last, const std::string& idNum,
        const std::string& hair, const std::string& cloth, const std::string& phoneNum) {
        firstName = first;
        lastName = last;
        id = idNum;
        appearance.setAppearance(hair, cloth);
        phone = phoneNum;
    }

    virtual void displayInfo() const { // Виртуальная функция 
        std::cout << "Resident Info: " << firstName << " " << lastName << ", ID: " << id << ", Phone: " << phone <<
            "\n";
    }

    const std::string& getFirstName() const { return firstName; }
    const std::string& getLastName() const { return lastName; }
    const std::string& getId() const { return id; }
    const std::string& getPhone() const { return phone; }
    const Appearance& getAppearance() const { return appearance; }

    // Перегрузка оператора '+' 
    Resident operator+(const Resident& other) const {
        Resident newResident;
        newResident.firstName = this->firstName + " & " + other.firstName;
        newResident.lastName = this->lastName + " & " + other.lastName;
        newResident.id = this->id + ", " + other.id;
        newResident.phone = this->phone + ", " + other.phone;
        return newResident;
    }

    // Префиксный оператор '++' 
    Resident& operator++() {
        this->id = std::to_string(std::stoi(this->id) + 1);
        return *this;
    }

    // Постфиксный оператор '++' 
    Resident operator++(int) {
        Resident temp = *this;
        ++(*this); // Используем префиксный оператор 
        return temp;
    }

    // Перегрузка оператора присваивания 
    Resident& operator=(const Resident& other) {
        if (this != &other) {
            firstName = other.firstName;
            lastName = other.lastName;
            id = other.id;
            phone = other.phone;
            appearance = other.appearance;
        }
        return *this;
    }

    // Дружественная функция 
    friend std::ostream& operator<<(std::ostream& os, const Resident& resident) {
        os << "Resident Info: " << resident.firstName << " " << resident.lastName
            << ", ID: " << resident.id << ", Phone: " << resident.phone;
        return os;
    }
};

class VIPResident : public Resident {
private:
    std::string vipBenefits;

public:
    VIPResident(const std::string& first, const std::string& last, const std::string& idNum,
        const std::string& hair, const std::string& cloth, const std::string& phoneNum, const std::string& benefits)
        : Resident() {
        setResident(first, last, idNum, hair, cloth, phoneNum);
        vipBenefits = benefits;
    }

    void displayInfo() const override {
        Resident::displayInfo(); // Вызов метода базового класса 
        std::cout << "VIP Benefits: " << vipBenefits << "\n";
    }
};

class VisitorPass {
private:
    std::string firstName;
    std::string lastName;
    std::string id;
    Appearance appearance;

public:
    VisitorPass() : firstName("Unknown"), lastName("Unknown"), id("00") {}

    void setVisitor(const std::string& first, const std::string& last, const std::string& idNum, const std::string& hair,
        const std::string& cloth) {
        firstName = first;
        lastName = last;
        id = idNum;
        appearance.setAppearance(hair, cloth);
    }

    const std::string& getFirstName() const { return firstName; }
    const std::string& getLastName() const { return lastName; }
    const std::string& getId() const { return id; }
    const Appearance& getAppearance() const { return appearance; }
};

class CheckResult {
public:
    Resident resident;
    int isMatch;
    std::string discrepancyReason;

    CheckResult() : isMatch(0), discrepancyReason("") {}
};

class Game {
private:
    Resident residents[RESIDENTS_COUNT];
    static int residentCount; // Статическое поле для количества жильцов 

public:
    Game() {
        residentCount = 0; // Инициализация статического поля 
        try {
            initializeResidents(residents);
        }
        catch (const std::runtime_error& e) {
            std::cerr << "Ошибка: " << e.what() << std::endl; // Обработка исключения 
            exit(1);
        }
    }

    static void initializeResidents(Resident* residents) { // Статический метод 
        std::ifstream file(FILENAME);
        if (!file) {
            throw std::runtime_error("Не удалось открыть файл жильцов."); // Генерация исключения 
        }

        while (true) {
            std::string first, last, id, hair, cloth, phone;
            if (!(file >> first >> last >> id >> hair >> cloth >> phone)) {
                if (file.eof()) break; // Достигнут конец файла 
                throw std::runtime_error("Ошибка чтения данных из файла.");
            }
            residents[residentCount].setResident(first, last, id, hair, cloth, phone);
            residentCount++;
        }
        if (residentCount == 0) {
            std::cout << "Ой. Похоже, что в этом доме ещё никто не живёт!\n";
            file.close();
            exit(1);
        }
        file.close();
    }

    void inputVisitorPass(VisitorPass& visitor) {
        std::string first, last, id, hair, cloth;
        std::cout << "Введите имя посетителя: ";
        std::cin >> first;
        std::cout << "Введите фамилию посетителя: ";
        std::cin >> last;
        std::cout << "Введите ID посетителя: ";
        std::cin >> id;
        std::cout << "Введите цвет волос: ";
        std::cin >> hair;
        std::cout << "Введите одежду: ";
        std::cin >> cloth;

        visitor.setVisitor(first, last, id, hair, cloth);
    }

    CheckResult checkVisitor(Resident* residents, VisitorPass visitor, int chel) {
        CheckResult result;

        int a = 0;
        if (residents[chel].getFirstName() != visitor.getFirstName()) {
            result.discrepancyReason = "Неправильное имя";
            a++;
        }
        else if (residents[chel].getLastName() != visitor.getLastName()) {
            result.discrepancyReason = "Неправильная фамилия";
            a++;
        }
        else if (residents[chel].getId() != visitor.getId()) {
            result.discrepancyReason = "Неправильный id";
            a++;
        }
        else if (residents[chel].getAppearance().getHairColor() != visitor.getAppearance().getHairColor()) {
            result.discrepancyReason = "Неправильный цвет волос";
            a++;
        }
        else if (residents[chel].getAppearance().getClothes() != visitor.getAppearance().getClothes()) {
            result.discrepancyReason = "Неправильная одежда";
            a++;
        }
        result.isMatch = (a == 0) ? 1 : 0;
        return result;
    }

    Resident& getResident(int index) {
        return residents[index];
    }

    Resident* getResidentPtr(int index) {
        return &residents[index];
    }

    void run() {
        FILE* resultFile = fopen(RESULT_FILENAME, "a");
        if (resultFile == NULL) {
            std::cout << "Не удалось открыть файл для записи.\n";
            return;
        }

        int proh = 0;

        VisitorPass visitor;
        int totalActualResidents = 0;
        int totalMonsters = 0;
        int totalActResNOT = 0;
        int totalMonsNOT = 0;

        while (true) {
            srand(time(0));
            int chel = rand() % RESIDENTS_COUNT;
            std::cout << "\nchel = " << chel << "\n";

            // Ввод данных пришедшего жильца 
            inputVisitorPass(visitor);

            char prop, zvon;
            int f = 0;
            CheckResult result = checkVisitor(residents, visitor, chel);
            if (result.isMatch) {
                std::cout << "Данные совпадают. Допуск разрешён.\n";
                std::cout << "\nВы хотите позвонить в квартиру жильца?(y/n)\n";
                std::cin >> zvon;
                if (zvon == 'y' || zvon == 'Y') callResident(getResidentPtr(chel), f); // Звонок настоящему жильцу 
                std::cout << "Вы хотите его пропустить? (y/n)\n";
                std::cin >> prop;
                if (prop == 'y' || prop == 'Y') {
                    totalActualResidents++;
                }
                else {
                    totalActResNOT++;
                }
            }
            else {
                std::cout << "\nДанные не совпадают. Возможно, это монстр-двойник! Причина: " <<
                    result.discrepancyReason << "\n";
                std::cout << "\nВы хотите позвонить в квартиру жильца?(y/n)\n";
                std::cin >> zvon;
                if (zvon == 'y' || zvon == 'Y') {
                    // Попробуем найти жильца по ID 
                    int residentIndex = findResidentById(residents, visitor.getId());
                    if (residentIndex != -1) {
                        f = 1;
                        callResident(getResidentPtr(chel), f); // Звонок настоящему жильцу 
                    }
                    else {
                        printf("Не удалось найти жильца с ID %s.\n", visitor.getId().c_str());
                    }
                }
                std::cout << "Вы хотите его пропустить? (y/n)\n";
                std::cin >> prop;
                if (prop == 'y' || prop == 'Y') {
                    totalMonsters++;
                    std::cout << "Вы убиты! Game over!\n";
                    break;
                }
                else {
                    totalMonsNOT++;
                }
            }

            proh++;
            if (proh == 5) {
                std::cout << "\nПоздравляем! Вы не погибли сегодня!\n";
                break;
            }

            char continueChecking;
            std::cout << "Хотите продолжить проверку? (y/n): ";
            std::cin >> continueChecking;
            if (continueChecking != 'y' && continueChecking != 'Y') {
                break;
            }
        }

        printf("\nРезультаты этой игры: \n");
        printf("Настоящих жильцов пропущено: %d\n", totalActualResidents);
        printf("Монстров-двойников пропущено: %d\n", totalMonsters);
        printf("Настоящих жильцов не пропущено: %d\n", totalActResNOT);
        printf("Монстров-двойников не пропущено: %d\n", totalMonsNOT);

        fprintf(resultFile, "Настоящих жильцов пропущено: %d\n", totalActualResidents);
        fprintf(resultFile, "Монстров-двойников пропущено: %d\n", totalMonsters);
        fprintf(resultFile, "Настоящих жильцов не пропущено: %d\n", totalActResNOT);
        fprintf(resultFile, "Монстров-двойников не пропущено: %d\n", totalMonsNOT);

        fclose(resultFile);
        std::cout << "Результаты проверки записаны в файл results.txt\n";
    }

private:
    void callResident(const Resident* resident, int f) {
        printf("Звонок на номер: %s\n", resident->getPhone().c_str());
        // Симуляция звонка 
        for (int i = 0; i < 3; ++i) {
            std::cout << "Звонок...\n";
            Sleep(1000); // Задержка 1 секунда 
        }
        if (f == 1) {
            std::cout << "Ответ: Я дома!\n"; // Двойник 
        }
        else {
            std::cout << "Ответа нет.\n"; // Настоящий житель не отвечает 
        }
    }

    // Функция для поиска жильца по ID 
    int findResidentById(const Resident residents[], const std::string& id) {
        for (int i = 0; i < RESIDENTS_COUNT; i++) {
            if (residents[i].getId() == id) {
                return i; // Возвращаем индекс жильца с совпадающим ID 
            }
        }
        return -1; // Нет совпадения   
    }
};

// Инициализация статического поля 
int Game::residentCount = 0;

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Game game;
    game.run();

    // Демонстрация перегрузки операторов 
    Resident resident1, resident2;
    resident1.setResident("John", "Doe", "01", "Brown", "Casual", "123456789");
    resident2.setResident("Jane", "Smith", "02", "Blonde", "Formal", "987654321");
    Resident combinedResident = resident1 + resident2; // Используем перегруженный оператор '+' 
    std::cout << combinedResident; // Используем перегруженный оператор '<<' 
    ++combinedResident; // Применяем префиксный оператор '++' 
    std::cout << combinedResident;
    combinedResident++; // Применяем постфиксный оператор '++' 
    std::cout << combinedResident;

    return 0;
}