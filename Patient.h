// AUTHOR:   Jacobie Fullerton
// FILENAME: Patient.h
// DATE:     11/11/2023
// PURPOSE:  Defines the Patient class, which encapsulates logic for storing,
//           sorting, and printing patient information. Overloaded operators
//           facilitate patient sorting, and an enumerator is utilized to
//           convert priority values to strings.
// INPUT:    Patient object, name, priority code as an integer, and arrival
//           time as an integer.
// PROCESS:  Handles the storage and sorting of patient values, and provides
//           comparison logic based on priority code and arrival time.
// OUTPUT:   String representation of the Patient object.

#ifndef P3_PATIENT_H
#define P3_PATIENT_H

#include <string>
#include <sstream>
#include <iostream>
#include "PatientPriorityQueuex.h"

using namespace std;

class Patient {
public:
    // Constructor
    Patient(const string& name, int priorityCode, int arrivalTime);

    // Destructor
    ~Patient();

    // Overloaded operators

    // Checks if the object is smaller than the other object
    // Precondition: none
    // Postcondition: Returns true if the object is less than the other
    bool operator<(const Patient& other) const;

    // Checks if the object is greater than the other object
    // Precondition: none
    // Postcondition: Returns true if the object is greater than the other
    bool operator>(const Patient& other) const;

    // Getters

    // Returns the value of the name variable
    // Precondition: none
    // Postcondition: Returns name
    string getName() const;

    // Returns an int value of the priority code
    // Precondition: none
    // Postcondition: Value representing the integer priority code
    int getPriorityCode() const;

    // Returns the arrival time of the patient
    // Precondition: none
    // Postcondition:Returns arrival time
    int getArrivalTime() const;

    // Setters

    void decrementArrival();

    // To string
    string to_string() const;

private:
    string name;
    int priorityCode;
    int arrivalTime;

    // Holds string representations of the priority codes
    enum Priority { Immediate = 1, Emergency = 2, Urgent = 3, Minimal = 4 };
};

// Constructor
Patient::Patient(const string& nameInput, int priorityCodeInput,
                 int arrivalTimeInput)
        : name(nameInput), priorityCode(priorityCodeInput),
          arrivalTime(arrivalTimeInput) {
}

Patient::~Patient(){}

// Name getter
string Patient::getName() const {
    return name;
}

// PriorityCode getter
int Patient::getPriorityCode() const {
    return priorityCode;
}

// ArrivalTime getter
int Patient::getArrivalTime() const {
    return arrivalTime;
}

// Decreases patient's arrival time by 1
void Patient::decrementArrival() {
    --arrivalTime;
}

// Returns the patient as a string
string Patient::to_string() const {
    stringstream ss;
    ss << arrivalTime <<  " " << getPriorityCode() <<  " " << name;
    return ss.str();
}

// Overloaded operator for comparing patients by priority code then arrival
bool Patient::operator<(const Patient& other) const {
    if (priorityCode < other.priorityCode) {
        return true;
    } else if (priorityCode == other.priorityCode) {
        return arrivalTime > other.arrivalTime;
    } else {
        return false;
    }
}

// Overloaded operator for comparing patients by priority code then arrival
bool Patient::operator>(const Patient& other) const {
    if (priorityCode > other.priorityCode) {
        return true;
    } else if (priorityCode == other.priorityCode) {
        return arrivalTime > other.arrivalTime;
    } else {
        return false;
    }
}

#endif //P3_PATIENT_H
