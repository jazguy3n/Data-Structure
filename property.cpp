#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <unordered_map>

class Property
{
public:
    std::string ads_id, prop_name, completion_year, monthly_rent, location, property_type,
        rooms, parking, bathroom, size, furnished, facilities, additional_facilities, region;

    Property() {}

    void display() const
    {
        std::cout << "Id: " << ads_id << "\n"
                  << "Property name: " << prop_name << "\n"
                  << "Completion year: " << completion_year << "\n"
                  << "Monthly rent: " << monthly_rent << "\n"
                  << "Location: " << location << "\n"
                  << "Property type: " << property_type << "\n"
                  << "Rooms: " << rooms << "\n"
                  << "Parking: " << parking << "\n"
                  << "Bathroom: " << bathroom << "\n"
                  << "Size: " << size << "\n"
                  << "Furnished: " << furnished << "\n"
                  << "Facilities: " << facilities << "\n"
                  << "Additional facilities: " << additional_facilities << "\n"
                  << "Region: " << region << "\n";
    }
};

namespace CSVHandler
{
    Property *importCSV(const std::string &fileName, int &numProperties)
    {
        std::string fname = fileName;
        Property *properties = nullptr;

        std::ifstream file(fname);
        if (file.is_open())
        {
            std::string line;

            // Read the header line to skip it
            getline(file, line);

            numProperties = 0;
            while (getline(file, line))
            {
                numProperties++;
            }
            file.clear();
            file.seekg(0, std::ios::beg);

            properties = new Property[numProperties];

            // Skip the header line again
            getline(file, line);

            for (int i = 0; i <= numProperties; i++)
            {
                getline(file, line);
                std::stringstream str(line);
                Property &property = properties[i];

                getline(str, property.ads_id, ',');
                getline(str, property.prop_name, ',');
                getline(str, property.completion_year, ',');
                getline(str, property.monthly_rent, ',');
                getline(str, property.location, ',');
                getline(str, property.property_type, ',');
                getline(str, property.rooms, ',');
                getline(str, property.parking, ',');
                getline(str, property.bathroom, ',');
                getline(str, property.size, ',');
                getline(str, property.furnished, ',');

                // Read facilities
                std::string facilities;
                char c;
                while (str.get(c) && c != '"')
                {
                }
                while (str.get(c) && c != '"')
                {
                    facilities += c;
                }
                property.facilities = facilities;

                // Read additional facilities
                std::string additionalFacilities;
                while (str.get(c) && c != '"')
                {
                }
                while (str.get(c) && c != '"')
                {
                    additionalFacilities += c;
                }
                property.additional_facilities = additionalFacilities;

                // Read the rest of the fields
                getline(str, property.region);

                // Remove leading comma from the region if present
                if (!property.region.empty() && property.region.front() == ',')
                {
                    property.region.erase(property.region.begin());
                }
            }
        }
        else
        {
            std::cout << "Could not open the file\n";
        }
        return properties;
    }

    void clearCSV(Property *properties)
    {
        delete[] properties;
    }
}

void printProperties(const Property *properties, int numProperties)
{
    for (int i = 0; i <= numProperties; i++)
    {
        std::cout << "Property " << i + 1 << ":\n";
        properties[i].display();
        std::cout << "\n";
    }
}

class FavoriteProperty
{
private:
    int capacity;

public:
    std::string *favoritePropertyIDs;
    int numFavorites;
    FavoriteProperty() : favoritePropertyIDs(nullptr), numFavorites(0), capacity(0) {}

    ~FavoriteProperty()
    {
        delete[] favoritePropertyIDs;
    }

    void addFavoriteProperty(const std::string &propertyID, const Property *properties, int numProperties)
    {
        // Check if the provided property ID is in the list of properties
        bool isValidProperty = false;
        for (int i = 0; i < numProperties; ++i)
        {
            if (properties[i].ads_id == propertyID)
            {
                isValidProperty = true;
                break;
            }
        }

        if (!isValidProperty)
        {
            std::cout << "Property with ID " << propertyID << " not found.\n";
            return;
        }

        // Check if the property ID is already in the favorites list
        for (int i = 0; i < numFavorites; ++i)
        {
            if (favoritePropertyIDs[i] == propertyID)
            {
                std::cout << "Property with ID " << propertyID << " is already in your favorites.\n";
                return;
            }
        }

        if (numFavorites >= capacity)
        {
            int newCapacity = (capacity == 0) ? 1 : capacity * 2;
            std::string *newArray = new std::string[newCapacity];
            for (int i = 0; i < numFavorites; ++i)
            {
                newArray[i] = favoritePropertyIDs[i];
            }
            delete[] favoritePropertyIDs;
            favoritePropertyIDs = newArray;
            capacity = newCapacity;
        }
        favoritePropertyIDs[numFavorites] = propertyID;
        ++numFavorites;
        std::cout << "Property with ID " << propertyID << " added to your favorites.\n";
    }

    void displayFavoriteProperties(const Property *properties, int numProperties)
    {
        std::cout << "\nFavorite Properties:\n";
        for (int i = 0; i < numFavorites; ++i)
        {
            const Property *foundProperty = nullptr;
            for (int j = 0; j < numProperties; ++j)
            {
                if (properties[j].ads_id == favoritePropertyIDs[i])
                {
                    foundProperty = &properties[j];
                    break;
                }
            }
            if (foundProperty)
            {
                foundProperty->display();
                std::cout << "\n";
            }
            else
            {
                std::cout << "- Property with ID " << favoritePropertyIDs[i] << " not found.\n";
            }
        }
    }

    bool isPropertyInFavorites(const std::string &propertyID) const
    {
        for (int i = 0; i < numFavorites; ++i)
        {
            if (favoritePropertyIDs[i] == propertyID)
            {
                return true;
            }
        }
        return false;
    }
};
