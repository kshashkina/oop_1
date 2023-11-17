#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
using namespace std;

class FlightTicket {
private:
    int ticketId;
    string travelerName;
    string journeyDate;
    string planeNumber;
    string seatId;
    bool isBooked;
    float ticketPrice;

public:
    FlightTicket(string traveler, string date, string seat, string plane, float price)
            : travelerName(traveler), journeyDate(date), seatId(seat), planeNumber(plane),
              ticketPrice(price), isBooked(false) {}

    int getTicketId() const { return ticketId; }
    string getSeatId() const { return seatId; }
    float getTicketPrice() const { return ticketPrice; }
    bool getIsBooked() const { return isBooked; }
    string getTravelerName() const { return travelerName; }
    string getPlaneNumber() const { return planeNumber; }
    string getJourneyDate() const { return journeyDate; }

    void setTravelerName(const string& name) {
        travelerName = name;
        isBooked = true;
    }

    void setTicketId(int id) { ticketId = id; }
};

class Traveler {
    string name;
    vector<FlightTicket> bookedTickets;

public:
    Traveler(string name) : name(name) {}

    void bookTicket(const FlightTicket& ticket) {
        bookedTickets.push_back(ticket);
    }

    string getName() const { return name; }
    vector<FlightTicket> getBookedTickets() const { return bookedTickets; }
};

class Flight {
private:
    string departureDate;
    string flightCode;
    vector<FlightTicket> flightTickets;

public:
    Flight(string flightCode, string date): flightCode(flightCode), departureDate(date) {}

    void addFlightTicket(const FlightTicket& ticket) {
        flightTickets.push_back(ticket);
    }

    string getDepartureDate() const { return departureDate; }
    string getFlightCode() const { return flightCode; }

    vector<FlightTicket> availableTickets() const {
        vector<FlightTicket> available;
        for (const auto& ticket : flightTickets) {
            if (!ticket.getIsBooked()) {
                available.push_back(ticket);
            }
        }
        return available;
    }


    void reserveSeat(const string& passengerName, const string& seat, int ticketId, vector<FlightTicket>& booked, vector<Traveler>& travelers) {
        for (auto& ticket : flightTickets) {
            if (ticket.getSeatId() == seat && !ticket.getIsBooked()) { // Check if the ticket is not booked
                ticket.setTravelerName(passengerName);
                ticket.setTicketId(ticketId);
                booked.push_back(ticket);

                bool foundTraveler = false;
                for (auto& traveler : travelers) {
                    if (traveler.getName() == passengerName) {
                        traveler.bookTicket(ticket);
                        foundTraveler = true;
                        break;
                    }
                }

                if (!foundTraveler) {
                    Traveler newTraveler(passengerName);
                    newTraveler.bookTicket(ticket);
                    travelers.push_back(newTraveler);
                }

                cout << "Booking confirmed with ID: " << ticketId << endl;
                return;
            }
        }
        cout << "Seat not available or already reserved." << endl;
    }

};

class FlightConfiguration {
public:
    vector<Flight> loadFlights(string filePath) {
        ifstream file(filePath);
        vector<Flight> flights;
        string line;

        if (!file.is_open()) {
            cout << "Error opening file." << endl;
            return flights;
        }

        while (getline(file, line)){
            stringstream ss(line);
            string date, flightNum;
            int rowStart, rowEnd, seatsPerRow;
            float price;
            char delimiter, seatLetter;

            ss >> date >> flightNum >> seatsPerRow;
            Flight flight(flightNum, date);

            while (ss >> rowStart >> delimiter >> rowEnd >> price >> delimiter) {
                for (int row = rowStart; row <= rowEnd; ++row) {
                    for (int seat = 0; seat < seatsPerRow; ++seat) {
                        seatLetter = 'A' + seat;
                        string seatId = to_string(row) + seatLetter;
                        FlightTicket ticket("", date, seatId, flightNum, price);
                        flight.addFlightTicket(ticket);
                    }
                }
            }
            flights.push_back(flight);
        }
        return flights;
    }
};

class BookingSystem {
private:
    vector<Flight> flights;
    vector<Traveler> travelers;
    vector<FlightTicket> bookedTickets;
    int nextId = 1;

public:
    BookingSystem(const string& configFilePath) {
        FlightConfiguration configReader;
        flights = configReader.loadFlights(configFilePath);
    }

    void check(const string& date, const string& flightNumber) {
        for (const auto& flight : flights) {
            if (flight.getDepartureDate() == date && flight.getFlightCode() == flightNumber) {
                auto availableTickets = flight.availableTickets();
                if (!availableTickets.empty()) {
                    for (const auto& ticket : availableTickets) {
                        cout << "Seat: " << ticket.getSeatId() << ", Price: $" << ticket.getTicketPrice() << endl;
                    }
                } else {
                    cout << "No available seats." << endl;
                }
                return;
            }
        }
        cout << "Flight not found." << endl;
    }

    void book(const string& date, const string& flightNumber, const string& seatId, const string& username) {
        for (auto& flight : flights) {
            if (flight.getDepartureDate() == date && flight.getFlightCode() == flightNumber) {
                flight.reserveSeat(username, seatId, nextId, bookedTickets, travelers);
                nextId++;
                return;
            }
        }
        cout << "Flight not found or seat number is incorrect." << endl;
    }

    void returnTicket(int id) {
        auto it = std::find_if(bookedTickets.begin(), bookedTickets.end(), [id](const FlightTicket& ticket) {
            return ticket.getTicketId() == id;
        });

        if (it != bookedTickets.end()) {
            // Process the refund here as necessary
            cout << "Confirmed " << it->getTicketPrice() << "$ refund for " << it->getTravelerName() << endl;
            // Erase by iterator
            bookedTickets.erase(it);
        } else {
            cout << "Ticket with the given ID not found." << endl;
        }
    }


    void view(int id) {
        auto it = std::find_if(bookedTickets.begin(), bookedTickets.end(), [id](const FlightTicket& ticket) {
            return ticket.getTicketId() == id;
        });
        if (it != bookedTickets.end()) {
            const auto& ticket = *it; // Corrected line
            cout << "Ticket details: Flight " << ticket.getPlaneNumber()
                 << ", Date: " << ticket.getJourneyDate() << ", Seat: " << ticket.getSeatId()
                 << ", Price: $" << ticket.getTicketPrice() << ", Passenger: " << ticket.getTravelerName() << endl;
        } else {
            cout << "Ticket with given ID not found." << endl;
        }
    }


    void view(const string& username) {
        auto it = find_if(travelers.begin(), travelers.end(), [&](const Traveler& t) { return t.getName() == username; });
        if (it != travelers.end()) {
            const auto& tickets = it->getBookedTickets();
            if (!tickets.empty()) {
                cout << "Tickets for " << username << ":" << endl;
                for (const auto& ticket : tickets) {
                    cout << "Flight " << ticket.getPlaneNumber() << ", Date: " << ticket.getJourneyDate()
                         << ", Seat: " << ticket.getSeatId() << ", Price: $" << ticket.getTicketPrice() << endl;
                }
            } else {
                cout << "No tickets found for passenger: " << username << endl;
            }
        } else {
            cout << "Passenger not found." << endl;
        }
    }
};


int main() {
    BookingSystem system("C:\\KSE IT\\oop_1\\text");

    string commandLine;
    while (true) {
        cout << "Enter command: ";
        getline(cin, commandLine);
        if (commandLine == "exit") {
            break;
        }

        stringstream ss(commandLine);
        string command, date, flightNumber, seatId, username;
        int id;

        ss >> command;
        if (command == "check") {
            ss >> date >> flightNumber;
            system.check(date, flightNumber);
        }else if (command == "book") {
            ss >> date >> flightNumber >> seatId >> username;
            system.book(date, flightNumber, seatId, username);
        } else if (command == "return") {
            ss >> id;
            system.returnTicket(id);
        } else if (command == "view") {
            ss >> username;
            if (stringstream(username) >> id) {
                system.view(id); // View by ticket ID
            } else {
                system.view(username); // View all tickets for username
            }
        } else {
            cout << "Unknown command." << endl;
        }
    }
    return 0;
}
