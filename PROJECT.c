#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Constants
#define MAXSTACK 5
#define MAX_DISASTERS 100
#define MAX_NAME 50
#define MAX_DESC 200
#define MAXQUES 100

// Structures
typedef struct {
    int itemID;
    char item[100];
} Item;

typedef struct {
    int disasterID;
    char disasterName[MAX_NAME];
    char disasterDescription[MAX_DESC];
    int isSet;
} Disaster;

typedef struct {
    char stockID[10];
    char stockName[20];
    int stockQuantity;
    char stockExpiredDate[15];
    char stockCategory[20];
    char stockStatus[20];
} Stock;

// Global variables for checklist (stack)
Disaster disasters[MAX_DISASTERS];
Item stack[MAX_DISASTERS][MAXSTACK];
int top[MAX_DISASTERS];
int disasterCount = 0;

// Global variables for supplies (queue)
Stock queue[MAXQUES];
int front = 0;
int rear = MAXQUES - 1;
int count = 0;

// Function declarations
void manageChecklist();
void inputDisasterInfo();
void push();
void pop();
void display();
int getDisasterIndexByID(int id);

void trackSupplies();
void addStock();
void viewStocks();
Stock dequeue();
void enqueue(Stock nstock);

void mainmenu();

// ----------------------- MANAGE CHECKLIST -------------------------

void manageChecklist() {
    int choice1;
    for (int i = 0; i < MAX_DISASTERS; i++) top[i] = -1;

    do {
        printf("\n==================================\n");
        printf("      DISASTER CHECKLIST MENU     \n");
        printf("==================================\n");
        printf("\n[1] Enter Disaster Information");
        printf("\n[2] Add Item to Checklist");
        printf("\n[3] Remove Item from Checklist");
        printf("\n[4] Display Checklist");
        printf("\n[5] Return to Main Menu");
        printf("\nEnter Choice: ");
        scanf("%d", &choice1);

        switch (choice1) {
            case 1: inputDisasterInfo(); break;
            case 2: push(); break;
            case 3: pop(); break;
            case 4: display(); break;
            case 5: printf("\nReturning to Main Menu...\n"); break;
            default: printf("\nInvalid choice.\n");
        }
    } while (choice1 != 5);
}

void inputDisasterInfo() {
    char option;
    printf("\n[--- Enter Disaster Information ---]");

    do {
        if (disasterCount >= MAX_DISASTERS) {
            printf("\nMaximum disaster limit reached.\n");
            return;
        }

        printf("\n\nDisaster #%d", disasterCount + 1);
        printf("\nEnter Disaster ID: ");
        scanf("%d", &disasters[disasterCount].disasterID);

        printf("Enter Disaster Name: ");
        scanf(" %[^\n]", disasters[disasterCount].disasterName);

        printf("Enter Disaster Description: ");
        scanf(" %[^\n]", disasters[disasterCount].disasterDescription);

        disasters[disasterCount].isSet = 1;
        disasterCount++;

        printf("\nAdd another disaster? (Y/N): ");
        scanf(" %c", &option);
        option = toupper(option);

    } while (option == 'Y');
}

void push() {
    int id, index;
    printf("\nEnter Disaster ID to add item: ");
    scanf("%d", &id);

    index = getDisasterIndexByID(id);
    if (index == -1) {
        printf("Disaster ID not found.\n");
        return;
    }

    if (top[index] == MAXSTACK - 1) {
        printf("Checklist is full for %s.\n", disasters[index].disasterName);
    } else {
        top[index]++;
        printf("Enter Item ID: ");
        scanf("%d", &stack[index][top[index]].itemID);
        printf("Enter item name to add to %s checklist: ", disasters[index].disasterName);
        scanf(" %[^\n]", stack[index][top[index]].item);
        printf("Item added.\n");
    }
}

void pop() {
    int id, index;
    printf("\nEnter Disaster ID to remove item: ");
    scanf("%d", &id);

    index = getDisasterIndexByID(id);
    if (index == -1) {
        printf("Disaster ID not found.\n");
        return;
    }

    if (top[index] == -1) {
        printf("Checklist for %s is empty.\n", disasters[index].disasterName);
    } else {
        printf("Removed item: [ID: %d] %s\n", stack[index][top[index]].itemID, stack[index][top[index]].item);
        top[index]--;
    }
}

void display() {
    int id, index;
    printf("\nEnter Disaster ID to display checklist: ");
    scanf("%d", &id);

    index = getDisasterIndexByID(id);
    if (index == -1) {
        printf("Disaster ID not found.\n");
        return;
    }

    printf("\n--- %s ---\n", disasters[index].disasterName);
    printf("Description: %s\n", disasters[index].disasterDescription);

    if (top[index] == -1) {
        printf("Checklist is empty.\n");
    } else {
        printf("Checklist:\n");
        for (int i = top[index]; i >= 0; i--) {
            printf("%d. [ID: %d] %s\n", i + 1, stack[index][i].itemID, stack[index][i].item);
        }
    }
}

int getDisasterIndexByID(int id) {
    for (int i = 0; i < disasterCount; i++) {
        if (disasters[i].isSet && disasters[i].disasterID == id) {
            return i;
        }
    }
    return -1;
}

// --------------------- TRACK SUPPLIES -----------------------

void enqueue(Stock nstock) {
    if (count == MAXQUES) {
        printf("\n!!! Queue OVERFLOW - You can't insert into queue !!!\n");
    } else {
        rear = (rear + 1) % MAXQUES;
        queue[rear] = nstock;
        count++;
        printf("\nStock added successfully\n");
    }
}

Stock dequeue() {
    Stock empty = {"", "", 0, "", "", ""};

    if (count == 0) {
        printf("\nQueue is empty. No stock to remove.\n");
        return empty;
    }

    Stock removed = queue[front];
    front = (front + 1) % MAXQUES;
    count--;
    return removed;
}

void addStock() {
    Stock newStock;

    printf("\nEnter Stock ID: ");
    scanf(" %9s", newStock.stockID);

    int i = front;
    for (int c = 0; c < count; c++) {
        if (strcmp(queue[i].stockID, newStock.stockID) == 0) {
            printf("\nError: Stock ID already exists.\n");
            return;
        }
        i = (i + 1) % MAXQUES;
    }

    getchar();
    printf("Enter Stock Name: ");
    fgets(newStock.stockName, sizeof(newStock.stockName), stdin);
    newStock.stockName[strcspn(newStock.stockName, "\n")] = 0;

    printf("Enter Stock Quantity: ");
    scanf("%d", &newStock.stockQuantity);

    getchar();
    printf("Enter Stock Entry Date (DD-MM-YYYY): ");
    fgets(newStock.stockExpiredDate, sizeof(newStock.stockExpiredDate), stdin);
    newStock.stockExpiredDate[strcspn(newStock.stockExpiredDate, "\n")] = 0;

    printf("Enter Category (food/drink/etc): ");
    scanf("%s", newStock.stockCategory);

    getchar();
    printf("Enter Status (in stock/low/expired): ");
    fgets(newStock.stockStatus, sizeof(newStock.stockStatus), stdin);
    newStock.stockStatus[strcspn(newStock.stockStatus, "\n")] = 0;

    enqueue(newStock);
}

void viewStocks() {
    if (count == 0) {
        printf("\nNo stocks available.\n");
        return;
    }

    printf("\n----------------------------------------------------------------------------------------------\n");
    printf("| %-10s | %-20s | %-8s | %-15s | %-10s | %-12s |\n",
           "ID", "Name", "Quantity", "Expired Date", "Category", "Status");
    printf("----------------------------------------------------------------------------------------------\n");

    int i = front;
    for (int c = 0; c < count; c++) {
        printf("| %-10s | %-20s | %-8d | %-15s | %-10s | %-12s |\n",
               queue[i].stockID,
               queue[i].stockName,
               queue[i].stockQuantity,
               queue[i].stockExpiredDate,
               queue[i].stockCategory,
               queue[i].stockStatus);
        i = (i + 1) % MAXQUES;
    }

    printf("----------------------------------------------------------------------------------------------\n");
}

void trackSupplies() {
    int choice2;

    do {
        printf("\n===========================\n");
        printf("       TRACK SUPPLIES      \n");
        printf("===========================\n");
        printf("[1] Add Stock\n");
        printf("[2] View Stocks\n");
        printf("[3] Remove Oldest Stock\n");
        printf("[4] Return to Main Menu\n");
        printf("===========================\n");
        printf("Enter choice: ");
        scanf("%d", &choice2);

        switch (choice2) {
            case 1: addStock(); break;
            case 2: viewStocks(); break;
            case 3: {
                Stock removed = dequeue();
                if (strcmp(removed.stockID, "") != 0) {
                    printf("\nRemoved stock: %s (%s)\n", removed.stockID, removed.stockName);
                }
                break;
            }
            case 4: printf("\nReturning to main menu...\n"); break;
            default: printf("\nInvalid choice.\n");
        }
    } while (choice2 != 4);
}

// ---------------------EMERGENCY CONTACT -----------------
// Structure for Personal Emergency Contact
struct EmergencyContact {
    char id[20];
    char name[50];
    char phone[20];
    char relationship[30];
    struct EmergencyContact* next;
};

struct EmergencyContact* head = NULL;

// Structure for Authority Emergency Contact
struct AuthorityContact {
    char authorityName[50];
    char phone[20];
    int disasterID; // Changed from char[] to int
    struct AuthorityContact* next;
};

struct AuthorityContact* authorityHead = NULL;

// Function to add Personal Emergency Contact
void addPersonalContact() {
    struct EmergencyContact* newContact = (struct EmergencyContact*)malloc(sizeof(struct EmergencyContact));
    if (!newContact) {
        printf("Memory allocation failed.\n");
        return;
    }

    printf("\n--- Add Personal Emergency Contact ---\n");
    printf("Enter ID: ");
    scanf(" %[^\n]", newContact->id);
    printf("Enter Name: ");
    scanf(" %[^\n]", newContact->name);
    printf("Enter Phone Number: ");
    scanf(" %[^\n]", newContact->phone);
    printf("Enter Relationship: ");
    scanf(" %[^\n]", newContact->relationship);

    newContact->next = head;
    head = newContact;

    printf("✅ Personal contact added successfully.\n");
}

// Function to add Authority Emergency Contact
void addAuthorityContact() {
    struct AuthorityContact* newAuthority = (struct AuthorityContact*)malloc(sizeof(struct AuthorityContact));
    if (!newAuthority) {
        printf("Memory allocation failed.\n");
        return;
    }

    printf("\n--- Add Authority Contact ---\n");
    printf("Enter Authority Name: ");
    scanf(" %[^\n]", newAuthority->authorityName);
    printf("Enter Phone Number: ");
    scanf(" %[^\n]", newAuthority->phone);
    printf("Enter Disaster ID (integer): ");
    scanf("%d", &newAuthority->disasterID); // Now accepting int

    newAuthority->next = authorityHead;
    authorityHead = newAuthority;

    printf("✅ Authority contact added successfully.\n");
}

// Function to display all Personal Emergency Contacts
void viewPersonalContacts() {
    if (head == NULL) {
        printf("\nNo personal emergency contacts found.\n");
        return;
    }

    printf("\n--- List of Personal Emergency Contacts ---\n");
    struct EmergencyContact* temp = head;
    while (temp != NULL) {
        printf("ID: %s\nName: %s\nPhone: %s\nRelationship: %s\n-------------------------\n",
               temp->id, temp->name, temp->phone, temp->relationship);
        temp = temp->next;
    }
}

// Function to display all Authority Emergency Contacts
void viewAuthorityContacts() {
    if (authorityHead == NULL) {
        printf("\nNo authority contacts found.\n");
        return;
    }

    printf("\n--- List of Authority Contacts ---\n");
    struct AuthorityContact* temp = authorityHead;
    while (temp != NULL) {
        printf("Authority Name: %s\nPhone: %s\nDisaster ID: %d\n-------------------------\n",
               temp->authorityName, temp->phone, temp->disasterID);
        temp = temp->next;
    }
}

// Emergency Contact Menu
void emergencyContactMenu() {
    int choice3;

    do {
        printf("\n===============================\n");
        printf("        EMERGENCY CONTACTS      \n");
        printf("===============================\n");
        printf("[1] Add Personal Contact\n");
        printf("[2] Add Authority Contact\n");
        printf("[3] View Personal Contacts\n");
        printf("[4] View Authority Contacts\n");
        printf("[5] Return to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice3);

        switch (choice3) {
            case 1:
                addPersonalContact();
                break;
            case 2:
                addAuthorityContact();
                break;
            case 3:
                viewPersonalContacts();
                break;
            case 4:
                viewAuthorityContacts();
                break;
            case 5:
                printf("\nExiting Emergency Contact Menu...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (choice3 != 5);
}

//-----------sorting ---------------------------------------

void sortDisastersByID() {
    if (disasterCount == 0) {
        printf("\nThe file is empty. No disaster data available.\n");
        return;
    }
    for (int i = 0; i < disasterCount - 1; i++) {
        for (int j = 0; j < disasterCount - i - 1; j++) {
            if (disasters[j].disasterID > disasters[j + 1].disasterID) {
                // Swap disasters
                Disaster temp = disasters[j];
                disasters[j] = disasters[j + 1];
                disasters[j + 1] = temp;

                // Swap corresponding stack
                Item tempStack[MAXSTACK];
                memcpy(tempStack, stack[j], sizeof(tempStack));
                memcpy(stack[j], stack[j + 1], sizeof(tempStack));
                memcpy(stack[j + 1], tempStack, sizeof(tempStack));

                // Swap top index
                int tempTop = top[j];
                top[j] = top[j + 1];
                top[j + 1] = tempTop;
            }
        }
    }

    printf("\n Disasters sorted by Disaster ID.\n");

    // Display all sorted disasters
    for (int i = 0; i < disasterCount; i++) {
        printf("\nDisaster ID: %d\nName: %s\nDescription: %s\n",
               disasters[i].disasterID,
               disasters[i].disasterName,
               disasters[i].disasterDescription);
    }
}

void sortItemsByID() {
    if (disasterCount == 0) {
        printf("\nThe file is empty. No disaster data available.\n");
        return;
    }
    for (int d = 0; d < disasterCount; d++) {
        for (int i = 0; i < top[d]; i++) {
            for (int j = 0; j < top[d] - i; j++) {
                if (stack[d][j].itemID > stack[d][j + 1].itemID) {
                    Item temp = stack[d][j];
                    stack[d][j] = stack[d][j + 1];
                    stack[d][j + 1] = temp;
                }
            }
        }
    }

    printf("\n Items sorted by Item ID (per disaster).\n");

    // Display checklist for each disaster
    for (int d = 0; d < disasterCount; d++) {
        printf("\n--- Checklist for %s (Disaster ID: %d) ---\n",
               disasters[d].disasterName,
               disasters[d].disasterID);

        if (top[d] == -1) {
            printf("Checklist is empty.\n");
        } else {
            for (int i = 0; i <= top[d]; i++) {
                printf("   %d. [ID: %d] %s\n", i + 1, stack[d][i].itemID, stack[d][i].item);
            }
        }
    }
}

void sortMenu() {
    int choice4;

    do{
        printf("\n==================================\n");
        printf("           SORT MENU            \n");
        printf("================================\n");
        printf("\n[1] Sort Disasters by Disaster ID");
        printf("\n[2] Sort Items by Item ID (per Disaster)");
        printf("\n[3] Return to Main Menu");
        printf("\nEnter your choice: ");
        scanf("%d", &choice4);

        switch (choice4) {
            case 1:
                sortDisastersByID();  // will also display disasters
                break;
            case 2:
                sortItemsByID();  // will also display items
                break;
            case 3:
                printf("\nReturning to Main Menu...\n");
                break;
            default:
                printf("\n Invalid choice.\n");
        }

    }while (choice4 != 3);
}

// ======================= REVIEW EMERGENCY  ========================

void reviewEmergency() {
    int id, found = 0;
    printf("\n======================================\n");
    printf("          REVIEW EMERGENCY            \n");
    printf("======================================\n");
    printf("Enter Disaster ID: ");
    scanf("%d", &id);

    // Search for matching disaster
    for (int i = 0; i < disasterCount; i++) {
        if (disasters[i].isSet && disasters[i].disasterID == id) {
            found = 1;

            // Display Disaster Info
            printf("\n========== DISASTER INFORMATION ==========\n");
            printf("Disaster ID      : %d\n", disasters[i].disasterID);
            printf("Disaster Name    : %s\n", disasters[i].disasterName);
            printf("Description      : %s\n", disasters[i].disasterDescription);

            // Display Authority Contacts
            printf("\n========== AUTHORITY CONTACT(S) ==========\n");
            struct AuthorityContact* temp = authorityHead;
            int contactFound = 0;

            while (temp != NULL) {
                if (temp->disasterID == id) {
                    printf("Authority Name   : %s\n", temp->authorityName);
                    printf("Phone Number     : %s\n", temp->phone);
                    printf("--------------------------------------\n");
                    contactFound++;
                }
                temp = temp->next;
            }

            if (contactFound == 0) {
                printf("No authority contact found for this disaster.\n");
            } else {
                printf("Total authority contact(s) found: %d\n", contactFound);
            }

            break;
        }
    }

    if (!found) {
        printf("\n❌ Disaster ID not found. Please try again.\n");
    }

    printf("\nReturning to main menu...\n");
}

// ---------------------- Main Menu -----------------------

void mainmenu() {
    int choice;

    do {
        printf("\n============================\n");
        printf("         MAIN MENU          \n");
        printf("============================\n");
        printf("[1] MANAGE CHECKLIST\n");
        printf("[2] TRACK SUPPLIES\n");
        printf("[3] EMERGENCY CONTACT\n");
        printf("[4] DISASTER INFORMATION\n");
        printf("[5] REVIEW EMERGENCY\n");
        printf("[6] EXIT\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: manageChecklist(); break;
            case 2: trackSupplies(); break;
            case 3: emergencyContactMenu(); break;
            case 4: sortMenu(); break;
            case 5: reviewEmergency(); break;
            case 6:
                printf("\nExiting program. Goodbye!\n");
                break;
            default:
                printf("\nInvalid choice.\n");
        }
    } while (choice != 6);
}

// ---------------------- Main -----------------------

int main() {
    mainmenu();
    return 0;
}
