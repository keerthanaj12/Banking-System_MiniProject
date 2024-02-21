// Bank-account program reads a random-access file sequentially,
// updates data already written to the file, creates new data to
// be placed in the file, and deletes data previously in the file.
#include <stdio.h>
#include <stdlib.h>
// clientData structure definition
struct clientData {
  unsigned int acctNum; // account number
  char lastName[15];    // account last name
  char firstName[10];   // account first name
  double balance;       // account balance
  int pin;
}; // end structure clientData

// prototypes
unsigned int enterChoice(void);
void textFile(FILE *fPtr);
void updateRecord(FILE *fPtr, FILE *rPtr);
void newRecord(FILE *fPtr, FILE *rPtr);
void deleteRecord(FILE *fPtr, FILE *rPtr);
int main(int argc, char *argv[]) {
  FILE *cfPtr;             // credit.dat file pointer
  unsigned int choice, ch; // user's choice
  printf("********************   WELCOME TO AXIS BANK   ********************");
  // fopen opens the file; exits if file cannot be opened
  if ((cfPtr = fopen("credit.dat", "rb+")) == NULL) {
    printf("%s: File could not be opened.\n", argv[0]);
    exit(-1);
  }

  // enable user to specify action
  while ((choice = enterChoice()) != 6) {
    switch (choice) {
    // create text file from record file
    case 0:
      textFile(cfPtr);
      break;
    // update record
    case 1:
      updateRecord(cfPtr, cfPtr);
      break;
    case 2:
      newRecord(cfPtr, cfPtr);
      break;
    // create record
    case 3:
      deleteRecord(cfPtr, cfPtr);
      break;
    // delete existing record
    case 4:
      return -1;

    default:
      puts("Incorrect choice");
      break;
    } // end switch
    printf("\nDo you want to continue?\nYes Press(1)\nExit Press(2)\n?");
    scanf("%d", &ch);
    if (ch == 1) {
      continue;
    } else {
      break;
    }
  } // end while

  fclose(cfPtr); // fclose closes the file
} // end main

// create formatted text file for printing
void textFile(FILE *rPtr) {
  FILE *wPtr; // accounts.txt file pointer
  int res;     // used to test whether fread read any bytes
  // create clientData with default information
  struct clientData client = {0, "", "", 0.0};

  // fopen opens the file; exits if file cannot be opened
  if ((wPtr = fopen("accounts.txt", "w")) == NULL) {
    puts("File could not be opened.");
  } // end if
  else {
    rewind(rPtr); // sets pointer to beginning of file
    fprintf(wPtr, "%-30s%-20s%-20s%20s\n", "Acct", "First Name",
            "Last Name", "Balance");

    // copy all records from random-access file into text file
    while (!feof(rPtr)) {
      res = fread(&client, sizeof(struct clientData), 1, rPtr);

      // write single record to text file
      if (res != 0 && client.acctNum != 0) {
        fprintf(wPtr, "%-30d%-20s%-20s%20.2f\n", client.acctNum,
                client.firstName, client.lastName, client.balance);
      } // end if
    }   // end while

    fclose(wPtr); // fclose closes the file
  }                   // end else
} // end function textFile

void updateRecord(FILE *fPtr, FILE *rPtr) {
  unsigned int account, cpin, ch; // account number
  double transaction;             // transaction amount
  // create clientData with no information
  struct clientData client = {0, "", "", 0.0};
  FILE *wPtr; // accounts.txt file pointer
  int result;     // used to test whether fread read any bytes
  // create clientData with default information

  // obtain number of account to update
  printf("%s", "Enter account number to update ( 1-100 ): ");
  scanf("%d", &account);

  // move file pointer to correct record in file
  fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
  // read record from file
  fread(&client, sizeof(struct clientData), 1, fPtr);
  // display error if account does not exist
  if (client.acctNum == 0) {
    printf("Account #%d has no information.\n", account);
  } else { // update record
    printf("Enter 4 Digit valid pin:");
    scanf("%d", &cpin);
    if (cpin != client.pin) {
      printf("You have entered a Invalid Pin");

    } else {
      printf("%-6d%-16s%-11s%10.2f\n\n", client.acctNum, client.firstName,
             client.lastName, client.balance);

      // request transaction amount from user
      printf("%s",
             "Press(1) for Deposit Amount\nPress(2) for withdraw Amount\n");
      scanf("%d", &ch);
      if (ch == 1) {
        printf("Enter Amount :");
        scanf("%lf", &transaction);
        client.balance += transaction; // update record balance
      } else {
        if (client.balance <= 1000) {
          printf("You have insufficient account balance\n");
        } else {
          printf("Enter Amount :");
          scanf("%lf", &transaction);
          client.balance -= transaction;
        }
      }
      printf("%-6d%-16s%-11s%10.2f\n", client.acctNum, client.firstName,
             client.lastName, client.balance);

      // move file pointer to correct record in file
      // move back by 1 record length
      fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);
      // write updated record over old record in file
      fwrite(&client, sizeof(struct clientData), 1, fPtr);
      if ((wPtr = fopen("accounts.txt", "w")) == NULL) {
        puts("File could not be opened.");
      } // end if
      else {
        rewind(rPtr); // sets pointer to beginning of file
        fprintf(wPtr, "%-30s%-20s%-20s%20s\n", "Acct", "First Name",
                "Last Name", "Balance");

        // copy all records from random-access file into text file
        while (!feof(rPtr)) {
          result = fread(&client, sizeof(struct clientData), 1, rPtr);

          // write single record to text file
          if (result != 0 && client.acctNum != 0) {
            fprintf(wPtr, "%-30d%-20s%-20s%20.2f\n", client.acctNum,
                    client.firstName, client.lastName, client.balance);
          } // end if
        }   // end while

        fclose(wPtr); // fclose closes the file
      }
    }
  } // end else
} // end function updateRecord

// delete an existing record
void deleteRecord(FILE *fPtr, FILE *rPtr) {
  struct clientData client; // stores record read from file
  struct clientData blankClient = {0, "", "", 0}; // blank client
  unsigned int accountNum, ch, cpin, confirmpin;  // account number

  FILE *wPtr; // accounts.txt file pointer
  int result;     // used to test whether fread read any bytes
  // create clientData with default information
  // obtain number of account to delete
  printf("%s", "Enter account number to delete ( 1-100):\n");
  scanf("%d", &accountNum);

  // move file pointer to correct record in file
  fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
  // read record from file
  fread(&client, sizeof(struct clientData), 1, fPtr);
  // display error if record does not exist
  if (client.acctNum == 0) {
    printf("Account %d does not exist.\n", accountNum);
  } // end if
  else {
    printf("Enter Your 4 digit pin:\n");
    scanf("%d", &confirmpin);
    if (confirmpin != client.pin) {
      printf("You have Entered an invalid Pin\n");
    } else {
      printf("%-6d%-16s%-11s%10.2f\n", client.acctNum, client.firstName,
             client.lastName, client.balance);
      printf("Do you want to delete your account?\n[1]Yes\n[0]EXIT\n");
      scanf("%d", &ch);
      if (ch == 1) {
        printf("Enter your PIN to confirm: \n");
        scanf("%d", &cpin);
        if (cpin != client.pin) {
          printf("You have entered a Invalid Pin");

        }

        else {
          // delete record
          // move file pointer to correct record in file
          fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
          // replace existing record with blank record
          fwrite(&blankClient, sizeof(struct clientData), 1, fPtr);
          printf("\nYour account has been deleted successfully!\n");
        }
      } else if (ch == 0) {
        printf("Operation Aborted!");
      }
      if ((wPtr = fopen("accounts.txt", "w")) == NULL) {
        puts("File could not be opened.");
      } // end if
      else {
        rewind(rPtr); // sets pointer to beginning of file
        fprintf(wPtr, "%-30s%-20s%-20s%20s\n", "Acct", "First Name",
                "Last Name", "Balance");

        // copy all records from random-access file into text file
        while (!feof(rPtr)) {
          result = fread(&client, sizeof(struct clientData), 1, rPtr);

          // write single record to text file
          if (result != 0 && client.acctNum != 0) {
            fprintf(wPtr, "%-30d%-20s%-20s%20.2f\n", client.acctNum,
                    client.firstName, client.lastName, client.balance);
          } // end if
        }   // end while

        fclose(wPtr); // fclose closes the file
      }
    }
  }

  // end else

} // end function deleteRecord

// create and insert record
void newRecord(FILE *fPtr, FILE *readPtr) {
  // create clientData with default information
  struct clientData client = {0, "", "", 0.0};
  unsigned int accountNum; // account number

  FILE *writePtr; // accounts.txt file pointer
  int result;     // used to test whether fread read any bytes
  // create clientData with default information
  // obtain number of account to create
  printf("%s", "Enter new account number ( 1-100 ): ");
  scanf("%d", &accountNum);

  // move file pointer to correct record in file
  fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
  // read record from file
  fread(&client, sizeof(struct clientData), 1, fPtr);
  // display error if account already exists
  if (client.acctNum != 0) {
    printf("Account #%d already contains information.\n", client.acctNum);
  }      // end if
  else { // create record
    // user enters last name, first name and balance
    printf("%s", "Enter  FirstName,LastName, Balance,Pin\n? ");
    scanf("%14s%9s%lf%d", client.firstName, client.lastName, &client.balance,
          &client.pin);

    client.acctNum = accountNum;
    // move file pointer to correct record in file
    fseek(fPtr, (client.acctNum - 1) * sizeof(struct clientData), SEEK_SET);
    // insert record in file
    fwrite(&client, sizeof(struct clientData), 1, fPtr);
    if ((writePtr = fopen("accounts.txt", "w")) == NULL) {
      puts("File could not be opened.");
    } // end if
    else {
      rewind(readPtr); // sets pointer to beginning of file
      fprintf(writePtr, "%-30s%-20s%-20s%20s\n", "Acct", "First Name",
              "Last Name", "Balance");

      // copy all records from random-access file into text file
      while (!feof(readPtr)) {
        result = fread(&client, sizeof(struct clientData), 1, readPtr);

        // write single record to text file
        if (result != 0 && client.acctNum != 0) {
          fprintf(writePtr, "%-30d%-20s%-20s%20.2f\n", client.acctNum,
                  client.firstName, client.lastName, client.balance);
        } // end if
      }   // end while

      fclose(writePtr); // fclose closes the file
    }
  } // end else
} // end function newRecord

// enable user to input menu choice
unsigned int enterChoice(void) {
  unsigned int menuChoice; // variable to store user's choice
  // display available options
  printf("%s", "\n\n\nPress (0) for creating a text file\n"
               "Press (1) for updating your account\n"
               "Press (2) for creating a new account\n"
               "Press (3) for delete your current account\n"
               "Press (4) for Exit\n? ");

  scanf("%u", &menuChoice); // receive choice from user
  return menuChoice;
} // end function enterChoice
