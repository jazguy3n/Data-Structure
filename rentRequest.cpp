#include <string>
#include <iostream>
#include <iomanip>

class RentRequest
{
public:
    std::string propertyID;
    std::string tenantID;
    std::string requestStatus;

    RentRequest(const std::string &propID, const std::string &tID, const std::string &status)
        : propertyID(propID), tenantID(tID), requestStatus(status) {}
};

class Node
{
public:
    RentRequest *data;
    Node *prev;
    Node *next;

    Node(RentRequest *requestData) : data(requestData), prev(nullptr), next(nullptr) {}
};

class DoublyLinkedList
{
private:
    Node *rentRequestsHead;

public:
    DoublyLinkedList() : rentRequestsHead(nullptr) {}

    void addRentRequest(const std::string &propID, const std::string &tID, const std::string &status)
    {
        // Check if a request with the same property ID and tenant ID already exists
        Node *current = rentRequestsHead;
        while (current)
        {
            if (current->data->propertyID == propID && current->data->tenantID == tID)
            {
                std::cout << "Duplicate rent request: Property " << propID << " already requested by Tenant " << tID << ".\n";
                return;
            }
            current = current->next;
        }

        RentRequest *newRequest = new RentRequest(propID, tID, status);
        Node *newNode = new Node(newRequest);

        if (!rentRequestsHead)
        {
            rentRequestsHead = newNode;
        }
        else
        {
            Node *lastNode = rentRequestsHead;
            while (lastNode->next)
            {
                lastNode = lastNode->next;
            }
            lastNode->next = newNode;
            newNode->prev = lastNode;
        }
    }

    void displayRentRequests()
    {
        std::cout << "\nRent Requests:\n";
        Node *current = rentRequestsHead;
        std::cout << "-------------------------------------------\n";
        std::cout << std::left << std::setw(15) << "Property ID" << std::setw(15)
                  << "Tenant ID" << std::setw(15) << "Request Status"
                  << "\n";
        std::cout << "-------------------------------------------\n";
        while (current)
        {
            std::cout << std::left << std::setw(15) << current->data->propertyID << std::setw(15)
                      << current->data->tenantID << std::setw(15) << current->data->requestStatus << "\n";
            current = current->next;
        }
    }

    void displayRentingHistory(const std::string &tenantID) const
    {
        std::cout << "\nRenting History for Tenant ID: " << tenantID << "\n";
        Node *current = rentRequestsHead;
        std::cout << "-------------------------------------------\n";
        std::cout << std::left << std::setw(15) << "Property ID" << std::setw(15)
                  << "Tenant ID" << std::setw(15) << "Request Status"
                  << "\n";
        std::cout << "-------------------------------------------\n";
        while (current)
        {
            if (current->data->tenantID == tenantID)
            {
                std::cout << std::left << std::setw(15) << current->data->propertyID << std::setw(15)
                          << current->data->tenantID << std::setw(15) << current->data->requestStatus << "\n";
            }
            current = current->next;
        }
    }

    ~DoublyLinkedList()
    {
        while (rentRequestsHead)
        {
            Node *temp = rentRequestsHead;
            rentRequestsHead = rentRequestsHead->next;
            delete temp->data; // Delete the RentRequest object
            delete temp;       // Delete the node
        }
    }
};
