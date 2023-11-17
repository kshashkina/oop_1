#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
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
              ticketPrice(price), isBooked(true) {}

    int getTicketId() const { return ticketId; }
    string getSeatId() const { return seatId; }
    float getTicketPrice() const { return ticketPrice; }
    bool getIsBooked() const { return isBooked; }
    string getTravelerName() const { return travelerName; }
    string getPlaneNumber() const { return planeNumber; }
    string getJourneyDate() const { return journeyDate; }

    void setTravelerName(const string& name) {
        travelerName = name;
        isBooked = false;
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


