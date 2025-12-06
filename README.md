# Address Book GUI (C++ / wxWidgets)

A simple address book application written in **C++** with **wxWidgets**.  
It lets you manage a list of employees and stores data in a CSV file.

## Features

- Add / update / remove employees
- Store basic information:
  - Name
  - Age
  - Address
  - Phone number
  - Job title
  - Salary (string, e.g. `10.000.000`, `15000000`, etc.)
- Show all employees in a separate table window
- Filters:
  - **Filter Age** – filter by age range
  - **Filter Title** – filter by job title  
    (normalized, case-insensitive, handles `_`, spaces, etc.)
  - **Filter Salary** – filter by salary range  
    (strips dots/commas/letters, only keeps digits)
- Automatically:
  - **Load** data from `employees.csv` when the app starts
  - **Save** data to `employees.csv` when the app closes

## Project structure (main files)

```
.
├─ address_book.h        // dataEmployee struct and PeopleBook class declarations
├─ run_backend.cpp       // PeopleBook implementation: logic, filters, CSV I/O
├─ main_frame.h          // MainFrame and ShowAllFrame (GUI) declarations
├─ main_frame.cpp        // GUI implementation: buttons Add/Update/Remove/Show/Filter
├─ main.cpp              // Entry point, starts wxWidgets app and MainFrame
└─ employees.csv         // Data file for storing employees (auto-created)

```
Requirements:
1/ Visual Studio with C++ Desktop workload

2/ wxWidgets (built and configured with your project)

Build instructions (short version)

Install and build wxWidgets (debug/release, static/dynamic – up to you).

Create a C++ project (Console / Empty Project) in Visual Studio.

Add the .cpp and .h files from this repo to the project.

Configure:

C/C++ → Additional Include Directories: point to wxWidgets include folder.

Linker → Additional Library Directories: point to the built wxWidgets lib folder.

Linker → Input → Additional Dependencies: add required wxWidgets libs
(for example wxmsw32u_core.lib, wxbase32u.lib, etc. depending on your build).

Build and run.

Note: if employees.csv does not exist on first run, the app starts with an empty list.
After you add / edit / delete data and close the app, employees.csv is written automatically.

Usage

Start the program. You will see a form with the fields: Name, Age, Address, Phone, Title, Salary

Fill in all fields and press Add to create a new employee.

To update:

Enter the same Name as an existing record and the new data → press Update.

To remove:

Enter the Name of the employee → press Remove.

Press Show All to open a window listing all employees.

Filter Age / Title / Salary:

Click the corresponding filter button and enter the requested values.
The app opens a new window showing only the matching employees.
