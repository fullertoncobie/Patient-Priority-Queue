// AUTHOR:   Jacobie Fullerton
// FILENAME: PatientPriorityQueuex.h
// DATE:     11/11/2023
// PURPOSE:  Defines the PatientPriorityQueuex class that creates and manages
//           a heap of patients using a vector. The overloaded operators are
//           used to sort a vector containing the patients into heap order.
// INPUT:    Patients can be added to the heap using the add methods.
// PROCESS:  Upon adding, removing, or modifying patients, the heap is
//           reordered.
// OUTPUT:   A heap represented by a vector of patients that can be used as
//           a triage system.

#ifndef P3_PATIENTPRIORITYQUEUE_H
#define P3_PATIENTPRIORITYQUEUE_H

#include <cassert>
#include <sstream>
#include <vector>
#include <iomanip>
#include "Patient.h"

// Class representing a priority queue of patients
class PatientPriorityQueuex {
public:
    // Constructor
    PatientPriorityQueuex();

    // Destructor
    ~PatientPriorityQueuex();

    // Adds a patient to the priority queue
    // Precondition: none
    // Postcondition: Patient is added to the priority queue
    void add(const Patient&);

    // Removes the highest priority patient from the priority queue
    // Precondition: Priority queue is not empty
    // Postcondition: Highest priority patient is removed from the priority queue
    void remove();

    // Returns the current size of the priority queue
    // Precondition: none
    // Postcondition: Returns the current size of the priority queue
    int size() const;

    // Returns the name of the highest priority patient without removing them
    // Precondition: Priority queue is not empty
    // Postcondition: Returns the name of the highest priority patient
    string peek() const;

    // Converts the priority queue to a formatted string for display
    // Precondition: none
    // Postcondition: Returns a string representation of the priority queue
    string to_string();

    // Converts Exports the commands used to build the priority queue to
    // a string each on new lines
    // Precondition: none
    // Postcondition: Returns a lines of strings that comprise the queue
    string save();

    // Changes the priority of a patient in the priority queue
    // Precondition: none
    // Postcondition: Changes the patient, returns a string detailing
    // the change
    string change(int, int);

private:
    vector<Patient> data; // Vector to store patient data
    int heapSize;         // Size of the priority queue

    // Sorting helper methods

    // Moves the element at the given index up the heap to maintain heap property
    // Precondition: none
    // Postcondition: Element at the given index is moved up the heap as needed
    void siftUp(int);

    // Moves the element at the given index down the heap to maintain heap property
    // Precondition: none
    // Postcondition: Element at the given index is moved down the heap as needed
    void siftDown(int);

    // Getters for heap navigation

    // Returns the index of the parent of the object at the input index parameter
    // Precondition: none
    // Postcondition: Returns the index of the parent of the object at the input index
    static int getParent(int) ;

    // Returns the index of the left child for the input index parameter object
    // Precondition: none
    // Postcondition: Returns the index of the left child for the input index
    static int getLeftChild(int) ;

    // Returns the index of the right child for the input index parameter object
    // Precondition: none
    // Postcondition: Returns the index of the right child for the input index
    static int getRightChild(int) ;

    // Returns a string value representing the priority code
    // Precondition: none
    // Postcondition: String representation of the priority code
    string getPriorityString() const;

    // Returns the value of the priority code as a string
    // Precondition: none
    // Postcondition: Returns a string representing the priority code
    static string getPriorityString(int priority);

    // Sorts the queue by arrival number in ascending order
    // Precondition: none
    // Postcondition: Returns a sorted copy of the array
    vector<Patient> sortByArrival();
};

// Constructor
PatientPriorityQueuex::PatientPriorityQueuex() {
    heapSize = 0;
}

// Destructor
PatientPriorityQueuex::~PatientPriorityQueuex() {
}

// Adds a patient to the priority queue
void PatientPriorityQueuex::add(const Patient& patient) {
    heapSize++;
    data.push_back(patient);
    siftUp(heapSize - 1);
}

void PatientPriorityQueuex::remove() {
    assert(heapSize != 0);

    // Decrements the arrival time of all patients after the removed patient
    for (int i = 0; i < heapSize; i++) {
        if (data[i].getArrivalTime() > data[0].getArrivalTime()) {
            data[i].decrementArrival();
        }
    }

    data[0] = data[--heapSize];
    data.pop_back();
    if (heapSize > 1)
        siftDown(0);
}

string PatientPriorityQueuex::change(int arrivalID, int newPriority) {
    for (int i = 0; i < heapSize; i++) {
        if (data[i].getArrivalTime() == arrivalID) {
            Patient replacedCode(data[i].getName(), newPriority,
                                 data[i].getPriorityCode());
            data[i] = replacedCode;
            return "Changed patient " + data[i].getName() +
                   "'s priority to " + getPriorityString(data[i].getPriorityCode());
        }
    }
    return "Patient with given id was not found.";
}

// Returns the current size of the priority queue
int PatientPriorityQueuex::size() const {
    return heapSize;
}

// Returns the name of the highest priority patient without removing them
string PatientPriorityQueuex::peek() const {
    assert(heapSize != 0);
    return data.front().getName();
}

// Converts the priority queue to a formatted string for display
string PatientPriorityQueuex::to_string() {
    std::stringstream ss;

    for (int i = 0; i < heapSize; ++i) {
        ss << right << setw(7) << data[i].getArrivalTime() << "\t";
        ss << left << "\t" << setw(13) << getPriorityString(data[i].getPriorityCode());
        ss << setw(16) << data[i].getName();
        if (i < heapSize - 1) {
            ss << "\n";
        }
    }
    ss << "\n";

    return ss.str();
}

string PatientPriorityQueuex::getPriorityString(int priority) {
    vector<string> priorities = {"immediate", "emergency", "urgent", "minimal"};

    return priorities[priority - 1];
}

// Moves the element at the given index up the heap to maintain heap property
void PatientPriorityQueuex::siftUp(int index) {
    int parentIdx;
    if (index != 0) {
        parentIdx = getParent(index);

        // Overloaded operators for comparing patient objects
        if (data[parentIdx] > data[index]) {
            swap(data[parentIdx], data[index]);
            siftUp(parentIdx);
        }
    }
}

// Moves the element at the given index down the heap to maintain heap property
void PatientPriorityQueuex::siftDown(int index) {
    size_t leftIdx, rightIdx, maxIdx;
    leftIdx = getLeftChild(index);
    rightIdx = getRightChild(index);

    if (rightIdx >= data.size()) {
        if (leftIdx >= data.size())
            return;
        else
            maxIdx = leftIdx;
    } else {
        // Overloaded operators for comparing patient objects
        if (data[leftIdx] > data[rightIdx])
            maxIdx = leftIdx;
        else
            maxIdx = rightIdx;
    }
    // Overloaded operators for comparing patient objects
    if (data[index] < data[maxIdx]) {
        swap(data[maxIdx], data[index]);
        siftDown(maxIdx);
    }
}

// Returns the index of the parent of the object at the input index parameter
int PatientPriorityQueuex::getParent(int index) {
    return (index - 1) / 2;
}

// Returns the index of the left child for the input index parameter object
int PatientPriorityQueuex::getLeftChild(int index) {
    return 2 * index + 1;
}

// Returns the index of the right child for the input index parameter object
int PatientPriorityQueuex::getRightChild(int index) {
    return 2 * index + 2;
}

vector<Patient> PatientPriorityQueuex::sortByArrival() {
    // Create a copy of the data vector
    vector<Patient> copy = data;

    // Bubble sort based on arrival times
    for (int i = 0; i < heapSize - 1; ++i) {
        for (int j = 0; j < heapSize - i - 1; ++j) {
            if (copy[j].getArrivalTime() > copy[j + 1].getArrivalTime()) {
                // Swap if the current element is greater than the next element
                swap(copy[j], copy[j + 1]);
            }
        }
    }

    return copy;
}

string PatientPriorityQueuex::save() {
    vector<Patient> copy = sortByArrival();

    // Create a formatted string for displaying the sorted data
    std::stringstream ss;

    for (int i = 0; i < heapSize; ++i) {
        ss << "add " << getPriorityString(copy[i].getPriorityCode()) <<
        " " << copy[i].getName();
        if (i < heapSize - 1) {
            ss << "\n";
        }
    }
    ss << "\n";

    // Return the formatted string
    return ss.str();
}
#endif //P3_PATIENTPRIORITYQUEUE_H
