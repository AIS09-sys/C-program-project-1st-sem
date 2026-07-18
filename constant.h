#ifndef CONSTANT_H
#define CONSTANT_H

#include "raylib.h"
#include <stdbool.h>

//------------------- File paths -------------------
#define dat  "database\\userdata.bin"
#define book "database\\bookings.bin"
#define tic "database\\userticketdetail.bin"
#define tictemp "database\\tickettemp.bin"
#define MAX_TICKETS 3

//------------------- Macros -------------------
#define MAX_INPUT 32
#define OFFWHITE (Color){ 245, 245, 245, 255 }
#define DRAW_BUTTON(Btn, text, btnColor, txtColor) \
    HoverButton(Btn.x, Btn.y, Btn.width, Btn.height, text, btnColor, txtColor)

//------------------- Seat Map -------------------
#define SEAT_ROWS 8
#define SEAT_COLS 10
#define MAX_MOVIES 20
#define TICKET_PRICE 350
#define MAX_SEAT_LIMIT 5

//------------------- Loyalty Rewards -------------------
#define REWARD_THRESHOLD 15   // this many bookings earns a free ticket

//------------------- Ticket Type Splitter -------------------
typedef enum {
    TICKET_ADULT = 0,
    TICKET_CHILD,
    TICKET_SENIOR,
    TICKET_STUDENT,
    TICKET_TYPE_COUNT
} TicketType;

//------------------- Payment Simulation -------------------
#define PAYMENT_KHALTI 0
#define PAYMENT_ESEWA  1
#define PAYMENT_CASH   2

//------------------- Structures -------------------
typedef struct
{
    char username[64];
    char email[64];
    char password[64];
} newUsers;

typedef struct
{
    char username[64];
    char password[64];
} Userlog;

typedef struct
{
    char name[50];
    char time[20];
    char genre[30];
    float rating;
} Movie;

typedef struct
{
    char ticketNumber[16];
    char movieName[64];
    char movieTime[16];
    char userName[64];
    char passengerName[64];
    char userPhone[12];
    char bookedSeatsList[64];
    int seatCount;

    // ---- Ticket type splitter breakdown ----
    int typeCount[TICKET_TYPE_COUNT];
    float subtotal;
    float taxTotal;
    float totalPrice;

    // ---- Payment simulation ----
    char paymentMethod[16];
} TicketDetail;

//------------------- Global Variables (EXTERN ONLY) -------------------
extern int selectedSeatsCount;
extern bool bookedSeats[MAX_MOVIES][SEAT_ROWS][SEAT_COLS];
extern bool selectedSeats[SEAT_ROWS][SEAT_COLS];
extern int selectedMovieIndex;

extern char formName[64];
extern char formPhone[16];

extern bool showDetailsForm;
extern bool showRewardsScreen;
extern bool showMyBookingsScreen;

//------------------- Login / Register state -------------------
extern bool showSign;
extern bool showReg;
extern int activeBox;

extern char loginUsername[64];
extern char loginPassword[64];
extern char loginEmail[64];

extern char regUsername[64];
extern char regEmail[64];
extern char regPassword[64];
extern char regConfirmPassword[64];

//------------------- User session state -------------------
extern bool isLoggedIn;
extern bool showLogout;
extern bool showDashboard;
extern bool showCategoryBar;
extern bool showBookingScreen;
extern bool pendingBooking;

//------------------- Movies -------------------
extern Movie movies[];
extern int movieCount;
extern Movie selectedMovie;
extern char selectedGenre[20];
extern bool exitProgram;

//------------------- Ticket Type Splitter -------------------
extern const char *ticketTypeNames[TICKET_TYPE_COUNT];
extern const float ticketTypeBasePrice[TICKET_TYPE_COUNT];
extern const float ticketTypeTaxPercent[TICKET_TYPE_COUNT];
extern int seatTicketType[SEAT_ROWS][SEAT_COLS]; // 0 (TICKET_ADULT) by default when unassigned

//------------------- Payment Simulation -------------------
extern bool showPaymentScreen;
extern int paymentMethodSelected;

//------------------- Ticket Printing / Retrieval Simulation -------------------
extern bool showTicketScreen;
extern TicketDetail lastTicket;

extern bool showScanRetrieval;
extern char retrievalCode[16];
extern bool retrievalFound;
extern bool retrievalSearched;
extern TicketDetail retrievedTicket;

#endif