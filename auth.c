#include "raylib.h"
#include <stdio.h>
#include "function.h"
#include <string.h>
#include <ctype.h>

//------------------- Hover Button -------------------
void HoverButton(float x, float y, float w, float h, const char *text, Color buttonColor, Color textColor) {
	Rectangle bounds = { x, y, w, h };
	bool hover = CheckCollisionPointRec(GetMousePosition(), bounds);
	Color borderColor = DARKGRAY;

	if (hover) {
		buttonColor = ColorBrightness(buttonColor, 0.25f);
		borderColor = WHITE;
		bounds.x -= 2;
		bounds.y -= 2;
		bounds.width += 4;
		bounds.height += 4;
	}

	DrawRectangleRounded(bounds, 0.25f, 12, buttonColor);
	DrawRectangleRoundedLines(bounds, 0.25f, 12, borderColor);

	DrawLineEx((Vector2) {
		bounds.x + 8, bounds.y + 3
	},
	(Vector2) {
		bounds.x + bounds.width - 8, bounds.y + 3
	},
	2, Fade(WHITE, 0.45f));

	int fontSize = 22;
	int textWidth = MeasureText(text, fontSize);
	int textX = (int)(bounds.x + (bounds.width - textWidth) / 2);
	int textY = (int)(bounds.y + (bounds.height - fontSize) / 2);

	DrawText(text, textX, textY, fontSize, textColor);
}

//------------------- Popup -------------------
void Popup(const char *message) {
	static float timer = 0;
	static char text[128] = "";

	if (message != NULL) {
		strncpy(text, message, sizeof(text) - 1);
		text[sizeof(text) - 1] = '\0';
		timer = 2.5f;
	}

	if (timer <= 0) return;

	timer -= GetFrameTime();
	DrawRectangle(660, 20, 600, 60, LIGHTGRAY);
	int textWidth = MeasureText(text, 25);
	DrawText(text, 660 + (600 - textWidth) / 2, 30, 25, BLACK);
}

//------------------- Text Input Box -------------------
void TextInputBox(int id, Rectangle box, char *buf, int maxLen, bool pass) {
	int len = strlen(buf);

	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), box)) {
		activeBox = id;
	}

	if (activeBox == id) {
		int key = GetCharPressed();
		while (key > 0) {
			if (key >= 32 && key <= 125 && len < maxLen) {
				buf[len++] = (char)key;
			}
			key = GetCharPressed();
		}
		buf[len] = '\0';

		if (IsKeyPressed(KEY_BACKSPACE) && len > 0) {
			buf[len - 1] = '\0';
		}
	}

	DrawRectangleRounded(box, 0.15f, 10, RAYWHITE);
	DrawRectangleRoundedLines(box, 0.15f, 10, activeBox == id ? SKYBLUE : LIGHTGRAY);

	char shown[64];
	strncpy(shown, buf, sizeof(shown) - 1);
	shown[sizeof(shown) - 1] = '\0';

	if (pass) {
		for (int i = 0; shown[i]; i++) shown[i] = '*';
	}

	DrawText(shown, box.x + 15, box.y + box.height / 2 - 10, 22, BLACK);

	if (activeBox == id) {
		DrawText("|", box.x + 25 + MeasureText(shown, 22), box.y + box.height / 2 - 10, 22, BLACK);
	}
}

//------------------- Helper -------------------
void toLower(char *str) {
	for (int i = 0; str[i] != '\0'; i++) {
		str[i] = tolower(str[i]);
	}
}

//------------------- Get User Total Tickets -------------------
int getUserTotalTickets(int *outEarnedFreeTickets, int *outProgressToNext) {
	FILE *fp = fopen(tic, "rb");
	int totalBookingsMade = 0;

	if (fp != NULL) {
		TicketDetail t;
		while (fread(&t, sizeof(TicketDetail), 1, fp) == 1) {
			if (strcmp(t.userName, loginUsername) == 0) {
				// Rewards count how many separate bookings were made, not
				// how many seats each booking had.
				totalBookingsMade++;
			}
		}
		fclose(fp);
	}

	*outEarnedFreeTickets = totalBookingsMade / REWARD_THRESHOLD;
	*outProgressToNext = totalBookingsMade % REWARD_THRESHOLD;
	return totalBookingsMade;
}

//------------------- Count User Tickets -------------------
int countUserTickets(void) {
	FILE *fp = fopen(tic, "rb");
	int count = 0;

	if (fp != NULL) {
		TicketDetail t;
		while (fread(&t, sizeof(TicketDetail), 1, fp) == 1) {
			if (strcmp(t.userName, loginUsername) == 0) count++;
		}
		fclose(fp);
	}

	return count;
}

//------------------- Delete Ticket -------------------
void deleteTicket(int recordIndex) {
	FILE *fp = fopen(tic, "rb");
	if (fp == NULL) return;

	FILE *temp = fopen(tictemp, "wb");
	if (temp == NULL) {
		fclose(fp);
		return;
	}

	TicketDetail t;
	int i = 0;
	while (fread(&t, sizeof(TicketDetail), 1, fp) == 1) {
		if (i != recordIndex) {
			fwrite(&t, sizeof(TicketDetail), 1, temp);
		}
		i++;
	}
	fclose(fp);
	fclose(temp);

	remove(tic);
	rename(tictemp, tic);
	Popup("Ticket deleted successfully!");
}

//------------------- Register Logic -------------------
void regFunct(void) {
	newUsers us;
	FILE *fp;

	if (strlen(regUsername) == 0 && strlen(regEmail) == 0 && strlen(regPassword) == 0 && strlen(regConfirmPassword) == 0) {
		Popup("You cannot leave all fields empty!");
		return;
	}
	if (strlen(regUsername) == 0) {
		Popup("Username cannot be empty!");
		return;
	}

	bool validUsername = true;
	for (int i = 0; regUsername[i] != '\0'; i++) {
		if (!isalpha((unsigned char)regUsername[i]) && regUsername[i] != ' ') {
			validUsername = false;
			break;
		}
	}
	if (!validUsername) {
		Popup("Username must contain only letters!");
		return;
	}

	if (strlen(regEmail) == 0) {
		Popup("Email cannot be empty!");
		return;
	}

	int len = strlen(regEmail);
	int atCount = 0;
	int atPos = -1;
	int lastDot = -1;

	for (int i = 0; i < len; i++) {


		if (regEmail[i] == ' ') {
			Popup("Email cannot contain spaces!");
			return;
		}


		if (regEmail[i] == '@') {
			atCount++;
			atPos = i;
		}


		if (regEmail[i] == '.') {
			lastDot = i;
		}
	}


	if (atCount != 1) {
		Popup("Invalid email!");
		return;
	}


	if (regEmail[0] == '@' || regEmail[0] == '.') {
		Popup("Invalid email!");
		return;
	}


	if (regEmail[len - 1] == '@' || regEmail[len - 1] == '.') {
		Popup("Invalid email!");
		return;
	}


	if (atPos <= 0) {
		Popup("Invalid email!");
		return;
	}


	if (lastDot < atPos + 2) {
		Popup("Invalid email!");
		return;
	}

	if (!isalpha((unsigned char)regEmail[0])) {
		Popup("Email must start with a letter!");
		return;
	}

	if (lastDot >= len - 2) {
		Popup("Invalid email!");
		return;
	}


	if (regEmail[atPos - 1] == '.' || regEmail[atPos + 1] == '.') {
		Popup("Invalid email!");
		return;
	}

	if (strlen(regPassword) == 0) {
		Popup("Password cannot be empty!");
		return;
	}
	if (strlen(regPassword) < 6) {
		Popup("Password must be at least 6 characters!");
		return;
	}
	if (strcmp(regPassword, regConfirmPassword) != 0) {
		Popup("Passwords do not match!");
		return;
	}

	char newUsername[64], newEmail[64];
	strcpy(newUsername, regUsername);
	strcpy(newEmail, regEmail);
	toLower(newUsername);
	toLower(newEmail);

	fp = fopen(dat, "rb");
	if (fp != NULL) {
		newUsers existing;
		while (fread(&existing, sizeof(newUsers), 1, fp) == 1) {
			char savedUsername[64], savedEmail[64];
			strcpy(savedUsername, existing.username);
			strcpy(savedEmail, existing.email);
			toLower(savedUsername);
			toLower(savedEmail);

			if (strcmp(savedEmail, newEmail) == 0) {
				Popup("Email already registered!");
				fclose(fp);
				return;
			}
			if (strcmp(savedUsername, newUsername) == 0) {
				Popup("Username already taken!");
				fclose(fp);
				return;
			}
		}
		fclose(fp);
	}

	fp = fopen(dat, "ab");
	if (fp == NULL) {
		Popup("File could not be opened!");
		return;
	}

	strcpy(us.username, regUsername);
	strcpy(us.email, regEmail);
	strcpy(us.password, regPassword);

	if (fwrite(&us, sizeof(newUsers), 1, fp) == 1) {
		Popup("Registration Successful!");
		strcpy(regUsername, "");
		strcpy(regEmail, "");
		strcpy(regPassword, "");
		strcpy(regConfirmPassword, "");
		showReg = false;
		showSign = true;
	} else {
		Popup("Registration Failed!");
	}
	fclose(fp);
}

//------------------- Login Logic -------------------
void loginFunct(void) {
	newUsers fileUser;
	Userlog existingUser;
	FILE *fp;
	int found = 0;
	char inputUsername[64], inputPassword[64], savedUsername[64];

	if (strlen(loginUsername) == 0 && strlen(loginPassword) == 0) {
		Popup("Please enter your username and password!");
		return;
	}
	if (strlen(loginUsername) == 0) {
		Popup("Username cannot be empty!");
		return;
	}
	if (strlen(loginPassword) == 0) {
		Popup("Password cannot be empty!");
		return;
	}

	strcpy(inputUsername, loginUsername);
	strcpy(inputPassword, loginPassword);
	toLower(inputUsername);

	fp = fopen(dat, "rb");
	if (fp == NULL) {
		Popup("No registered users found!");
		return;
	}

	while (fread(&fileUser, sizeof(newUsers), 1, fp) == 1) {
		strcpy(savedUsername, fileUser.username);
		toLower(savedUsername);
		if (strcmp(savedUsername, inputUsername) == 0 && strcmp(fileUser.password, inputPassword) == 0) {
			found = 1;
			strcpy(existingUser.username, fileUser.username);
			strcpy(existingUser.password, fileUser.password);
			break;
		}
	}
	fclose(fp);

	if (found) {
		strcpy(loginUsername, existingUser.username);
		char message[100];
		snprintf(message, sizeof(message), "Login Successful! Welcome, %s", loginUsername);
		Popup(message);
		isLoggedIn = true;
		showSign = false;
		showReg = false;
		showLogout = false;
		showDashboard = true;

		if (pendingBooking) {
			showBookingScreen = true;
			pendingBooking = false;
		}
	} else {
		Popup("Wrong username or password!");
		strcpy(loginPassword, "");
	}
}

//------------------- Login Card -------------------
void loginCard(void) {
	if (!showSign) return;

	float cardW = 500, cardH = 550;
	float cardX = (GetScreenWidth() - cardW) / 2.0f;
	float cardY = (GetScreenHeight() - cardH) / 2.0f;

	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.45f));
	DrawRectangleRounded((Rectangle) {
		cardX, cardY, cardW, cardH
	}, 0.08f, 10, WHITE);
	DrawRectangleRoundedLines((Rectangle) {
		cardX, cardY, cardW, cardH
	}, 0.08f, 10, LIGHTGRAY);

	DrawText("Sign In", cardX + 50, cardY + 30, 40, BLACK);
	DrawText("Welcome back to CineApp", cardX + 50, cardY + 80, 22, DARKGRAY);

	DrawText("Username", cardX + 50, cardY + 140, 22, BLACK);
	TextInputBox(1, (Rectangle) {
		cardX + 50, cardY + 170, 400, 55
	}, loginUsername, 63, false);

	DrawText("Password", cardX + 50, cardY + 250, 22, BLACK);
	TextInputBox(2, (Rectangle) {
		cardX + 50, cardY + 280, 400, 55
	}, loginPassword, 63, true);

	Rectangle closeBtn = { cardX + 440, cardY + 20, 30, 30 };
	DrawText("x", cardX + 448, cardY + 22, 44, DARKGRAY);

	if (CheckCollisionPointRec(GetMousePosition(), closeBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		showSign = false;
	}

	Rectangle registerBtn = { cardX + 50, cardY + 450, 190, 45 };
	Rectangle loginBtn = { cardX + 260, cardY + 450, 190, 45 };

	DRAW_BUTTON(registerBtn, "REGISTER", BLACK, WHITE);
	if (CheckCollisionPointRec(GetMousePosition(), registerBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		showSign = false;
		showReg = true;
	}

	DRAW_BUTTON(loginBtn, "LOGIN", BLACK, WHITE);
	if (CheckCollisionPointRec(GetMousePosition(), loginBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		loginFunct();
	}
}

//------------------- Register Card -------------------
void registerCard(void) {
	if (!showReg) return;

	float cardW = 500, cardH = 680;
	float cardX = (GetScreenWidth() - cardW) / 2.0f;
	float cardY = (GetScreenHeight() - cardH) / 2.0f;

	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.45f));
	DrawRectangleRounded((Rectangle) {
		cardX + 8, cardY + 8, cardW, cardH
	}, 0.08f, 10, Fade(BLACK, 0.15f));
	DrawRectangleRounded((Rectangle) {
		cardX, cardY, cardW, cardH
	}, 0.08f, 10, WHITE);
	DrawRectangleRoundedLines((Rectangle) {
		cardX, cardY, cardW, cardH
	}, 0.08f, 10, LIGHTGRAY);

	Rectangle closeBtn = { cardX + 445, cardY + 15, 25, 25 };
	DrawText("x", cardX + 450, cardY + 17, 40, DARKGRAY);

	if (CheckCollisionPointRec(GetMousePosition(), closeBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		showReg = false;
	}

	DrawText("Register", cardX + 50, cardY + 30, 40, BLACK);
	DrawText("Create your CineApp account", cardX + 50, cardY + 75, 22, DARKGRAY);

	DrawText("Username", cardX + 50, cardY + 130, 22, BLACK);
	TextInputBox(3, (Rectangle) {
		cardX + 50, cardY + 160, 400, 50
	}, regUsername, 63, false);

	DrawText("Email", cardX + 50, cardY + 230, 22, BLACK);
	TextInputBox(4, (Rectangle) {
		cardX + 50, cardY + 260, 400, 50
	}, regEmail, 63, false);

	DrawText("Password", cardX + 50, cardY + 330, 22, BLACK);
	TextInputBox(5, (Rectangle) {
		cardX + 50, cardY + 360, 400, 50
	}, regPassword, 63, true);

	DrawText("Confirm Password", cardX + 50, cardY + 430, 22, BLACK);
	TextInputBox(6, (Rectangle) {
		cardX + 50, cardY + 460, 400, 50
	}, regConfirmPassword, 63, true);

	Rectangle regBtn = { cardX + 50, cardY + 540, 400, 50 };
	DRAW_BUTTON(regBtn, "REGISTER", BLACK, WHITE);
	if (CheckCollisionPointRec(GetMousePosition(), regBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		regFunct();
	}

	Rectangle backBtn = { cardX + 50, cardY + 610, 400, 45 };
	DRAW_BUTTON(backBtn, "BACK TO LOGIN", BLACK, WHITE);
	if (CheckCollisionPointRec(GetMousePosition(), backBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		showReg = false;
		showSign = true;
	}
}

//------------------- Save / Load Bookings -------------------
void saveBookings(void) {
	FILE *fp = fopen(book, "wb");
	if (fp == NULL) {
		Popup("Could not save bookings!");
		return;
	}
	fwrite(bookedSeats, sizeof(bookedSeats), 1, fp);
	fclose(fp);
}

void loadBookings(void) {
	FILE *fp = fopen(book, "rb");
	if (fp == NULL) return;
	fread(bookedSeats, sizeof(bookedSeats), 1, fp);
	fclose(fp);
}

//------------------- Avatar / Logout -------------------
void avatar(void) {
	Rectangle ava = {30, 1000, 250, 70};
	DRAW_BUTTON(ava, loginUsername, BLACK, WHITE);
	DrawCircle(ava.x + ava.width - 20, ava.y + 35, 10, GREEN);

	if (CheckCollisionPointRec(GetMousePosition(), ava) &&
	        IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		showLogout = !showLogout;
	}

	if (showLogout) {
		Rectangle logoutBtn = {300, 1010, 250, 50};
		DRAW_BUTTON(logoutBtn, "Logout", RED, WHITE);

		if (CheckCollisionPointRec(GetMousePosition(), logoutBtn) &&
		        IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			isLoggedIn = false;
			showSign = false;
			showReg = false;
			showLogout = false;

			strcpy(loginUsername, "");
			strcpy(loginPassword, "");

			Popup("Logout Successful! See you again.");
		}
	}
}

//------------------- Category Bar -------------------
void categoryBar(void) {
	if (!showCategoryBar) return;

	DrawText("Browse Movies:", 350, 300, 40, BLACK);
	const char *labels[] = { "ALL", "ACTION", "COMEDY", "HORROR", "ROMANCE", "SCI-FI" };
	const char *genres[] = { "ALL", "Action", "Comedy", "Horror", "Romance", "Sci-Fi" };

	float x = 700, y = 305, gap = 15;

	for (int i = 0; i < 6; i++) {
		float width = MeasureText(labels[i], 22) + 40;
		Rectangle btn = { x, y, width, 40 };

		DRAW_BUTTON(btn, labels[i], BLACK, WHITE);
		if (CheckCollisionPointRec(GetMousePosition(), btn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			strcpy(selectedGenre, genres[i]);
		}
		x += width + gap;
	}
}

//------------------- Reset Seats -------------------
void resetSelectedSeats(void) {
	for (int row = 0; row < SEAT_ROWS; row++) {
		for (int col = 0; col < SEAT_COLS; col++) {
			selectedSeats[row][col] = false;
			seatTicketType[row][col] = TICKET_ADULT;
		}
	}
	selectedSeatsCount = 0;
}

//------------------- Movie Card -------------------
void movieCard(float x, float y, Movie *m, int index) {
	float w = 380, h = 180;
	Rectangle card = { x, y, w, h };
	DrawRectangleRounded(card, 0.10f, 10, WHITE);
	DrawRectangleRoundedLines(card, 0.10f, 10, LIGHTGRAY);

	DrawText(m->name, x + 15, y + 12, 28, BLACK);
	DrawLine(x + 15, y + 55, x + w - 15, y + 55, LIGHTGRAY);
	DrawText("Time:", x + 15, y + 70, 20, DARKGRAY);
	DrawText(m->time, x + 85, y + 70, 20, BLACK);
	DrawText("Genre:", x + 15, y + 100, 20, DARKGRAY);
	DrawText(m->genre, x + 85, y + 100, 20, BLACK);
	DrawText("Rating:", x + 15, y + 130, 20, DARKGRAY);

	char rateText[16];
	sprintf(rateText, "%.1f/10", m->rating);
	DrawText(rateText, x + 85, y + 130, 20, GOLD);

	Rectangle bookBtn = { x + w - 95, y + h - 40, 80, 28 };
	DRAW_BUTTON(bookBtn, "BOOK", BLACK, WHITE);

	if (CheckCollisionPointRec(GetMousePosition(), bookBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		if (!isLoggedIn) {
			Popup("Please sign in first to book!");
		} else {
			selectedMovie = *m;
			selectedMovieIndex = index;
			resetSelectedSeats();
			showBookingScreen = true;
			showCategoryBar = false;
		}
	}
}

//------------------- Count Seats -------------------
int countSelectedSeats(void) {
	int count = 0;
	for (int row = 0; row < SEAT_ROWS; row++) {
		for (int col = 0; col < SEAT_COLS; col++) {
			if (selectedSeats[row][col]) count++;
		}
	}
	return count;
}

//------------------- Draw One Seat -------------------
void drawOneSeat(int row, int col, float x, float y, float size) {
	Rectangle seat = { x, y, size, size };
	Color color = LIGHTGRAY;

	if (bookedSeats[selectedMovieIndex][row][col]) color = RED;
	else if (selectedSeats[row][col]) color = SKYBLUE;

	DrawRectangleRounded(seat, 0.3f, 6, color);
	DrawRectangleRoundedLines(seat, 0.3f, 6, DARKGRAY);

	char seatNumber[4];
	sprintf(seatNumber, "%d", col + 1);
	DrawText(seatNumber, x + 15, y + 14, 16, BLACK);

	if (!showDetailsForm && !bookedSeats[selectedMovieIndex][row][col] &&
	        CheckCollisionPointRec(GetMousePosition(), seat) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		if (selectedSeats[row][col]) {
			selectedSeats[row][col] = false;
			seatTicketType[row][col] = TICKET_ADULT;
		} else {
			if (countSelectedSeats() < 5) {
				selectedSeats[row][col] = true;
				seatTicketType[row][col] = TICKET_ADULT;
			} else Popup("Maximum limit of 5 seats reached!");
		}
	}
}

//------------------- Draw Seat Grid -------------------
void drawSeatGrid(float startX, float startY) {
	float seatSize = 45, gap = 12;
	for (int row = 0; row < SEAT_ROWS; row++) {
		char rowLabel[2] = { (char)('A' + row), '\0' };
		float rowY = startY + row * (seatSize + gap);
		DrawText(rowLabel, startX - 30, rowY + 10, 20, BLACK);

		for (int col = 0; col < SEAT_COLS; col++) {
			drawOneSeat(row, col, startX + col * (seatSize + gap), rowY, seatSize);
		}
	}
}

//------------------- Draw Seat Legend -------------------
void drawSeatLegend(float x, float y) {
	DrawRectangleRounded((Rectangle) {
		x, y, 25, 25
	}, 0.3f, 6, LIGHTGRAY);
	DrawText("Available", x + 35, y + 3, 18, BLACK);
	DrawRectangleRounded((Rectangle) {
		x + 180, y, 25, 25
	}, 0.3f, 6, SKYBLUE);
	DrawText("Selected", x + 215, y + 3, 18, BLACK);
	DrawRectangleRounded((Rectangle) {
		x + 360, y, 25, 25
	}, 0.3f, 6, RED);
	DrawText("Booked", x + 395, y + 3, 18, BLACK);
}

//------------------- Draw Details Form -------------------
void drawDetailsForm(int selectedCount) {
	float cardW = 500, cardH = 450;
	float cardX = (GetScreenWidth() - cardW) / 2.0f;
	float cardY = (GetScreenHeight() - cardH) / 2.0f;

	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.45f));
	DrawRectangleRounded((Rectangle) {
		cardX, cardY, cardW, cardH
	}, 0.08f, 10, WHITE);
	DrawRectangleRoundedLines((Rectangle) {
		cardX, cardY, cardW, cardH
	}, 0.08f, 10, LIGHTGRAY);

	DrawText("Viewer Details", cardX + 50, cardY + 30, 32, BLACK);
	DrawText("Full Name", cardX + 50, cardY + 90, 18, DARKGRAY);
	TextInputBox(7, (Rectangle) {
		cardX + 50, cardY + 115, 400, 50
	}, formName, 63, false);

	DrawText("Phone Number", cardX + 50, cardY + 190, 18, DARKGRAY);
	TextInputBox(8, (Rectangle) {
		cardX + 50, cardY + 215, 400, 50
	}, formPhone, 10, false);

	Rectangle closeBtn = { cardX + 50, cardY + 320, 180, 50 };
	Rectangle submitBtn = { cardX + 270, cardY + 320, 180, 50 };

	DRAW_BUTTON(closeBtn, "CANCEL", BLACK, WHITE);
	DRAW_BUTTON(submitBtn, "SUBMIT", BLACK, WHITE);

	if (CheckCollisionPointRec(GetMousePosition(), closeBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		showDetailsForm = false;
	}

	if (CheckCollisionPointRec(GetMousePosition(), submitBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		if (strlen(formName) == 0) {
			Popup("Name cannot be empty!");
			return;
		}
		if (strlen(formPhone) != 10) {
			Popup("Phone must be 10 digits!");
			return;
		}
		for (int i = 0; formPhone[i] != '\0'; i++) {
			if (!isdigit((unsigned char)formPhone[i])) {
				Popup("Phone must contain only digits!");
				return;
			}
		}


		// Viewer details are valid - hand off to the simulated payment
		// layer. The ticket file isn't written until payment "succeeds"
		// (see finalizeBookingAfterPayment in kiosk_sim.c).
		showDetailsForm = false;
		showPaymentScreen = true;
		paymentMethodSelected = -1;

	}
}

//------------------- Main Booking Screen -------------------
void seatBooking(void) {
	ClearBackground(WHITE);

	float startX = 715, startY = 480, legendY = startY + 470;

	DrawText("Select Your Seats", 650, 320, 40, BLACK);
	DrawText(TextFormat("Movie: %s", selectedMovie.name), 650, 250, 24, DARKGRAY);
	DrawText(TextFormat("Time: %s", selectedMovie.time), 650, 280, 24, DARKGRAY);

	drawSeatGrid(startX, startY);
	drawSeatLegend(startX, legendY);
	drawTicketTypePanel(startX + 640, startY);

	int selectedCount = countSelectedSeats();
	int freeCredits = 0, progress = 0;
	getUserTotalTickets(&freeCredits, &progress);

	float totalCost = getSelectedSeatsSubtotal() + getSelectedSeatsTax();
	bool isFreeTicketApplied = (freeCredits > 0);
	if (isFreeTicketApplied) totalCost = 0.00f;

	DrawText(TextFormat("Seats Selected: %d / 5", selectedCount), startX, legendY + 50, 22, BLACK);
	if (isFreeTicketApplied) DrawText(TextFormat("Total Price: RS: 0.00 (%dx Reward Applied!)", REWARD_THRESHOLD), startX, legendY + 80, 22, GREEN);
	else DrawText(TextFormat("Total Price: RS: %.2f", totalCost), startX, legendY + 80, 22, MAROON);

	Rectangle backBtn = { startX, legendY + 130, 180, 50 };
	Rectangle confirmBtn = { startX + 200, legendY + 130, 220, 50 };

	DRAW_BUTTON(backBtn, "BACK", BLACK, WHITE);
	DRAW_BUTTON(confirmBtn, "CONFIRM BOOKING", BLACK, WHITE);

	// Only respond to these clicks when the details form isn't open on top
	// of them - otherwise a tap inside the form was also being read as a
	// tap on whatever seat/button sits underneath it on screen.
	if (!showDetailsForm && CheckCollisionPointRec(GetMousePosition(), backBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		resetSelectedSeats();
		showBookingScreen = false;
		showCategoryBar = true;
	}
	if (!showDetailsForm && CheckCollisionPointRec(GetMousePosition(), confirmBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		if (selectedCount == 0) Popup("Please select at least one seat!");
		else {
			showDetailsForm = true;
			strcpy(formName, "");
			strcpy(formPhone, "");
			activeBox = 7;
		}
	}
	if (showDetailsForm) drawDetailsForm(selectedCount);
}

//------------------- Draw My Bookings -------------------
void drawMyBookings(void) {
	FILE *fp = fopen(tic, "rb");
	if (fp == NULL) {
		DrawText("No bookings found yet!", 350, 300, 24, DARKGRAY);
		return;
	}

	TicketDetail t;
	int index = 0;
	int recordIndex = 0;
	float startX = 350, startY = 300, rowW = 1100, rowH = 70, gap = 15;
	DrawText("Your Booking History", startX, 240, 36, BLACK);

	while (fread(&t, sizeof(TicketDetail), 1, fp) == 1) {
		int currentRecord = recordIndex;
		recordIndex++;

		if (strcmp(t.userName, loginUsername) != 0) continue;

		float y = startY + index * (rowH + gap);
		Rectangle row = { startX, y, rowW, rowH };
		DrawRectangleRounded(row, 0.08f, 4, WHITE);
		DrawRectangleRoundedLines(row, 0.08f, 4, LIGHTGRAY);

		DrawText(t.ticketNumber, startX + 20, y + 24, 20, ORANGE);
		DrawText(t.passengerName, startX + 190, y + 24, 20, BLACK);
		DrawText(t.movieName, startX + 400, y + 24, 20, DARKGRAY);
		DrawText(t.movieTime, startX + 680, y + 24, 20, DARKGRAY);
		DrawText(TextFormat("RS: %.2f", t.totalPrice), startX + 830, y + 24, 20, MAROON);

		Rectangle deleteBtn = { startX + rowW - 100, y + 12, 80, 46 };
		DRAW_BUTTON(deleteBtn, "DELETE", RED, WHITE);
		if (CheckCollisionPointRec(GetMousePosition(), deleteBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			fclose(fp);
			deleteTicket(currentRecord);
			return;
		}

		index++;
	}

	if (index == 0) {
		DrawText("No bookings yet.", startX, startY, 22, DARKGRAY);
	}

	fclose(fp);
}

//------------------- Rewards Screen -------------------
void drawRewardsScreen(void) {
	int freeCredits = 0, progress = 0;
	int total = getUserTotalTickets(&freeCredits, &progress);
	int needed = REWARD_THRESHOLD - progress;
	float startX = 350, startY = 240, cardW = 700, cardH = 350;

	DrawText("Your Loyalty Rewards", startX, startY, 36, BLACK);
	Rectangle card = { startX, startY + 60, cardW, cardH };
	DrawRectangleRounded(card, 0.05f, 8, WHITE);
	DrawRectangleRoundedLines(card, 0.05f, 8, LIGHTGRAY);

	DrawText(TextFormat("Total Bookings Made: %d", total), startX + 40, startY + 100, 24, DARKGRAY);

	if (freeCredits > 0) {
		DrawRectangleRounded((Rectangle) {
			startX + 40, startY + 140, 420, 40
		}, 0.2f, 6, Fade(GREEN, 0.15f));
		DrawText(TextFormat(" You have %d FREE TICKET(S) available!", freeCredits), startX + 50, startY + 148, 20, GREEN);
	} else {
		DrawText(TextFormat("Make %d bookings total to get your next ticket 100%% free!", REWARD_THRESHOLD), startX + 40, startY + 150, 20, MAROON);
	}

	float barX = startX + 40, barY = startY + 230, barWidth = 620, barHeight = 35;
	DrawText(TextFormat("Progress to Next Free Ticket: %d / %d", progress, REWARD_THRESHOLD), barX, barY - 30, 20, BLACK);
	DrawRectangleRounded((Rectangle) {
		barX, barY, barWidth, barHeight
	}, 0.5f, 6, LIGHTGRAY);

	float fillWidth = (progress / (float)REWARD_THRESHOLD) * barWidth;
	if (fillWidth > 0) {
		if (fillWidth < 20) fillWidth = 20;
		DrawRectangleRounded((Rectangle) {
			barX, barY, fillWidth, barHeight
		}, 0.5f, 6, SKYBLUE);
	}

	if (progress == 0 && freeCredits == 0) {
		DrawText("Start booking to earn your first milestone rewards!", barX, barY + 50, 18, DARKGRAY);
	} else {
		DrawText(TextFormat("%d more bookings needed to cash out your next freebie reward credit.", needed), barX, barY + 50, 18, DARKGRAY);
	}
}