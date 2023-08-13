void initializeData(Property *&properties, int &numProperties, User **&users, int &numUsers)
{
    // Load properties from CSV
    properties = CSVHandler::importCSV("mudah-apartment-kl-selangor.csv", numProperties);

    // Initialize users
    int capacity = 8;
    users = new User *[capacity];

    users[numUsers++] = new Tenant("tenant1", "tenantpass1", 30, "John", "0177663555", "john@gmail.com", "Active");
    users[numUsers++] = new Tenant("tenant2", "tenantpass2", 24, "Mary", "0170726486", "mary@gmail.com", "Active");
    users[numUsers++] = new Tenant("tenant3", "tenantpass3", 18, "Clara", "0162630697", "clara@hotmail.com", "Inactive");
    users[numUsers++] = new Tenant("tenant4", "tenantpass4", 27, "David", "0188365828", "david@gmail.com", "Active");
    users[numUsers++] = new Tenant("tenant5", "tenantpass5", 35, "Peter", "0183205921", "peter@hotmail.com", "Inactive");
    users[numUsers++] = new Tenant("tenant6", "tenantpass6", 35, "Jay", "0183205921", "peter@hotmail.com", "Inactive");
    users[numUsers++] = new Manager("manager", "managerpass", "M1", "Active");
    users[numUsers++] = new Admin("admin", "adminpass", "A1", "Active");

    // Resize the users dynamic array if needed
    if (numUsers >= capacity)
    {
        capacity *= 2; // Double the capacity
        User **newUsers = new User *[capacity];

        // Copy existing elements
        for (int i = 0; i < numUsers; ++i)
        {
            newUsers[i] = users[i];
        }

        delete[] users;   // Deallocate old memory
        users = newUsers; // Update pointer to the new memory block
    }
}

void initializeFavoriteProperties(User **users, int numUsers, Property *properties, int numProperties)
{
    for (int i = 0; i < numUsers; ++i)
    {
        Tenant *tenant = dynamic_cast<Tenant *>(users[i]);
        if (tenant)
        {
            if (tenant->getUsername() == "tenant1")
            {
                tenant->addFavoriteProperty("100316974", properties, numProperties);
                tenant->addFavoriteProperty("100316158", properties, numProperties);
                tenant->addFavoriteProperty("100082207", properties, numProperties);
            }
            else if (tenant->getUsername() == "tenant2")
            {
                tenant->addFavoriteProperty("100314685", properties, numProperties);
                tenant->addFavoriteProperty("100314147", properties, numProperties);
                tenant->addFavoriteProperty("100313848", properties, numProperties);
                tenant->addFavoriteProperty("100082207", properties, numProperties);
            }
            else if (tenant->getUsername() == "tenant4")
            {
                tenant->addFavoriteProperty("100263541", properties, numProperties);
                tenant->addFavoriteProperty("98180610", properties, numProperties);
                tenant->addFavoriteProperty("100316158", properties, numProperties);
                tenant->addFavoriteProperty("100082207", properties, numProperties);
                tenant->addFavoriteProperty("100179047", properties, numProperties);
                tenant->addFavoriteProperty("100322962", properties, numProperties);
            }
        }
    }
}