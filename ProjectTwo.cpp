//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Shayal Singh
// Version     : 1.0
// Copyright   : Copyright 2026 SNHU COCE
// Description : CS 300 Project Two - Advising Assistance Program
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

// Structure to store course information
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Node for the Binary Search Tree
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course aCourse) {
        course = aCourse;
        left = nullptr;
        right = nullptr;
    }
};

// Binary Search Tree class
class BinarySearchTree {
private:
    Node* root;

    // Insert a course recursively
    Node* insertNode(Node* node, Course course) {
        if (node == nullptr) {
            return new Node(course);
        }

        if (course.courseNumber < node->course.courseNumber) {
            node->left = insertNode(node->left, course);
        }
        else if (course.courseNumber > node->course.courseNumber) {
            node->right = insertNode(node->right, course);
        }
        else {
            node->course = course;
        }

        return node;
    }

    // In-order traversal prints courses in alphanumeric order
    void printInOrder(Node* node) const {
        if (node != nullptr) {
            printInOrder(node->left);

            cout << node->course.courseNumber
                 << ", " << node->course.courseTitle << endl;

            printInOrder(node->right);
        }
    }

    // Delete all nodes from the tree
    void clearTree(Node* node) {
        if (node != nullptr) {
            clearTree(node->left);
            clearTree(node->right);
            delete node;
        }
    }

public:
    BinarySearchTree() {
        root = nullptr;
    }

    ~BinarySearchTree() {
        clear();
    }

    // Insert a course into the BST
    void insert(Course course) {
        root = insertNode(root, course);
    }

    // Search for a course by course number
    Course* search(string courseNumber) {
        Node* current = root;

        while (current != nullptr) {
            if (courseNumber == current->course.courseNumber) {
                return &(current->course);
            }
            else if (courseNumber < current->course.courseNumber) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }

        return nullptr;
    }

    // Print all courses
    void printCourseList() const {
        printInOrder(root);
    }

    // Clear the tree
    void clear() {
        clearTree(root);
        root = nullptr;
    }

    bool isEmpty() const {
        return root == nullptr;
    }
};

// Remove extra spaces from strings
string trim(const string& text) {
    size_t first = text.find_first_not_of(" \t\r\n");

    if (first == string::npos) {
        return "";
    }

    size_t last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

// Convert course numbers to uppercase
string toUpperCase(string text) {
    transform(text.begin(), text.end(), text.begin(),
        [](unsigned char character) {
            return toupper(character);
        });

    return text;
}

// Load course information from CSV file
bool loadCourses(string fileName, BinarySearchTree& courseTree) {
    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "Error: Unable to open file " << fileName << "." << endl;
        return false;
    }

    courseTree.clear();

    string line;
    int courseCount = 0;

    while (getline(file, line)) {
        if (trim(line).empty()) {
            continue;
        }

        stringstream lineStream(line);
        string value;
        vector<string> fields;

        while (getline(lineStream, value, ',')) {
            fields.push_back(trim(value));
        }

        // Each course must contain a number and title
        if (fields.size() < 2) {
            cout << "Warning: Invalid course data skipped." << endl;
            continue;
        }

        Course course;
        course.courseNumber = toUpperCase(fields[0]);
        course.courseTitle = fields[1];

        // Remaining fields contain prerequisite course numbers
        for (size_t i = 2; i < fields.size(); ++i) {
            if (!fields[i].empty()) {
                course.prerequisites.push_back(
                    toUpperCase(fields[i])
                );
            }
        }

        courseTree.insert(course);
        courseCount++;
    }

    file.close();

    if (courseCount == 0) {
        cout << "Error: No valid course information was found." << endl;
        return false;
    }

    cout << courseCount << " courses successfully loaded." << endl;
    return true;
}

// Display one course and its prerequisites
void displayCourseInformation(
    BinarySearchTree& courseTree,
    string courseNumber) {

    courseNumber = toUpperCase(trim(courseNumber));

    Course* course = courseTree.search(courseNumber);

    if (course == nullptr) {
        cout << "Course " << courseNumber << " not found." << endl;
        return;
    }

    cout << endl;
    cout << course->courseNumber
         << ", " << course->courseTitle << endl;

    cout << "Prerequisites: ";

    if (course->prerequisites.empty()) {
        cout << "None" << endl;
        return;
    }

    for (size_t i = 0; i < course->prerequisites.size(); ++i) {

        string prerequisiteNumber = course->prerequisites[i];

        Course* prerequisite =
            courseTree.search(prerequisiteNumber);

        cout << prerequisiteNumber;

        if (prerequisite != nullptr) {
            cout << ", " << prerequisite->courseTitle;
        }

        if (i < course->prerequisites.size() - 1) {
            cout << "; ";
        }
    }

    cout << endl;
}

// Main program
int main() {
    BinarySearchTree courseTree;

    string menuInput;
    string fileName;
    string courseNumber;

    bool running = true;

    cout << "Welcome to the ABCU course planner." << endl;

    while (running) {

        cout << endl;
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;
        cout << endl;
        cout << "What would you like to do? ";

        getline(cin, menuInput);

        if (menuInput == "1") {

            cout << "Enter the course data file name: ";
            getline(cin, fileName);

            loadCourses(trim(fileName), courseTree);
        }

        else if (menuInput == "2") {

            if (courseTree.isEmpty()) {
                cout << "Please load the course data first." << endl;
            }
            else {
                cout << endl;
                cout << "Here is a sample schedule:" << endl;
                cout << endl;

                courseTree.printCourseList();
            }
        }

        else if (menuInput == "3") {

            if (courseTree.isEmpty()) {
                cout << "Please load the course data first." << endl;
            }
            else {
                cout << "What course do you want to know about? ";
                getline(cin, courseNumber);

                displayCourseInformation(
                    courseTree,
                    courseNumber
                );
            }
        }

        else if (menuInput == "9") {
            cout << "Thank you for using the course planner!" << endl;
            running = false;
        }

        else {
            cout << menuInput << " is not a valid option." << endl;
        }
    }

    return 0;
}
