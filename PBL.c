#include<stdio.h>
#include<stdlib.h>
#include<string.h>
/* =========================================================
   STRUCTURES
   ========================================================= */
// Structure to store user details
struct User {
    int userID;
    char username[20];
    char password[20];
    float balance;
    float loan;
};
// Structure to store transaction records
struct Transaction {
    char username[20];
    char type[10];
    float amount;
};
/* =========================================================
   USER ID CHECK FUNCTION
   ========================================================= */
// Checks whether a given user ID already exists
int userIDExists(int id)
{
    FILE *fp = fopen("user.dat", "rb");
    struct User u;

    if(fp == NULL)
        return 0;

    while(fread(&u, sizeof(u), 1, fp))
    {
        if(u.userID == id)
        {
            fclose(fp);
            return 1;   // ID exists

        }
    }

    fclose(fp);
    return 0;  // ID does not exist
};
// Login panel for the user inputs user ID and password and searches in user.dat file
int loginUser(struct User *loggedInUser)
{
    FILE *fp = fopen("user.dat", "rb");
    struct User u;
    int id;
    char pass[20];

    if(fp == NULL)
    {
         printf("\n*****************************************************************************\n");
        printf("!!!!!!!!!!!!!!NO USERS FOUND!!!!!!!!!!!!!!");
         printf("\n*****************************************************************************\n");
        return 0;
    }

    printf("ENTER USER ID: ");
    scanf("%d", &id);

    printf("ENTER PASSWORD: ");
    scanf(" %19s", pass);

    while(fread(&u, sizeof(u), 1, fp))
    {
        if(u.userID == id && strcmp(u.password, pass) == 0)
        {
            *loggedInUser = u;
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
};
/* =========================================================
   TRANSACTION HANDLING
   ========================================================= */
// Records a transaction into file
void recordTransaction(char username[], char type[], float amount)
{
    FILE *fp = fopen("trans.dat", "ab");

    if(fp == NULL)
    {
        printf("FILE ERROR!\n");
        return;
    }

    struct Transaction t;

    strcpy(t.username, username);
    strcpy(t.type, type);
    t.amount = amount;

    fwrite(&t, sizeof(t), 1, fp);

    fclose(fp);
};
// Displays all transactions to the admin
void viewAllTransactions()
{
    FILE *fp = fopen("trans.dat", "rb");
    struct Transaction t;

    if(fp == NULL)
    {
        printf("\n*****************************************************************************\n");
        printf("NO TRANSACTIONS FOUND.\n");
        printf("\n*****************************************************************************\n");
        return;
    }

    printf("\n========== ALL TRANSACTIONS ==========\n");

    while(fread(&t, sizeof(t), 1, fp))
    {
        printf("USER: %s | TYPE: %s | AMOUNT: %.2f\n",
               t.username, t.type, t.amount);
    }

    printf("=====================================\n");

    fclose(fp);
};
// Displays transactions of particular user to the admin
void searchTransactions()
{
    FILE *fp = fopen("trans.dat", "rb");
    struct Transaction t;
    int id, found = 0;
    struct User u;

    if(fp == NULL)
    {
        printf("\n*****************************************************************************\n");
        printf("NO TRANSACTIONS FOUND.\n");
        printf("\n*****************************************************************************\n");
        return;
    }

    printf("ENTER USER ID: ");
    scanf("%d", &id);

    FILE *ufp = fopen("user.dat", "rb");

    if(ufp == NULL)
    {
        printf("USER FILE ERROR!\n");
        fclose(fp);
        return;
    }

    char username[20];
    int userFound = 0;

    while(fread(&u, sizeof(u), 1, ufp))
    {
        if(u.userID == id)
        {
            strcpy(username, u.username);
            userFound = 1;
            break;
        }
    }

    fclose(ufp);

    if(!userFound)
    {
        printf("\n*****************************************************************************\n");
        printf("USER NOT FOUND!\n");
        printf("\n*****************************************************************************\n");
        fclose(fp);
        return;
    }

    printf("\n====== TRANSACTIONS FOR %s (ID: %d) ======\n", username, id);

    while(fread(&t, sizeof(t), 1, fp))
    {
        if(strcmp(t.username, username) == 0)
        {
            printf("TYPE: %s | AMOUNT: %.2f\n", t.type, t.amount);
            found = 1;
        }
    }

    if(!found)
        printf("NO TRANSACTIONS FOUND.\n");

    printf("=======================================================\n");

    fclose(fp);
};
// Displays user transactions to the user
void viewMyTransactions(char username[])
{
    FILE *fp = fopen("trans.dat", "rb");
    struct Transaction t;
    int found = 0;

    if(fp == NULL)
    {
        printf("NO TRANSACTIONS FOUND.\n");
        return;
    }

    printf("\n========== YOUR TRANSACTIONS ==========\n");

    while(fread(&t, sizeof(t), 1, fp))
    {
        if(strcmp(t.username, username) == 0)
        {
            printf("TYPE: %s | AMOUNT: %.2f\n", t.type, t.amount);
            found = 1;
        }
    }

    if(!found)
        printf("NO TRANSACTIONS FOUND.\n");

    printf("======================================\n");

    fclose(fp);
};
/* =========================================================
   ADMIN FUNCTIONS
   ========================================================= */
// Adds a new user to the system by admin
void addUser()
{
    int result;
    FILE *fp = fopen("user.dat", "ab");

    if(fp == NULL)
    {
        printf("FILE ERROR!\n");
        return;
    }

    struct User u;

     // Input validation for user ID
    do
    {
    printf("ENTER USER ID: ");
    result = scanf("%d", &u.userID);
    if(result != 1)
    {
        printf("\n*****************************************************************************\n");
        printf("INVALID INPUT! PLEASE INPUT A NUMBER.\n");
        while(getchar() != '\n');
    }
    } while(result != 1);

    // Check if ID already exists
    if(userIDExists(u.userID)==0)
    {
        printf("ENTER USERNAME: ");
        getchar();
        fgets(u.username, 20, stdin);
        u.username[strcspn(u.username, "\n")] = '\0';

        printf("ENTER PASSWORD: ");
        fgets(u.password, 20, stdin);
        u.password[strcspn(u.password, "\n")] = '\0';

        u.balance = 0;
        u.loan = 0;

        fwrite(&u, sizeof(u), 1, fp);

        fclose(fp);

        printf("!!!!!!!!!!!!!!USER ADDED SUCCESSFULLY!!!!!!!!!!!!!!\n");
    }
    else
    {
        printf("\n!!!!!!!!!!!!!!USER ID ALREADY EXISTS!!!!!!!!!!!!!!\n");
        fclose(fp);
    }
};
// Displays all of the current users to admin
void viewUsers()
{
    FILE *fp = fopen("user.dat", "rb");
    struct User u;

    if(fp == NULL)
    {
        printf("\nNO USERS FOUND.\n");
        return;
    }

    printf("\n=================================== USER LIST ===================================\n");

    while(fread(&u, sizeof(u), 1, fp))
    {
        printf("ID: %d | USERNAME: %s | PASSWORD: %s | BALANCE: %.2f | LOAN: %.2f\n",
               u.userID, u.username, u.password, u.balance, u.loan);
    }

    printf("=================================================================================\n");

    fclose(fp);
};
// Removes a user from the system by admin
void deleteUser()
{
    FILE *fp = fopen("user.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");

    struct User u;
    int id, found = 0, result;

    if(fp == NULL || temp == NULL)
    {
        printf("FILE ERROR!\n");
        return;
    }

    //this loop is to input user ID
    do
    {
        printf("ENTER USER ID: ");
        result = scanf("%d", &id);
        //this condition checks if the input ID is not anything other than integer
        if(result != 1)
        {
            printf("INVALID INPUT! PLEASE INPUT A NUMBER.\n");
            while(getchar() != '\n');
        }
    } while(result != 1);

    //this loop searches for input ID in user.dat file
    while(fread(&u, sizeof(u), 1, fp))
    {
        if(u.userID != id)
        {
            fwrite(&u, sizeof(u), 1, temp);
        }
        else
        {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    remove("user.dat");
    rename("temp.dat", "user.dat");

    if(found)
        printf("USER DELETED SUCCESSFULLY!\n");
    else
        printf("USER NOT FOUND!\n");
};
// Displays and Edits the loan interest rate
void adminLoanInterest()
{
    FILE *fp;
    float rate, currentRate;
    char choice;

    printf("\n*****************************************************************************\n");

    fp = fopen("loan.dat", "rb");

    if(fp != NULL)
    {
        fread(&currentRate, sizeof(currentRate), 1, fp);
        fclose(fp);
        printf("CURRENT INTEREST RATE: %.2f%%\n", currentRate);
    }
    else
    {
        printf("NO INTEREST RATE SET YET.\n");
    }

    printf("\nDO YOU WANT TO UPDATE INTEREST RATE? (Y/N): ");
    scanf(" %c", &choice);

    if(choice != 'Y' && choice != 'y')
    {
        printf("\n*****************************************************************************\n");
        printf("NO CHANGES MADE.");
        printf("\n*****************************************************************************\n");
        return;
    }

    printf("ENTER NEW LOAN INTEREST RATE (in %%): ");
    scanf("%f", &rate);

    if(rate < 0)
    {
        printf("\n*****************************************************************************\n");
        printf("!!!!!!!!!!!!!!INVALID RATE!!!!!!!!!!!!!!\n");
        printf("*****************************************************************************\n");
        return;
    }

    fp = fopen("loan.dat", "wb");

    if(fp == NULL)
    {
        printf("FILE ERROR!\n");
        return;
    }

    fwrite(&rate, sizeof(rate), 1, fp);
    fclose(fp);

    printf("\n*****************************************************************************\n");
    printf("!!!!!!!!!!!!!!INTEREST RATE UPDATED SUCCESSFULLY!!!!!!!!!!!!!!\n");
    printf("*****************************************************************************\n");
}
/* =========================================================
   ADMIN PANEL
   ========================================================= */
// Displays multiple options for the admin to perform
void adminMenu()
{
    char c,p1,p2,p3;
    int k=0;
    printf("\n*************\n*HELLO ADMIN*\n*************\n");
    do
    {
        viewUsers();
        printf("\n*****************************************************************************\n");
        printf("PLEASE SELECT FROM FOLLOWING OPTIONS\nA. ADD USER\nB. DELETE USER\nC. SET LOAN INTEREST RATE\nD. VIEW ALL TRANSACTIONS\nE. SEARCH USER TRANSACTIONS\nF. EXIT");
        printf("\nENTER YOU CHOICE HERE:");
        scanf(" %c",&c);
        switch(c)
        {
            case 'A':
            case 'a':
                addUser();
                printf("\n*****************************************************************************\n");
                printf("DO YOU WANT TO ADD ANOTHER USER?(Y/N)\n");
                scanf(" %c", &p1);
                if(p1=='n'||p1=='N')
                {
                    printf("\n*****************************************************************************\n");
                    printf("DO YOU WANT TO EXIT?(Y/N)\n");
                    scanf(" %c", &p2);
                    if(p2=='y'||p2=='Y')
                    {
                        c='f';
                        break;
                    }
                    else if(p2=='n'||p2=='N')
                    {
                        break;
                    }
                    else
                    {
                        printf("!!!!!!!!!!!!!!PLEASE SELECT A VALID OPTION!!!!!!!!!!!!!!");
                    }
                }
                else if(p1=='y'||p1=='Y')
                {
                    p3='y';
                    do
                    {
                        if(p3=='Y'||p3=='y')
                        {
                            addUser();
                        }
                        printf("\n*****************************************************************************\n");
                        printf("DO YOU WANT TO ADD ANOTHER USER?(Y/N)\n");
                        scanf(" %c",&p3);
                        if(p3!='N'&&p3!='n'&&p3!='Y'&&p3!='y')
                        {
                            printf("!!!!!!!!!!!!!!PLEASE SELECT A VALID OPTION!!!!!!!!!!!!!!");
                        }
                        }while(p3=='Y'||p3=='y');
                        break;
                        }
            case 'B':
            case 'b':
                deleteUser();
                printf("\n*****************************************************************************\n");
                printf("DO YOU WANT TO DELETE ANOTHER USER?(Y/N)\n");
                scanf(" %c", &p1);
                if(p1=='n'||p1=='N')
                {
                    printf("\n*****************************************************************************\n");
                    printf("DO YOU WANT TO EXIT?(Y/N)\n");
                    scanf(" %c", &p2);
                    if(p2=='y'||p2=='Y')
                    {
                        c='f';
                        break;
                    }
                    else if(p2=='n'||p2=='N')
                    {
                        break;
                    }
                    else
                    {
                        printf("!!!!!!!!!!!!!!PLEASE SELECT A VALID OPTION!!!!!!!!!!!!!!");
                    }
                }
                else if(p1=='y'||p1=='Y')
                {
                    p3='y';
                    do
                    {
                        if(p3=='Y'||p3=='y')
                        {
                            deleteUser();
                        }
                        printf("\n*****************************************************************************\n");
                        printf("DO YOU WANT TO DELETE ANOTHER USER?(Y/N)\n");
                        scanf(" %c",&p3);
                        if(p3!='N'&&p3!='n'&&p3!='Y'&&p3!='y')
                        {
                            printf("!!!!!!!!!!!!!!PLEASE SELECT A VALID OPTION!!!!!!!!!!!!!!");
                        }
                    }while(p3=='Y'||p3=='y');
                    break;
                }
            case 'c':
            case 'C':
                adminLoanInterest();
                do
                {
                    printf("\n*****************************************************************************\n");
                    printf("DO YOU WANT TO EXIT?(Y/N)\n");
                    scanf(" %c", &p2);
                    if(p2=='y'||p2=='Y')
                    {
                        c='f';
                        k=1;
                        break;
                    }
                    else if(p2=='n'||p2=='N')
                    {
                        k=1;
                        break;
                    }
                    else
                    {
                        printf("!!!!!!!!!!!!!!PLEASE SELECT A VALID OPTION!!!!!!!!!!!!!!");
                    }
                }while(k!=0);
                break;

            case 'd':
            case 'D':
                viewAllTransactions();
                do
                {
                    printf("\n*****************************************************************************\n");
                    printf("DO YOU WANT TO EXIT?(Y/N)\n");
                    scanf(" %c", &p2);
                    if(p2=='y'||p2=='Y')
                    {
                        c='f';
                        k=1;
                        break;
                    }
                    else if(p2=='n'||p2=='N')
                    {
                        k=1;
                        break;
                    }
                    else
                    {
                        printf("!!!!!!!!!!!!!!PLEASE SELECT A VALID OPTION!!!!!!!!!!!!!!");
                    }
                }while(k!=0);
                break;

            case 'e':
            case 'E':
                searchTransactions();
                do
                {
                    printf("\n*****************************************************************************\n");
                    printf("DO YOU WANT TO EXIT?(Y/N)\n");
                    scanf(" %c", &p2);
                    if(p2=='y'||p2=='Y')
                    {
                        c='f';
                        k=1;
                        break;
                    }
                    else if(p2=='n'||p2=='N')
                    {
                        k=1;
                        break;
                    }
                    else
                    {
                        printf("!!!!!!!!!!!!!!PLEASE SELECT A VALID OPTION!!!!!!!!!!!!!!");
                    }
                }while(k!=0);
                break;

            case 'f':
            case 'F':
                break;

            default:
                printf("!!!!!!!!!!!!!!PLEASE SELECT A VALID OPTION!!!!!!!!!!!!!!");
        }
    }while(c!='f'&&c!='F');
};
/* =========================================================
   USER OPERATIONS (BANKING)
   ========================================================= */
//updates the user.dat file
void updateUser(struct User updatedUser)
{
    FILE *fp = fopen("user.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");

    struct User u;

    if(fp == NULL || temp == NULL)
    {
        printf("FILE ERROR!\n");
        return;
    }

    while(fread(&u, sizeof(u), 1, fp))
    {
        if(u.userID == updatedUser.userID)
        {
            fwrite(&updatedUser, sizeof(updatedUser), 1, temp);
        }
        else
        {
            fwrite(&u, sizeof(u), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    remove("user.dat");
    rename("temp.dat", "user.dat");
};
// Issues loan from bank into the account
void takeLoan(struct User *u)
{
    float amount;

    printf("ENTER LOAN AMOUNT: ");
    scanf("%f", &amount);

    if(amount <= 0)
    {
        printf("\n*****************************************************************************\n");
        printf("!!!!!!!!!!!!!!INVALID AMOUNT!!!!!!!!!!!!!!");
        printf("\n*****************************************************************************\n");
        return;
    }

    u->loan += amount;
    u->balance += amount;

    printf("\n*****************************************************************************\n");
    printf("\n!!!!!!!!!!!!!!LOAN APPROVED!!!!!!!!!!!!!!\n");
    printf("UPDATED BALANCE: %.2f\n", u->balance);
    printf("TOTAL LOAN: %.2f\n", u->loan);
    printf("\n*****************************************************************************\n");
    recordTransaction(u->username, "LOAN ISSUED", amount);

    updateUser(*u);
};
// Deposit money into account
void deposit(struct User *u)
{
    float amount;

    printf("ENTER AMOUNT TO DEPOSIT: ");
    scanf("%f", &amount);

    if(amount <= 0)
    {
        printf("\n*****************************************************************************\n");
        printf("!!!!!!!!!!!!!!INVALID AMOUNT!!!!!!!!!!!!!!");
        printf("\n*****************************************************************************\n");
        return;
    }

    u->balance += amount;

    printf("\n*****************************************************************************\n");
    printf("!!!!!!!!!!!!!!DEPOSIT SUCCESSFUL!!!!!!!!!!!!!!");
    printf("\n*****************************************************************************\n");
    recordTransaction(u->username, "DEPOSIT", amount);

    // Update user in file
    updateUser(*u);
};
/* =========================================================
   USER OPERATIONS (LOAN)
   ========================================================= */
// Withdraw money from account
void withdraw(struct User *u)
{
    float amount;

    printf("ENTER AMOUNT TO WITHDRAW: ");
    scanf("%f", &amount);

    if(amount <= 0)
    {
        printf("\n*****************************************************************************\n");
        printf("!!!!!!!!!!!!!!INVALID AMOUNT!!!!!!!!!!!!!!");
        printf("\n*****************************************************************************\n");
        return;
    }

    if(amount > u->balance)
    {
        printf("\n*****************************************************************************\n");
        printf("INSUFFICIENT BALANCE!!!!!!!!!!!!!!");
        printf("\n*****************************************************************************\n");
        return;
    }

    u->balance -= amount;

    printf("\n*****************************************************************************\n");
    printf("!!!!!!!!!!!!!!WITHDRAWAL SUCCESSFUL!!!!!!!!!!!!!!");
    printf("\n*****************************************************************************\n");
    recordTransaction(u->username, "WITHDRAW", amount);

    updateUser(*u);
};
// Pays loan from account
void payLoan(struct User *u)
{
    float amount;

    printf("ENTER AMOUNT TO PAY LOAN: ");
    scanf("%f", &amount);

    if(amount <= 0)
    {
        printf("\n*****************************************************************************\n");
        printf("!!!!!!!!!!!!!!INVALID AMOUNT!!!!!!!!!!!!!!");
        printf("\n*****************************************************************************\n");
        return;
    }

    if(amount > u->balance)
    {
        printf("\n*****************************************************************************\n");
        printf("!!!!!!!!!!!!!!NOT ENOUGH BALANCE!!!!!!!!!!!!!!");
        printf("\n*****************************************************************************\n");
        return;
    }

    if(u->loan <= 0)
    {
        printf("\n*****************************************************************************\n");
        printf("!!!!!!!!!!!!!!NO LOAN TO PAY!!!!!!!!!!!!!!");
        printf("\n*****************************************************************************\n");
        return;
    }

    if(amount > u->loan)
        amount = u->loan;

    u->loan -= amount;
    u->balance -= amount;

    printf("\n*****************************************************************************\n");
    printf("!!!!!!!!!!!!!!LOAN PAYMENT SUCCESSFUL!!!!!!!!!!!!!!");
    printf("\n*****************************************************************************\n");
    recordTransaction(u->username, "LOAN PAYMENT", amount);

    updateUser(*u);
};
// Gives interest rate information to getLoanInterest() function
float getLoanInterest()
{
    FILE *fp = fopen("loan.dat", "rb");
    float rate;

    if(fp == NULL)
    {
        return 0;
    }

    fread(&rate, sizeof(rate), 1, fp);
    fclose(fp);

    return rate;
}
// Calculates simple interest on loan
void calculateLoanDetails(struct User *u)
{
    float rate = getLoanInterest();
    float time, interest, total;

    if(u->loan <= 0)
    {
        printf("\n*****************************************************************************\n");
        printf("!!!!!!!!!!!!!!NO ACTIVE LOAN!!!!!!!!!!!!!!");
        printf("\n*****************************************************************************\n");
        return;
    }

    printf("ENTER TIME (IN YEARS): ");
    scanf("%f", &time);

    if(time <= 0)
    {
        printf("\n*****************************************************************************\n");
        printf("!!!!!!!!!!!!!!INVALID TIME!!!!!!!!!!!!!!");
        printf("\n*****************************************************************************\n");
        return;
    }

    if(rate == 0)
    {
        printf("\n*****************************************************************************\n");
        printf("!!!!!!!!!!!!!!INTEREST RATE NOT SET BY ADMIN!!!!!!!!!!!!!!");
        printf("\n*****************************************************************************\n");
        return;
    }

    // Simple Interest Formula
    interest = (u->loan * rate * time) / 100;
    total = u->loan + interest;

    printf("\n================ LOAN DETAILS ================\n");
    printf("CURRENT LOAN: %.2f\n", u->loan);
    printf("INTEREST RATE: %.2f%%\n", rate);
    printf("TIME: %.2f YEARS\n", time);
    printf("INTEREST: %.2f\n", interest);
    printf("TOTAL PAYABLE: %.2f\n", total);
    printf("=============================================\n");
};
/* =========================================================
   USER PANEL
   ========================================================= */
// Displays multiple options for the user to perform
void userMenu(struct User *u)
{
    int choice;

    do
    {
        printf("\n*****************************************************************************\n");
        printf("CURRENT BALANCE: %.2f | LOAN: %.2f\n", u->balance, u->loan);
        printf("\n*****************************************************************************\n");

        printf("1. DEPOSIT\n");
        printf("2. WITHDRAW\n");
        printf("3. PAY LOAN\n");
        printf("4. GET LOAN\n");
        printf("5. VIEW LOAN DETAILS\n");
        printf("6. EXIT\n");
        printf("ENTER CHOICE: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                deposit(u);
                break;
            case 2:
                withdraw(u);
                break;
            case 3:
                payLoan(u);
                break;
            case 4:
                takeLoan(u);
                break;
            case 5:
                calculateLoanDetails(u);
                break;
            case 6:
                break;
            default:
                printf("!!!!!!!!!!!!!!PLEASE SELECT A VALID OPTION!!!!!!!!!!!!!!");
        }

    } while(choice != 6);
};
/* =========================================================
   MAIN PANEL
   ========================================================= */
// Main function
int main()
{
    FILE *fp;
    char aid[]={"1234"},ap[]={"1234"},k[20],p[20];
    int s,o;
    float balance,loan,wa,da,la;
    printf("*************************************============================================*************************************\n");
    printf("*************************************============================================*************************************\n");
    printf("*************************************  __ _  __ _  __ _  | |__   __ _ _ __ | | __*************************************\n");
    printf("************************************* / _` |/ _` |/ _` | | '_ \\ / _` | '_ \\| |/ /*************************************\n");
    printf("*************************************| (_| | (_| | (_| | | |_) | (_| | | | |   < *************************************\n");
    printf("************************************* \\__,_|\\__,_|\\__,_| |_.__/ \\__,_|_| |_|_|\\_\\*************************************\n");
    printf("*************************************============================================*************************************\n");
    printf("*************************************============================================*************************************\n");
    printf("LOGIN AS:-\n1-ADMIN\n2-USER\n");
    scanf(" %d",&o);
    switch(o)
    {
        case 1:
            printf("PLEASE ENTER ADMIN ID:");
            getchar();
            fgets(k,20,stdin);
            k[strcspn(k, "\n")] = '\0';
            printf("PLEASE ENTER PASSWORD:");
            fgets(p,20,stdin);
            p[strcspn(p, "\n")] = '\0';
            if(strcmp(k,aid)==0&&strcmp(p,ap)==0)
            {
                adminMenu();
            }
            else if(strcmp(k,aid)!=0&&strcmp(p,ap)==0)
            {
                printf("\n*****************************************************************************\n");
                printf("INCORRECT ID! PLEASE TRY AGAIN NEXT TIME");
                printf("\n*****************************************************************************\n");
            }
            else if(strcmp(k,aid)==0&&strcmp(p,ap)!=0)
            {
                printf("\n*****************************************************************************\n");
                printf("INCORRECT PASSWORD! PLEASE TRY AGAIN NEXT TIME");
                printf("\n*****************************************************************************\n");
            }
            else
            {
                printf("\n*****************************************************************************\n");
                printf("INCORRECT ID AND PASSWORD! PLEASE TRY AGAIN NEXT TIME");
                printf("\n*****************************************************************************\n");
            }
            break;

        case 2:
            struct User currentUser;

            if(loginUser(&currentUser))
            {
                printf("\nLOGIN SUCCESSFUL! WELCOME %s\n", currentUser.username);
                userMenu(&currentUser);
            }
            else
            {
                printf("\n*****************************************************************************\n");
                printf("INCORRECT ID OR PASSWORD! PLEASE TRY AGAIN NEXT TIME");
                printf("\n*****************************************************************************\n");
            }

            break;

        default:
            printf("\n*****************************************************************************\n");
            printf("INVALID OPTION!!PLEASE TRY AGAIN NEXT TIME");
            printf("\n*****************************************************************************\n");
    }
    return 0;
}
