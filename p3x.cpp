// AUTHOR:   Jacobie Fullerton
// FILENAME: p3x.cpp
// DATE:     11/11/2023
// PURPOSE:  Implements the main functionality of the hospital triage system,
//           including user interactions and command processing.
// INPUT:    User commands from the console or a file.
// PROCESS:  Executes commands to manipulate the patient priority queue.
// OUTPUT:   Displays information about patients and the triage system.

#include "PatientPriorityQueuex.h"
#include <array>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// Function prototypes

// Prints welcome message.
void welcome();

// Prints goodbye message.
void goodbye();

// Prints help menu.
void help();

// Process the line entered from the user or read from the file.
// Precondition: Input is initiated and the line can be delimited
// Postcondition: The patient is added to the priority queue.
bool processLine(string, PatientPriorityQueuex &);

// Adds the patient to the waiting room.
// Precondition: The input string contains valid priority code / patient name.
// Postcondition: The patient is added to the priority queue.
void addPatientCmd(string, PatientPriorityQueuex &);

// Changes the priority code of the patient referenced by their arrival
// Precondition: The input string contains valid priority code / patient name.
// Postcondition: The patient's priority code is changed.
void change(string, PatientPriorityQueuex &);

// Displays the next patient in the waiting room that will be called.
// Precondition: The priority queue is not empty.
// Postcondition: The highest priority patient is printed.
void peekNextCmd(PatientPriorityQueuex &);

// Removes a patient from the waiting room and displays the name on the screen.
// Precondition: The priority queue is not empty.
// Postcondition: The highest priority patient is removed from the queue.
void removePatientCmd(PatientPriorityQueuex &);

// Displays the list of patients in the waiting room.
// Precondition: The priority queue may be empty.
// Postcondition: The list of patients is printed.
void showPatientListCmd(PatientPriorityQueuex &);

// Reads a text file with each command on a separate line and executes the
// lines as if they were typed into the command prompt.
// Precondition: The file with the given filename exists.
// Postcondition: The commands from the file are executed.
void execCommandsFromFileCmd(string&, PatientPriorityQueuex &);

// Delimits (by space) the string from user or file input.
// Precondition: None
// Postcondition: Returns subsection of line before first space.
string delimitBySpace(string &);

// Convert an entire string to lower case.
// Precondition: None
// Postcondition: Returns full lower case string.
string toLower(const string&);

// Saves the current patient queue to a file.
// Precondition: None
// Postcondition: Saves the patient queue at the file path given
void save(string, PatientPriorityQueuex &);

int main() {
    // declare variables
    string line;

    // welcome message
    welcome();

    // process commands
    PatientPriorityQueuex priQueue;

    do {
        cout << "\ntriage> ";
        getline(cin, line);
        line += " ";
    } while (processLine(line, priQueue));

    // goodbye message
    goodbye();
}

// Processes a line of input and executes the corresponding command
bool processLine(string line, PatientPriorityQueuex &priQueue) {
    // get command
    string cmd = delimitBySpace(line);
    if (cmd.length() == 0) {
        cout << "Error: no command given.";
        return false;
    }

    cmd = toLower(cmd);

    // process user input
    if (cmd == "help")
        help();
    else if (cmd == "add")
        addPatientCmd(line, priQueue);
    else if (cmd == "change")
        change(line, priQueue);
    else if (cmd == "peek")
        peekNextCmd(priQueue);
    else if (cmd == "next")
        removePatientCmd(priQueue);
    else if (cmd == "list")
        showPatientListCmd(priQueue);
    else if (cmd == "load")
        execCommandsFromFileCmd(line, priQueue);
    else if (cmd == "save")
        save(line, priQueue);
    else if (cmd == "quit")
        return false;
    else
        cout << "Error: unrecognized command: " << cmd << endl;
    return true;
}

// Trims leading and trailing whitespace from a string
string trim(const string& str) {
    int start = str.find_first_not_of(' ');
    int end = str.find_last_not_of(' ');

    if (start == -1 || end == -1) {
        // String is empty or contains only whitespaces
        return "";
    }

    return str.substr(start, end - start + 1);
}

// Parses input for the "add" command and extracts priority code and patient name
bool parseAddPatientInput(string line, string &priority, string &name) {
    // Removes leading and trailing whitespace
    line = trim(line);

    priority = delimitBySpace(line);
    priority = toLower(priority);

    if (priority.length() == 0) {
        cout << "Error: no priority code given.\n";
        return false;
    }

    name = line;
    name = trim(name);

    if (name.length() == 0) {
        cout << "Error: no patient name given.\n";
        return false;
    }

    return true;
}

// Maps priority codes to their corresponding index
int getPriorityCode(string priority) {
    array<string, 4> priorities = {"immediate", "emergency", "urgent", "minimal"};

    for (int i = 0; i < 4; i++) {
        if (priorities[i] == priority) {
            return i + 1;
        }
    }

    return -1; // Invalid priority code
}

// Executes the "add" command to add a patient to the priority queue
void addPatientCmd(string line, PatientPriorityQueuex &priQueue) {
    // Parse input
    string priority, name;
    if (!parseAddPatientInput(line, priority, name)) {
        return; // Error occurred during input parsing
    }

    // Assign priority code
    int priorityCode = getPriorityCode(priority);
    if (priorityCode == -1) {
        cout << "Error: invalid priority code.\n";
        return;
    }

    // Add patient to the priority system
    priQueue.add(Patient(name, priorityCode, priQueue.size() + 1));
    cout << " Patient " + name + " added to the priority system\n";
}

void change(string line, PatientPriorityQueuex &priQueue) {
    int arrivalID, priorityCode;
    stringstream ss;

    if (line.length() == 0) {
        cout << "Error: no patient id given.\n";
        return;
    }

    ss << delimitBySpace(line);
    ss >> arrivalID;

    line = toLower(line);
    line = trim(line);
    priorityCode = getPriorityCode(line);

    if (priorityCode == -1) {
        cout << "Error: invalid priority code.\n";
        return;
    }

    cout << priQueue.change(arrivalID, priorityCode);
}

// Executes the "peek" command to display the next patient in line
void peekNextCmd(PatientPriorityQueuex &priQueue) {
    // Check if queue is empty
    if (priQueue.size() == 0) {
        cout << "Queue is empty.\n";
        return;
    }

    // Peek at the next patient
    cout << "Highest priority patient to be called next: " << priQueue.peek();
}

// Executes the "next" command to remove the next patient from the queue
void removePatientCmd(PatientPriorityQueuex &priQueue) {
    // Check if queue is empty
    if (priQueue.size() == 0) {
        cout << "Queue is empty.\n";
        return;
    }

    // Remove the next patient from the queue
    cout << "This patient will now be seen: " << priQueue.peek();
    priQueue.remove();
}

// Executes the "list" command to display the list of patients in the waiting room
void showPatientListCmd(PatientPriorityQueuex &priQueue) {
    cout << "# patients waiting: " << priQueue.size() << endl;
    cout << "  Arrival #   Priority Code   Patient Name\n"
         << "+-----------+---------------+--------------+\n";
    cout << priQueue.to_string();
}

// Executes the "load" command to read and execute commands from a file
void execCommandsFromFileCmd(string& filename, PatientPriorityQueuex &priQueue) {
    ifstream infile;
    string line;

    // Removes leading or trailing whitespace
    filename = trim(filename);

    // open and read from file
    infile.open(filename);

    if (infile) {
        while (getline(infile, line)) {
            cout << "\ntriage> " << line;
            // process file input
            processLine(line, priQueue);
        }
    } else {
        cout << "Error: could not open file." << endl;
    }

    // close file
    infile.close();
}

// Delimits (by space) the string from user or file input.
string delimitBySpace(string &s) {
    const char SPACE = ' ';
    size_t pos = 0;
    string result;

    pos = s.find(SPACE);
    if (pos == string::npos)
        return s;

    result = s.substr(0, pos);
    s.erase(0, pos + 1);
    return result;
}


// Saves all patients in the queue to a file
void save(string fileName, PatientPriorityQueuex &priQueue) {
    ofstream ofile;

    // Removes leading and trailing whitespace
    fileName = trim(fileName);
    if (fileName.length() == 0) {
        cout << "Error: no file name given.\n";
        return;
    }

    // Open the file
    ofile.open(fileName, ios::out);

    if (!ofile.is_open()) {
        cout << "Error: Unable to open the file.\n";
        return;
    }

    // Write data to the file
    ofile << priQueue.save();

    // Close the file
    ofile.close();

    cout << "File saved successfully.\n";
}

// Converts a string to all lower case
string toLower(const string& str) {
    string result = str;
    // Iterates through string and converts each upper char to a lower
    for (char &c : result) {
        c = tolower(c);
    }
    return result;
}

// Prints a welcome message to the user
void welcome() {
	cout << "Welcome to the hospital triage system. \nEnter your commands "
            "below to use the priority queueing system." "\nUse command "
            "\"help\" for a list of commands\n";
}

// Prints a goodbye message to the user
void goodbye() {
	cout << "Exiting...";
}

// Prints a help message to the user
void help() {
	cout << "add <priority-code> <patient-name>\n"
<< "            Adds the patient to the triage system.\n"
<< "            <priority-code> must be one of the 4 accepted priority codes:\n"
<< "                1. immediate 2. emergency 3. urgent 4. minimal\n"
<< "            <patient-name>: patient's full legal name (may contain spaces)\n"
<< "change <arrival-number> <priority-code>\n"
<< "            Changes the patients priority code within the queue, but not\n"
<< "            their arrival number.\n"
<< "next        Announces the patient to be seen next. Takes into account the\n"
<< "            type of emergency and the patient's arrival order.\n"
<< "peek        Displays the patient that is next in line, but keeps in queue\n"
<< "list        Displays the list of all patients that are still waiting\n"
<< "            in the order that they have arrived.\n"
<< "save <file> Saves the exporting the command for each patient\n"
<< "load <file> Reads the file and executes the command on each line\n"
<< "help        Displays this menu\n"
<< "quit        Exits the program\n";
}
