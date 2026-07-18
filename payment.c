#include "raylib.h"
#include "function.h"
#include "constant.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>



//------------------- Get Selected Seats Subtotal -------------------
float getSelectedSeatsSubtotal(void) {
	float subtotal = 0.0f;
	for (int r = 0; r < SEAT_ROWS; r++) {
		for (int c = 0; c < SEAT_COLS; c++) {
			if (!selectedSeats[r][c]) continue;
			int type = seatTicketType[r][c];
			subtotal += ticketTypeBasePrice[type];
		}
	}
	return subtotal;
}

//------------------- Get Selected Seats Tax -------------------
float getSelectedSeatsTax(void) {
	float tax = 0.0f;
	for (int r = 0; r < SEAT_ROWS; r++) {
		for (int c = 0; c < SEAT_COLS; c++) {
			if (!selectedSeats[r][c]) continue;
			int type = seatTicketType[r][c];
			tax += ticketTypeBasePrice[type] * (ticketTypeTaxPercent[type] / 100.0f);
		}
	}
	return tax;
}

//------------------- Draw Ticket Type Panel -------------------
void drawTicketTypePanel(float x, float y) {
	DrawText("Ticket Types", x, y, 26, BLACK);
	DrawText("(tap a seat's tier to change it)", x, y + 30, 16, DARKGRAY);

	float rowH = 46;
	int row = 0;

	for (int r = 0; r < SEAT_ROWS; r++) {
		for (int c = 0; c < SEAT_COLS; c++) {
			if (!selectedSeats[r][c]) continue;

			float rowY = y + 60 + row * rowH;
			int type = seatTicketType[r][c];

			char label[8];
			snprintf(label, sizeof(label), "%c%d", 'A' + r, c + 1);
			DrawText(label, x, rowY + 10, 20, BLACK);

			Rectangle typeBtn = { x + 60, rowY, 170, 36 };
			char typeText[48];
			snprintf(typeText, sizeof(typeText), "%s  Rs.%.0f", ticketTypeNames[type], ticketTypeBasePrice[type]);
			DRAW_BUTTON(typeBtn, typeText, BLACK, WHITE);

			if (CheckCollisionPointRec(GetMousePosition(), typeBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				seatTicketType[r][c] = (type + 1) % TICKET_TYPE_COUNT;
			}

			row++;
		}
	}

	float summaryY = y + 60 + row * rowH + 15;
	float subtotal = getSelectedSeatsSubtotal();
	float tax = getSelectedSeatsTax();
	DrawText(TextFormat("Subtotal: Rs.%.2f", subtotal), x, summaryY, 18, DARKGRAY);
	DrawText(TextFormat("Tax:      Rs.%.2f", tax), x, summaryY + 24, 18, DARKGRAY);
}


//------------------- Confirm Payment -------------------
void confirmPayment(int method) {
	paymentMethodSelected = method;
	finalizeBookingAfterPayment();
	showPaymentScreen = false;
	showTicketScreen = true;
}

//------------------- Draw Payment Screen -------------------
void drawPaymentScreen(void) {
	ClearBackground(WHITE);

	float cardW = 560, cardH = 550;
	float cardX = (GetScreenWidth() - cardW) / 2.0f;
	float cardY = (GetScreenHeight() - cardH) / 2.0f;

	DrawRectangleRounded((Rectangle) {
		cardX, cardY, cardW, cardH
	}, 0.06f, 10, WHITE);
	DrawRectangleRoundedLines((Rectangle) {
		cardX, cardY, cardW, cardH
	}, 0.06f, 10, LIGHTGRAY);

	DrawText("Payment", cardX + 40, cardY + 30, 34, BLACK);

	int freeCredits = 0, progress = 0;
	getUserTotalTickets(&freeCredits, &progress);
	bool isFreeTicketApplied = (freeCredits > 0);

	float subtotal = getSelectedSeatsSubtotal();
	float tax = getSelectedSeatsTax();
	float total = isFreeTicketApplied ? 0.0f : (subtotal + tax);

	DrawText(TextFormat("Subtotal: Rs.%.2f", subtotal), cardX + 40, cardY + 90, 20, DARKGRAY);
	DrawText(TextFormat("Tax: Rs.%.2f", tax), cardX + 40, cardY + 115, 20, DARKGRAY);
	if (isFreeTicketApplied) {
		DrawText("Total: Rs.0.00 (Reward ticket applied)", cardX + 40, cardY + 145, 22, GREEN);
	} else {
		DrawText(TextFormat("Total Due: Rs.%.2f", total), cardX + 40, cardY + 145, 22, MAROON);
	}

	DrawText("Choose a payment method:", cardX + 40, cardY + 200, 20, BLACK);

	Rectangle khaltiBtn = { cardX + 40, cardY + 240, 480, 55 };
	Rectangle esewaBtn  = { cardX + 40, cardY + 315, 480, 55 };
	Rectangle cashBtn   = { cardX + 40, cardY + 390, 480, 55 };
	Rectangle cancelBtn = { cardX + 40, cardY + cardH - 85, 480, 45 };

	DRAW_BUTTON(khaltiBtn, "KHALTI", BLACK, WHITE);
	DRAW_BUTTON(esewaBtn, "ESEWA", BLACK, WHITE);
	DRAW_BUTTON(cashBtn, "CASH", BLACK, WHITE);
	DRAW_BUTTON(cancelBtn, "CANCEL", RED, WHITE);

	if (CheckCollisionPointRec(GetMousePosition(), khaltiBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		confirmPayment(PAYMENT_KHALTI);
	}
	if (CheckCollisionPointRec(GetMousePosition(), esewaBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		confirmPayment(PAYMENT_ESEWA);
	}
	if (CheckCollisionPointRec(GetMousePosition(), cashBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		confirmPayment(PAYMENT_CASH);
	}
	if (CheckCollisionPointRec(GetMousePosition(), cancelBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		showPaymentScreen = false;
		showDetailsForm = true;
	}
}

//------------------- Collect Selected Seats -------------------
static void collectSelectedSeats(TicketDetail *ticket) {
	strcpy(ticket->bookedSeatsList, "");
	ticket->seatCount = 0;

	for (int r = 0; r < SEAT_ROWS; r++) {
		for (int c = 0; c < SEAT_COLS; c++) {
			if (!selectedSeats[r][c]) continue;

			char seatLabel[10];
			snprintf(seatLabel, sizeof(seatLabel), "%c%d ", 'A' + r, c + 1);
			strcat(ticket->bookedSeatsList, seatLabel);

			ticket->typeCount[seatTicketType[r][c]]++;
			ticket->seatCount++;
		}
	}
}

//------------------- Mark Seats As Booked -------------------
static void markSeatsAsBooked(void) {
	for (int r = 0; r < SEAT_ROWS; r++) {
		for (int c = 0; c < SEAT_COLS; c++) {
			if (selectedSeats[r][c]) bookedSeats[selectedMovieIndex][r][c] = true;
		}
	}
}
static const char *paymentMethodNames[3] = { "Khalti", "eSewa", "Cash" };

//------------------- Finalize Booking After Payment -------------------
void finalizeBookingAfterPayment(void) {
	FILE *ticketFile = fopen(tic, "ab");
	if (ticketFile == NULL) {
		Popup("Could not save ticket! Make sure the 'database' folder exists.");
		return;
	}

	int freeCredits = 0, progress = 0;
	getUserTotalTickets(&freeCredits, &progress);
	bool isFreeTicketApplied = (freeCredits > 0);

	TicketDetail ticket;
	memset(&ticket, 0, sizeof(TicketDetail));
	snprintf(ticket.ticketNumber, sizeof(ticket.ticketNumber), "TIC-%05d", GetRandomValue(10000, 99999));
	strcpy(ticket.movieName, selectedMovie.name);
	strcpy(ticket.movieTime, selectedMovie.time);
	strcpy(ticket.userName, loginUsername);
	strcpy(ticket.passengerName, formName);
	strcpy(ticket.userPhone, formPhone);

	collectSelectedSeats(&ticket);

	ticket.subtotal = getSelectedSeatsSubtotal();
	ticket.taxTotal = getSelectedSeatsTax();
	ticket.totalPrice = isFreeTicketApplied ? 0.0f : (ticket.subtotal + ticket.taxTotal);
	strncpy(ticket.paymentMethod, paymentMethodNames[paymentMethodSelected], sizeof(ticket.paymentMethod) - 1);

	fwrite(&ticket, sizeof(TicketDetail), 1, ticketFile);
	fclose(ticketFile);

	markSeatsAsBooked();
	saveBookings();

	lastTicket = ticket;
}

//------------------- Draw Fake Barcode -------------------
static void drawFakeBarcode(const char *seed, float x, float y, float width, float height) {
	int barCount = 50;
	float barWidth = width / barCount;
	int seedLen = strlen(seed);

	DrawRectangle((int)x, (int)y, (int)width, (int)height, WHITE);

	for (int i = 0; i < barCount; i++) {
		// Walk through the ticket number's characters, looping back to the
		// start once we run out - each character's value decides whether
		// its bar is black or white.
		char c = seed[i % seedLen];
		bool isBlack = (c % 2 == 0);

		if (isBlack) {
			DrawRectangle((int)(x + i * barWidth), (int)y, (int)barWidth + 1, (int)height, BLACK);
		}
	}
}

//------------------- Draw Printed Ticket -------------------
void drawPrintedTicket(TicketDetail *ticket, float x, float y, float w, float h) {
	DrawRectangleRounded((Rectangle) {
		x, y, w, h
	}, 0.04f, 8, WHITE);
	DrawRectangleRoundedLines((Rectangle) {
		x, y, w, h
	}, 0.04f, 8, LIGHTGRAY);

	DrawText("CINEMA BOOKING SYS", x + 30, y + 25, 26, BLACK);
	DrawText(ticket->movieName, x + 30, y + 65, 30, BLACK);
	DrawText(TextFormat("Time: %s", ticket->movieTime), x + 30, y + 105, 20, DARKGRAY);
	DrawText(TextFormat("Passenger: %s", ticket->passengerName), x + 30, y + 135, 20, DARKGRAY);
	DrawText(TextFormat("Phone: %s", ticket->userPhone), x + 30, y + 165, 18, DARKGRAY);
	DrawText(TextFormat("Seats: %s", ticket->bookedSeatsList), x + 30, y + 195, 20, MAROON);

	float breakdownY = y + 225;
	for (int i = 0; i < TICKET_TYPE_COUNT; i++) {
		if (ticket->typeCount[i] <= 0) continue;
		DrawText(TextFormat("%s x%d", ticketTypeNames[i], ticket->typeCount[i]), x + 30, breakdownY, 18, DARKGRAY);
		breakdownY += 22;
	}

	DrawText(TextFormat("Subtotal: Rs.%.2f", ticket->subtotal), x + 30, breakdownY + 10, 18, DARKGRAY);
	DrawText(TextFormat("Tax: Rs.%.2f", ticket->taxTotal), x + 30, breakdownY + 32, 18, DARKGRAY);
	DrawText(TextFormat("Total Paid: Rs.%.2f (%s)", ticket->totalPrice, ticket->paymentMethod), x + 30, breakdownY + 58, 20, DARKGREEN);

	DrawText("TICKET ID", x + w - 200, y + 25, 16, DARKGRAY);
	DrawText(ticket->ticketNumber, x + w - 200, y + 45, 22, ORANGE);

	drawFakeBarcode(ticket->ticketNumber, x + w - 200, y + 90, 170, 70);
	DrawText("Scan at entry", x + w - 200, y + 170, 14, DARKGRAY);
}

//------------------- Draw Ticket Screen -------------------
void drawTicketScreen(void) {
	ClearBackground(WHITE);

	float cardW = 780, cardH = 420;
	float cardX = (GetScreenWidth() - cardW) / 2.0f;
	float cardY = (GetScreenHeight() - cardH) / 2.0f;

	DrawText("Ticket Printed!", cardX, cardY - 50, 32, BLACK);
	drawPrintedTicket(&lastTicket, cardX, cardY, cardW, cardH);

	Rectangle doneBtn = { cardX + cardW / 2 - 100, cardY + cardH + 30, 200, 55 };
	DRAW_BUTTON(doneBtn, "DONE", BLACK, WHITE);

	if (CheckCollisionPointRec(GetMousePosition(), doneBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Popup(TextFormat("Congrats! Ticket Booked. ID: %s", lastTicket.ticketNumber));

		showTicketScreen = false;
		showDetailsForm = false;
		resetSelectedSeats();
		showBookingScreen = false;
		showCategoryBar = true;
	}
}

//------------------- Lookup Ticket By Code -------------------
void lookupTicketByCode(const char *code) {
	retrievalSearched = true;
	retrievalFound = false;

	FILE *fp = fopen(tic, "rb");
	if (fp == NULL) return;

	TicketDetail t;
	while (fread(&t, sizeof(TicketDetail), 1, fp) == 1) {
		if (strcmp(t.ticketNumber, code) == 0) {
			retrievedTicket = t;
			retrievalFound = true;
			break;
		}
	}
	fclose(fp);
}


//------------------- Draw Scan Retrieval -------------------
void drawScanRetrieval(void) {

	ClearBackground(WHITE);

	float cardW =800, cardH = 240;

	float cardX = (GetScreenWidth() - cardW) / 2.0f;

	float cardY = 350;

	DrawText("Find a Ticket:", cardX, cardY - 40, 34, BLACK);



	DrawText("Ticket ID", cardX + 250, cardY - 45, 18, BLACK);

	TextInputBox(20, (Rectangle) {
		cardX + 250, cardY - 45, 400, 50
	}, retrievalCode, 15, false);

	Rectangle searchBtn = { cardX + 700, cardY -45, 180, 50 };

	DRAW_BUTTON(searchBtn, "SEARCH", BLACK, WHITE);

	if (CheckCollisionPointRec(GetMousePosition(), searchBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

		if (strlen(retrievalCode) == 0) Popup("Enter a ticket ID first!");

		else lookupTicketByCode(retrievalCode);

	}

	if (retrievalSearched) {

		if (retrievalFound) {

			drawPrintedTicket(&retrievedTicket, cardX, cardY - cardH+300, cardW, 420);

		} else {

			DrawText("No matching ticket found.", cardX, cardY - cardH + 300, 20, RED);

		}

	}

}