# 🎬 Cinema Booking System (CBS)

A desktop-based **Cinema Booking System** developed in **C using Raylib** as a **First Semester Project**. The application provides a modern graphical user interface for user authentication, movie browsing, seat booking, ticket management, and loyalty rewards using binary file handling.

---

# 📂 Project Structure

```text
CBS/
│
├── database/
│   ├── bookings.bin
│   ├── userdata.bin
│   └── userticketdetail.bin
│
├── include/
│   └── raylib.h
│
├── lib/
│   └── libraylib.a
│
├── auth.c
├── constant.c
├── constant.h
├── function.c
├── function.h
├── index.c
├── logic.c
├── main.c
│
├── CBS.dev
├── Makefile.win
├── .gitignore
└── README.md
```

---

# ✨ Features

- 🔐 User Registration
- 🔑 User Login Authentication
- 📧 Email Validation
- 🔒 Password Validation
- 🎬 Browse Movies
- 🎭 Movie Category Filtering
- 💺 Interactive Seat Selection
- 🚫 Booked Seat Detection
- 🎟 Ticket Booking
- 🆔 Automatic Ticket ID Generation
- 📋 My Bookings Management
- 🗑 Delete Booked Tickets
- 🎁 Loyalty Reward System
- 💾 Binary File Storage
- 🖥 Graphical User Interface using Raylib

---

# 📸 Application Screenshots

## Login Screen

![Login](ss/screenshot001.png)

---

## Registration Screen

![Registration](ss/screenshot000.png)

---

## Home Screen

![Home](ss/screenshot005.png)

---

## Seat Booking

![Seat Booking](ss/screenshot002.png)

---

## My Bookings

![My Bookings](ss/screenshot003.png)

---

## Loyalty Rewards

![Rewards](ss/screenshot004.png)

---

# 🛠 Technologies Used

- **Programming Language:** C
- **Graphics Library:** Raylib
- **IDE:** Dev-C++ / VS Code
- **Compiler:** MinGW GCC
- **Database:** Binary Files (.bin)
- **Build Tool:** Makefile

---

# 💾 Database

The application stores all data using binary files.

```text
database/
├── userdata.bin
├── bookings.bin
└── userticketdetail.bin
```

### userdata.bin

Stores:

- Username
- Email
- Password

### bookings.bin

Stores:

- Movie Name
- Seat Number
- Ticket Price
- Ticket ID

### userticketdetail.bin

Stores:

- User Information
- Phone Number
- Booking Details
- Selected Seats

---

# 📦 Project Modules

## 🔐 Authentication Module

- User Registration
- User Login
- Email Validation
- Password Confirmation
- Duplicate User Checking

---

## 🎬 Movie Module

- Browse Movies
- Category Filtering
- Movie Selection
- Movie Information

---

## 💺 Seat Booking Module

- Interactive Seat Grid
- Available Seat Detection
- Prevent Duplicate Booking
- Automatic Ticket Generation

---

## 🎟 Booking Management Module

- View Booked Tickets
- Delete Tickets
- Booking History
- Binary File Update

---

## 🎁 Loyalty Rewards Module

- Ticket Purchase Counter
- Reward Progress
- Free Ticket Eligibility
- Progress Bar

---

# ⚙️ How to Build

### Using Dev-C++

Open

```text
CBS.dev
```

Compile and Run the project.

### Using Makefile

```bash
make
```

or

```bash
mingw32-make
```

---

# 📚 Dependencies

- Raylib
- MinGW GCC Compiler
- Windows Operating System

---

# 🌟 Project Highlights

- Modern GUI using Raylib
- Complete User Authentication System
- Binary File Database
- Interactive Seat Reservation
- Ticket Management
- Loyalty Reward System
- Modular Programming
- File Handling
- Event Handling
- Input Validation

---

# 📖 Learning Outcomes

This project demonstrates practical implementation of:

- C Programming
- Structures
- Functions
- Modular Programming
- Header Files
- Binary File Handling
- Authentication System
- GUI Programming
- Event Handling
- Data Validation
- Dynamic User Interface
- Software Design

---

# 🚀 Future Improvements

- Online Payment Gateway
- QR Code Ticket Generation
- Admin Dashboard
- Movie Poster Images
- Search & Filter Movies
- Email Ticket Confirmation
- Password Encryption
- Online Database Integration
- Multiple Cinema Halls
- Responsive UI

---

# 📄 License

This project was developed as a **First Semester Project** for educational purposes.

© 2026 Cinema Booking System.
