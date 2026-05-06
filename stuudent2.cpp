#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <limits>
using namespace std;

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

class Student {
private:
    int rollNo;
    char name[50];
    int age;
    float marks;
    char email[50];

public:
    // Constructor
    Student() {
        rollNo = 0;
        age = 0;
        marks = 0.0;
        memset(name, 0, sizeof(name));
        memset(email, 0, sizeof(email));
    }

    // Setter methods
    void setRollNo(int newRoll) {
        rollNo = newRoll;
    }
    
    void setAge(int newAge) {
        age = newAge;
    }
    
    void setMarks(float newMarks) {
        marks = newMarks;
    }
    
    void setName(const char* newName) {
        strcpy(name, newName);
    }
    
    void setEmail(const char* newEmail) {
        strcpy(email, newEmail);
    }

    void getData(bool askForRoll = true) {
        if (askForRoll) {
            cout << "\n\t[+] Enter Roll Number: ";
            while (!(cin >> rollNo)) {
                cout << "\t[!] Invalid! Enter numbers only: ";
                cin.clear();
                cin.ignore(1000, '\n');
            }
        }

        cout << "\t[+] Enter Name: ";
        cin.ignore();
        cin.getline(name, 50);
        name[49] = '\0';

        cout << "\t[+] Enter Age: ";
        while (!(cin >> age) || age <= 0 || age > 150) {
            cout << "\t[!] Invalid! Enter valid age (1-150): ";
            cin.clear();
            cin.ignore(1000, '\n');
        }

        cout << "\t[+] Enter Marks (0-100): ";
        while (!(cin >> marks) || marks < 0 || marks > 100) {
            cout << "\t[!] Invalid! Enter marks between 0-100: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }

        cout << "\t[+] Enter Email: ";
        cin >> email;
        email[49] = '\0';
        
        cout << "\t[*] Grade Calculated: " << calculateGrade() << endl;
    }

    char calculateGrade() const {
        if (marks >= 90) return 'A';
        else if (marks >= 80) return 'B';
        else if (marks >= 70) return 'C';
        else if (marks >= 60) return 'D';
        else if (marks >= 50) return 'E';
        else return 'F';
    }

    void displayData() {
        char gradeLetter = calculateGrade();
        cout << "| " << left << setw(10) << rollNo
             << "| " << setw(20) << name
             << "| " << setw(5) << age
             << "| " << setw(7) << marks
             << "| " << setw(6) << gradeLetter
             << "| " << setw(25) << email << " |" << endl;
    }

    // Getter methods
    int getRollNo() const { return rollNo; }
    float getMarks() const { return marks; }
    char* getName() { return name; }
    int getAge() const { return age; }
    char* getEmail() { return email; }
    const char* getName() const { return name; }
    const char* getEmail() const { return email; }
    
    bool isValid() const {
        return (rollNo > 0 && rollNo < 10000 && 
                age > 0 && age < 150 && 
                marks >= 0 && marks <= 100 &&
                name[0] != '\0');
    }
};

void clearScreen() {
    system(CLEAR);
}

void header() {
    cout << "\n==================================================\n";
    cout << "     STUDENT MANAGEMENT SYSTEM     \n";
    cout << "==================================================\n";
}

void waitForUser() {
    cout << "\n\tPress Enter to continue...";
    cin.ignore();
    cin.get();
}

int getValidChoice() {
    int choice;
    while (true) {
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\t[!] Invalid input! Enter a number (1-7): ";
        } else {
            cin.ignore(1000, '\n');
            return choice;
        }
    }
}

// FIXED: Add student without asking for roll number twice
void addStudent() {
    int newRoll;
    int newAge;
    float newMarks;
    char newName[50];
    char newEmail[50];
    
    cout << "\n\t[ADD NEW STUDENT]\n";
    cout << "\t-----------------\n";
    
    // Ask for roll number once
    cout << "\t[+] Enter Roll Number: ";
    while (!(cin >> newRoll)) {
        cout << "\t[!] Invalid! Enter numbers only: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    
    // Check if roll number already exists
    ifstream checkFile("students.dat", ios::binary);
    Student temp;
    
    if (checkFile) {
        while (checkFile.read((char*)&temp, sizeof(temp))) {
            if (temp.isValid() && temp.getRollNo() == newRoll) {
                cout << "\n\t[!] Error: Roll Number " << newRoll << " already exists!\n";
                checkFile.close();
                return;
            }
        }
    }
    checkFile.close();
    
    // Get the rest of the data
    cout << "\t[+] Enter Name: ";
    cin.ignore();
    cin.getline(newName, 50);
    newName[49] = '\0';
    
    cout << "\t[+] Enter Age: ";
    while (!(cin >> newAge) || newAge <= 0 || newAge > 150) {
        cout << "\t[!] Invalid! Enter valid age (1-150): ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    
    cout << "\t[+] Enter Marks (0-100): ";
    while (!(cin >> newMarks) || newMarks < 0 || newMarks > 100) {
        cout << "\t[!] Invalid! Enter marks between 0-100: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    
    cout << "\t[+] Enter Email: ";
    cin >> newEmail;
    newEmail[49] = '\0';
    
    // Create student object and set all values
    Student s;
    s.setRollNo(newRoll);
    s.setName(newName);
    s.setAge(newAge);
    s.setMarks(newMarks);
    s.setEmail(newEmail);
    
    // Show grade
    cout << "\t[*] Grade Calculated: " << s.calculateGrade() << endl;
    
    // Open file for appending
    ofstream file("students.dat", ios::binary | ios::app);
    if (!file) {
        cout << "\n\t[!] Error opening file!\n";
        return;
    }
    
    // Write to file
    file.write((char*)&s, sizeof(s));
    file.close();
    
    cout << "\n\t[+] Student added successfully!\n";
}

void displayStudents() {
    ifstream file("students.dat", ios::binary);
    
    if (!file) {
        cout << "\n\t[!] No records found! Please add students first.\n";
        return;
    }
    
    file.seekg(0, ios::end);
    if (file.tellg() == 0) {
        cout << "\n\t[!] No records found! Please add students first.\n";
        file.close();
        return;
    }
    file.seekg(0, ios::beg);
    
    Student s;
    bool hasRecords = false;
    int count = 0;
    
    cout << "\n\t[ALL STUDENT RECORDS]\n";
    cout << "\t+------------+----------------------+-------+---------+--------+---------------------------+\n";
    cout << "\t| Roll No    | Name                 | Age   | Marks   | Grade  | Email                     |\n";
    cout << "\t+------------+----------------------+-------+---------+--------+---------------------------+\n";
    
    while (file.read((char*)&s, sizeof(s))) {
        if (s.isValid()) {
            s.displayData();
            hasRecords = true;
            count++;
        }
    }
    
    cout << "\t+------------+----------------------+-------+---------+--------+---------------------------+\n";
    cout << "\n\t[*] Total Students Displayed: " << count << endl;
    
    if (!hasRecords) {
        cout << "\n\t[!] No valid records found!\n";
    }
    
    file.close();
}

void updateStudent() {
    fstream file("students.dat", ios::binary | ios::in | ios::out);
    
    if (!file) {
        cout << "\n\t[!] No records found!\n";
        return;
    }
    
    Student s;
    int searchRoll;
    bool found = false;
    streampos position;
    
    cout << "\n\t[UPDATE STUDENT RECORD]\n";
    cout << "\tEnter Roll Number to update: ";
    cin >> searchRoll;
    
    file.clear();
    file.seekg(0, ios::beg);
    
    while (file.read((char*)&s, sizeof(s))) {
        position = file.tellg();
        
        if (s.isValid() && s.getRollNo() == searchRoll) {
            cout << "\n\t[+] RECORD FOUND!\n";
            cout << "\n\tCurrent Details:\n";
            cout << "\t   Roll No: " << s.getRollNo() << endl;
            cout << "\t   Name: " << s.getName() << endl;
            cout << "\t   Age: " << s.getAge() << endl;
            cout << "\t   Marks: " << s.getMarks() << "%" << endl;
            cout << "\t   Email: " << s.getEmail() << endl;
            
            cout << "\n\tEnter NEW details:\n";
            Student newS;
            newS.getData(true);  // Ask for all data including roll number
            
            streamoff offset = static_cast<streamoff>(sizeof(s));
            file.seekp(position - offset, ios::beg);
            file.write((char*)&newS, sizeof(s));
            file.flush();
            
            found = true;
            cout << "\n\t[+] Student updated successfully!\n";
            break;
        }
    }
    
    if (!found) {
        cout << "\n\t[!] Student with Roll Number " << searchRoll << " not found!\n";
    }
    
    file.close();
}

void deleteStudent() {
    ifstream file("students.dat", ios::binary);
    
    if (!file) {
        cout << "\n\t[!] No records found!\n";
        return;
    }
    
    ofstream temp("temp.dat", ios::binary);
    Student s;
    int deleteRoll;
    bool found = false;
    
    cout << "\n\t[DELETE STUDENT RECORD]\n";
    cout << "\tEnter Roll Number to delete: ";
    cin >> deleteRoll;
    
    while (file.read((char*)&s, sizeof(s))) {
        if (s.isValid() && s.getRollNo() != deleteRoll) {
            temp.write((char*)&s, sizeof(s));
        } else if (s.isValid() && s.getRollNo() == deleteRoll) {
            found = true;
        }
    }
    
    file.close();
    temp.close();
    
    remove("students.dat");
    rename("temp.dat", "students.dat");
    
    if (found) {
        cout << "\n\t[+] Student deleted successfully!\n";
    } else {
        cout << "\n\t[!] Student not found!\n";
    }
}

void searchStudent() {
    ifstream file("students.dat", ios::binary);
    if (!file) {
        cout << "\n\t[!] No records found!\n";
        return;
    }
    
    int searchRoll;
    bool found = false;
    Student s;
    
    cout << "\n\t[SEARCH STUDENT]\n";
    cout << "\tEnter Roll Number to search: ";
    cin >> searchRoll;
    
    while (file.read((char*)&s, sizeof(s))) {
        if (s.isValid() && s.getRollNo() == searchRoll) {
            cout << "\n\t[+] STUDENT FOUND!\n";
            cout << "\t+----------------------------------------+\n";
            cout << "\t   Roll Number: " << s.getRollNo() << endl;
            cout << "\t   Name: " << s.getName() << endl;
            cout << "\t   Age: " << s.getAge() << endl;
            cout << "\t   Marks: " << s.getMarks() << "%" << endl;
            cout << "\t   Grade: " << s.calculateGrade() << endl;
            cout << "\t   Email: " << s.getEmail() << endl;
            cout << "\t+----------------------------------------+\n";
            found = true;
            break;
        }
    }
    
    if (!found) {
        cout << "\n\t[!] Student with Roll Number " << searchRoll << " not found!\n";
    }
    
    file.close();
}

void showStatistics() {
    ifstream file("students.dat", ios::binary);
    if (!file) {
        cout << "\n\t[!] No records found!\n";
        return;
    }
    
    Student s;
    int totalStudents = 0;
    float totalMarks = 0;
    float topScore = 0;
    int failCount = 0;
    string topStudent;
    
    while (file.read((char*)&s, sizeof(s))) {
        if (s.isValid()) {
            totalStudents++;
            totalMarks += s.getMarks();
            if (s.getMarks() > topScore) {
                topScore = s.getMarks();
                topStudent = s.getName();
            }
            if (s.getMarks() < 50) failCount++;
        }
    }
    file.close();
    
    float classAverage = (totalStudents > 0) ? totalMarks / totalStudents : 0;
    
    cout << "\n\t[CLASS STATISTICS]\n";
    cout << "\t-------------------\n";
    cout << "\t[*] Total Students: " << totalStudents << endl;
    cout << "\t[*] Class Average: " << fixed << setprecision(2) << classAverage << "%" << endl;
    cout << "\t[*] Top Performer: ";
    if (topScore > 0) cout << topStudent << " (" << topScore << "%)" << endl;
    else cout << "N/A" << endl;
    cout << "\t[*] Students Below 50%: " << failCount << endl;
    if (totalStudents > 0) {
        cout << "\t[*] Pass Percentage: " << ((totalStudents - failCount) * 100 / totalStudents) << "%" << endl;
    }
}

void showMenu() {
    cout << "\n\t+-------------------------------------------+\n";
    cout << "\t|            MAIN MENU                       |\n";
    cout << "\t+-------------------------------------------+\n";
    cout << "\t|  1. Add New Student                       |\n";
    cout << "\t|  2. Display All Students                  |\n";
    cout << "\t|  3. Update Student Record                 |\n";
    cout << "\t|  4. Delete Student Record                 |\n";
    cout << "\t|  5. Search Student                        |\n";
    cout << "\t|  6. View Statistics                       |\n";
    cout << "\t|  7. Exit                                  |\n";
    cout << "\t+-------------------------------------------+\n";
    cout << "\n\tEnter your choice (1-7): ";
}

int main() {
    int choice;
    
    // Delete corrupted file if it exists
    ifstream test("students.dat", ios::binary);
    if (test) {
        Student testStudent;
        test.read((char*)&testStudent, sizeof(testStudent));
        if (!testStudent.isValid()) {
            test.close();
            remove("students.dat");
            cout << "\n[!] Corrupted data file detected and removed.\n";
            cout << "[*] Please add students again.\n";
        } else {
            test.close();
        }
    }
    
    system(CLEAR);
    
    header();
    cout << "\n\tWelcome to Student Management System!\n";
    cout << "\tDeveloped for Thiranex Internship\n";
    cout << "\tDue Date: 13 May 2026\n";
    waitForUser();
    
    do {
        system(CLEAR);
        header();
        showMenu();
        
        choice = getValidChoice();
        
        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                displayStudents();
                break;
            case 3:
                updateStudent();
                break;
            case 4:
                deleteStudent();
                break;
            case 5:
                searchStudent();
                break;
            case 6:
                showStatistics();
                break;
            case 7:
                cout << "\n\tThank you for using Student Management System!\n";
                cout << "\tExiting program...\n";
                break;
            default:
                cout << "\n\t[!] Invalid choice! Please enter 1-7.\n";
        }
        
        if (choice != 7) {
            waitForUser();
        }
        
    } while (choice != 7);
    
    return 0;
}