#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <cctype>
#include <map>
#include <unordered_set>
#include "rentRequest.cpp"

class User
{
private:
    bool loggedIn;

public:
    std::string username;
    std::string password;
    std::string role;
    std::string status;

    virtual ~User() {}
    User(const std::string &uname, const std::string &pass, const std::string &userRole, const std::string &userStatus)
        : username(uname), password(pass), role(userRole), loggedIn(false), status(userStatus)
    {
    }

    virtual std::string getUserType() const = 0;

    bool login(const std::string &pass)
    {
        if (status != "Active")
        {
            std::cout << "User status is not Active. Cannot log in.\n";
            return false;
        }

        if (pass == password)
        {
            loggedIn = true;
            return true;
        }
        return false;
    }

    void logout()
    {
        loggedIn = false;
    }

    bool isLoggedIn() const
    {
        return loggedIn;
    }
};

class Tenant : public User
{
private:
    static int tenantCount;

public:
    FavoriteProperty favoriteProperty;
    std::string tenantID;
    int age;
    std::string fullName;
    std::string phoneNumber;
    std::string email;

    Tenant(const std::string &uname, const std::string &pass, int age, const std::string &name,
           const std::string &phone, const std::string &email, const std::string &status)
        : User(uname, pass, "Tenant", status), age(age), fullName(name), phoneNumber(phone), email(email)
    {
        tenantCount++;
        tenantID = "T" + std::to_string(tenantCount);
    }

    std::string getUsername() const
    {
        return username;
    }

    FavoriteProperty *getFavoriteProperty()
    {
        return &favoriteProperty;
    }

    std::string getUserType() const override
    {
        return "Tenant";
    }

    bool matchesSearchCriteria(const std::string &nameCriteria, const std::string &statusCriteria) const
    {
        if (!nameCriteria.empty() && !statusCriteria.empty())
        {
            return (fullName.find(nameCriteria) == 0) && (status == statusCriteria);
        }
        else if (!nameCriteria.empty())
        {
            return (fullName.find(nameCriteria) == 0);
        }
        else if (!statusCriteria.empty())
        {
            return (status == statusCriteria);
        }
        else
        {
            return false;
        }
    }

    static void registerTenant(User **users, int &numUsers)
    {
        std::string newUsername;
        std::string newPassword;

        bool usernameExists = true;
        while (usernameExists)
        {
            std::cout << "Enter new username: ";
            std::cin >> newUsername;

            // Check if the username already exists
            usernameExists = false;
            for (int i = 0; i < numUsers; ++i)
            {
                if (users[i]->username == newUsername)
                {
                    std::cout << "Username already exists. Please choose a different username.\n";
                    usernameExists = true;
                    break;
                }
            }
        }

        std::cout << "Enter new password: ";
        std::cin >> newPassword;

        int age;
        std::string fullName;
        std::string phoneNumber;
        std::string email;
        std::string status = "Active";

        std::cout << "Enter age: ";
        std::cin >> age;

        std::cout << "Enter full name: ";
        std::cin.ignore(); // Clear newline character from previous input
        std::getline(std::cin, fullName);

        std::cout << "Enter phone number: ";
        std::cin >> phoneNumber;

        std::cout << "Enter email: ";
        std::cin >> email;

        users[numUsers++] = new Tenant(newUsername, newPassword, age, fullName, phoneNumber, email, status);

        std::cout << "Tenant registered successfully.\n";
    }

    void sortMenu(Tenant &loggedInTenant, Property *properties, int numProperties)
    {
        int sortChoice;
        std::cout << "Sort options:\n";
        std::cout << "1. QuickSort\n";
        std::cout << "2. MergeSort\n";
        std::cout << "Enter your choice: ";
        std::cin >> sortChoice;

        switch (sortChoice)
        {
        case 1:
            loggedInTenant.quickSortProperties(properties, 0, numProperties - 1);
            std::cout << "Sorted properties using QuickSort:\n";
            break;
        case 2:
            loggedInTenant.mergeSortProperties(properties, 0, numProperties - 1);
            std::cout << "Sorted properties using MergeSort:\n";
            break;
        default:
            std::cout << "Invalid sort choice.\n";
            return;
        }

        printProperties(properties, numProperties);
    }

    void quickSortProperties(Property *properties, int left, int right)
    {
        if (left < right)
        {
            // Choose pivot and partition properties
            int pivotIndex = (left + right) / 2;
            Property pivotProperty = properties[pivotIndex];
            int i = left - 1;
            int j = right + 1;

            while (true)
            {
                do
                {
                    i++;
                } while (properties[i].monthly_rent > pivotProperty.monthly_rent ||
                         (properties[i].monthly_rent == pivotProperty.monthly_rent &&
                          properties[i].location.compare(pivotProperty.location) > 0) ||
                         (properties[i].monthly_rent == pivotProperty.monthly_rent &&
                          properties[i].location == pivotProperty.location &&
                          properties[i].size > pivotProperty.size));

                do
                {
                    j--;
                } while (properties[j].monthly_rent < pivotProperty.monthly_rent ||
                         (properties[j].monthly_rent == pivotProperty.monthly_rent &&
                          properties[j].location.compare(pivotProperty.location) < 0) ||
                         (properties[j].monthly_rent == pivotProperty.monthly_rent &&
                          properties[j].location == pivotProperty.location &&
                          properties[j].size < pivotProperty.size));

                if (i < j)
                    std::swap(properties[i], properties[j]);
                else
                    break;
            }

            quickSortProperties(properties, left, j);
            quickSortProperties(properties, j + 1, right);
        }
    }

    // Merge two sorted halves of the array during the merge step of Mergesort
    void merge(Property *properties, int left, int mid, int right)
    {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        Property *leftArr = new Property[n1];
        Property *rightArr = new Property[n2];

        for (int i = 0; i < n1; i++)
            leftArr[i] = properties[left + i];
        for (int j = 0; j < n2; j++)
            rightArr[j] = properties[mid + 1 + j];

        int i = 0, j = 0, k = left;

        while (i < n1 && j < n2)
        {
            if (leftArr[i].monthly_rent >= rightArr[j].monthly_rent ||
                (leftArr[i].monthly_rent == rightArr[j].monthly_rent &&
                 leftArr[i].location.compare(rightArr[j].location) <= 0) ||
                (leftArr[i].monthly_rent == rightArr[j].monthly_rent &&
                 leftArr[i].location == rightArr[j].location &&
                 leftArr[i].size >= rightArr[j].size))
            {
                properties[k] = leftArr[i];
                i++;
            }
            else
            {
                properties[k] = rightArr[j];
                j++;
            }
            k++;
        }

        while (i < n1)
        {
            properties[k] = leftArr[i];
            i++;
            k++;
        }

        while (j < n2)
        {
            properties[k] = rightArr[j];
            j++;
            k++;
        }

        delete[] leftArr;
        delete[] rightArr;
    }

    // Apply Mergesort algorithm to sort properties
    void mergeSortProperties(Property *properties, int left, int right)
    {
        if (left < right)
        {
            int mid = left + (right - left) / 2;
            mergeSortProperties(properties, left, mid);
            mergeSortProperties(properties, mid + 1, right);
            merge(properties, left, mid, right);
        }
    }

    void linearSearchProperties(const Property *properties, int numProperties, int searchOption) const
    {
        switch (searchOption)
        {
        case 1: // Search by Rooms
        {
            std::string numRoomsStr;
            std::cout << "Enter number of rooms: ";
            std::cin >> numRoomsStr;

            bool hasInvalidInput = false; // Biến để kiểm tra có lỗi không

            for (int i = 0; i < numProperties; ++i)
            {
                const Property &property = properties[i];

                try
                {
                    int numRooms = std::stoi(numRoomsStr);
                    if (std::stoi(property.rooms) == numRooms)
                    {
                        property.display();
                        std::cout << "\n";
                    }
                }
                catch (const std::invalid_argument &e)
                {
                    hasInvalidInput = true; // Đánh dấu có lỗi
                    break;                  // Thoát vòng lặp khi gặp lỗi
                }
            }

            if (!hasInvalidInput)
            {
                std::cout << "Search results:\n";
            }
            break;
        }
        case 2: // Search by Furnished
        {
            std::string furnishedChoice;
            std::cout << "Choose furnished type:\n";
            std::cout << "1. Fully Furnished\n";
            std::cout << "2. Partially Furnished\n";
            std::cout << "3. Not Furnished\n";
            std::cout << "Enter your choice: ";
            std::cin >> furnishedChoice;

            std::cout << "Search results:\n";

            for (int i = 0; i < numProperties; ++i)
            {
                const Property &property = properties[i];

                if ((furnishedChoice == "1" && property.furnished == "Fully Furnished") ||
                    (furnishedChoice == "2" && property.furnished == "Partially Furnished") ||
                    (furnishedChoice == "3" && property.furnished == "Not Furnished"))
                {
                    property.display();
                    std::cout << "\n";
                }
            }
            break;
        }
        case 3: // Search by Property Type
        {
            std::string propertyType;
            std::cout << "Choose property type:\n";
            std::cout << "1. Condominium\n";
            std::cout << "2. Apartment\n";
            std::cout << "3. Service Residence\n";
            std::cout << "4. Studio\n";
            std::cout << "5. Flat\n";
            std::cout << "6. Others\n";
            std::cout << "Enter your choice: ";
            std::cin >> propertyType;

            std::cout << "Search results:\n";

            for (int i = 0; i < numProperties; ++i)
            {
                const Property &property = properties[i];

                if ((propertyType == "1" && property.property_type == "Condominium") ||
                    (propertyType == "2" && property.property_type == "Apartment") ||
                    (propertyType == "3" && property.property_type == "Service Residence") ||
                    (propertyType == "4" && property.property_type == "Studio") ||
                    (propertyType == "5" && property.property_type == "Flat") ||
                    (propertyType == "6" && property.property_type == "Others"))
                {
                    property.display();
                    std::cout << "\n";
                }
            }
            break;
        }
        default:
            std::cout << "Invalid search option.\n";
            break;
        }
    }

    static bool comparePropertiesByRooms(const Property &a, const Property &b)
    {
        int numRoomsA = std::stoi(a.rooms);
        int numRoomsB = std::stoi(b.rooms);

        if (numRoomsA != numRoomsB)
        {
            return numRoomsA < numRoomsB;
        }

        // If the number of rooms is the same, compare other attributes
        // For example, you can use location or size as tie-breakers
        if (a.location != b.location)
        {
            return a.location < b.location;
        }

        return true; // Corrected: Return true for less than comparison
    }

    static bool comparePropertiesByFurnished(const Property &a, const Property &b)
    {
        return a.furnished < b.furnished;
    }

    static bool comparePropertiesByPropertyType(const Property &a, const Property &b)
    {
        return a.property_type < b.property_type;
    }

    void binarySearchProperties(const Property *properties, int numProperties, int searchOption) const
    {
        switch (searchOption)
        {
        case 1: // Search by Rooms
        {
            std::string numRoomsStr;
            std::cout << "Enter number of rooms: ";
            std::cin >> numRoomsStr;

            bool hasInvalidInput = false;

            try
            {
                int numRooms = std::stoi(numRoomsStr);

                Property keyProperty;
                keyProperty.rooms = numRoomsStr;

                // Perform binary search
                const Property *foundProperty = std::lower_bound(properties, properties + numProperties, keyProperty, Tenant::comparePropertiesByRooms);

                if (foundProperty != properties + numProperties && std::stoi(foundProperty->rooms) == numRooms)
                {
                    // Display the found property
                    foundProperty->display();
                }
                else
                {
                    std::cout << "Property with " << numRooms << " rooms not found.\n";
                }
            }
            catch (const std::invalid_argument &e)
            {
                hasInvalidInput = true;
            }

            if (hasInvalidInput)
            {
                std::cout << "Invalid input. Please enter a valid number of rooms.\n";
            }
            break;
        }

        case 2: // Search by Furnished
        {
            std::string furnishedChoice;
            std::cout << "Choose furnished type:\n";
            std::cout << "1. Fully Furnished\n";
            std::cout << "2. Partially Furnished\n";
            std::cout << "3. Not Furnished\n";
            std::cout << "Enter your choice: ";
            std::cin >> furnishedChoice;

            std::cout << "Search results:\n";

            for (int i = 0; i < numProperties; ++i)
            {
                const Property &property = properties[i];

                if ((furnishedChoice == "1" && property.furnished == "Fully Furnished") ||
                    (furnishedChoice == "2" && property.furnished == "Partially Furnished") ||
                    (furnishedChoice == "3" && property.furnished == "Not Furnished"))
                {
                    property.display();
                    std::cout << "\n";
                }
            }
            break;
        }

        case 3: // Search by Property Type
        {
            std::string propertyType;
            std::cout << "Choose property type:\n";
            std::cout << "1. Condominium\n";
            std::cout << "2. Apartment\n";
            std::cout << "3. Service Residence\n";
            std::cout << "4. Studio\n";
            std::cout << "5. Flat\n";
            std::cout << "6. Others\n";
            std::cout << "Enter your choice: ";
            std::cin >> propertyType;

            std::cout << "Search results:\n";

            for (int i = 0; i < numProperties; ++i)
            {
                const Property &property = properties[i];

                if ((propertyType == "1" && property.property_type == "Condominium") ||
                    (propertyType == "2" && property.property_type == "Apartment") ||
                    (propertyType == "3" && property.property_type == "Service Residence") ||
                    (propertyType == "4" && property.property_type == "Studio") ||
                    (propertyType == "5" && property.property_type == "Flat") ||
                    (propertyType == "6" && property.property_type == "Others"))
                {
                    property.display();
                    std::cout << "\n";
                }
            }
            break;
        }

        default:
            std::cout << "Invalid search option.\n";
            break;
        }
    }

    void searchMenu(const Property *properties, int numProperties)
    {
        int searchOption;

        while (true)
        {
            std::cout << "Search Menu:\n";
            std::cout << "1. Linear Search\n";
            std::cout << "2. Binary Search\n";
            std::cout << "3. Back to Main Menu\n";
            std::cout << "Enter your choice: ";
            std::cin >> searchOption;

            if (searchOption == 3)
            {
                break;
            }

            if (searchOption != 1 && searchOption != 2)
            {
                std::cout << "Invalid choice. Please enter a valid option.\n";
                continue;
            }

            std::cout << "Search by:\n";
            std::cout << "1. Rooms\n";
            std::cout << "2. Furnished\n";
            std::cout << "3. Property Type\n";
            int searchType;
            std::cout << "Enter your choice: ";
            std::cin >> searchType;

            switch (searchOption)
            {
            case 1:
                linearSearchProperties(properties, numProperties, searchType);
                break;
            case 2:
                binarySearchProperties(properties, numProperties, searchType);
                break;
            }
        }
    }

    void addFavoriteProperty(const std::string &propertyID, const Property *properties, int numProperties)
    {
        favoriteProperty.addFavoriteProperty(propertyID, properties, numProperties);
    }

    void displayFavoriteProperties(const Property *properties, int numProperties)
    {
        favoriteProperty.displayFavoriteProperties(properties, numProperties);
    }

    bool isPropertyInFavorites(const std::string &propertyID) const
    {
        return favoriteProperty.isPropertyInFavorites(propertyID);
    }

    std::string getUserID() const
    {
        return tenantID; // Replace with the actual way you retrieve the tenant's ID
    }

    void placeRentRequest(DoublyLinkedList &rentRequestList, Property *properties, int numProperties)
    {
        std::cout << "Favorite Properties:\n";
        favoriteProperty.displayFavoriteProperties(properties, numProperties);

        std::string propertyID;
        std::cout << "Enter the property ID to place a rent request: ";
        std::cin >> propertyID;

        // Check if the property is in the favorites list
        if (isPropertyInFavorites(propertyID))
        {
            // Find the selected property
            Property *selectedProperty = nullptr;
            for (int i = 0; i < numProperties; ++i)
            {
                if (properties[i].ads_id == propertyID)
                {
                    selectedProperty = &properties[i];
                    break;
                }
            }

            if (selectedProperty)
            {
                // Get the tenant's ID
                std::string tenantID = getUserID();

                // Place the rent request using the rentRequestList
                rentRequestList.addRentRequest(propertyID, tenantID, "Pending");
                std::cout << "Rent request placed for property: " << selectedProperty->prop_name << "\n";
            }
            else
            {
                std::cout << "Property with ID " << propertyID << " not found.\n";
            }
        }
        else
        {
            std::cout << "You can only place a rent request for properties in your favorites.\n";
        }
    }
    void displayRentingHistory(const DoublyLinkedList &rentRequestList) const
    {
        std::string tenantID = getUserID();
        rentRequestList.displayRentingHistory(tenantID);
    }
};

class Manager : public User
{
private:
    static int managerCount;

public:
    std::string managerID;

    Manager(const std::string &uname, const std::string &pass, const std::string &id, const std::string &managerStatus)
        : User(uname, pass, "manager", managerStatus), managerID(id)
    {
        managerCount++;
        managerID = "M" + std::to_string(managerCount);
    }

    void displayAllTenantsDetails(User **users, int numUsers) const
    {
        std::cout << "Displaying all registered tenant's details:\n";

        std::cout << std::left << std::setw(12) << "Tenant ID" << std::setw(20) << "Full Name"
                  << std::setw(5) << "Age" << std::setw(15) << "Phone Number"
                  << std::setw(25) << "Email" << std::setw(10) << "Status"
                  << "\n";

        for (int i = 0; i < numUsers; ++i)
        {
            Tenant *tenant = dynamic_cast<Tenant *>(users[i]);
            if (tenant)
            {
                std::cout << std::left << std::setw(12) << tenant->tenantID << std::setw(20) << tenant->fullName
                          << std::setw(5) << tenant->age << std::setw(15) << tenant->phoneNumber
                          << std::setw(25) << tenant->email << std::setw(10) << tenant->status << "\n";
            }
        }
    }

    void searchTenantDetails(User **users, int numUsers)
    {
        int searchOption;
        std::cout << "Search by:\n";
        std::cout << "1. Name\n";
        std::cout << "2. Status\n";
        std::cout << "Enter your choice: ";
        std::cin >> searchOption;

        if (searchOption == 1)
        {
            std::string nameCriteria;
            std::cout << "Enter starting characters of tenant's name: ";
            std::cin.ignore(); // Clear newline character from previous input
            std::getline(std::cin, nameCriteria);

            // Convert the input to uppercase for case-insensitive search
            std::transform(nameCriteria.begin(), nameCriteria.end(), nameCriteria.begin(), ::toupper);

            std::cout << "Matching tenants by name:\n";
            bool found = false;
            for (int i = 0; i < numUsers; ++i)
            {
                Tenant *tenant = dynamic_cast<Tenant *>(users[i]);
                if (tenant && tenant->matchesSearchCriteria(nameCriteria, ""))
                {
                    found = true;
                    std::cout << "Tenant ID: " << tenant->tenantID << "\n";
                    std::cout << "Full Name: " << tenant->fullName << "\n";
                    std::cout << "Age: " << tenant->age << "\n";
                    std::cout << "Phone Number: " << tenant->phoneNumber << "\n";
                    std::cout << "Email: " << tenant->email << "\n";
                    std::cout << "Status: " << tenant->status << "\n";
                    std::cout << "---------------------------\n";
                }
            }

            if (!found)
            {
                std::cout << "No tenants match the search criteria.\n";
            }
        }

        else if (searchOption == 2)
        {
            std::string statusCriteria;
            std::cout << "Enter tenant's status (Active/Inactive): ";
            std::cin >> statusCriteria;

            std::cout << "Matching tenants by status:\n";
            bool found = false;
            for (int i = 0; i < numUsers; ++i)
            {
                Tenant *tenant = dynamic_cast<Tenant *>(users[i]);
                if (tenant && tenant->matchesSearchCriteria("", statusCriteria))
                {
                    found = true;
                    std::cout << "Tenant ID: " << tenant->tenantID << "\n";
                    std::cout << "Full Name: " << tenant->fullName << "\n";
                    std::cout << "Age: " << tenant->age << "\n";
                    std::cout << "Phone Number: " << tenant->phoneNumber << "\n";
                    std::cout << "Email: " << tenant->email << "\n";
                    std::cout << "Status: " << tenant->status << "\n";
                    std::cout << "---------------------------\n";
                }
            }

            if (!found)
            {
                std::cout << "No tenants match the search criteria.\n";
            }
        }
        else
        {
            std::cout << "Invalid search option.\n";
        }
    }

    void deleteInactiveTenants(User **users, int &numUsers)
    {
        // Filter inactive tenants
        std::vector<Tenant *> inactiveTenants;
        for (int i = 0; i < numUsers; ++i)
        {
            Tenant *tenant = dynamic_cast<Tenant *>(users[i]);
            if (tenant && tenant->status == "Inactive")
            {
                inactiveTenants.push_back(tenant);
            }
        }

        if (inactiveTenants.empty())
        {
            std::cout << "No inactive tenants found.\n";
            return;
        }

        // Display inactive tenants
        std::cout << "Inactive tenants:\n";
        for (size_t i = 0; i < inactiveTenants.size(); ++i)
        {
            std::cout << i + 1 << ". Tenant ID: " << inactiveTenants[i]->tenantID << " - Full Name: " << inactiveTenants[i]->fullName << "\n";
        }

        // Ask user for action
        int choice;
        std::cout << "Enter your choice:\n";
        std::cout << "1. Delete individual tenant\n";
        std::cout << "2. Delete all inactive tenants\n";
        std::cout << "Choice: ";
        std::cin >> choice;

        if (choice == 1)
        {
            // Ask user for tenant ID to delete
            int tenantChoice;
            std::cout << "Enter the number of the tenant you want to delete: ";
            std::cin >> tenantChoice;

            if (tenantChoice >= 1 && tenantChoice <= inactiveTenants.size())
            {
                Tenant *tenantToDelete = inactiveTenants[tenantChoice - 1];
                deleteTenant(users, numUsers, tenantToDelete);
            }
            else
            {
                std::cout << "Invalid choice.\n";
            }
        }
        else if (choice == 2)
        {
            // Delete all inactive tenants
            for (size_t i = 0; i < inactiveTenants.size(); ++i)
            {
                deleteTenant(users, numUsers, inactiveTenants[i]);
            }
            std::cout << "All inactive tenants deleted successfully.\n";
        }
        else
        {
            std::cout << "Invalid choice.\n";
        }
    }

    void deleteTenant(User **users, int &numUsers, Tenant *tenantToDelete)
    {
        // Find the position of tenantToDelete in the users array
        int positionToDelete = -1;
        for (int i = 0; i < numUsers; ++i)
        {
            if (users[i] == tenantToDelete)
            {
                positionToDelete = i;
                break;
            }
        }

        if (positionToDelete != -1)
        {
            // Shift elements to overwrite the tenantToDelete
            for (int i = positionToDelete; i < numUsers - 1; ++i)
            {
                users[i] = users[i + 1];
            }

            // Decrease the numUsers value
            numUsers--;

            // Delete the tenant object
            delete tenantToDelete;

            std::cout << "Tenant deleted successfully.\n";
        }
        else
        {
            std::cout << "Tenant not found.\n";
        }
    }

    void displayRentRequests(DoublyLinkedList &rentRequestList)
    {
        rentRequestList.displayRentRequests();
    }

    void displayAllTenantFavorites(User **users, int numUsers, Property *properties, int numProperties)
    {
        std::unordered_map<std::string, int> propertyCounts; // To keep track of Property ID counts

        for (int i = 0; i < numUsers; ++i)
        {
            if (users[i]->getUserType() == "Tenant")
            {
                Tenant *tenant = dynamic_cast<Tenant *>(users[i]);
                FavoriteProperty *tenantFavoriteProperty = tenant->getFavoriteProperty();

                for (int j = 0; j < tenantFavoriteProperty->numFavorites; ++j)
                {
                    const std::string &propertyID = tenantFavoriteProperty->favoritePropertyIDs[j];

                    // Update the Property ID count in the map
                    if (propertyCounts.find(propertyID) == propertyCounts.end())
                    {
                        propertyCounts[propertyID] = 1;
                    }
                    else
                    {
                        propertyCounts[propertyID]++;
                    }
                }
            }
        }

        std::cout << "\nTop 10 Favorite Properties:\n";
        std::cout << "----------------------------------------\n";
        std::cout << std::setw(15) << std::left << "Property ID" << std::setw(15) << std::right << "Frequency"
                  << "\n";
        std::cout << "----------------------------------------\n";
        int count = 0;
        for (const auto &entry : propertyCounts)
        {
            if (count >= 10)
                break;
            std::cout << std::setw(15) << std::left << entry.first << std::setw(10) << std::right << entry.second << "\n";
            count++;
        }
    }

    std::string getUserType() const override
    {
        return "Manager";
    }
};

class Admin : public User
{
public:
    std::string adminID;

    Admin(const std::string &uname, const std::string &pass, const std::string &id, const std::string &adminStatus)
        : User(uname, pass, "admin", adminStatus), adminID(id)
    {
    }

    std::string getUserType() const override
    {
        return "Admin";
    }

    static void addManager(User **users, int &numUsers)
    {
        std::string newUsername;
        std::string newPassword;
        std::string newManagerID;

        bool usernameExists = true;
        while (usernameExists)
        {
            std::cout << "Enter new username: ";
            std::cin >> newUsername;

            // Check if the username already exists
            usernameExists = false;
            for (int i = 0; i < numUsers; ++i)
            {
                if (users[i]->username == newUsername)
                {
                    std::cout << "Username already exists. Please choose a different username.\n";
                    usernameExists = true;
                    break;
                }
            }
        }

        std::cout << "Enter new password: ";
        std::cin >> newPassword;

        std::string managerStatus;
        std::cout << "Enter manager status: ";
        std::cin >> managerStatus;

        users[numUsers++] = new Manager(newUsername, newPassword, newManagerID, managerStatus);

        std::cout << "Manager registered successfully.\n";
    }

    static void modifyManagerStatus(User **users, int numUsers)
    {
        std::string targetUsername;
        std::string newStatus;

        std::cout << "\n--------------------------------------------------------------\n";
        std::cout << "Enter username of the manager to modify:\n";
        std::cout << "--------------------------------------------------------------\n";
        std::cin >> targetUsername;

        bool found = false;

        for (int i = 0; i < numUsers; ++i)
        {
            Manager *manager = dynamic_cast<Manager *>(users[i]);
            if (manager && manager->username == targetUsername)
            {
                std::cout << "Enter new status for the manager: ";
                std::cin >> newStatus;

                manager->status = newStatus;
                std::cout << "Manager status modified successfully.\n";

                found = true;
                break;
            }
        }

        if (!found)
        {
            std::cout << "Manager with the specified username not found.\n";
        }
    }

    void displayAllManagersDetails(User **users, int numUsers) const
    {
        std::cout << "--------------------------------------------------------------\n";
        std::cout << "Displaying all registered manager's info:\n";
        std::cout << "--------------------------------------------------------------\n";

        std::cout << std::left << std::setw(12) << "Manager ID" << std::setw(20) << "Manager Username"
                  << std::setw(20) << "Password" << std::setw(10) << "Status"
                  << "\n";

        for (int i = 0; i < numUsers; ++i)
        {
            Manager *manager = dynamic_cast<Manager *>(users[i]);
            if (manager)
            {
                std::cout << std::left << std::setw(12) << manager->managerID << std::setw(20) << manager->username
                          << std::setw(20) << manager->password << std::setw(10) << manager->status << "\n";
            }
        }
    }

    void displayFilteredTenants(User **users, int numUsers, const std::string &nameCriteria, const std::string &statusCriteria) const
    {
        // Display header
        std::cout << std::left << std::setw(12) << "Tenant ID" << std::setw(20) << "Full Name"
                  << std::setw(5) << "Age" << std::setw(15) << "Phone Number"
                  << std::setw(25) << "Email" << std::setw(10) << "Status"
                  << "\n";

        bool found = false;
        for (int i = 0; i < numUsers; ++i)
        {
            Tenant *tenant = dynamic_cast<Tenant *>(users[i]);
            if (tenant && tenant->matchesSearchCriteria(nameCriteria, statusCriteria))
            {
                found = true;
                std::cout << std::left << std::setw(12) << tenant->tenantID << std::setw(20) << tenant->fullName
                          << std::setw(5) << tenant->age << std::setw(15) << tenant->phoneNumber
                          << std::setw(25) << tenant->email << std::setw(10) << tenant->status << "\n";
            }
        }

        if (!found)
        {
            std::cout << "No tenants match the filter criteria.\n";
        }
    }

    void displayAllTenantsDetailsWithFilter(User **users, int numUsers) const
    {
        int filterOption;
        std::cout << "Filter by:\n";
        std::cout << "1. Name\n";
        std::cout << "2. Status\n";
        std::cout << "Enter your choice: ";
        std::cin >> filterOption;

        if (filterOption == 1)
        {
            std::string nameCriteria;
            std::cout << "Enter starting characters of tenant's name: ";
            std::cin.ignore(); // Clear newline character from previous input
            std::getline(std::cin, nameCriteria);

            // Convert the input to uppercase for case-insensitive search
            std::transform(nameCriteria.begin(), nameCriteria.end(), nameCriteria.begin(), ::toupper);

            std::cout << "Matching tenants by name:\n";
            displayFilteredTenants(users, numUsers, nameCriteria, "");
        }
        else if (filterOption == 2)
        {
            std::string statusCriteria;
            std::cout << "Enter tenant's status (Active/Inactive): ";
            std::cin >> statusCriteria;

            std::cout << "Matching tenants by status:\n";
            displayFilteredTenants(users, numUsers, "", statusCriteria);
        }
        else
        {
            std::cout << "Invalid filter option.\n";
        }
    }

    void displayAllPropertiesWithFilter(const Property *properties, int numProperties)
    {
        int filterOption;
        std::cout << "Filter properties by:\n";
        std::cout << "1. Completion Year\n";
        std::cout << "2. Property Name\n";
        std::cout << "Enter your choice: ";
        std::cin >> filterOption;

        if (filterOption == 1)
        {
            std::string yearCriteria;
            std::cout << "Enter a specific completion year to filter properties: ";
            std::cin >> yearCriteria;

            std::cout << "Matching properties with completion year " << yearCriteria << ":\n";
            bool found = false;
            for (int i = 0; i < numProperties; ++i)
            {
                if (properties[i].completion_year == yearCriteria)
                {
                    properties[i].display();
                    std::cout << "\n";
                    found = true;
                }
            }

            if (!found)
            {
                std::cout << "No properties match the specified completion year.\n";
            }
        }
        else if (filterOption == 2)
        {
            std::string nameCriteria;
            std::cout << "Enter property name to filter properties: ";
            std::cin.ignore(); // Clear newline character from previous input
            std::getline(std::cin, nameCriteria);

            std::transform(nameCriteria.begin(), nameCriteria.end(), nameCriteria.begin(), ::toupper);

            std::cout << "Matching properties by name:\n";
            bool found = false;
            for (int i = 0; i < numProperties; ++i)
            {
                if (properties[i].prop_name.find(nameCriteria) != std::string::npos)
                {
                    properties[i].display();
                    std::cout << "\n";
                    found = true;
                }
            }

            if (!found)
            {
                std::cout << "No properties match the name criteria.\n";
            }
        }
        else
        {
            std::cout << "Invalid filter option.\n";
        }
    }

    void displayMenu(User **users, int numUsers, const Property *properties, int numProperties)
    {
        int adminChoice;
        std::cout << "Display information for:\n";
        std::cout << "1. Tenant\n";
        std::cout << "2. Property\n";
        std::cout << "Enter your choice: ";
        std::cin >> adminChoice;

        if (adminChoice == 1)
        {
            displayAllTenantsDetailsWithFilter(users, numUsers);
        }
        else if (adminChoice == 2)
        {
            displayAllPropertiesWithFilter(properties, numProperties);
        }
        else
        {
            std::cout << "Invalid choice.\n";
        }
    }
};