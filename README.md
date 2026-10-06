# 🚗 Ride-Sharing Capacity Management System

A **C-based Data Structures project** that manages rides, passengers, bookings, cancellations, waiting lists, and vehicle capacity.

The system is designed to demonstrate how **Arrays, Linked Lists, and Queues** can be combined to solve a practical ride-sharing management problem.

## 📌 Problem Statement

In a ride-sharing system, multiple passengers may travel in the same vehicle but may have different pickup and drop-off points. Simply checking the total number of booked passengers is not enough.

For example, a vehicle with a capacity of 4 may carry:

- Passenger 1: Stop 1 → Stop 5
- Passenger 2: Stop 2 → Stop 4
- Passenger 3: Stop 5 → Stop 8

The system must calculate the **maximum number of passengers inside the vehicle at any point of the route** before accepting a new booking.

This project solves this problem using a **route-based capacity management algorithm**.

## 🎯 Objectives

- Manage multiple rides and their capacities
- Add and manage passengers
- Book and cancel rides
- Maintain a waiting list using Queue
- Calculate maximum occupancy at different stops
- Check whether a new passenger can be booked
- Update available seats dynamically
- Demonstrate practical applications of Data Structures

## 🧠 Data Structures Used

### 1. Array

Arrays are used to store:

- Ride information
- Waiting queue
- Available route stops
- Occupancy events

The project supports a maximum of **20 rides, 30 waiting passengers, and 15 stops**. 

### 2. Linked List

A singly linked list is used to maintain the passenger database.

Each passenger node stores:

- Passenger ID
- Name
- Phone number
- Ride ID
- Pickup location
- Drop location
- Booking status
- Pointer to the next passenger

This allows passengers to be dynamically added to the system.

### 3. Queue

A queue is used to manage passengers who cannot currently be accommodated.

It follows the **FIFO (First In, First Out)** principle.

Main queue operations:

```text
enqueue()
dequeue()
showQueue()
```

### 4. Route-Based Capacity Algorithm

The project represents each passenger's journey as:

```text
Pickup Stop → Drop Stop
```

For every booking:

```text
Pickup  → +1 passenger
Drop    → -1 passenger
```

A running total is calculated across the route to determine the **maximum occupancy**.

A new booking is accepted only when:

```text
Maximum Occupancy ≤ Vehicle Capacity
```

The implementation performs this calculation through `maxOccupancy()` and checks booking feasibility through `canBook()`.

## 🚘 Main Features

### Manage Rides

Users can:

- Add a new ride
- Display all rides
- View driver and vehicle information
- View route
- View vehicle capacity
- View available seats
- View fare

### Manage Passengers

Users can:

- Add passengers
- Display passenger details
- Mark passengers as completed

Passenger information is maintained using a linked list.

### Book a Ride

The system:

1. Searches for the passenger
2. Finds the selected ride
3. Checks route validity
4. Calculates maximum occupancy
5. Accepts or rejects the booking

If the vehicle is full for that passenger's route, the passenger can be placed into the waiting queue.

### Cancel Booking

A booked passenger can cancel their ride.

After cancellation:

- Passenger status becomes `Cancelled`
- Available seats are recalculated
- The next eligible passenger in the waiting queue can be considered for booking

### Waiting List

The waiting list manages passengers who could not be booked immediately.

Example:

```text
Passenger 105 → Passenger 108 → Passenger 112
```

When space becomes available, the system searches the queue and attempts to book an eligible passenger.

### Capacity Management

The system provides:

- Ride capacity checking
- Maximum occupancy calculation
- Occupancy at each stop
- Booking feasibility checking

The occupancy display shows how the number of passengers changes across the route.

## 📍 Sample Stops

The system currently contains 15 predefined stops, including:

```text
1. Indore Railway Station
2. Navlakha
3. Bapat Square
4. Bhawarkua
5. LIG Square
6. Palasia
7. Geeta Bhawan
8. Tower Square
9. Vijay Nagar
10. MR 10
11. Rau
12. Rajendra Nagar
13. Sudama Nagar
14. Rajwada
15. Bengali Square
```

These stops are used to determine passenger routes and calculate occupancy.

## 🏗️ System Flow

```text
                    ┌─────────────────────┐
                    │       MAIN MENU     │
                    └──────────┬──────────┘
                               │
        ┌──────────────────────┼──────────────────────┐
        │                      │                      │
        ▼                      ▼                      ▼
   Manage Rides         Manage Passengers       Book Ride
        │                      │                      │
        │                      │                      ▼
        │                      │              Capacity Check
        │                      │                      │
        │                      │          ┌───────────┴───────────┐
        │                      │          │                       │
        │                      │       Available               Full
        │                      │          │                       │
        │                      │          ▼                       ▼
        │                      │       Booked               Waiting Queue
        │                      │
        └──────────────────────┼───────────────────────────────┐
                               │                               │
                               ▼                               ▼
                         Cancel Booking                   Waiting List
                               │                               │
                               └──────────► Capacity ◄─────────┘
```

## 📊 Booking Logic

Suppose the vehicle capacity is:

```text
Capacity = 4
```

Current passengers:

```text
Passenger A : Stop 1 → Stop 5
Passenger B : Stop 2 → Stop 6
Passenger C : Stop 3 → Stop 4
```

Occupancy becomes:

```text
Stop 1 : 1
Stop 2 : 2
Stop 3 : 3
Stop 4 : 2
Stop 5 : 1
Stop 6 : 0
```

Maximum occupancy:

```text
3
```

Since:

```text
3 ≤ 4
```

another suitable passenger may be accepted depending on their pickup and drop-off points.

## 🖥️ Main Menu

```text
================================================
      RIDE-SHARING CAPACITY MANAGEMENT SYSTEM
================================================
1. Manage Rides
2. Manage Passengers
3. Book a Ride
4. Cancel a Booking
5. Waiting List
6. Capacity Management
7. Exit
================================================
```

## 📁 Project Structure

```text
Ride-Sharing-Capacity-Management-System/
│
├── dsa_casestudy.c
├── passengers.csv
└── README.md
```

The program loads passenger information from a CSV file during startup.

> **Note:** Update the `FILE_PATH` in the C program according to the location of your `passengers.csv` file before running it.

## ⚙️ Technologies Used

- **Language:** C
- **Compiler:** GCC / MinGW
- **Concepts:** Data Structures and Algorithms
- **Data Structures:** Array, Linked List, Queue
- **File Handling:** CSV file
- **Memory Management:** Dynamic memory allocation

## ▶️ How to Run

### Clone the Repository

```bash
git clone https://github.com/your-username/ride-sharing-capacity-management.git
```

### Navigate to the Project

```bash
cd ride-sharing-capacity-management
```

### Compile

```bash
gcc dsa_casestudy.c -o ride_system
```

### Run

```bash
./ride_system
```

For Windows:

```bash
ride_system.exe
```

## 🔄 Passenger Status

Passengers can have the following statuses:

```text
Waiting
Booked
Completed
Cancelled
```

These statuses are used to control booking, cancellation, completion, and waiting-list operations.

## ⏱️ Complexity Overview

| Operation | Data Structure | Approx. Complexity |
|---|---|---:|
| Find Ride | Array | O(n) |
| Find Passenger | Linked List | O(n) |
| Add Passenger | Linked List | O(n) |
| Enqueue | Queue | O(1) |
| Dequeue | Queue | O(1) |
| Capacity Check | Route Events | O(n) |

Here, `n` represents the relevant number of rides, passengers, or stops.

## 🎓 DSA Concepts Demonstrated

This project demonstrates practical implementation of:

- Structures
- Arrays
- Singly Linked Lists
- Queues
- FIFO principle
- Dynamic memory allocation
- Searching
- File handling
- Route-based occupancy calculation
- Conditional booking
- Capacity management

## 🚀 Future Improvements

Possible extensions include:

- Persistent ride database using files
- Graph-based route management
- Priority-based waiting lists
- User authentication
- Real-time ride tracking
- Fare calculation based on distance
- GUI/web interface
- Circular queue implementation

## 👩‍💻 Academic Project

**Project:** Ride-Sharing Capacity Management System  
**Subject:** Data Structures and Algorithms  
**Language:** C

---

⭐ **A practical DSA implementation combining Queue, Linked List, Array, and route-based capacity management.**
