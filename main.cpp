#include "property.cpp"
#include "user.cpp"
#include "init.hpp"

int Manager::managerCount = 0;
int Tenant::tenantCount = 0;

int main()
{
    Property *properties = nullptr;
    int numProperties = 0;

    User **users = nullptr;
    int numUsers = 0;

    DoublyLinkedList rentRequestList;

    initializeData(properties, numProperties, users, numUsers);
    initializeFavoriteProperties(users, numUsers, properties, numProperties);

    int choice;
    bool running = true;

    while (running)
    {
        std::cout << "\nWelcome to Klang Valley Accommodation:\n";
        std::cout << "1. Register Tenant\n";
        std::cout << "2. Display all property's details\n";
        std::cout << "3. Log in\n";
        std::cout << "4. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice)
        {
        case 1:
        {
            Tenant::registerTenant(users, numUsers);
            break;
        }
        case 2:
            printProperties(properties, numProperties);
            break;
        case 3:
        {
            std::string inputUsername;
            std::string inputPassword;

            std::cout << "Enter your username: ";
            std::cin >> inputUsername;

            std::cout << "Enter your password: ";
            std::cin >> inputPassword;

            bool loggedIn = false;
            User *loggedInUser = nullptr;

            for (int i = 0; i < numUsers; ++i)
            {
                if (users[i]->username == inputUsername && users[i]->login(inputPassword))
                {
                    loggedIn = true;
                    loggedInUser = users[i];
                    break;
                }
            }

            if (loggedIn)
            {
                std::cout << "Logged in successfully as " << loggedInUser->role << ".\n";

                Tenant *tenant = dynamic_cast<Tenant *>(loggedInUser);
                Manager *manager = dynamic_cast<Manager *>(loggedInUser);
                Admin *admin = dynamic_cast<Admin *>(loggedInUser);

                if (tenant)
                {
                    int choice;
                    bool loggedIn = true;

                    while (loggedIn)
                    {
                        std::cout << "\nTenant Menu:\n";
                        std::cout << "1. Sort property\n";
                        std::cout << "2. Search property\n";
                        std::cout << "3. Save favourite property\n";
                        std::cout << "4. Place rent request\n";
                        std::cout << "5. Display renting history\n";
                        std::cout << "6. Logout\n";
                        std::cout << "Enter your choice: ";
                        std::cin >> choice;

                        switch (choice)
                        {
                        case 1:
                        {
                            Tenant loggedInTenant = dynamic_cast<Tenant &>(*loggedInUser);
                            tenant->sortMenu(loggedInTenant, properties, numProperties);
                            break;
                        }
                        case 2:
                            tenant->searchMenu(properties, numProperties);
                            break;
                        case 3:
                        {
                            int subChoice;
                            std::cout << "\nMenu:\n";
                            std::cout << "1. Add favorite property\n";
                            std::cout << "2. Display favorite properties\n";
                            std::cout << "Enter your choice: ";
                            std::cin >> subChoice;

                            if (subChoice == 1)
                            {
                                std::string propertyID;
                                std::cout << "Enter property ID to add to favorites: ";
                                std::cin >> propertyID;
                                tenant->addFavoriteProperty(propertyID, properties, numProperties);
                            }
                            else if (subChoice == 2)
                            {
                                tenant->displayFavoriteProperties(properties, numProperties);
                            }
                            else
                            {
                                std::cout << "Invalid choice. Try again.\n";
                            }
                            break;
                        }
                        case 4:
                        {
                            tenant->placeRentRequest(rentRequestList, properties, numProperties); // Pass rentRequestList
                            break;
                        }
                        case 5:
                            tenant->displayRentingHistory(rentRequestList);
                            break;
                        case 6:
                            tenant->logout();
                            std::cout << "Logged out.\n";
                            loggedIn = false;
                            break;
                        default:
                            std::cout << "Invalid choice. Try again.\n";
                            break;
                        }
                    }
                }
                else if (manager)
                {
                    int choice;
                    bool loggedIn = true;

                    while (loggedIn)
                    {
                        std::cout << "\nManager Menu:\n";
                        std::cout << "1. Display all registered tenant's detail\n";
                        std::cout << "2. Search tenant's details\n";
                        std::cout << "3. Delete tenant\n";
                        std::cout << "4. Summarize top 10 property\n";
                        std::cout << "5. Manage tenancy process\n";
                        std::cout << "6. Manage payment\n";
                        std::cout << "7. Logout\n";
                        std::cout << "Enter your choice: ";
                        std::cin >> choice;

                        switch (choice)
                        {
                        case 1:
                            manager->displayAllTenantsDetails(users, numUsers);
                            break;
                        case 2:
                            manager->searchTenantDetails(users, numUsers);
                            break;
                        case 3:
                            manager->deleteInactiveTenants(users, numUsers);
                            break;
                        case 4:
                            manager->displayAllTenantFavorites(users, numUsers, properties, numProperties);
                            break;
                        case 5:
                            manager->displayRentRequests(rentRequestList);
                            break;
                        case 6:
                            // Manage payment
                            // ...
                            break;
                        case 7:
                            manager->logout();
                            std::cout << "Logged out.\n";
                            loggedIn = false;
                            break;
                        default:
                            std::cout << "Invalid choice. Try again.\n";
                            break;
                        }
                    }
                }
                else if (admin)
                {
                    int choice;
                    bool loggedIn = true;

                    while (loggedIn)
                    {
                        std::cout << "\nAdmin Menu:\n";
                        std::cout << "1. Add new manager\n";
                        std::cout << "2. Modify manager's status\n";
                        std::cout << "3. Display tenants & property information\n";
                        std::cout << "4. Logout\n";
                        std::cout << "Enter your choice: ";
                        std::cin >> choice;

                        switch (choice)
                        {
                        case 1:
                            admin->addManager(users, numUsers);
                            break;
                        case 2:
                            admin->displayAllManagersDetails(users, numUsers);
                            admin->modifyManagerStatus(users, numUsers);
                            break;
                        case 3:
                            admin->displayMenu(users, numUsers, properties, numProperties);
                            break;
                        case 4:
                            admin->logout();
                            std::cout << "Logged out.\n";
                            loggedIn = false;
                            break;
                        default:
                            std::cout << "Invalid choice. Try again.\n";
                            break;
                        }
                    }
                }
            }
            else
            {
                std::cout << "Login failed\n";
            }
            break;
        }
        case 4:
            running = false;
            break;
        default:
            std::cout << "Invalid choice. Try again.\n";
            break;
        }
    }

    for (int i = 0; i < numUsers; ++i)
    {
        delete users[i];
    }
    delete[] users;
    CSVHandler::clearCSV(properties);
    return 0;
}