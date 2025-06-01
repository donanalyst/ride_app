#include "distance_calculator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <filesystem>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <vector>
#include <limits>

using namespace std;
namespace fs = std::filesystem;

// Static member initialization
const map<string, VehicleRate> DistanceCalculator::VEHICLE_RATES = {
    {"Sedan", {40.0, 15.0}},
    {"SUV", {60.0, 20.0}},
    {"Truck", {100.0, 30.0}},
    {"Van", {80.0, 18.0}},
    {"Motorcycle", {30.0, 10.0}},
    {"Bus", {150.0, 8.0}},
    {"Train", {15.0, 2.0}} 
};

const string DistanceCalculator::PESO_SIGN = "PHP ";

// Helper functions
string trim(const string& s) {
    const char* whitespace = " \t\n\r\f\v\"'";
    size_t start = s.find_first_not_of(whitespace);
    size_t end = s.find_last_not_of(whitespace);
    return (start == string::npos) ? "" : s.substr(start, end - start + 1);
}

string normalizeName(const string& s) {
    string result;
    for (char c : s) {
        if (isalnum(c)) {
            result += tolower(c);
        }
    }
    return result;
}

// Member function implementations
map<string, City> DistanceCalculator::loadAllCities() {
    map<string, City> cities;
    vector<string> csv_files = {
        "Cities.csv", "Ejeep.csv", "LRT-2.csv", "LRT.csv",
        "Major_Bus.csv", "MRT-3.csv", "PNR.csv"
    };

    for (const auto& file : csv_files) {
        vector<fs::path> possible_paths = {
            fs::current_path() / "Data_Csv" / file,
            fs::current_path().parent_path() / "Data_Csv" / file,
            fs::current_path() / file,
            fs::current_path().parent_path() / file
        };

        fs::path csv_path;
        bool found = false;
        
        for (const auto& path : possible_paths) {
            if (fs::exists(path)) {
                csv_path = path;
                found = true;
                break;
            }
        }

        if (!found) {
            cerr << "Warning: File not found in any searched location - " << file << endl;
            continue;
        }

        ifstream file_stream(csv_path);
        if (!file_stream.is_open()) {
            cerr << "Error: Could not open " << csv_path << endl;
            continue;
        }

        string line;
        int line_num = 0;
        int loaded = 0;

        while (getline(file_stream, line)) {
            line_num++;
            line = trim(line);
            if (line.empty() || line_num == 1) continue;

            stringstream ss(line);
            string name, lat_str, lon_str;

            try {
                getline(ss, name, ',');
                getline(ss, lat_str, ',');
                getline(ss, lon_str);

                name = trim(name);
                lat_str = trim(lat_str);
                lon_str = trim(lon_str);

                if (name.empty() || lat_str.empty() || lon_str.empty()) {
                    cerr << "Warning: Incomplete data in " << file << " at line " << line_num << endl;
                    continue;
                }

                // Create normalized version for flexible matching
                string normalized = normalizeName(name);
                
                // Check for duplicates using normalized name
                bool duplicate = false;
                for (const auto& pair : cities) {
                    if (normalizeName(pair.first) == normalized) {
                        cerr << "Warning: Duplicate city found - " << name 
                             << " (similar to " << pair.first << ") in " << file << endl;
                        duplicate = true;
                        break;
                    }
                }
                if (duplicate) continue;

                cities[name] = {
                    name,
                    stod(lat_str),
                    stod(lon_str)
                };
                loaded++;
            } catch (const exception& e) {
                cerr << "Error parsing line " << line_num << " in " << file << ": " << e.what() << endl;
                cerr << "Problematic line: " << line << endl;
            }
        }

        
    }

    
    return cities;
}

double DistanceCalculator::calculateDistance(const City& a, const City& b) {
    constexpr double R = 6371.0;
    constexpr double PI_DIV_180 = M_PI / 180.0;

    double lat1 = a.lat * PI_DIV_180;
    double lat2 = b.lat * PI_DIV_180;
    double dlat = (b.lat - a.lat) * PI_DIV_180;
    double dlon = (b.lon - a.lon) * PI_DIV_180;

    double a_harv = sin(dlat/2) * sin(dlat/2) + cos(lat1) * cos(lat2) * sin(dlon/2) * sin(dlon/2);
    return 2 * R * atan2(sqrt(a_harv), sqrt(1-a_harv));
}

const City* DistanceCalculator::findCity(const map<string, City>& cities, const string& name) {
    auto it = cities.find(name);
    if (it != cities.end()) return &it->second;

    string trimmed = trim(name);
    it = cities.find(trimmed);
    if (it != cities.end()) return &it->second;

    string normalized = normalizeName(name);
    for (const auto& pair : cities) {
        if (normalizeName(pair.first) == normalized) {
            return &pair.second;
        }
    }
    return nullptr;
}

void DistanceCalculator::showSuggestions(const map<string, City>& cities, const string& name) {
    string normalized = normalizeName(name);
    cout << "Did you mean one of these?" << endl;
    for (const auto& pair : cities) {
        if (pair.first.find(name) != string::npos || 
            normalizeName(pair.first).find(normalized) != string::npos) {
            cout << " - " << pair.first << endl;
        }
    }
}

void DistanceCalculator::printAllCities(const map<string, City>& cities) {
    cout << "\n=== Available Locations (" << cities.size() << ") ===" << endl;
    for (const auto& pair : cities) {
        cout << pair.first << endl;
    }
    cout << "==============================" << endl;
}

pair<const City*, const City*> DistanceCalculator::selectLocations(const map<string, City>& cities) {
    string from, to;
    const City* fromCity = nullptr;
    const City* toCity = nullptr;

    // Clear any leftover input
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Pickup location selection
    while (true) {
        cout << "\nEnter pickup location: ";
        getline(cin, from);
        fromCity = findCity(cities, from);
        if (!fromCity) {
            cout << "\nNo matching locations found! Please try again!" << endl;
        } else {
            break;
        }
    }

    // Dropoff location selection
    while (true) {
        cout << "\nEnter dropoff location: ";
        getline(cin, to);
        toCity = findCity(cities, to);
        if (!toCity) {
            cout << "\nNo matching locations found! Please try again!" << endl;
        } else if (to == from) {
            cout << "Dropoff cannot be the same as pickup!" << endl;
        } else {
            break;
        }
    }

    return {fromCity, toCity};
}


