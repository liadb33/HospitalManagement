# Hospital Management System in C

## Project Overview

This Hospital Management System, developed in C, manages various hospital operations such as handling departments, doctors, patients, and treatments. The project is designed with modularity in mind, utilizing a menu-driven command-line interface and offering both text and binary file storage for data persistence.

## Features

- **Hospital Operations**: 
  - Manage departments, doctors, patients, and treatments.
- **Data Persistence**: 
  - Supports both text (`hospital_data.txt`) and binary (`hospital_data.bin`) file formats.
- **Dynamic Memory Management**: 
  - Uses dynamic memory allocation to efficiently manage data.
- **Error Handling and Input Validation**: 
  - Ensures system robustness by validating user inputs and handling errors.
- **Sorting and Searching**: 
  - Provides sorting and searching functionalities for patient records.
- **Updating Doctor Specialties**: 
  - Allows modification of doctor specialties.
- **Hospital Data Validation**: 
  - Verifies the integrity and consistency of hospital data.
- **User-Friendly Menu Interface**: 
  - Offers an intuitive menu for easy navigation and operation.

## Setup Instructions

### Prerequisites

- A C compiler such as GCC.
- Basic knowledge of C programming.
- A terminal or command-line interface to run the compiled program.

### Menu Options

When the program starts, you will be prompted to initialize the hospital data in one of three ways:

1. Load data from a text file.
2. Load data from a binary file.
3. Manually enter hospital data.

After initialization, the following menu options are available:

- Print hospital data.
- Print all doctors in the hospital.
- Add a new department.
- Add a new doctor.
- Add a new treatment.
- Add a new patient.
- Sort patient records.
- Search for a patient.
- Replace a doctor's specialty.
- Validate the integrity of doctor experience with treatments.

### Data Persistence

Upon exit, the program saves the hospital data to both a text file and a binary file:

- **Text File**: `hospital_data.txt`
- **Binary File**: `hospital_data.bin`

This ensures that data can be reloaded when the program is restarted.


