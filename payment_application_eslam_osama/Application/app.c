#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include"app.h"
#include"../Server/server.c"

void appStart()
{
    uint8_t choice;

    while(1)
    {
        void (*menu[])(void) = {Do_Trans , Show_DB , trans_list , Trans_Elem};
        printf("\t\tplease enter choice: \n");
        printf("\t\t(1) Do transaction: \n");
        printf("\t\t(2) Show Data Base: \n");
        printf("\t\t(3) Show transaction List: \n");
        printf("\t\t(4) Search for specific transaction: \n");
        printf("\t\t(5) End the program: \n");
        printf("enter the choice: ");
        scanf(" %d" , &choice);

        if((choice <= 5) && (choice >= 1))
        {
            if(choice == 1)
                Do_Trans();
            else if(choice == 2)
                Show_DB();
            else if(choice == 3)
                trans_list();
            else if(choice == 4)
                Trans_Elem();
            else if(choice == 5)
                break;
        }
        else
            printf("WARNING! WRONG CHOICE NOT LISTED IN THE MENUE\n");
        printf("\n=========================================================================================================\n");
        getch();
        system("cls");

    }
}

// function used to print the array-> used to show the update in database after each transaction
void printArray(ST_accountsDB_t *dataBase, uint8_t size)
{
        printf("\n======================================\n");
        for (int z = 0; z < size; z++)
        {
            printf("%s\t%0.2f", dataBase[z].primaryAccountNumber, dataBase[z].balance);

            if(dataBase[z].state == BLOCKED)
                printf("\tBLOCKED\n");
            else if(dataBase[z].state == RUNNING)
                printf("\tRUNNING\n");
        }
        printf("\n======================================\n");

}

void Do_Trans()
{
    ST_transaction_t trans;
    ST_transaction_t *ptr_trans = &trans;

    EN_serverError_t check_saveTransaction = saveTransaction(ptr_trans);
    if(check_saveTransaction == OK_server)
        printf("saving transaction successfully done\n");

    if(check_saveTransaction == EXPIRED_CARD_S)
        printf("DECLINED TRANSACTION DUE TO CARD IS EXPIRED\n");

    else if(check_saveTransaction == EXCEED_MAX_AMOUNT_S)
        printf("DECLINED TRANSACTION DUE TO EXCEEDING MAX AMOUNT\n");

    else if(check_saveTransaction == ACCOUNT_NOT_FOUND)
        printf("DECLINED TRANSACTION DUE TO CARD NUMBER IS NOT IN THE SYSTEM\n");

    else if(check_saveTransaction == BLOCKED_ACCOUNT)
        printf("DECLINED TRANSACTION DUE TO CARD IS BLOCKED\n");

    else if(check_saveTransaction == LOW_BALANCE)
        printf("DECLINED TRANSACTION DUE TO LOW BALANCE\n");

    else if(check_saveTransaction == SAVING_FAILED)
        printf("FAILED TRANSACTION\n");
}

void Show_DB()
{
    printArray(dataBase, 10);
}

void Trans_Elem()
{
    uint32_t sequence_number;

    printf("please enter sequence number of the transaction = ");
    scanf(" %ui" , &sequence_number);
    EN_serverError_t check_getTransaction =  getTransaction(sequence_number, (sizeof(Transactions_DataBase) / sizeof(Transactions_DataBase[0])) ,Transactions_DataBase);
    if(check_getTransaction == OK_server)
    {
        if(Transactions_DataBase[sequence_number].terminalData.transAmount != 0.00)
        {
            printf("=========================================================================================================\n");
            printf("Sequence Number\t\tTransaction Date\t\tTransaction Amountt\t\tTransaction State\n");
            printf("=========================================================================================================\n");
            printf("%d\t\t\t" , Transactions_DataBase[sequence_number].transactionSequenceNumber);
            printf("%s\t\t\t" , Transactions_DataBase[sequence_number].terminalData.transactionDate);
            printf("%.2lf\t\t\t\t" , Transactions_DataBase[sequence_number].terminalData.transAmount);

            if(Transactions_DataBase[sequence_number].transState == APPROVED)
            {
                printf("APPROVED\n");
            }
            else if(Transactions_DataBase[sequence_number].transState == EXPIRED_CARD_SS)
            {
                printf("EXPIRED CARD\n");
            }

            else if(Transactions_DataBase[sequence_number].transState == EXCEED_MAX_AMOUNT_SS)
            {
                printf("EXCEED MAX AMOUNT\n");
            }

            else if(Transactions_DataBase[sequence_number].transState == FRAUD_CARD)
            {
                printf("INVALID ACCOUNT\n");
            }

            else if(Transactions_DataBase[sequence_number].transState == DECLINED_STOLEN_CARD)
            {
                printf("BLOCKED ACCOUNT\n");
            }

            else if(Transactions_DataBase[sequence_number].transState == DECLINED_INSUFFECIENT_FUND)
            {
                printf("LOW BALANCE\n");
            }
            else
                printf("TRANSACTION NOT FOUND\n");
        }
        else
            printf("TRANSACTION NOT FOUND\n");

    }
    else if(check_getTransaction == TRANSACTION_NOT_FOUND)
        printf("TRANSACTION NOT FOUND\n");
}

void trans_list()
{
    if(Transactions_DataBase[0].terminalData.transAmount != 0.00)
    {
        printf("\n======================================\n");
        printf("Sequence Number\t\tTransaction Date\t\tTransaction Amountt\t\tTransaction State\n");
        printf("=========================================================================================================\n");
        for (uint8_t y = 0 ; y <= SequenceNumber ; y++)
        {
            if(Transactions_DataBase[y].terminalData.transAmount != 0.00)
            {
                printf("%d\t\t\t" , Transactions_DataBase[y].transactionSequenceNumber);
                printf("%s\t\t\t" , Transactions_DataBase[y].terminalData.transactionDate);
                printf("%.2lf\t\t\t\t" , Transactions_DataBase[y].terminalData.transAmount);

                if(Transactions_DataBase[y].transState == APPROVED)
                {
                    printf("APPROVED\n");
                }
                else if(Transactions_DataBase[y].transState == EXPIRED_CARD_SS)
                {
                    printf("EXPIRED CARD\n");
                }

                else if(Transactions_DataBase[y].transState == EXCEED_MAX_AMOUNT_SS)
                {
                    printf("EXCEED MAX AMOUNT\n");
                }

                else if(Transactions_DataBase[y].transState == FRAUD_CARD)
                {
                    printf("INVALID ACCOUNT\n");
                }

                else if(Transactions_DataBase[y].transState == DECLINED_STOLEN_CARD)
                {
                    printf("BLOCKED ACCOUNT\n");
                }

                else if(Transactions_DataBase[y].transState == DECLINED_INSUFFECIENT_FUND)
                {
                    printf("LOW BALANCE\n");
                }
            }
            else
                break;

        printf("=========================================================================================================\n");
        }
    }
    else
        printf("TRANSACTION LIST IS EMPTY\n");
}
