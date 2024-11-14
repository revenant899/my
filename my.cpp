#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>

using namespace std;

class Contact {
private:
    char* name;              // ϲ� � �������� �������� ���'����
    string homePhone;
    string workPhone;
    string mobilePhone;
    string additionalInfo;

public:
    // ����������� � �����������
    Contact(const char* name, const string& home, const string& work, const string& mobile, const string& info)
        : homePhone(home), workPhone(work), mobilePhone(mobile), additionalInfo(info) {
        this->name = new char[strlen(name) + 1];
        strcpy(this->name, name);
    }

    // ����������� ���������
    Contact(const Contact& other) {
        name = new char[strlen(other.name) + 1];
        strcpy(name, other.name);
        homePhone = other.homePhone;
        workPhone = other.workPhone;
        mobilePhone = other.mobilePhone;
        additionalInfo = other.additionalInfo;
    }

    // �������� ���������
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

    // ����������
    ~Contact() {
        delete[] name;
    }

    // ������� ��� ��������� ������� ����
    const char* getName() const { return name; }
    const string& getHomePhone() const { return homePhone; }
    const string& getWorkPhone() const { return workPhone; }
    const string& getMobilePhone() const { return mobilePhone; }
    const string& getAdditionalInfo() const { return additionalInfo; }

    // ����� ��� ��������� ���������� ��� �������
    void display() const {
        cout << "ϲ�: " << name << "\n������� �������: " << homePhone
            << "\n������� �������: " << workPhone << "\n�������� �������: " << mobilePhone
            << "\n��������� ����������: " << additionalInfo << endl;
    }

    // ���������� �������� � ����
    void saveToFile(ofstream& ofs) const {
        size_t len = strlen(name);
        ofs.write((char*)&len, sizeof(len));
        ofs.write(name, len);
        ofs << homePhone << '\n' << workPhone << '\n' << mobilePhone << '\n' << additionalInfo << '\n';
    }

    // ������������ �������� � �����
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
    // ��������� ��������
    void addContact(const Contact& contact) {
        contacts.push_back(contact);
    }

    // ��������� �������� �� ϲ�
    void removeContact(const char* name) {
        for (auto it = contacts.begin(); it != contacts.end(); ++it) {
            if (strcmp(it->getName(), name) == 0) {
                contacts.erase(it);
                cout << "������� ��������.\n";
                return;
            }
        }
        cout << "������� �� ��������.\n";
    }

    // ����� �������� �� ϲ�
    void searchContact(const char* name) const {
        for (const auto& contact : contacts) {
            if (strcmp(contact.getName(), name) == 0) {
                contact.display();
                return;
            }
        }
        cout << "������� �� ��������.\n";
    }

    // ��������� ��� ��������
    void displayAll() const {
        for (const auto& contact : contacts) {
            contact.display();
            cout << "--------------------------\n";
        }
    }

    // ���������� ��������� ����� � ����
    void saveToFile(const string& filename) const {
        ofstream ofs(filename, ios::binary);
        if (!ofs) {
            cerr << "������� �������� �����.\n";
            return;
        }
        size_t size = contacts.size();
        ofs.write((char*)&size, sizeof(size));
        for (const auto& contact : contacts) {
            contact.saveToFile(ofs);
        }
    }

    // ������������ ��������� ����� � �����
    void loadFromFile(const string& filename) {
        ifstream ifs(filename, ios::binary);
        if (!ifs) {
            cerr << "������� �������� �����.\n";
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
        cout << "\n1. ������ �������\n2. �������� �������\n3. ������ �������\n4. �������� �� ��������\n5. �������� � ����\n6. ����������� � �����\n0. �����\n��� ����: ";
        cin >> choice;

        if (choice == 0) break;

        if (choice == 1) {
            char name[100];
            string home, work, mobile, info;
            cout << "������ ϲ� (��� ������): ";
            cin >> name;  // ϲ� ��� ������
            cout << "������ ������� �������: ";
            cin >> home;
            cout << "������ ������� �������: ";
            cin >> work;
            cout << "������ �������� �������: ";
            cin >> mobile;
            cout << "������ ��������� ����������: ";
            cin >> info;
            phoneBook.addContact(Contact(name, home, work, mobile, info));
        }
        else if (choice == 2) {
            char name[100];
            cout << "������ ϲ� ��� ���������: ";
            cin >> name;
            phoneBook.removeContact(name);
        }
        else if (choice == 3) {
            char name[100];
            cout << "������ ϲ� ��� ������: ";
            cin >> name;
            phoneBook.searchContact(name);
        }
        else if (choice == 4) {
            phoneBook.displayAll();
        }
        else if (choice == 5) {
            phoneBook.saveToFile("phonebook.dat");
            cout << "�������� ���������.\n";
        }
        else if (choice == 6) {
            phoneBook.loadFromFile("phonebook.dat");
            cout << "�������� �����������.\n";
        }
        else {
            cout << "������� ����.\n";
        }
    }
    return 0;
}

