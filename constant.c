#include "constant.h"
#include <stdbool.h>

//------------------- Login / Register state -------------------
bool showSign = false;
bool showReg = false;
int activeBox = -1;

char loginUsername[64] = "";
char loginPassword[64] = "";
char loginEmail[64] = "";

char regUsername[64] = "";
char regEmail[64] = "";
char regPassword[64] = "";
char regConfirmPassword[64] = "";

//------------------- User session state -------------------
bool isLoggedIn = false;
bool showLogout = false;
bool showDashboard = true;
bool showBookingScreen = false;
bool showCategoryBar = true;
bool pendingBooking = false;

//------------------- Navigation / Panel views -------------------
bool showMyBookingsScreen = false;
bool showRewardsScreen = false;
bool showDetailsForm = false;

//------------------- Movies -------------------
Movie selectedMovie;
char selectedGenre[20] = "ALL";

Movie movies[] = {
    { "Oppenheimer",               "6:00 PM",  "Drama",   9.0 },
   { "Spider Brand New Day",           "10:00 PM", "Sci-Fi",  7.6 },
    { "Pathaan",                    "5:00 PM",  "Action",  7.5 },
    { "3 Idiots",                   "4:15 PM",  "Comedy",  8.4 },
    { "Kabhi Khushi Kabhie Gham",  "8:00 PM",  "Romance", 7.8 },
    { "Jhola",                     "6:45 PM",  "Drama",   7.9 },
    { "Prem Geet",                 "9:00 PM",  "Romance", 7.2 },
    { "A Quite Place 3",           "10:00 PM", "Horror",  7.6 }
};

int movieCount = 8;

//------------------- Seat Map -------------------
bool bookedSeats[MAX_MOVIES][SEAT_ROWS][SEAT_COLS];
bool selectedSeats[SEAT_ROWS][SEAT_COLS];
int selectedMovieIndex = -1;

//------------------- Form & Selection state -------------------
int selectedSeatsCount = 0;
char formName[64] = "";
char formPhone[16] = "";
bool exitProgram = false;

//------------------- Ticket Type Splitter -------------------
const char *ticketTypeNames[TICKET_TYPE_COUNT]      = { "Adult", "Child", "Senior", "Student" };
const float ticketTypeBasePrice[TICKET_TYPE_COUNT]  = { 350.0f,  200.0f,  220.0f,   280.0f };
const float ticketTypeTaxPercent[TICKET_TYPE_COUNT] = { 13.0f,   5.0f,    5.0f,     8.0f };
int seatTicketType[SEAT_ROWS][SEAT_COLS];

//------------------- Payment Simulation -------------------
bool showPaymentScreen = false;
int paymentMethodSelected = -1;

//------------------- Ticket Printing / Retrieval Simulation -------------------
bool showTicketScreen = false;
TicketDetail lastTicket;

bool showScanRetrieval = false;
char retrievalCode[16] = "";
bool retrievalFound = false;
bool retrievalSearched = false;
TicketDetail retrievedTicket;