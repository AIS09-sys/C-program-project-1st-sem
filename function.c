#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "function.h"
#include "constant.h"

//------------------- Movie Grid -------------------
void drawMovieGrid(void) {
	int moviesPerRow = 4;
	float cardWidth = 380;
	float cardHeight = 180;
	float spaceBetween = 20;
	float gridStartX = 350;
	float gridStartY = 400;

	int shownCount = 0;

	for (int i = 0; i < movieCount; i++) {
		bool genreMatches = (strcmp(selectedGenre, "ALL") == 0) ||
		                    (strcmp(movies[i].genre, selectedGenre) == 0);

		if (!genreMatches) {
			continue;
		}

		int column = shownCount % moviesPerRow;
		int row = shownCount / moviesPerRow;

		float cardX = gridStartX + column * (cardWidth + spaceBetween);
		float cardY = gridStartY + row * (cardHeight + spaceBetween);

		movieCard(cardX, cardY, &movies[i], i);
		shownCount++;
	}
}

//------------------- Main Drawing -------------------
void Drawing(void) {
	DrawRectangle(0, 0, 300, 1200, OFFWHITE);
	DrawText("CBS", 50, 40, 80, BLACK);

	DrawRectangle(510, 100, 1000, 120, OFFWHITE);
	DrawText("CINEMA BOOKING SYS", 700, 130, 60, BLACK);

	drawSidebarButtons();

	if (!showBookingScreen && !showMyBookingsScreen && !showRewardsScreen) {
		categoryBar();
	}

	if (isLoggedIn) {
		avatar();
	}

	if (showTicketScreen) {
		drawTicketScreen();
		return;
	}

	if (showPaymentScreen) {
		drawPaymentScreen();
		return;
	}

	if (showScanRetrieval) {
		drawScanRetrieval();
		return;
	}

	if (showBookingScreen) {
		seatBooking();
		return;
	}

	if (showMyBookingsScreen) {
		drawMyBookings();
		return;
	}

	if (showRewardsScreen) {
		drawRewardsScreen();
		return;
	}

	if (showSign) {
		loginCard();
	} else if (showReg) {
		registerCard();
	}

	if (showDashboard && !showSign && !showReg) {
		drawMovieGrid();
	}
}

//------------------- Sidebar Buttons -------------------
void drawSidebarButtons(void) {
	Rectangle homeBtn = { 30, 300, 250, 70 };
	DRAW_BUTTON(homeBtn, "HOME", BLACK, WHITE);
	if (CheckCollisionPointRec(GetMousePosition(), homeBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		showMyBookingsScreen = false;
		showRewardsScreen = false;
		showScanRetrieval = false;
		retrievalSearched = false;
		retrievalFound = false;
		strcpy(retrievalCode, "");
		showDashboard = true;
		showBookingScreen = false;
		showCategoryBar = true;
	}

	Rectangle bookBtn = { 30, 400, 250, 70 };
	DRAW_BUTTON(bookBtn, "MY-BOOKINGS", BLACK, WHITE);
	if (CheckCollisionPointRec(GetMousePosition(), bookBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		if (!isLoggedIn) {
			Popup("Please sign in to view your bookings!");
		} else {
			showMyBookingsScreen = true;
			showRewardsScreen = false;
			showScanRetrieval = false;
			retrievalSearched = false;
			retrievalFound = false;
			strcpy(retrievalCode, "");
			showDashboard = false;
			showBookingScreen = false;
			showCategoryBar = false;
		}
	}

	Rectangle rewardBtn = { 30, 500, 250, 70 };
	DRAW_BUTTON(rewardBtn, "REWARDS", BLACK, WHITE);
	if (CheckCollisionPointRec(GetMousePosition(), rewardBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		if (!isLoggedIn) {
			Popup("Please sign in to view rewards!");
		} else {
			showRewardsScreen = true;
			showMyBookingsScreen = false;
			showScanRetrieval = false;
			retrievalSearched = false;
			retrievalFound = false;
			strcpy(retrievalCode, "");
			showDashboard = false;
			showBookingScreen = false;
			showCategoryBar = false;
		}
	}


	Rectangle scanBtn = { 30, 600, 250, 70 };
	DRAW_BUTTON(scanBtn, "SCAN TICKET", BLACK, WHITE);
	if (CheckCollisionPointRec(GetMousePosition(), scanBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		if (!isLoggedIn) {
			Popup("Please sign in to scan a ticket!");
		} else {
			showScanRetrieval = true;
			retrievalSearched = false;
			retrievalFound = false;
			strcpy(retrievalCode, "");
			showDashboard = false;
			showBookingScreen = false;
			showMyBookingsScreen = false;
			showRewardsScreen = false;
			showCategoryBar = false;
		}
	}

	if (!isLoggedIn) {
		Rectangle signBtn = { 30, 1000, 250, 70 };
		DRAW_BUTTON(signBtn, "SIGN-IN", BLACK, WHITE);

		if (CheckCollisionPointRec(GetMousePosition(), signBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			showSign = true;
		}
	}

	// EXIT lives in the sidebar alongside the other nav buttons, so it is
	// drawn and clickable on every screen, not just the home dashboard.
	Rectangle exitBtn = { 30, 1100, 250, 70 };
	DRAW_BUTTON(exitBtn, "EXIT", RED, WHITE);

	if (!showLogout &&
	        CheckCollisionPointRec(GetMousePosition(), exitBtn) &&
	        IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		exitProgram = true;
	}
}