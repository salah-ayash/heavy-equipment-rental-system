<div align="center">

# 🚜 Heavy Equipment Rental Management System

### C++ & MySQL | Equipment Management · Rental Workflows · Financial Reporting

A console-based application for managing **heavy equipment rentals**, customer and staff operations, equipment availability, and rental revenue analysis.

![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![MySQL](https://img.shields.io/badge/MySQL-4479A1?style=for-the-badge&logo=mysql&logoColor=white)
![Visual Studio](https://img.shields.io/badge/Visual%20Studio-5C2D91?style=for-the-badge&logo=visualstudio&logoColor=white)
![Windows](https://img.shields.io/badge/Platform-Windows-172554?style=for-the-badge)

[**Explore Source Code**](https://github.com/salah804/heavy-equipment-rental-system) · [**Rental Module**](Heavy%20equipment%20Rental%20Sys/rent.cpp) · [**Reporting Module**](Heavy%20equipment%20Rental%20Sys/report.cpp)

</div>

---

## 📖 Project Overview

**Heavy Equipment Rental Management System** is an academic Windows console application developed using **C++**, **object-oriented programming**, and **MySQL**. It organizes core rental business tasks with separate **Customer** and **Staff** workflows, persistent equipment and rental records, approval/cancellation functionality, and SQL-based analysis.

> **Note:** This repository contains the **original C++ console project**. Any modern interactive web demonstration shown on my developer portfolio is a separate simulation, not the original application's interface.

## ✨ Core Features

| Module | Features |
| --- | --- |
| 🔐 Accounts | User registration, login, and customer/staff roles |
| 🚜 Inventory | Equipment models, types, rental rates, and availability |
| 📅 Rentals | Requests, rental dates, durations, and totals |
| ✅ Staff Review | Approval and cancellation of rental requests |
| 📋 Rental History | Browse linked customer, equipment, and rental records |
| 📊 Operational Analytics | Customer, equipment type, and equipment status analysis |
| 💰 Financial Reports | Total rental revenue, monthly and yearly reports, custom periods |

### Rental Workflow

```text
Customer selects equipment
           |
           v
Rental request created
           |
           v
    Not Approved Yet
        /       \
       v         v
   Approved    Canceled
       |
       v
Equipment status updated
```

The original code uses rental statuses **`Not Approved Yet`**, **`Approved`**, and **`Canceled`**, alongside equipment statuses such as **`Available`**, **`Reserved`**, and **`Rented`**.

## 📈 Reporting & Analytics

The [`report.cpp`](Heavy%20equipment%20Rental%20Sys/report.cpp) module includes:

- **General analysis:** customer rental activity, equipment categories, and equipment status counts.
- **Total rental revenue:** aggregated rental totals grouped by equipment model.
- **Monthly reports:** approved rental revenue by month and equipment model, plus custom month selection.
- **Yearly reports:** approved rental revenue by year and equipment model, plus custom year selection.

The SQL reporting logic uses **joins**, **`SUM()`**, **date grouping**, and status filters. Monetary amounts are displayed in **MYR (Malaysian Ringgit)**.

> **Financial accuracy:** The available C++ implementation reports **rental revenue**, not audited net profit. Calculating profit requires accounting for expenses and operating costs. Any graphical revenue/profit figures in the separate portfolio demo are illustrative sample data.

## 🛠️ Tech Stack

| Category | Technology |
| --- | --- |
| Language | C++ |
| Programming approach | Object-oriented programming |
| Database | MySQL |
| Database interface | MySQL C API (`mysql.h`) |
| IDE | Microsoft Visual Studio |
| Platform | Windows console |
| Reporting | SQL joins, aggregations, and date filtering |

## 🗂️ Project Structure

```text
heavy-equipment-rental-system/
├── .gitignore
├── README.md
├── Heavy equipment Rental Sys.sln
└── Heavy equipment Rental Sys/
    ├── Heavy equipment Rental Sys.vcxproj
    ├── main.cpp
    ├── connection.cpp / connection.h
    ├── user.cpp / user.h
    ├── customer.cpp / customer.h
    ├── staff.cpp / staff.h
    ├── equipment.cpp / equipment.h
    ├── equipment_type.cpp / equipment_type.h
    ├── rent.cpp / rent.h
    └── report.cpp / report.h
```

### Database Tables

The source references these relational MySQL tables:

`user` · `customer` · `equipment_type` · `equipment` · `rental` · `rental_details`

**The repository does not currently include a complete database initialization script**, so an existing compatible database is required to run the original application.

## 🚀 Getting Started

### Requirements

- Windows and **Microsoft Visual Studio** with **Desktop development with C++**
- A running **MySQL Server** and its C client development headers/libraries
- An existing **`heavy_equipment`** database with the expected tables
- MySQL include and linker directories configured in the Visual Studio project for your installation

> The project's Visual Studio file contains machine-specific MySQL paths. You may need to update these for your computer. Compiled DLLs and the third-party MySQL header are not committed to this repository.

### 1. Clone the repository

```bash
git clone https://github.com/salah804/heavy-equipment-rental-system.git
```

Open `Heavy equipment Rental Sys.sln` in Visual Studio.

### 2. Configure the MySQL connection

The application currently expects:

| Setting | Value |
| --- | --- |
| Host | `localhost` |
| Port | `3306` |
| Username | `root` |
| Database | `heavy_equipment` |
| Password | Windows environment variable `RENTAL_DB_PASSWORD` |

On Windows, open **System Properties → Advanced → Environment Variables**, create a **User variable** named `RENTAL_DB_PASSWORD`, and set its value to your *local* MySQL password. Restart Visual Studio afterward.

**Never commit a real password, database dump with private data, or local credentials.**

### 3. Build and run

1. Select an appropriate **x64** configuration in Visual Studio.
2. Check the MySQL header and library paths in project properties.
3. Choose **Build → Rebuild Solution**.
4. Run with **Ctrl + F5** and connect to your local test database.

A fresh clone is **not plug-and-play** until the MySQL dependencies and expected database schema are available.

## 🔒 Security & Project Limitations

This repository is an **academic project**, not a production-ready rental service.

- The current connection code reads its MySQL password from a **Windows environment variable**.
- Account authentication and some SQL operations use **legacy query construction and plaintext password comparisons**. Production use would require **prepared statements, password hashing, input validation, least-privilege credentials**, and security review.
- Earlier commits may retain credentials that were later removed. **Rotate any password ever committed to the repository**; editing the latest file does not erase Git history.
- Use a test database only; do not expose personal customer data.

## 🎯 Skills Demonstrated

C++ · Object-Oriented Programming · MySQL · Relational Database Design · SQL Joins · Business Logic · User Roles · Rental Status Management · Data Reporting · Debugging

## 👨‍💻 Author

**Salah Ayash**  
Computer Science Graduate · Universiti Teknikal Malaysia Melaka (UTeM)

[**GitHub Profile**](https://github.com/salah-ayash)

---

<div align="center">

**Developed with C++ and MySQL · Academic Software Engineering Project**

</div>
