#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_PATH "D:\\DOCUMENTS\\sem3\\projects and presentations\\passengers.csv"
#define MAX_RIDES 20
#define MAX_QUEUE 30
#define MAX_STOPS 15

struct Ride
{
    int id;
    char driver[40];
    char vehicle[20];
    char source[40];
    char destination[40];
    int capacity;
    int seats;
    float fare;
};

struct Passenger
{
    int id;
    char name[40];
    char phone[15];
    int rideId;
    char pickup[40];
    char drop[40];
    char status[15];
    struct Passenger *next;
};

struct Queue
{
    int data[MAX_QUEUE];
    int front;
    int rear;
};

struct Ride rides[MAX_RIDES];
int rideCount = 0;

struct Passenger *head = NULL;
struct Queue waitQ;

char stopList[MAX_STOPS][40] =
{
    "Indore Railway Station",
    "Navlakha",
    "Bapat Square",
    "Bhawarkua",
    "LIG Square",
    "Palasia",
    "Geeta Bhawan",
    "Tower Square",
    "Vijay Nagar",
    "MR 10",
    "Rau",
    "Rajendra Nagar",
    "Sudama Nagar",
    "Rajwada",
    "Bengali Square"
};

void readLine(char s[], int n);
void initQueue();
void loadRides();
void loadPassengers();
void addPassengerNode(int id, char name[], char phone[], int rideId,
                      char pickup[], char drop[], char status[]);
void updateAllSeats();

struct Ride *findRide(int id);
struct Passenger *findPassenger(int id);

void manageRides();
void addRide();
void showRides();

void managePassengers();
void addPassenger();
void showPassengers();
void completePassenger();

void bookRide();
void cancelBooking();

void waitingMenu();
void enqueue(int id);
int dequeue();
void showQueue();
void servePassenger(int rideId);

void capacityMenu();
void showRideCapacity();
void showOccupancy();
void checkBooking();

void displayStops();
int stopIndex(char point[]);
int maxOccupancy(int rideId, struct Passenger *extra);
int canBook(struct Passenger *p, int rideId);

void freePassengers();
void readLine(char s[], int n)
{
    fgets(s, n, stdin);
    s[strcspn(s, "\n")] = '\0';
}

void initQueue()
{
    waitQ.front = 0;
    waitQ.rear = -1;
}

void loadRides()
{
    rideCount = 5;

    rides[0] = (struct Ride){101, "Rahul Verma", "MP09AB1010",
                             "Indore Railway Station", "Rajwada", 3, 3, 180};

    rides[1] = (struct Ride){102, "Neha Sharma", "MP09CD2020",
                             "Bhawarkua", "Vijay Nagar", 3, 3, 160};

    rides[2] = (struct Ride){103, "Amit Jain", "MP09EF3030",
                             "Palasia", "Bengali Square", 4, 4, 150};

    rides[3] = (struct Ride){104, "Pooja Mehta", "MP09GH4040",
                             "Rau", "Sudama Nagar", 4, 4, 140};

    rides[4] = (struct Ride){105, "Karan Shah", "MP09IJ5050",
                             "Navlakha", "Rajwada", 4, 4, 170};
}

void loadPassengers()
{
    FILE *file;
    char line[250];
    char name[40], phone[15], pickup[40], drop[40], status[15];
    int id, rideId;

    file = fopen(FILE_PATH, "r");

    if (file == NULL)
    {
        printf("\nPassenger database not found.\n");
        printf("Check this path:\n%s\n", FILE_PATH);
        return;
    }

    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "%d,%39[^,],%14[^,],%d,%39[^,],%39[^,],%14[^\r\n]",
                   &id, name, phone, &rideId, pickup, drop, status) == 7)
        {
            addPassengerNode(id, name, phone, rideId,
                             pickup, drop, status);

            if (strcmp(status, "Waiting") == 0)
                enqueue(id);
        }
    }

    fclose(file);

    printf("\nPassenger database loaded successfully.\n");
}

void addPassengerNode(int id, char name[], char phone[], int rideId,
                      char pickup[], char drop[], char status[])
{
    struct Passenger *newNode;
    struct Passenger *temp;

    newNode = malloc(sizeof(struct Passenger));

    if (newNode == NULL)
        return;

    newNode->id = id;
    strcpy(newNode->name, name);
    strcpy(newNode->phone, phone);
    newNode->rideId = rideId;
    strcpy(newNode->pickup, pickup);
    strcpy(newNode->drop, drop);
    strcpy(newNode->status, status);
    newNode->next = NULL;

    if (head == NULL)
    {
        head = newNode;
    }
    else
    {
        temp = head;

        while (temp->next != NULL)
            temp = temp->next;

        temp->next = newNode;
    }
}

void updateAllSeats()
{
    int i;

    for (i = 0; i < rideCount; i++)
    {
        rides[i].seats = rides[i].capacity -
                         maxOccupancy(rides[i].id, NULL);

        if (rides[i].seats < 0)
            rides[i].seats = 0;
    }
}

struct Ride *findRide(int id)
{
    int i;

    for (i = 0; i < rideCount; i++)
    {
        if (rides[i].id == id)
            return &rides[i];
    }

    return NULL;
}

struct Passenger *findPassenger(int id)
{
    struct Passenger *temp = head;

    while (temp != NULL)
    {
        if (temp->id == id)
            return temp;

        temp = temp->next;
    }

    return NULL;
}

void manageRides()
{
    int choice;

    while (1)
    {
        printf("\n---------------- MANAGE RIDES ----------------\n");
        printf("1. Add Ride\n");
        printf("2. Display Rides\n");
        printf("3. Back\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1)
            addRide();
        else if (choice == 2)
            showRides();
        else if (choice == 3)
            return;
        else
            printf("\nInvalid choice.\n");
    }
}

void addRide()
{
    struct Ride *r;

    if (rideCount >= MAX_RIDES)
    {
        printf("\nRide limit reached.\n");
        return;
    }

    r = &rides[rideCount];

    printf("\nRide ID: ");
    scanf("%d", &r->id);
    getchar();

    printf("Driver Name: ");
    readLine(r->driver, 40);

    printf("Vehicle Number: ");
    readLine(r->vehicle, 20);

    printf("Source: ");
    readLine(r->source, 40);

    printf("Destination: ");
    readLine(r->destination, 40);

    printf("Capacity: ");
    scanf("%d", &r->capacity);

    printf("Fare: ");
    scanf("%f", &r->fare);
    getchar();

    r->seats = r->capacity;
    rideCount++;

    printf("\nRide added successfully.\n");
}

void showRides()
{
    int i;

    if (rideCount == 0)
    {
        printf("\nNo rides available.\n");
        return;
    }

    printf("\n---------------- ALL RIDES ----------------\n");

    for (i = 0; i < rideCount; i++)
    {
        printf("\nRide ID         : %d\n", rides[i].id);
        printf("Driver          : %s\n", rides[i].driver);
        printf("Vehicle         : %s\n", rides[i].vehicle);
        printf("Route           : %s -> %s\n",
               rides[i].source, rides[i].destination);
        printf("Capacity        : %d\n", rides[i].capacity);
        printf("Available Seats : %d\n", rides[i].seats);
        printf("Fare            : %.2f\n", rides[i].fare);
    }
}

void managePassengers()
{
    int choice;

    while (1)
    {
        printf("\n------------- MANAGE PASSENGERS -------------\n");
        printf("1. Add Passenger\n");
        printf("2. Display Passengers\n");
        printf("3. Mark Passenger as Completed\n");
        printf("4. Back\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1)
            addPassenger();
        else if (choice == 2)
            showPassengers();
        else if (choice == 3)
            completePassenger();
        else if (choice == 4)
            return;
        else
            printf("\nInvalid choice.\n");
    }
}

void addPassenger()
{
    struct Passenger *newNode;
    struct Passenger *temp;
    int id = 100;
    int rideId;
    int pickup;
    int drop;

    temp = head;

    while (temp != NULL)
    {
        if (temp->id > id)
            id = temp->id;

        temp = temp->next;
    }

    newNode = malloc(sizeof(struct Passenger));

    if (newNode == NULL)
    {
        printf("\nMemory allocation failed.\n");
        return;
    }

    newNode->id = id + 1;

    printf("\nGenerated Passenger ID: %d\n", newNode->id);

    printf("Name: ");
    readLine(newNode->name, 40);

    printf("Phone: ");
    readLine(newNode->phone, 15);

    printf("Ride ID: ");
    scanf("%d", &rideId);
    getchar();

    if (findRide(rideId) == NULL)
    {
        printf("\nRide not found.\n");
        free(newNode);
        return;
    }

    displayStops();

    printf("Pickup Stop Number: ");
    scanf("%d", &pickup);

    printf("Drop Stop Number: ");
    scanf("%d", &drop);
    getchar();

    if (pickup < 1 || pickup > MAX_STOPS ||
        drop < 1 || drop > MAX_STOPS ||
        pickup == drop)
    {
        printf("\nInvalid stop numbers.\n");
        free(newNode);
        return;
    }

    strcpy(newNode->pickup, stopList[pickup - 1]);
    strcpy(newNode->drop, stopList[drop - 1]);
    newNode->rideId = rideId;
    strcpy(newNode->status, "Waiting");
    newNode->next = NULL;

    if (head == NULL)
    {
        head = newNode;
    }
    else
    {
        temp = head;

        while (temp->next != NULL)
            temp = temp->next;

        temp->next = newNode;
    }

    enqueue(newNode->id);

    printf("\nPassenger added to waiting list.\n");
}

void showPassengers()
{
    struct Passenger *temp = head;

    if (temp == NULL)
    {
        printf("\nNo passengers available.\n");
        return;
    }

    printf("\n---------------- PASSENGERS ----------------\n");

    while (temp != NULL)
    {
        printf("\nPassenger ID : %d\n", temp->id);
        printf("Name         : %s\n", temp->name);
        printf("Phone        : %s\n", temp->phone);
        printf("Ride ID      : %d\n", temp->rideId);
        printf("Pickup       : %s\n", temp->pickup);
        printf("Drop         : %s\n", temp->drop);
        printf("Status       : %s\n", temp->status);

        temp = temp->next;
    }
}

void completePassenger()
{
    int id;
    struct Passenger *p;
    struct Ride *r;

    printf("\nPassenger ID: ");
    scanf("%d", &id);
    getchar();

    p = findPassenger(id);

    if (p == NULL)
    {
        printf("\nPassenger not found.\n");
        return;
    }

    printf("\nPassenger Name : %s\n", p->name);
    printf("Current Status : %s\n", p->status);

    if (strcmp(p->status, "Booked") == 0)
    {
        strcpy(p->status, "Completed");

        r = findRide(p->rideId);

        if (r != NULL)
            r->seats = r->capacity - maxOccupancy(r->id, NULL);

        printf("\nPassenger marked as completed.\n");
    }
    else if (strcmp(p->status, "Waiting") == 0)
    {
        printf("\nPassenger is waiting and has not taken the ride yet.\n");
        printf("Passenger must be booked before being marked as completed.\n");
    }
    else if (strcmp(p->status, "Completed") == 0)
    {
        printf("\nPassenger has already completed the ride.\n");
    }
    else if (strcmp(p->status, "Cancelled") == 0)
    {
        printf("\nPassenger's booking was cancelled.\n");
        printf("Cancelled passengers cannot be marked as completed.\n");
    }
    else
    {
        printf("\nPassenger cannot be marked as completed.\n");
    }
}

void bookRide()
{
    int id;
    int rideId;
    int choice;
    struct Passenger *p;
    struct Ride *r;

    printf("\nPassenger ID: ");
    scanf("%d", &id);
    getchar();

    p = findPassenger(id);

    if (p == NULL)
    {
        printf("\nPassenger not found.\n");
        return;
    }

    if (strcmp(p->status, "Booked") == 0)
    {
        printf("\nPassenger is already booked.\n");
        return;
    }

    if (strcmp(p->status, "Completed") == 0)
    {
        printf("\nCompleted passenger cannot be booked again.\n");
        return;
    }

    printf("Ride ID: ");
    scanf("%d", &rideId);
    getchar();

    r = findRide(rideId);

    if (r == NULL)
    {
        printf("\nRide not found.\n");
        return;
    }

    p->rideId = rideId;

    if (canBook(p, rideId))
    {
        strcpy(p->status, "Booked");
        r->seats = r->capacity - maxOccupancy(rideId, NULL);

        printf("\nBooking successful.\n");
        printf("Available seats: %d\n", r->seats);
    }
    else
    {
        printf("\nVehicle capacity is full for this passenger route.\n");
        printf("1. Add to Waiting Queue\n");
        printf("2. Cancel\n");
        printf("Choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1)
        {
            strcpy(p->status, "Waiting");
            enqueue(p->id);
            printf("\nPassenger added to waiting queue.\n");
        }
    }
}

void cancelBooking()
{
    int id;
    struct Passenger *p;
    struct Ride *r;

    printf("\nPassenger ID: ");
    scanf("%d", &id);
    getchar();

    p = findPassenger(id);

    if (p == NULL)
    {
        printf("\nPassenger not found.\n");
        return;
    }

    if (strcmp(p->status, "Booked") != 0)
    {
        printf("\nPassenger is not currently booked.\n");
        return;
    }

    r = findRide(p->rideId);

    strcpy(p->status, "Cancelled");

    if (r != NULL)
        r->seats = r->capacity - maxOccupancy(r->id, NULL);

    printf("\nBooking cancelled successfully.\n");

    servePassenger(p->rideId);
}

void waitingMenu()
{
    int choice;
    int rideId;

    while (1)
    {
        printf("\n--------------- WAITING LIST ---------------\n");
        printf("1. Display Waiting Queue\n");
        printf("2. Try to Book Next Passenger for a Ride\n");
        printf("3. Back\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1)
        {
            showQueue();
        }
        else if (choice == 2)
        {
            printf("\nRide ID: ");
            scanf("%d", &rideId);
            getchar();

            servePassenger(rideId);
        }
        else if (choice == 3)
        {
            return;
        }
        else
        {
            printf("\nInvalid choice.\n");
        }
    }
}

void enqueue(int id)
{
    if (waitQ.rear >= MAX_QUEUE - 1)
    {
        printf("\nWaiting queue is full.\n");
        return;
    }

    waitQ.rear++;
    waitQ.data[waitQ.rear] = id;
}

int dequeue()
{
    int id;

    if (waitQ.front > waitQ.rear)
        return -1;

    id = waitQ.data[waitQ.front];
    waitQ.front++;

    if (waitQ.front > waitQ.rear)
    {
        waitQ.front = 0;
        waitQ.rear = -1;
    }

    return id;
}

void showQueue()
{
    int i;

    if (waitQ.front > waitQ.rear)
    {
        printf("\nWaiting queue is empty.\n");
        return;
    }

    printf("\nWaiting Queue: ");

    for (i = waitQ.front; i <= waitQ.rear; i++)
    {
        printf("%d", waitQ.data[i]);

        if (i < waitQ.rear)
            printf(" -> ");
    }

    printf("\n");
}

void servePassenger(int rideId)
{
    int count;
    int i;
    int id;
    int selected = -1;
    struct Passenger *p;
    struct Ride *r;

    if (waitQ.front > waitQ.rear)
    {
        printf("\nWaiting queue is empty.\n");
        return;
    }

    r = findRide(rideId);

    if (r == NULL)
    {
        printf("\nRide not found.\n");
        return;
    }

    count = waitQ.rear - waitQ.front + 1;

    for (i = 0; i < count; i++)
    {
        id = dequeue();

        p = findPassenger(id);

        if (p != NULL &&
            strcmp(p->status, "Waiting") == 0 &&
            p->rideId == rideId &&
            selected == -1)
        {
            if (canBook(p, rideId))
                selected = id;
        }

        if (id != selected)
            enqueue(id);
    }

    if (selected == -1)
    {
        printf("\nNo waiting passenger can be booked for this ride now.\n");
        return;
    }

    p = findPassenger(selected);

    strcpy(p->status, "Booked");
    r->seats = r->capacity - maxOccupancy(rideId, NULL);

    printf("\nPassenger %d moved from waiting queue to booked.\n",
           selected);

    printf("Available seats: %d\n", r->seats);
}

void capacityMenu()
{
    int choice;

    while (1)
    {
        printf("\n------------- CAPACITY MANAGEMENT -------------\n");
        printf("1. Check Ride Capacity\n");
        printf("2. Show Occupancy at Each Stop\n");
        printf("3. Check Booking Feasibility\n");
        printf("4. Back\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1)
            showRideCapacity();
        else if (choice == 2)
            showOccupancy();
        else if (choice == 3)
            checkBooking();
        else if (choice == 4)
            return;
        else
            printf("\nInvalid choice.\n");
    }
}

void showRideCapacity()
{
    int rideId;
    struct Ride *r;

    printf("\nRide ID: ");
    scanf("%d", &rideId);
    getchar();

    r = findRide(rideId);

    if (r == NULL)
    {
        printf("\nRide not found.\n");
        return;
    }

    r->seats = r->capacity - maxOccupancy(r->id, NULL);

    printf("\nRide ID         : %d\n", r->id);
    printf("Capacity        : %d\n", r->capacity);
    printf("Maximum Occupancy: %d\n",
           maxOccupancy(r->id, NULL));
    printf("Available Seats : %d\n", r->seats);
}

void showOccupancy()
{
    int rideId;
    int events[MAX_STOPS + 1] = {0};
    int current = 0;
    int maximum = 0;
    int i;
    int start;
    int end;
    struct Passenger *p;
    struct Ride *r;

    printf("\nRide ID: ");
    scanf("%d", &rideId);
    getchar();

    r = findRide(rideId);

    if (r == NULL)
    {
        printf("\nRide not found.\n");
        return;
    }

    p = head;

    while (p != NULL)
    {
        if (strcmp(p->status, "Booked") == 0 &&
            p->rideId == rideId)
        {
            start = stopIndex(p->pickup);
            end = stopIndex(p->drop);

            if (start != -1 && end != -1)
            {
                if (start > end)
                {
                    int t = start;
                    start = end;
                    end = t;
                }

                events[start]++;
                events[end]--;
            }
        }

        p = p->next;
    }

    printf("\nStop No.   Stop                         Occupancy\n");
    printf("----------------------------------------------------\n");

    for (i = 1; i <= MAX_STOPS; i++)
    {
        current += events[i];

        if (current > maximum)
            maximum = current;

        printf("%-10d %-28s %d\n",
               i, stopList[i - 1], current);
    }

    printf("\nMaximum Occupancy: %d\n", maximum);
    printf("Vehicle Capacity : %d\n", r->capacity);
    printf("Available Seats  : %d\n",
           r->capacity - maximum);
}

void checkBooking()
{
    int id;
    struct Passenger *p;
    struct Ride *r;
    int maximum;

    printf("\nPassenger ID: ");
    scanf("%d", &id);
    getchar();

    p = findPassenger(id);

    if (p == NULL)
    {
        printf("\nPassenger not found.\n");
        return;
    }

    if (strcmp(p->status, "Booked") == 0)
    {
        printf("\nPassenger is already booked.\n");
        return;
    }

    r = findRide(p->rideId);

    if (r == NULL)
    {
        printf("\nRide not found.\n");
        return;
    }

    maximum = maxOccupancy(p->rideId, p);

    printf("\nRide Capacity: %d\n", r->capacity);
    printf("Maximum Occupancy After Booking: %d\n", maximum);

    if (maximum <= r->capacity)
        printf("Booking can be accepted.\n");
    else
        printf("Booking cannot be accepted.\n");
}

int stopIndex(char point[])
{
    int i;

    for (i = 0; i < MAX_STOPS; i++)
    {
        if (strcmp(point, stopList[i]) == 0)
            return i + 1;
    }

    return -1;
}

int maxOccupancy(int rideId, struct Passenger *extra)
{
    int events[MAX_STOPS + 1] = {0};
    struct Passenger *p = head;
    int start;
    int end;
    int i;
    int current = 0;
    int maximum = 0;

    while (p != NULL)
    {
        if (strcmp(p->status, "Booked") == 0 &&
            p->rideId == rideId)
        {
            start = stopIndex(p->pickup);
            end = stopIndex(p->drop);

            if (start != -1 && end != -1)
            {
                if (start > end)
                {
                    int t = start;
                    start = end;
                    end = t;
                }

                events[start]++;
                events[end]--;
            }
        }

        p = p->next;
    }

    if (extra != NULL)
    {
        start = stopIndex(extra->pickup);
        end = stopIndex(extra->drop);

        if (start != -1 && end != -1)
        {
            if (start > end)
            {
                int t = start;
                start = end;
                end = t;
            }

            events[start]++;
            events[end]--;
        }
    }

    for (i = 1; i <= MAX_STOPS; i++)
    {
        current += events[i];

        if (current > maximum)
            maximum = current;
    }

    return maximum;
}

int canBook(struct Passenger *p, int rideId)
{
    struct Ride *r;
    int maximum;

    r = findRide(rideId);

    if (r == NULL)
        return 0;

    if (stopIndex(p->pickup) == -1 ||
        stopIndex(p->drop) == -1)
        return 0;

    maximum = maxOccupancy(rideId, p);

    if (maximum <= r->capacity)
        return 1;

    return 0;
}

void displayStops()
{
    int i;

    printf("\nAvailable Stops\n");
    printf("-------------------------------\n");

    for (i = 0; i < MAX_STOPS; i++)
        printf("%d. %s\n", i + 1, stopList[i]);
}

void freePassengers()
{
    struct Passenger *temp;

    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}
int main()
{
    int choice;

    initQueue();
    loadRides();
    loadPassengers();
    updateAllSeats();

    while (1)
    {
        printf("\n================================================\n");
        printf("      RIDE-SHARING CAPACITY MANAGEMENT SYSTEM\n");
        printf("================================================\n");
        printf("1. Manage Rides\n");
        printf("2. Manage Passengers\n");
        printf("3. Book a Ride\n");
        printf("4. Cancel a Booking\n");
        printf("5. Waiting List\n");
        printf("6. Capacity Management\n");
        printf("7. Exit\n");
        printf("================================================\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
            case 1:
                manageRides();
                break;

            case 2:
                managePassengers();
                break;

            case 3:
                bookRide();
                break;

            case 4:
                cancelBooking();
                break;

            case 5:
                waitingMenu();
                break;

            case 6:
                capacityMenu();
                break;

            case 7:
                freePassengers();
                printf("\nProgram ended.\n");
                return 0;

            default:
                printf("\nInvalid choice.\n");
        }
    }
}
