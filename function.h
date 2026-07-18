#ifndef FUNCTION_H
#define FUNCTION_H

#include "raylib.h"
#include "constant.h"
void Drawing();
//------------------- UI helpers -------------------
void HoverButton(float x, float y, float w, float h, const char *text, Color buttonColor, Color textColor);
void Popup(const char *message);
void TextInputBox(int id, Rectangle box, char *buf, int maxLen, bool pass);

//------------------- Screens / cards -------------------
void loginCard(void);
void registerCard(void);
void categoryBar(void);
void avatar(void);

void drawSidebarButtons(void);

//------------------- Movies -------------------
void movieCard(float x, float y, Movie *m, int index);
void drawMovieGrid(void);

//------------------- Auth logic -------------------
void regFunct(void);
void loginFunct(void);
void toLower(char *str);

//------------------- Booking -------------------
void resetSelectedSeats(void);
int countSelectedSeats(void);
void drawOneSeat(int row, int col, float x, float y, float size);
void drawSeatGrid(float startX, float startY);
void drawSeatLegend(float x, float y);
void seatBooking(void);
void saveBookings(void);
void loadBookings(void);
void drawDetailsForm(int selectedCount);
void drawMyBookings(void);
void drawRewardsScreen(void);
int getUserTotalTickets(int *outEarnedFreeTickets, int *outProgressToNext);
int countUserTickets(void);
void deleteTicket(int recordIndex);

//------------------- Ticket Type Splitter -------------------
void drawTicketTypePanel(float x, float y);
float getSelectedSeatsSubtotal(void);
float getSelectedSeatsTax(void);

//------------------- Payment Simulation -------------------
void drawPaymentScreen(void);
void confirmPayment(int method);

//------------------- Ticket Printing / Retrieval Simulation -------------------
void finalizeBookingAfterPayment(void);
void drawPrintedTicket(TicketDetail *ticket, float x, float y, float w, float h);
void drawTicketScreen(void);
void drawScanRetrieval(void);
void lookupTicketByCode(const char *code);

#endif