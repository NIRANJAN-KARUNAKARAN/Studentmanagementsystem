#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstring>
using namespace std;

// ---------- Student Structure ----------
struct Student {
    int rollNo;
    char name[50];
    char department[30];
    int semester;
    float gpa;
};

const char* FILENAME = "students.dat";

// ---------- Function Prototypes ----------
void addStudent();
void viewAllStudents();
void searchStudent();
void updateStudent();
void deleteStudent();
void showMenu();

int main() {
    int choice;

    do {
        showMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewAllStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: cout << "\nExiting program. Goodbye!\n"; break;
            default: cout << "\nInvalid choice! Please try again.\n";
        }
        cout << endl;

    } while (choice != 6);

    return 0;
}

// ---------- Menu ----------
void showMenu() {
    cout << "\n===== STUDENT MANAGEMENT SYSTEM =====\n";
    cout << "1. Add Student\n";
    cout << "2. View All Students\n";
    cout << "3. Search Student\n";
    cout << "4. Update Student\n";
    cout << "5. Delete Student\n";
    cout << "6. Exit\n";
    cout << "======================================\n";
}

// ---------- Add Student ----------
void addStudent() {
    ofstream outFile(FILENAME, ios::binary | ios::app);
    if (!outFile) {
        cout << "Error opening file!\n";
        return;
    }

    Student s;
    cout << "\n--- Add New Student ---\n";
    cout << "Enter Roll Number: ";
    cin >> s.rollNo;
    cin.ignore();

    cout << "Enter Name: ";
    cin.getline(s.name, 50);

    cout << "Enter Department: ";
    cin.getline(s.department, 30);

    cout << "Enter Semester: ";
    cin >> s.semester;

    cout << "Enter GPA: ";
    cin >> s.gpa;

    outFile.write(reinterpret_cast<char*>(&s), sizeof(Student));
    outFile.close();

    cout << "\nStudent record added successfully!\n";
}

// ---------- View All Students ----------
void viewAllStudents() {
    ifstream inFile(FILENAME, ios::binary);
    if (!inFile) {
        cout << "\nNo records found!\n";
        return;
    }

    Student s;
    bool found = false;

    cout << "\n--- All Student Records ---\n";
    cout << left << setw(10) << "Roll No" << setw(20) << "Name"
         << setw(20) << "Department" << setw(10) << "Semester"
         << setw(6) << "GPA" << endl;
    cout << string(66, '-') << endl;

    while (inFile.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
        cout << left << setw(10) << s.rollNo << setw(20) << s.name
             << setw(20) << s.department << setw(10) << s.semester
             << setw(6) << s.gpa << endl;
        found = true;
    }

    if (!found) cout << "No records found!\n";
    inFile.close();
}

// ---------- Search Student ----------
void searchStudent() {
    ifstream inFile(FILENAME, ios::binary);
    if (!inFile) {
        cout << "\nNo records found!\n";
        return;
    }

    int roll;
    cout << "\nEnter Roll Number to search: ";
    cin >> roll;

    Student s;
    bool found = false;

    while (inFile.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
        if (s.rollNo == roll) {
            cout << "\n--- Student Found ---\n";
            cout << "Roll No: " << s.rollNo << endl;
            cout << "Name: " << s.name << endl;
            cout << "Department: " << s.department << endl;
            cout << "Semester: " << s.semester << endl;
            cout << "GPA: " << s.gpa << endl;
            found = true;
            break;
        }
    }

    if (!found) cout << "\nStudent with Roll No " << roll << " not found!\n";
    inFile.close();
}

// ---------- Update Student ----------
void updateStudent() {
    fstream file(FILENAME, ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "\nNo records found!\n";
        return;
    }

    int roll;
    cout << "\nEnter Roll Number to update: ";
    cin >> roll;
    cin.ignore();

    Student s;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
        if (s.rollNo == roll) {
            cout << "Enter new Name: ";
            cin.getline(s.name, 50);

            cout << "Enter new Department: ";
            cin.getline(s.department, 30);

            cout << "Enter new Semester: ";
            cin >> s.semester;

            cout << "Enter new GPA: ";
            cin >> s.gpa;

            // Move file pointer back to overwrite this record
            streampos pos = file.tellg();
            pos -= sizeof(Student);
            file.seekp(pos);
            file.write(reinterpret_cast<char*>(&s), sizeof(Student));

            found = true;
            cout << "\nRecord updated successfully!\n";
            break;
        }
    }

    if (!found) cout << "\nStudent with Roll No " << roll << " not found!\n";
    file.close();
}

// ---------- Delete Student ----------
void deleteStudent() {
    ifstream inFile(FILENAME, ios::binary);
    if (!inFile) {
        cout << "\nNo records found!\n";
        return;
    }

    int roll;
    cout << "\nEnter Roll Number to delete: ";
    cin >> roll;

    ofstream outFile("temp.dat", ios::binary);
    Student s;
    bool found = false;

    while (inFile.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
        if (s.rollNo == roll) {
            found = true;
            continue; // skip writing this record (delete it)
        }
        outFile.write(reinterpret_cast<char*>(&s), sizeof(Student));
    }

    inFile.close();
    outFile.close();

    remove(FILENAME);
    rename("temp.dat", FILENAME);

    if (found)
        cout << "\nRecord deleted successfully!\n";
    else
        cout << "\nStudent with Roll No " << roll << " not found!\n";
}
