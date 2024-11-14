#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>

using namespace std;

class Contact {
private:
    char* name;              // ПІБ з динамічно виділеною пам'яттю
    string homePhone;
    string workPhone;
    string mobilePhone;
    string additionalInfo;

public:
    // Конструктор з параметрами
    Contact(const char* name, const string& home, const string& work, const string& mobile, const string& info)
        : homePhone(home), workPhone(work), mobilePhone(mobile), additionalInfo(info) {
        this->name = new char[strlen(name) + 1];
        strcpy(this->name, name);
    }

    // Конструктор копіювання
    Contact(const Contact& other) {
        name = new char[strlen(other.name) + 1];
        strcpy(name, other.name);
        homePhone = other.homePhone;
        workPhone = other.workPhone;
        mobilePhone = other.mobilePhone;
        additionalInfo = other.additionalInfo;
    }

    // Оператор присвоєння
    Contact& operator=(const Contact& other) {
        if (this != &other) {
            delete[] name;
            name = new char[strlen(other.name) + 1];
            strcpy(name, other.name);
            homePhone = other.homePhone;
            workPhone = other.workPhone;
            mobilePhone = other.mobilePhone;
            additionalInfo = other.additionalInfo;
        }
        return *this;
    }

    // Деструктор
    ~Contact() {
        delete[] name;
    }

    // Геттери для отримання значень полів
    const char* getName() const { return name; }
    const string& getHomePhone() const { return homePhone; }
    const string& getWorkPhone() const { return workPhone; }
    const string& getMobilePhone() const { return mobilePhone; }
    const string& getAdditionalInfo() const { return additionalInfo; }

    // Метод для виведення інформації про контакт
    void display() const {
        cout << "ПІБ: " << name << "\nДомашній телефон: " << homePhone
            << "\nРобочий телефон: " << workPhone << "\nМобільний телефон: " << mobilePhone
            << "\nДодаткова інформація: " << additionalInfo << endl;
    }

    // Збереження контакту у файл
    void saveToFile(ofstream& ofs) const {
        size_t len = strlen(name);
        ofs.write((char*)&len, sizeof(len));
        ofs.write(name, len);
        ofs << homePhone << '\n' << workPhone << '\n' << mobilePhone << '\n' << additionalInfo << '\n';
    }

    // Завантаження контакту з файлу
    static Contact loadFromFile(ifstream& ifs) {
        size_t len;
        ifs.read((char*)&len, sizeof(len));
        char* tempName = new char[len + 1];
        ifs.read(tempName, len);
        tempName[len] = '\0';

        string home, work, mobile, info;
        ifs >> home >> work >> mobile >> info;

        Contact contact(tempName, home, work, mobile, info);
        delete[] tempName;
        return contact;
    }
};

class PhoneBook {
private:
    vector<Contact> contacts;

public:
    // Додавання контакту
    void addContact(const Contact& contact) {
        contacts.push_back(contact);
    }

    // Видалення контакту за ПІБ
    void removeContact(const char* name) {
        for (auto it = contacts.begin(); it != contacts.end(); ++it) {
            if (strcmp(it->getName(), name) == 0) {
                contacts.erase(it);
                cout << "Контакт видалено.\n";
                return;
            }
        }
        cout << "Контакт не знайдено.\n";
    }

    // Пошук контакту за ПІБ
    void searchContact(const char* name) const {
        for (const auto& contact : contacts) {
            if (strcmp(contact.getName(), name) == 0) {
                contact.display();
                return;
            }
        }
        cout << "Контакт не знайдено.\n";
    }

    // Виведення всіх контактів
    void displayAll() const {
        for (const auto& contact : contacts) {
            contact.display();
            cout << "--------------------------\n";
        }
    }

    // Збереження телефонної книги у файл
    void saveToFile(const string& filename) const {
        ofstream ofs(filename, ios::binary);
        if (!ofs) {
            cerr << "Помилка відкриття файлу.\n";
            return;
        }
        size_t size = contacts.size();
        ofs.write((char*)&size, sizeof(size));
        for (const auto& contact : contacts) {
            contact.saveToFile(ofs);
        }
    }

    // Завантаження телефонної книги з файлу
    void loadFromFile(const string& filename) {
        ifstream ifs(filename, ios::binary);
        if (!ifs) {
            cerr << "Помилка відкриття файлу.\n";
            return;
        }
        contacts.clear();
        size_t size;
        ifs.read((char*)&size, sizeof(size));
        for (size_t i = 0; i < size; ++i) {
            contacts.push_back(Contact::loadFromFile(ifs));
        }
    }
};

int main() {
    PhoneBook phoneBook;
    int choice;
    while (true) {
        cout << "\n1. Додати контакт\n2. Видалити контакт\n3. Знайти контакт\n4. Показати всі контакти\n5. Зберегти у файл\n6. Завантажити з файлу\n0. Вийти\nВаш вибір: ";
        cin >> choice;

        if (choice == 0) break;

        if (choice == 1) {
            char name[100];
            string home, work, mobile, info;
            cout << "Введіть ПІБ (без пробілів): ";
            cin >> name;  // ПІБ без пробілів
            cout << "Введіть домашній телефон: ";
            cin >> home;
            cout << "Введіть робочий телефон: ";
            cin >> work;
            cout << "Введіть мобільний телефон: ";
            cin >> mobile;
            cout << "Введіть додаткову інформацію: ";
            cin >> info;
            phoneBook.addContact(Contact(name, home, work, mobile, info));
        }
        else if (choice == 2) {
            char name[100];
            cout << "Введіть ПІБ для видалення: ";
            cin >> name;
            phoneBook.removeContact(name);
        }
        else if (choice == 3) {
            char name[100];
            cout << "Введіть ПІБ для пошуку: ";
            cin >> name;
            phoneBook.searchContact(name);
        }
        else if (choice == 4) {
            phoneBook.displayAll();
        }
        else if (choice == 5) {
            phoneBook.saveToFile("phonebook.dat");
            cout << "Контакти збережено.\n";
        }
        else if (choice == 6) {
            phoneBook.loadFromFile("phonebook.dat");
            cout << "Контакти завантажено.\n";
        }
        else {
            cout << "Невірний вибір.\n";
        }
    }
    return 0;
}

