#include "DistanceCalculator.h"
#include "calendar_picker.h"
#include <iostream>
#include <list>
#include <vector>
#include <iomanip>
#include <string>
#include <windows.h> // For clearScreen function
#include <limits>    // For numeric_limits
#include <algorithm> // For max_element
#include <cctype>    // For toupper function
#include <sstream>
#include <map>
#include <ctime>
#include <random>
#include <thread>
#include <chrono>
#include <ctime>

using namespace std;

struct person {
    string ride_id; 
    int num_of_persons;
    string fname;
    string lname;
    string phone;
    string pickup;
    string dropoff;
    int rideDay;
    int rideMonth;
    int rideYear;
    string vehicle;
    string status;
    bool isCurrentRide = false;
    double totalFare;
    int assignedDriverId = -1; // Added to store driver ID

    static const string Pending;
    static const string Confirmed;
    static const string Cancelled;
    static const string Completed;
    static const string OnRide;
};

const string person::OnRide = "OnRide";

struct Driver {
    int id;
    std::string name;
    std::string phone;
    std::string vehicle;
    bool available;

    Driver(int i, std::string n, std::string p, std::string v) 
        : id(i), name(n), phone(p), vehicle(v), available(true) {}
};

class DriverManager {
private:
    std::vector<Driver> drivers;

public:
    void addDriver(int id, std::string name, std::string phone, std::string vehicle) {
        drivers.emplace_back(id, name, phone, vehicle);
    }

    // Getter for drivers
    const std::vector<Driver>& getDrivers() const {
        return drivers;
    }

    Driver* assignDriver(const std::string& vehicleType) {
        for (auto& driver : drivers) {
            if (driver.available && driver.vehicle == vehicleType) {
                driver.available = false;
                return &driver;
            }
        }
        return nullptr; // No available driver
    }

    void releaseDriver(int driverId) {
        for (auto& driver : drivers) {
            if (driver.id == driverId) {
                driver.available = true;
                break;
            }
        }
    }

    void printAvailableDrivers() {
        std::cout << "\nAvailable Drivers:\n";
        std::cout << "ID  | Name           | Vehicle\n";
        std::cout << "----------------------------\n";
        for (const auto& driver : drivers) {
            if (driver.available) {
                std::cout << driver.id << " | " << driver.name << " | " << driver.vehicle << "\n";
            }
        }
    }
};

// Function Prototypes
void booking(list<struct person>& people_list, DriverManager& dm);
void searching(list<struct person>& people_list);
void clearScreen();
void onride(list<struct person>& people_list, DriverManager& dm);
void vehicle_type(string& vehicle);
void Sedan();
void view_all_rides(list<struct person>& people, DriverManager& dm);
void current_ride_details(list<person>& people, DriverManager& dm);
void editRides(list<struct person>& people, DriverManager& dm);
void searchRides(list<struct person>& people);
void cancelRides(list<struct person>& people, DriverManager& dm);
void deleteRides(list<struct person>& people, DriverManager& dm);
void goingRides(list<struct person>& people, DriverManager& dm);
int number_of_persons();
int countDuplicateVectors(const list<vector<string>>& data);
void startRideWithAnimation(list<person>& people, DriverManager& dm);

int main() {
    int choice;
    list<struct person> people;
    DriverManager dm; 
    
    // Initialize drivers
    dm.addDriver(1, "Sergio Dela Cruz", "09409798726", "Sedan");
    dm.addDriver(2, " Harold Salazar ", "09669458580", "SUV");
    dm.addDriver(3, "Jerome Gonzales", "09403210927", "Sedan");
    dm.addDriver(4, "Jessie Torres", "09547689400", "Truck");
    dm.addDriver(5, "Wilson Roxas", "09387398460", "Van");
    dm.addDriver(6, "Victor Fernandez", "09547689400", "Motorcycle");
    dm.addDriver(7, "Arnold Aguilar", "09483490869", "Bus");
    
    do {
        clearScreen();
        cout << "\n";
        cout << "Welcome to the Ride Management System\n";
        cout << "1. Book a ride\n";
        cout << "2. View all rides\n";
        cout << "3. Edit a ride\n";
        cout << "4. Delete a ride\n";
        cout << "5. Search for a ride\n";
        cout << "6. Cancel a ride\n";
        cout << "7. View available drivers\n";  // New option
        cout << "0. Exit\n";
        cout << "Please enter your choice: ";
        
        // Input validation
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number: ";
        }
        
        switch (choice) {
            case 1:
                clearScreen();
                booking(people, dm);
                break;
            case 2:
                clearScreen();
                view_all_rides(people, dm);
                break;
            case 3:
                clearScreen();
                editRides(people, dm);
                break;
            case 4:
                clearScreen();
                deleteRides(people, dm);
                break;
            case 5:
                clearScreen();
                searching(people);
                break;
            case 6:
                clearScreen();
                cancelRides(people, dm);
                break;
            case 7:
                clearScreen();
                dm.printAvailableDrivers();
                break;
            case 0:
                cout << "Exiting the system...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
        
        if (choice != 0) {
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
        
    } while (choice != 0);
    
    cout << "Thank you for using the Ride Management System!\n";
    return 0;
}

void clearScreen() {
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = {0, 0};
    DWORD count;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    
    GetConsoleScreenBufferInfo(hStdout, &csbi);
    FillConsoleOutputCharacter(hStdout, ' ', csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
    SetConsoleCursorPosition(hStdout, coord);
}


void playRideAnimation(const list<struct person>& people, const DriverManager& dm) {
    clearScreen();
    cout << "Starting ride animation...\n\n";
    
    startRideWithAnimation(const_cast<list<person>&>(people), const_cast<DriverManager&>(dm));
    for (int i = 0; i < 3; i++) {
        cout << "Driving " << string(i + 1, '.') << endl;
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Pause for effect
    }
    
    cout << "\nRide started!\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
}






void onride(list<struct person>& people_list, DriverManager& dm) {
    clearScreen();

    // ===== 1️⃣ Check for rides & group them (same as before) =====
    if (people_list.empty()) {
        cout << "No active rides.\n";
        return;
    }

    map<tuple<string, int, int, int, string, string>, vector<person*>> groupedRides;
    for (auto& p : people_list) {
        if (p.status == "OnRide" || p.status == "Confirmed") {
            auto key = make_tuple(p.vehicle, p.rideDay, p.rideMonth, p.rideYear, p.pickup, p.dropoff);
            groupedRides[key].push_back(&p);
        }
    }

    if (groupedRides.empty()) {
        cout << "No active rides.\n";
        cout << "\nPress Enter to return...";
        cin.ignore(); cin.get();
        return;
    }

    // ===== 2️⃣ Store ride details before animation =====
    vector<string> rideDetails; // Stores driver/route info to redisplay later

    for (const auto& group : groupedRides) {
        const auto& [vehicle, day, month, year, pickup, dropoff] = group.first;
        const auto& riders = group.second;

        stringstream ss;
        ss << "\n=== RIDE DETAILS ===\n"
           << "Vehicle: " << vehicle << "\n"
           << "Date: " << month << "/" << day << "/" << year << "\n"
           << "Route: " << pickup << " -> " << dropoff << "\n";

        // Add driver info (if available)
        if (riders[0]->assignedDriverId != -1) {
            for (const auto& driver : dm.getDrivers()) {
                if (driver.id == riders[0]->assignedDriverId) {
                    ss << "Driver: " << driver.name << " (Contact: " << driver.phone << ")\n";
                    break;
                }
            }
        }
        rideDetails.push_back(ss.str()); // Save this ride's details
    }

    // ===== 3️⃣ Display ride details FIRST =====
    cout << "=====================\n"
         << "| CURRENTLY ON RIDE |\n"
         << "=====================\n";

    for (const auto& details : rideDetails) {
        cout << details; // Show driver/route/vehicle info
    }

    // ===== 4️⃣ Trigger animation =====
    cout << "\nPress Enter to start the ride...";
    cin.ignore(); cin.get();

    clearScreen();
    startRideWithAnimation(people_list, dm); // Play your animation

    // ===== 5️⃣ Redisplay key info after animation =====
    clearScreen();
    cout << "====================================================\n"
         << "| THANK YOU FOR RIDING WITH US! HAVE A GREAT DAY!  |\n"
         << "====================================================\n"
         << "                                  by: GalaxyRideTeam";

    cout << "\nPress Enter to return to menu...";
    cin.ignore(); cin.get();
}


void vehicle_type(string& vehicle) {
    int choice;
    vehicle = "";

    cout << "\nPlease select the vehicle type: \n";
    cout << "1. Sedan\n";
    cout << "2. SUV\n";
    cout << "3. Truck\n";
    cout << "4. Van\n";
    cout << "5. Motorcycle\n";
    cout << "6. Bus\n";
    cout << "7. Train\n";
    cout << "Please enter your choice: ";
    
    while (!(cin >> choice) || choice < 1 || choice > 7) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid choice. Please enter a number between 1-7: ";
    }

    switch (choice) {
        case 1: vehicle = "Sedan"; break;
        case 2: vehicle = "SUV"; break;
        case 3: vehicle = "Truck"; break;
        case 4: vehicle = "Van"; break;
        case 5: vehicle = "Motorcycle"; break;
        case 6: vehicle = "Bus"; break;
        case 7: vehicle = "Train"; break;
    }
}

void Sedan() {
    cout << "Your ride is on the way...\n";
    cout << "Please wait...\n";
}

void searching(list<struct person>& people) {
    string fname, lname;
    cout << "\nEnter the first name of the person to search: ";
    cin >> fname;
    cout << "Enter the last name of the person to search: ";
    cin >> lname;

    bool found = false;
    for (const auto& p : people) {
        if (p.fname == fname && p.lname == lname) {
            found = true;
            cout << "\nRide details for " << p.fname << " " << p.lname << ":\n";
            cout << "Phone Number: " << p.phone << "\n";
            cout << "Pickup Location: " << p.pickup << "\n";
            cout << "Dropoff Location: " << p.dropoff << "\n";
            cout << "Date of Ride: " << p.rideYear << "-" 
                << setw(2) << setfill('0') << p.rideMonth << "-" 
                << setw(2) << setfill('0') << p.rideDay << "\n";
            cout << "Vehicle Type: " << p.vehicle << "\n";
            cout << string(30, '=') << endl;
            cout << "Ride is Pending\n";
            cout << string(30, '=') << endl;
            
            cout << "Do you wish to proceed with the ride? (y/n): ";
            char choice;
            cin >> choice;
            if (toupper(choice) == 'Y') {
                cout << "Ride is confirmed.\n";
                return;
            } else {
                cout << "Ride is cancelled.\n";
            }
            return;
        }
    }
    
    if (!found) {
        cout << "Ride not found.\n";
    }
}

int number_of_persons() {
    int num;
    cout << "Please enter the number of persons: ";
    
    while (!(cin >> num) || num <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a positive number: ";
    }
    
    cout << "Number of persons: " << num << endl;
    return num;
}

void booking(list<struct person>& people, DriverManager& dm) {
    DistanceCalculator calculator;
    auto cities = calculator.loadAllCities();

    cout << "========= BOOKING RIDE MODE =========\n";
    
    int num_persons = number_of_persons();
    
    cout << "\n\nNote: Please read the following information before proceeding!\n";
    cout << "* The ride will be charged based on the distance and vehicle type.\n";
    cout << "* Provide accurate pickup and dropoff locations.\n";
    cout << "* If 2 or more persons are traveling together, they must have the same pickup and dropoff locations.\n";
    cout << "* Otherwise, rides will be registered separately.\n\n";

    cout << "Do you wish to proceed with the booking? (y/n): ";
    char proceed;
    cin >> proceed;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (toupper(proceed) != 'Y') {
        cout << "Booking cancelled.\n";
        return;
    }

    // First select locations that will be shared by all persons in this booking
    

    for (int i = 0; i < num_persons; ++i) {
        clearScreen();
        struct person p;

        // Generate a simple unique ride_id using current time and loop index
        p.ride_id = to_string(static_cast<long long>(time(nullptr))) + "_" + to_string(i);

        cout << "\nBooking for person " << (i + 1) << ":\n";
        cout << "\nACCOUNT DETAILS:\n\n";

        cout << "Please enter your first name: ";
        getline(cin, p.fname);

        cout << "Please enter your last name: ";
        getline(cin, p.lname);

        cout << "Please enter your phone number: ";
        getline(cin, p.phone);

        // Set the locations that were already validated


        cout << "\nLOCATION DETAILS:";

        auto [fromCity, toCity] = calculator.selectLocations(cities);
        string from = fromCity->name;
        string to = toCity->name;
        
        // Calculate distance and fare rates once since they're the same for all persons
        double distance = calculator.calculateDistance(*fromCity, *toCity);
        
        // Show route information once
        cout << fixed << setprecision(2);
        cout << "\nRoute: " << from << " -> " << to << endl;
        cout << "Distance: " << distance << " km" << endl;
        cout << "==========================" << endl;





        p.pickup = from;
        p.dropoff = to;

        // Select vehicle type
        vehicle_type(p.vehicle);
        
        // Assign a driver
        Driver* assignedDriver = dm.assignDriver(p.vehicle);
        if (assignedDriver) {
            cout << "\nAssigned Driver: " << assignedDriver->name 
                 << " (" << assignedDriver->phone << ")\n";
            p.assignedDriverId = assignedDriver->id;
        } else {
            cout << "\nNo available drivers for " << p.vehicle << " at the moment.\n";
            p.assignedDriverId = -1; // No driver assigned
        }

        cout << "\nPlease select the date for your ride:\n";
        selectDate(p.rideMonth, p.rideYear, p.rideDay);

        p.status = "Pending";
        p.num_of_persons = num_persons;
        p.isCurrentRide = true;

        people.push_back(p);

        cout << "\nYour ride has been successfully booked for "
             << p.fname << " " << p.lname << ".\n";

        if (i < num_persons - 1) {
            cout << "\nPress Enter to book for the next person...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    cout << "\n\nThank you for using our ride management system!\n";
    char choice;
    
    string prompt = (num_persons > 1) ? 
        "Do you want to proceed these bookings? (y/n): " : 
        "Do you want to proceed with the booking? (y/n): ";
    
    cout << prompt;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (toupper(choice) == 'Y') {
        current_ride_details(people, dm);
    } else {
        for (auto& p : people) {
            if (p.isCurrentRide) {
                if (p.assignedDriverId != -1) {
                    dm.releaseDriver(p.assignedDriverId);
                }
                p.isCurrentRide = false;
                p.status = "Cancelled";
                cout << "\nRide has been cancelled for " << p.fname << " " << p.lname;
            }
        }
    }
}

void current_ride_details(list<person>& people, DriverManager& dm) {
    DistanceCalculator calculator;
    auto cities = calculator.loadAllCities();

    vector<string> tempVehicles;
    std::list<std::vector<std::string>> allVehicles;

    clearScreen();
    string status;
    cout << "====== CURRENT RIDE BOOKINGS ======\n\n";
    int index = 1;
    double totalFare = 0.0;  // Variable to accumulate total fare

    for (auto& p : people) {
        if (p.isCurrentRide) {
            cout << "Ride #" << index++ << ":\n";
            cout << "Name: " << p.fname << " " << p.lname << endl;
            cout << "Phone: " << p.phone << endl;
            cout << "Pickup: " << p.pickup << endl;
            cout << "Dropoff: " << p.dropoff << endl;
            cout << "Date: " << setw(2) << setfill('0') << p.rideMonth
                << "/" << setw(2) << p.rideDay << "/" << p.rideYear << endl;
            cout << "Vehicle: " << p.vehicle << endl;
            if (p.assignedDriverId != -1) {
                cout << "Driver ID: " << p.assignedDriverId << endl;
            }
            cout << "Status: " << p.status << "\n";
            tempVehicles.push_back(p.vehicle);

            // Calculate fare for selected vehicle
            auto fromCityIt = cities.find(p.pickup);
            auto toCityIt = cities.find(p.dropoff);

            if (fromCityIt != cities.end() && toCityIt != cities.end()) {
                const City& fromCity = fromCityIt->second;
                const City& toCity = toCityIt->second;
                double distance = calculator.calculateDistance(fromCity, toCity);

                if (!DistanceCalculator::VEHICLE_RATES.count(p.vehicle)) {
                    cerr << "Unknown vehicle type: " << p.vehicle << endl;
                } else {
                    auto rate = DistanceCalculator::VEHICLE_RATES.at(p.vehicle);
                    double fare = rate.baseFare + (rate.perKmRate * distance);
                    totalFare += fare;  // Add to total fare

                    cout << fixed << setprecision(2);
                    cout << "Base fare: " << DistanceCalculator::PESO_SIGN << rate.baseFare << endl;
                    cout << "Per km rate: " << DistanceCalculator::PESO_SIGN << rate.perKmRate << endl;
                    cout << "Distance: " << distance << " km" << endl;
                    cout << "___________________" << endl;
                    cout << "Ride fare: " << DistanceCalculator::PESO_SIGN << fare << endl;
                    cout << "___________________" << endl;

                    p.totalFare = fare;
                }
            } else {
                if (fromCityIt == cities.end()) {
                    cerr << "Could not find pickup location: " << p.pickup << endl;
                }
                if (toCityIt == cities.end()) {
                    cerr << "Could not find dropoff location: " << p.dropoff << endl;
                }
            }
        }
    }
    cout << "\nTotal rides: " << index - 1 << endl;

    allVehicles.push_back(tempVehicles);
    tempVehicles.clear();
    
    int total = countDuplicateVectors(allVehicles);
         
    cout << fixed << setprecision(2);
    cout << "========================" << endl;
    cout << "TOTAL FARE FOR ALL RIDES: " << DistanceCalculator::PESO_SIGN << totalFare << endl;
    cout << "========================" << endl;

    char choice;
    if (index - 1 > 1) {
        cout << "\n\nBefore proceeding, please ensure that all details are correct.";
        cout << "\nDo you want to proceed with these rides? (y/n): ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } else {
        cout << "\n\nBefore proceeding, please ensure that all details are correct.";
        cout << "\nDo you want to proceed with the ride? (y/n): ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    if (toupper(choice) == 'Y') {
        cout << "Ride is confirmed.\n";
        for (auto& ride : people) {
            if (ride.isCurrentRide) {
                ride.isCurrentRide = false;
                ride.status = "Confirmed";
            }
        }
        onride(people, dm);
    } else {
        for (auto& p : people) {
            if (p.isCurrentRide) {
                if (p.assignedDriverId != -1) {
                    dm.releaseDriver(p.assignedDriverId);
                }
                p.isCurrentRide = false;
                p.status = "Cancelled";
            }
        }
        cout << "Ride is cancelled.\n";
    }
}

int countDuplicateVectors(const list<vector<string>>& data) {
    if (data.size() < 2) return 0; // Need at least 2 vectors to have duplicates
    
    // Convert to vector for easier indexing
    vector<vector<string>> temp(data.begin(), data.end());
    int duplicateTotal = 0;
    
    // Track which vectors we've already counted
    vector<bool> counted(temp.size(), false);
    
    for (size_t i = 0; i < temp.size(); ++i) {
        if (counted[i]) continue; // Skip already counted vectors
        
        int currentGroupCount = 1; // Count self
        
        for (size_t j = i + 1; j < temp.size(); ++j) {
            if (!counted[j] && temp[i] == temp[j]) {
                currentGroupCount++;
                counted[j] = true;
            }
        }
        
        if (currentGroupCount > 1) {
            duplicateTotal += (currentGroupCount - 1); // Count extras beyond first
            cout << "Found " << currentGroupCount << " copies of: ";
            for (const auto& val : temp[i]) cout << val << " ";
            cout << endl;
        }
    }
    
    return duplicateTotal;
}

void view_all_rides(list<struct person>& people, DriverManager& dm) {
    if (people.empty()) {
        cout << "No rides booked yet.\n";
        return;
    }

    string ride_id = "Ride ID";
    string first_name = "First Name";
    string last_name = "Last Name";
    string phone_number = "Phone Number";
    string pickup_location = "Pickup Location";
    string dropoff_location = "Dropoff Location";
    string date_of_ride = "Date of Ride";
    string vehicle_type = "Vehicle Type";
    string status = "Status";
    string driver_id = "Driver ID";

    int ride_id_len = ride_id.length();
    int first_name_len = first_name.length();
    int last_name_len = last_name.length();
    int phone_number_len = phone_number.length();
    int pickup_location_len = pickup_location.length();
    int dropoff_location_len = dropoff_location.length();
    int date_of_ride_len = date_of_ride.length();
    int vehicle_type_len = vehicle_type.length();
    int status_len = status.length();


    // Finding the longest string in the list
    int max_fname_len = max_element(people.begin(), people.end(), [](const person& a, const person& b) {
        return a.fname.size() < b.fname.size();
    })->fname.size();
    int max_lname_len = max_element(people.begin(), people.end(), [](const person& a, const person& b) {
        return a.lname.size() < b.lname.size();
    })->lname.size();
    int max_phone_len = max_element(people.begin(), people.end(), [](const person& a, const person& b) {
        return a.phone.size() < b.phone.size();
    })->phone.size();
    int max_pickup_len = max_element(people.begin(), people.end(), [](const person& a, const person& b) {
        return a.pickup.size() < b.pickup.size();
    })->pickup.size();
    int max_dropoff_len = max_element(people.begin(), people.end(), [](const person& a, const person& b) {
        return a.dropoff.size() < b.dropoff.size();
    })->dropoff.size();
    int max_date_len =  10; // 10 for the date format "YYYY-MM-DD"
    int max_vehicle_len = max_element(people.begin(), people.end(), [](const person& a, const person& b) {
        return a.vehicle.size() < b.vehicle.size();
    })->vehicle.size();
    int max_status_len = max_element(people.begin(), people.end(), [](const person& a, const person& b) {
        return a.status.size() < b.status.size();
    })->status.size();
    int max_ride_id_len = max_element(people.begin(), people.end(), [](const person& a, const person& b) {
        return a.ride_id.size() < b.ride_id.size();
    })->ride_id.size();


    // Calculate new lengths for each column

    int new_fname_len = max(max_fname_len, first_name_len);
    int new_lname_len = max(max_lname_len, last_name_len);
    int new_phone_len = max(max_phone_len, phone_number_len);
    int new_pickup_len = max(max_pickup_len, pickup_location_len);
    int new_dropoff_len = max(max_dropoff_len, dropoff_location_len);
    int new_date_len = max(max_date_len, date_of_ride_len);
    int new_vehicle_len = max(max_vehicle_len, vehicle_type_len);
    int new_status_len = max(max_status_len, status_len);
    int new_ride_id_len = max(max_ride_id_len, ride_id_len);

    
    // print border

    string up_border = string(new_ride_id_len + new_fname_len + new_lname_len + new_phone_len + 
                           new_pickup_len + new_dropoff_len + new_date_len + 
                           new_vehicle_len + new_status_len + 18, '=');
    string low_border = string(new_ride_id_len + new_fname_len + new_lname_len + new_phone_len + 
                           new_pickup_len + new_dropoff_len + new_date_len + 
                           new_vehicle_len + new_status_len + 18, '=');



    int up_border_len = up_border.length();
    string up_border_divided = up_border.substr(0, (up_border_len - 25) / 2);
    string up_space = string((up_border_len - 25) / 2, ' ');
    string dash = string(25, '-');

    //print Table Ttle


    cout << up_space << dash << up_space << "\n";
    cout << up_space << "| RIDE MANAGEMENT TABLE |"<< up_space << "\n";
    cout << up_space << dash << up_space << "\n";


    
    cout << "" << up_border << "\n";
    
    for (int space = 0; space < new_ride_id_len - ride_id_len + 1; space++) cout << " ";
    cout << ride_id << "|";
    for (int space = 0; space < new_fname_len - first_name_len + 1; space++) cout << " ";
    cout << first_name << "|";
    for (int space = 0; space < new_lname_len - last_name_len + 1; space++) cout << " ";
    cout << last_name << "|";
    for (int space = 0; space < new_phone_len - phone_number_len + 1; space++) cout << " ";
    cout << phone_number << "|";
    for (int space = 0; space < new_pickup_len - pickup_location_len + 1; space++) cout << " ";
    cout << pickup_location << "|";
    for (int space = 0; space < new_dropoff_len - dropoff_location_len + 1; space++) cout << " ";
    cout << dropoff_location << "|";
    for (int space = 0; space < new_date_len - date_of_ride_len + 1; space++) cout << " ";
    cout << date_of_ride << "|";
    for (int space = 0; space < new_vehicle_len - vehicle_type_len + 1; space++) cout << " ";
    cout << vehicle_type << "|";
    for (int space = 0; space < new_status_len - status_len + 1; space++) cout << " ";
    cout << status << "|\n";



    cout << up_border << "\n";

    for (const auto& p : people) 
    {
        {
            int number = p.ride_id.size();
            for (int space = 0; space < new_ride_id_len - number; space++) {
                cout << " ";
            }
            cout << " " << p.ride_id << "|";
        }

        {
            int fname = p.fname.size();
            for (int space = 0; space < new_fname_len - fname + 1; space++) {
                cout << " ";
            }
            cout << p.fname << "|";
        }

        {
            int lname = p.lname.size();
            for (int space = 0; space < new_lname_len - lname + 1; space++) {
                cout << " ";
            }
            cout << p.lname << "|";
        }

        {
            int phone = p.phone.size();
            for (int space = 0; space < new_phone_len - phone + 1; space++) {
                cout << " ";
            }
            cout << p.phone << "|";
        }

        {
            int pickup = p.pickup.size();
            for (int space = 0; space < new_pickup_len - pickup + 1; space++) {
                cout << " ";
            }
            cout << p.pickup << "|";
        }

        {
            int dropoff = p.dropoff.size();
            for (int space = 0; space < new_dropoff_len - dropoff + 1; space++) {
                cout << " ";
            }
            cout << p.dropoff << "|";
        }

        {
            stringstream date_ss;
            date_ss << p.rideYear << "-"
                    << setw(2) << setfill('0') << p.rideMonth << "-"
                    << setw(2) << setfill('0') << p.rideDay;
            string date = date_ss.str();
            int date_len = date.size();
            for (int space = 0; space < new_date_len - date_len + 1; space++) {
                cout << " ";
            }
            cout << date << "|";
        }

        {
            int vehicle = p.vehicle.size();
            for (int space = 0; space < new_vehicle_len - vehicle + 1; space++) {
                cout << " ";
            }
            cout << p.vehicle << "|";
        }

        {
            int status = p.status.size();
            for (int space = 0; space < new_status_len - status + 1; space++) {
                cout << " ";
            }
            cout << p.status << "|";
        }
        std::cout << endl;

    }

    
    cout << low_border << "\n";
    cout << "\nTotal rides: " << people.size() << "\n\n";



    cout << "--------------------" << "\t\t" << "----------------------" << "\t\t" << "----------------------" << "\t\t" << "----------------------" << "\t\t\n";
    cout << "| Press 1: To Edit |" << "\t\t" << "| Press 2: To Delete |" << "\t\t" << "|  Press 3: To Ride  |" << "\t\t" << "| Press 4: To Cancel |" << "\t\t\n";
    cout << "--------------------" << "\t\t" << "----------------------" << "\t\t" << "----------------------" << "\t\t" << "----------------------" << "\t\t\n";

    cout << "\n\nPlease enter your choice here or press 0 to Menu: ";
    int choice;
    cin >> choice;



    if (choice == 0) {
        cout << "\nExiting to Main Menu... \n";
        return;
    } else if (choice == 1){
        editRides(people, dm);
    } else if (choice == 2) {
        deleteRides(people, dm);
    } else if (choice == 3) {
        goingRides(people, dm);
    } else if (choice == 4) {
        cancelRides(people, dm);
    } else {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    
}


    // [Rest of your existing view_all_rides function implementation]
    // ... (keep all the existing view_all_rides code, just add driver ID display where appropriate)


// [Rest of your existing functions (editRides, deleteRides, goingRides, cancelRides) with DriverManager parameter added]
// Make sure to update these functions to handle driver assignment/release when needed

string toLower(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

string toTitleCase(const string& str) {
    stringstream ss(str);
    string word, result;
    vector<string> smallWords = {"a", "an", "the", "and", "but", "or", "for", "nor"};

    bool firstWord = true;

    while (ss >> word) {
        if (!firstWord) result += " ";

        if (!firstWord && find(smallWords.begin(), smallWords.end(), word) != smallWords.end()) {
            for (auto& c: word) c = ::tolower(c);
        } else {
            if (!word.empty()) {
                word[0] = ::toupper(word[0]);
                for (size_t i = 1; i < word.size(); ++i) {
                    word[i] = ::tolower(word[i]);
                }
            }
        }
        result += word;
        firstWord = false;
    }
    return result;
}


void editRides(list<struct person>& people, DriverManager& dm) {

    string user_input;
    string full_name;
    string full_name_title;
    int choice, rideMonth, rideYear, rideDay;
    string phone_number;
    string drop_off;
    string pick_up;
    int totalChanges = 0;

    
    int completeCount = count_if(people.begin(), people.end(),
        [](const person& p) {
            return p.status == "Pending" || p.status == "pending";
        });


    cout << "\n====== EDITING MODE ======";
    cout << "\nPending Status Verification...";


    bool anyPending = any_of(people.begin(), people.end(), 
            [] (const person& p) {  
                return p.status == "Pending" || p.status == "pending";});


    if (!anyPending) {
        cout << "\nNo pending rides found.\n";
        clearScreen();
        view_all_rides(people, dm);
        return;
    }

    cout << "\nVerification is complete...";
    cout << "\nTotal Pending Rides: " << completeCount;
    cout << "\nPlease enter the Ride ID number or first name you want to edit: ";
    getline(cin >> ws, user_input);

    
    bool ride_found = false;

    for (auto& p: people){

        if ((user_input == p.ride_id && p.status == "Pending") || (toLower(user_input) == toLower(p.fname) && p.status == "Pending")) {
            ride_found = true;
            full_name = p.fname + " " + p.lname;
            full_name_title = toTitleCase(full_name);


            cout << "\nEditing info for: " << full_name_title << endl;
            cout << "\nPlease select which information below do you want to edit: " <<endl;

            do {

                cout << "\nPress 1: Contact Number";
                cout << "\nPress 2: Pickup Location";
                cout << "\nPress 3: Dropoff Location";
                cout << "\nPress 4: Date of Ride";
                cout << "\nPress 5: Vehicle Type";
                cout << "\nPress 0: To Exit...";
                cout << "\nPlease Enter your choice here: ";

                while (!(cin >> choice)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "\nInvalid input. Please enter a number: ";
                }

                switch(choice) {
                    
                    case 1: 
                        cout << "\nPlese enter your new contact number: ";
                        cin >> phone_number;
                        p.phone = phone_number;
                        totalChanges++;
                        break;

                    case 2: 
                        cout << "\nPlese enter your new Pickup Location: ";
                        getline(cin >> ws, pick_up);
                        p.pickup = pick_up;
                        totalChanges++;
                        break;
                    
                    case 3: 
                        cout << "\nPlese enter your new Dropoff Location: ";
                        getline(cin >> ws, drop_off);
                        p.dropoff = drop_off;
                        totalChanges++;
                        break;

                    case 4: 
                        cout << "\nPlese enter your new Date of Ride: ";
                        selectDate(rideMonth, rideYear, rideDay);
                        p.rideMonth = rideMonth;
                        p.rideYear = rideYear;
                        p.rideDay = rideDay;
                        totalChanges++;
                        break;
                    
                    case 5: 
                        cout << "\nPlese enter your new Ride Vehicle: ";
                        vehicle_type(p.vehicle);
                        totalChanges++;
                        break;

                    
                    case 0:
                        cout << "Exiting the editing mode...\n";
                        break;
                    default:
                        cout << "Invalid choice. Please try again.\n";
                }

                if (choice != 0) {
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
                }
            
            } while (choice != 0);
            

            clearScreen();


            if (totalChanges > 0) {
                cout << "====== CURRENT RIDE UPDATED ======\n\n";
                cout << "Updated info for: " << full_name_title << endl;
                cout << "Ride #" << p.ride_id << ":\n";
                cout << "Name: " << p.fname << " " << p.lname << endl;
                cout << "Phone: " << p.phone << endl;
                cout << "Pickup: " << p.pickup << endl;
                cout << "Dropoff: " << p.dropoff << endl;
                cout << "Date: " << setw(2) << setfill('0') << p.rideMonth
                    << "/" << setw(2) << p.rideDay << "/" << p.rideYear << endl;
                cout << "Vehicle: " << p.vehicle << endl;
                cout << "Status: " << p.status << "\n";
                cout << "----------------------------------\n";
            } else {
                cout << "====== CURRENT RIDE DETAILS ======\n\n";
                cout << "No changes made for: " << full_name_title << endl;
                cout << "Ride #" << p.ride_id << ":\n";
                cout << "Name: " << p.fname << " " << p.lname << endl;
                cout << "Phone: " << p.phone << endl;
                cout << "Pickup: " << p.pickup << endl;
                cout << "Dropoff: " << p.dropoff << endl;
                cout << "Date: " << setw(2) << setfill('0') << p.rideMonth
                    << "/" << setw(2) << p.rideDay << "/" << p.rideYear << endl;
                cout << "Vehicle: " << p.vehicle << endl;
                cout << "Status: " << p.status << "\n";
                cout << "----------------------------------\n";
            }


            cout << "\nDo you wish to proceed with this booking? (y/n): ";
            string choice_2;
            cin >> choice_2;

            if (choice_2 == "y" || choice_2 == "Y"){
                ride_found = false;
                cout <<"\nRide is confirmed...";
                onride(people, dm);
                return;
                
            } else {
                ride_found = false;
                cout << "\nAll Changes saved!";
                cout << "\nGoing back to View Mode...";
                clearScreen();
                view_all_rides(people, dm);
                return;
            }
        
                
            }


        }
        if (!ride_found) {   
            cout << "Ride not found!\n";
            }
     }


void deleteRides(list<struct person>& people, DriverManager& dm) {

    string full_name;
    string full_name_title;
    string user_input;
    char choice;

    cout << "\n====== DELETING MODE ======";
    do {
    cout << "\nPlease enter the Ride ID Number or first name you want to delete: ";
    getline(cin >> ws, user_input);
    
    if (user_input.empty()) {
        cout << "Error: Input cannot be empty. Please try again.\n";
    }
    } while (user_input.empty());


    bool found = false;
    auto it = people.begin();

    string lowerInput = toLower(user_input);


    while (it != people.end()) {

        if (user_input == it->ride_id ||    
            toLower(it->fname).find(lowerInput) != string::npos || 
            toLower(it->lname).find(lowerInput) != string::npos) 
        {
            found = true;
            full_name = it-> fname + " " + it->lname;
            full_name_title = toTitleCase(full_name);
            
            cout << "\nFound ride for: " << full_name_title;
            cout << "\nRide ID: " << it->ride_id;
            cout << "\nStatus: " << it->status;
            cout << "\nDo you want to proceed to delete this record? (y/n): ";
            cin >> choice;

            if (tolower(choice) == 'y') {
                it = people.erase(it);
                cout << "\nRecord deleted successfully!";
                cout << "\nDelete another record? (y/n): ";
                cin >> choice;
                if (tolower(choice) != 'y') {
                    clearScreen();
                    view_all_rides(people, dm);
                    return;  
                }
                
               
                cout << "\nEnter new Ride Number ID or partial name: ";
                cin.ignore();
                getline(cin, user_input);
                lowerInput = toLower(user_input);
                it = people.erase(it); 
                continue;
            } else {
                cout << "\nDeletion cancelled.";
                ++it;  
            }
        } else {
            ++it;  
        }
    }

    if (!found) {
        cout << "\nNo matching records found.";
        cout << "Press any key to return to menu...";
        cin.ignore();
        cin.get();
    }

}


void goingRides(list<struct person>& people, DriverManager& dm) {

    string full_name;
    string full_name_title;
    string user_input;
    char choice;

    cout << "\n====== RIDING MODE ======";
    cout << "\nEnter the Ride ID Number or first name you want to continue with the Ride: ";
    cin.ignore();
    getline(cin, user_input);


    string lowerInput = toLower(user_input);
    bool ride_found = false;

    for (auto& p: people){
        if ((user_input == p.ride_id && p.status == "Pending") || (lowerInput == toLower(p.fname) && p.status == "Pending")) {
            bool ride_found = true;
            full_name = p.fname + " " + p.lname;
            full_name_title = toTitleCase(full_name);

            cout << "\nFound ride for: " << full_name_title;
            cout << "\nRide ID: " << p.ride_id;
            cout << "\nStatus: " << p.status;
            cout << "\nDo you want to continue booking with your selected record? (y/n): ";
            cin >> choice;


            if (tolower(choice) == 'y') {
                onride(people, dm);
                return;

                cout << "\nDo you want to book a ride again? (y/n): ";
                cin >> choice;
                if (tolower(choice) == 'y') {
                    clearScreen();
                    booking(people, dm);
                    return;
                }

                else {

                    cout << "Returning to Menu...";
                    return;
                }


            } 
            else {
                cout << "\nBooking cancelled.";
                clearScreen();
                view_all_rides(people, dm);
                return; 

            }
        }
    } 
        if (!ride_found) {   
            cout << "\nRide not found.\n";
            cout << "Press any key to return to menu...";
            cin.ignore();
            cin.get();
    } 


}


bool isNotCanceled(const string& status) {
    string lower = toLower(status);
    return lower != "canceled";
}


bool isCompleted(const string& status) {
    string lower = toLower(status);
    return lower == "completed";
}

bool isCanceled(const string& status) {
    string lower = toLower(status);
    return lower == "canceled";
}

void cancelRides(list<struct person>& people, DriverManager& dm) {
    cout << "\n====== RIDE CANCELLATION MODE ======";
    cout << "\nEnter the Ride ID Number or first name you want to cancel: ";
    
    string user_input;
    cin.ignore();
    getline(cin, user_input);

    bool ride_found = false;

    for (auto& p : people) {

        if (user_input != p.ride_id && toLower(user_input) != toLower(p.fname)) {
            continue;
        }

        ride_found = true;
        string full_name = p.fname + " " + p.lname;

        cout << "\nFound ride for: " << toTitleCase(full_name);
        cout << "\nRide ID: " << p.ride_id;
        cout << "\nStatus: " << p.status;


        if (isCompleted(p.status)) {
            cout << "\nError: Completed rides cannot be canceled.\n";
            break;
        }

        else if (isCanceled(p.status)) {
            cout << "\nRide is already canceled.\n";
            break;
        }

        else {
            cout << "\nCancel this ride? (y/n): ";
            char choice;
            cin >> choice;

            if (tolower(choice) == 'y') {
                p.status = "Canceled";
                if (p.assignedDriverId != -1) {
                    dm.releaseDriver(p.assignedDriverId);  
                }
                cout << "\nRide canceled successfully!\n";
            } else {
                cout << "\nCancellation aborted.\n";
            }
        }

        cout << "\nCancel another ride? (y/n): ";
        char another;
        cin >> another;
        if (tolower(another) != 'y') {
            clearScreen();
            view_all_rides(people, dm);
            return;
        } else {

            cout << "\nEnter new Ride ID or name: ";
            cin.ignore();
            getline(cin, user_input);
            ride_found = false;
            break;
        }
    }

    if (!ride_found) {
        cout << "\nRide not found!\n";
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
    }
}



const char* bus[] = {
    "   _____________________________",
    " _|   |   |    |   |   |    |___\\__",
    "|     |   |    |   |   |    |_     _|",
    "'-(_)------------------------(_)--'"
};

const char* motorcycle[] = {
    "       __o",
    "     _ \\<,_",
    "    (_)>(_)"
};

const char* car[] = {
    "     ______",
    " ___/[] []\\____",
    "|__ _       _ __|",
    "  (_)-------(_)"
};


const char* suv[] = {
    "   ____________",
    " __[___]  [___]\\____",
    "|__               ___|",
    "   (_)---------(_)"
};


const char* truck[] = {
    "       _______",
    " ______|__|__|\\___",
    "|  _           _  |",
    "'-(_)---------(_)--'"
};


const char* van[] = {
    "  _____________",
    " |___|__|__|__|\\_",
    " | _           _  |",
    "'-(_)---------(_)-'"
};


const char* train[] = {
    "___________________________________________________ o O      ",
    "__/|___|___|___|___|___|___|___|___|___|___|___|___||_[_   ",
    "<'--0--0--0--0--0--0--0--0--0--0--0--0--0--0--0--0--0--0--0'"
};

void animateVehicle(const string& vehicleType, const string& message = "Your ride is on the way!") {
    const char** art = nullptr;
    int lines = 0;
    
    if (vehicleType == "Bus") {
        art = bus;
        lines = 4;
    } 
    else if (vehicleType == "Motorcycle") {
        art = motorcycle;
        lines = 3;
    }
    else if (vehicleType == "Sedan") {
        art = car;
        lines = 4;
    } 
    else if (vehicleType == "SUV") {
        art = suv;
        lines = 4;
    }
    else if (vehicleType == "Truck") {
        art = truck;
        lines = 4;
    }
    else if (vehicleType == "Van") {
        art = van;
        lines = 4;
    }
    else if (vehicleType == "Train") {
        art = train;
        lines = 4;
    }
    // ... [Add other vehicle checks] ...
    else {
        art = car; // Default to car if type unknown
        lines = 4;
    }

    // Animate
    const int terminal_width = 80;
    for (int pos = 0; pos < 40; ++pos) {
        cout << "\033[2J\033[1;1H"; 
        if (!message.empty()) {
            int padding = (terminal_width - message.size()) / 2;
            cout << string(padding, ' ') << message << "\n\n";
        }
        for (int i = 0; i < lines; ++i) {
            cout << string(pos, ' ') << art[i] << endl;
        }
        this_thread::sleep_for(chrono::milliseconds(120));
    }
    this_thread::sleep_for(chrono::milliseconds(600));
}


tuple<int, int, int> getCurrentDate() {
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);
    return make_tuple(
        localTime->tm_mday,      // Day (1-31)
        localTime->tm_mon + 1,    // Month (1-12)
        localTime->tm_year + 1900 // Year
    );
}

// Check if a ride is scheduled for today
bool isRideToday(const person& p) {
    auto [currentDay, currentMonth, currentYear] = getCurrentDate();
    return (
        p.rideDay == currentDay &&
        p.rideMonth == currentMonth &&
        p.rideYear == currentYear
    );
}


void startRideWithAnimation(list<person>& people, DriverManager& dm) {
    clearScreen();
    cout << "====== STARTING RIDES SCHEDULED FOR TODAY ======\n\n";

    bool foundTodayRide = false;

    for (auto& p : people) {
        if ((p.status == "Confirmed" || p.status == "Pending") && isRideToday(p)) {
            foundTodayRide = true;
            cout << "Starting ride for: " << p.fname << " " << p.lname << "\n";
            cout << "Vehicle: " << p.vehicle << "\n";
            cout << "Route: " << p.pickup << " → " << p.dropoff << "\n\n";

            // Play the vehicle animation
            animateVehicle(p.vehicle, "Your ride is starting now!");

            // Update status to "OnRide"
            p.status = "Completed";
            cout << "\nRide completed! You have arrived!\n"; 
            cout << string(40, '=') << "\n\n";
        }
    }

    if (!foundTodayRide) {
        cout << "No rides scheduled for today.\n";
    }

    cout << "Press Enter to continue...";
    cin.ignore();
    cin.get();
}
