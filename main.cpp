#include "property_management.cpp"
#include "user.cpp"

int Manager::managerCount = 0;
int Tenant::tenantCount = 0; // Khởi tạo tenantCount bắt đầu từ 1

int main()
{
    int numProperties;
    Property *properties = CSVHandler::importCSV("mudah-apartment-kl-selangor.csv", numProperties);

    const int maxUsers = 10;
    User **users = new User *[maxUsers];
    int numUsers = 0;

    users[numUsers++] = new Tenant("tenant1", "tenantpass1", 30, "John", "0177663555", "john@gmail.com", "Active");
    users[numUsers++] = new Tenant("tenant2", "tenantpass2", 24, "Mary", "0170726486", "mary@gmail.com", "Active");
    users[numUsers++] = new Tenant("tenant3", "tenantpass3", 18, "Clara", "0162630697", "clara@hotmail.com", "Inactive");
    users[numUsers++] = new Tenant("tenant4", "tenantpass4", 27, "David", "0188365828", "david@gmail.com", "Active");
    users[numUsers++] = new Tenant("tenant5", "tenantpass5", 35, "Peter", "0183205921", "peter@hotmail.com", "Inactive");
    users[numUsers++] = new Manager("manager", "managerpass", "M1", "Active");
    users[numUsers++] = new Admin("admin", "adminpass", "A1");

    std::string newUsername;
    std::string newPassword;
    std::string newUserRole;

    int choice;
    bool running = true;

    while (running)
    {
        std::cout << "Main Menu:\n";
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
                        std::cout << "Tenant Menu:\n";
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
                            break;
                        }
                        case 4:
                            // Place rent request
                            // ...
                            break;
                        case 5:
                            // Display renting history
                            // ...
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
                        std::cout << "Manager Menu:\n";
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
                            // Search tenant's details
                            // ...
                            break;
                        case 3:
                            // Delete tenant
                            // ...
                            break;
                        case 4:
                            // Summarize top 10 property
                            // ...
                            break;
                        case 5:
                            // Manage tenancy process
                            // ...
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
                        std::cout << "Admin Menu:\n";
                        std::cout << "1. Add new manager\n";
                        std::cout << "2. Modify manager's status\n";
                        std::cout << "3. Display all tenants & property information\n";
                        std::cout << "4. Logout\n";
                        std::cout << "Enter your choice: ";
                        std::cin >> choice;

                        switch (choice)
                        {
                        case 1:
                            Manager::addManager(users, numUsers);
                            break;
                        case 2:
                            admin->displayAllManagersDetails(users, numUsers);
                            admin->modifyManagerStatus(users, numUsers);
                            break;
                        case 3:
                            // Display all tenants & property information
                            // ...
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
                std::cout << "Login failed. Invalid username or password.\n";
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