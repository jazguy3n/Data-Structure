#include "property_management.cpp"

int main()
{
    int numProperties;
    Property *properties = CSVHandler::importCSV("mudah-apartment-kl-selangor.csv", numProperties);

    printProperties(properties, numProperties);

    CSVHandler::clearCSV(properties);

    return 0;
}
