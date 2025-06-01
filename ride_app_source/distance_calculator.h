#ifndef DISTANCE_CALCULATOR_H
#define DISTANCE_CALCULATOR_H

#include <string>
#include <map>

struct City {
    std::string name;
    double lat, lon;
};

struct VehicleRate {
    double baseFare;
    double perKmRate;
};
class DistanceCalculator {
public:
    // Load all cities from CSV files
    std::map<std::string, City> loadAllCities();
    // In DistanceCalculator.h
    std::pair<const City*, const City*> selectLocations(const std::map<std::string, City>& cities);
    
    // Calculate distance between two cities
    double calculateDistance(const City& a, const City& b);
    
    // Find a city by name (with flexible matching)
    const City* findCity(const std::map<std::string, City>& cities, const std::string& name);
    
    // Show suggestions for similar city names
    void showSuggestions(const std::map<std::string, City>& cities, const std::string& name);
    
    // Print all loaded cities (for debugging)
    void printAllCities(const std::map<std::string, City>& cities);
    
    // Vehicle rates
    static const std::map<std::string, VehicleRate> VEHICLE_RATES;
    
    // Constants
    static const std::string PESO_SIGN;

};

#endif // DISTANCE_CALCULATOR_H