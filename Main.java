import java.io.*;
import java.util.*;

// Абстрактный класс Person
abstract class Person {
    protected String firstName;
    protected String lastName;

    public abstract void display();
}

// Интерфейс Identifiable
interface Identifiable {
    String getId();
}

class Appearance {
    private String hairColor;
    private String clothes;

    public Appearance() {
        this.hairColor = "Unknown";
        this.clothes = "Unknown";
    }

    public void setAppearance(String hair, String cloth) {
        this.hairColor = hair;
        this.clothes = cloth;
    }

    public String getHairColor() {
        return hairColor;
    }

    public String getClothes() {
        return clothes;
    }
}

// Класс Resident
class Resident extends Person implements Identifiable, Cloneable {
    protected String id; // Идентификационный номер из 2 цифр
    protected String phone; // Модификатор protected
    private Appearance appearance;

    public Resident() {
        this.firstName = "Unknown";
        this.lastName = "Unknown";
        this.id = "00";
        this.phone = "Unknown";
        this.appearance = new Appearance();
    }

    public void setResident(String first, String last, String idNum, String hair, String cloth, String phoneNum) {
        this.firstName = first;
        this.lastName = last;
        this.id = idNum;
        this.appearance.setAppearance(hair, cloth);
        this.phone = phoneNum;
    }

    public String getFirstName() { return firstName; }
    public String getLastName() { return lastName; }
    public String getId() { return id; }
    public String getPhone() { return phone; }
    public Appearance getAppearance() { return appearance; }

    @Override
    public String toString() {
        return "Resident: " + firstName + " " + lastName + ", ID: " + id + ", Phone: " + phone;
    }

    @Override
    protected Object clone() throws CloneNotSupportedException {
        return super.clone(); // Мелкое клонирование
    }

    @Override
    public void display() {
        System.out.println(this.toString());
    }
}

// Производный класс VIPResident
class VIPResident extends Resident {
    private String vipPhone;

    public VIPResident(String first, String last, String idNum, String hair, String cloth, String phoneNum, String vipPhone) {
        super(); // Вызов конструктора базового класса
        setResident(first, last, idNum, hair, cloth, phoneNum);
        this.vipPhone = vipPhone;
    }

    public String getVipPhone() {
        return vipPhone;
    }

    @Override
    public String toString() {
        return super.toString() + ", VIP Phone: " + vipPhone;
    }

    @Override
    public void display() {
        System.out.println(this.toString());
    }
}

// Класс VisitorPass
class VisitorPass extends Person implements Identifiable {
    private String id;
    private Appearance appearance;

    public VisitorPass() {
        this.firstName = "Unknown";
        this.lastName = "Unknown";
        this.id = "00";
        this.appearance = new Appearance();
    }

    public void setVisitor(String first, String last, String idNum, String hair, String cloth) {
        this.firstName = first;
        this.lastName = last;
        this.id = idNum;
        this.appearance.setAppearance(hair, cloth);
    }

    public String getFirstName() { return firstName; }
    public String getLastName() { return lastName; }
    public String getId() { return id; }
    public Appearance getAppearance() { return appearance; }

    @Override
    public void display() {
        System.out.println("Visitor: " + firstName + " " + lastName + ", ID: " + id);
    }
}

// Класс CheckResult
class CheckResult {
    public Resident resident;
    public int isMatch;
    public String discrepancyReason;

    public CheckResult() {
        this.isMatch = 0;
        this.discrepancyReason = "";
    }
}

// Шаблонный класс Container
class Container<T> {
    private List<T> items = new ArrayList<>();

    public void addItem(T item) {
        items.add(item);
    }

    public T getItem(int index) {
        return items.get(index);
    }
}

// Класс Game
class Game {
    private static final int RESIDENTS_COUNT = 5;
    private Resident[] residents;
    private int residentCount;

    public Game() {
        residents = new Resident[RESIDENTS_COUNT];
        residentCount = 0;
        initializeResidents();
    }

    private void initializeResidents() {
        // Блок try
        try (Scanner scanner = new Scanner(new File("D:/JAVA_PROGA/Classi/residents2.txt"))) {
            while (scanner.hasNextLine() && residentCount < RESIDENTS_COUNT) {
                String[] data = scanner.nextLine().split(" ");
                if (data.length != 6) {
                    // Блок throw
                    throw new IllegalArgumentException("Неверное количество данных для жильца.");
                }
                residents[residentCount] = new Resident();
                residents[residentCount].setResident(data[0], data[1], data[2], data[3], data[4], data[5]);
                residentCount++;
            }
        } // Блок catch
        catch (FileNotFoundException e) {
            System.err.println("Не удалось открыть файл жильцов.");
            System.exit(1);
        } // Блок catch для исключения throw
        catch (IllegalArgumentException e) {
            System.err.println(e.getMessage());
            System.exit(1);
        }
        if (residentCount == 0) {
            System.out.println("Ой. Похоже, что в этом доме ещё никто не живёт!");
            System.exit(1);
        }
    }

    static void inputVisitorPass(VisitorPass visitor) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Введите имя посетителя: ");
        String first = scanner.nextLine();
        System.out.print("Введите фамилию посетителя: ");
        String last = scanner.nextLine();
        System.out.print("Введите ID посетителя: ");
        String id = scanner.nextLine();
        System.out.print("Введите цвет волос: ");
        String hair = scanner.nextLine();
        System.out.print("Введите одежду: ");
        String cloth = scanner.nextLine();

        visitor.setVisitor(first, last, id, hair, cloth);
    }

    static CheckResult checkVisitor(Resident[] residents, VisitorPass visitor, int chel) {
        CheckResult result = new CheckResult();

        Resident resident = residents[chel];
        if (!resident.getFirstName().equals(visitor.getFirstName())) {
            result.discrepancyReason = "Неправильное имя";
        } else if (!resident.getLastName().equals(visitor.getLastName())) {
            result.discrepancyReason = "Неправильная фамилия";
        } else if (!resident.getId().equals(visitor.getId())) {
            result.discrepancyReason = "Неправильный id";
        } else if (!resident.getAppearance().getHairColor().equals(visitor.getAppearance().getHairColor())) {
            result.discrepancyReason = "Неправильный цвет волос";
        } else if (!resident.getAppearance().getClothes().equals(visitor.getAppearance().getClothes())) {
            result.discrepancyReason = "Неправильная одежда";
        } else {
            result.isMatch = 1;
            return result;
        }
        return result;
    }

    public void run() {
        Scanner scanner = new Scanner(System.in);
        // Блок try
        try (PrintWriter resultFile = new PrintWriter(new FileWriter("results2.txt", true))) {
            int totalActualResidents = 0;
            int totalMonsters = 0;
            int totalActResNOT = 0;
            int totalMonsNOT = 0;
            int proh = 0;

            VisitorPass visitor = new VisitorPass();

            while (true) {
                int chel = new Random().nextInt(RESIDENTS_COUNT); // 0, 1, 2, 3, 4
                System.out.println(chel);
                int f = 0;

                // Ввод данных пришедшего жильца
                Game.inputVisitorPass(visitor);

                CheckResult result = checkVisitor(residents, visitor, chel);
                if (result.isMatch == 1) {
                    System.out.println("Данные совпадают. Допуск разрешён.");
                    System.out.print("Вы хотите позвонить в квартиру жильца? (y/n): ");
                    char zvon = scanner.next().charAt(0);
                    if (zvon == 'y' || zvon == 'Y') {
                        callResident(residents[chel], f);
                    }
                    System.out.print("Вы хотите его пропустить? (y/n): ");
                    char prop = scanner.next().charAt(0);
                    if (prop == 'y' || prop == 'Y') {
                        totalActualResidents++;
                    } else {
                        totalActResNOT++;
                    }
                } else {
                    System.out.println("Данные не совпадают. Возможно, это монстр-двойник! Причина: " + result.discrepancyReason);
                    System.out.print("Вы хотите позвонить в квартиру жильца? (y/n): ");
                    char zvon = scanner.next().charAt(0);
                    if (zvon == 'y' || zvon == 'Y') {
                        int residentIndex = findResidentById(visitor.getId());
                        if (residentIndex != -1) {
                            f = 1;
                            callResident(residents[chel], f);
                        } else {
                            System.out.printf("Не удалось найти жильца с ID %s.\n", visitor.getId());
                        }
                    }
                    System.out.print("Вы хотите его пропустить? (y/n): ");
                    char prop = scanner.next().charAt(0);
                    if (prop == 'y' || prop == 'Y') {
                        totalMonsters++;
                        System.out.println("Вы убиты! Game over!");
                        break;
                    } else {
                        totalMonsNOT++;
                    }
                }

                proh++;
                if (proh == 5) {
                    System.out.println("Поздравляем! Вы не погибли сегодня!");
                    break;
                }

                System.out.print("Хотите продолжить проверку? (y/n): ");
                char continueChecking = scanner.next().charAt(0);
                if (continueChecking != 'y' && continueChecking != 'Y') {
                    break;
                }
            }
            // Вывод результатов
            System.out.print("\nРезультаты этой игры: \n");
            System.out.printf("Настоящих жильцов пропущено: %d\n", totalActualResidents);
            System.out.printf("Монстров-двойников пропущено: %d\n", totalMonsters);
            System.out.printf("Настоящих жильцов не пропущено: %d\n", totalActResNOT);
            System.out.printf("Монстров-двойников не пропущено: %d\n", totalMonsNOT);

            // Запись результатов
            resultFile.printf("Настоящих жильцов пропущено: %d\n", totalActualResidents);
            resultFile.printf("Монстров-двойников пропущено: %d\n", totalMonsters);
            resultFile.printf("Настоящих жильцов не пропущено: %d\n", totalActResNOT);
            resultFile.printf("Монстров-двойников не пропущено: %d\n", totalMonsNOT);
            System.out.println("Результаты проверки записаны в файл results2.txt");
        } // Блок catch
        catch (IOException e) {
            System.out.println("Ошибка при работе с файлом: " + e.getMessage());
        }
        // Чтение результатов
        System.out.print("\nХотите узнать рекорды за всё время?(y/n)\n");
        char rek = scanner.next().charAt(0);
        if (rek == 'y' || rek == 'Y') {
            readResults();
        }
    }

    private void callResident(Resident resident, int f) {
        System.out.printf("Звонок на номер: %s\n", resident.getPhone());
        // Симуляция звонка
        for (int i = 0; i < 3; ++i) {
            System.out.println("Звонок...");
            // Блок try
            try {
                Thread.sleep(1000); // Задержка 1 секунда
            } // Блок catch
            catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
        if (f == 1) {
            System.out.println("Ответ: Я дома!");
        } else {
            System.out.println("Ответа нет.");
        }
    }

    private int findResidentById(String id) {
        for (int i = 0; i < residentCount; i++) {
            if (residents[i].getId().equals(id)) {
                return i; // Возвращаем индекс жильца с совпадающим ID
            }
        }
        return -1; // Нет совпадения
    }

    private void readResults() {
        int[] A = new int[401];
        int i = 0;
        // Блок try
        try (Scanner scanner = new Scanner(new File("results2.txt"))) {
            while (scanner.hasNextLine()) {
                String[] s = scanner.nextLine().split(": ");
                A[i] = Integer.parseInt(s[1]);
                i++;
            }
        } // Блок catch
        catch (FileNotFoundException e) {
            System.err.println("Не удалось открыть файл.");
            System.exit(1);
        }
        int ItogI = i;
        i = 0;
        int itog1 = A[0], itog2 = A[1], itog3 = A[2], itog4 = A[3];
        while (i < (ItogI - 1)) {
            if (A[i + 4] > itog1) itog1 = A[i + 4];
            i++;
            if (A[i + 4] > itog2) itog2 = A[i + 4];
            i++;
            if (A[i + 4] > itog3) itog3 = A[i + 4];
            i++;
            if (A[i + 4] > itog4) itog4 = A[i + 4];
            i++;
        }
        System.out.printf("\nРекорд пропускания настоящих жильцов: %d\n", itog1);
        System.out.printf("Рекорд пропускания монстров-двойников: %d\n", itog2);
        System.out.printf("Больше всего не пропущенных настоящих жильцов: %d\n", itog3);
        System.out.printf("Больше всего не пропущенных монстров-двойников: %d\n", itog4);
    }

    public static void main(String[] args) {
        Game game = new Game();
        game.run();
    }
}
