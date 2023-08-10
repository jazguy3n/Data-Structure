#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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
            getline(file, line);

            numProperties = 0;
            while (getline(file, line))
            {
                numProperties++;
            }
            file.clear();
            file.seekg(0, std::ios::beg);

            properties = new Property[numProperties];

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
