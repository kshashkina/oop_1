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

