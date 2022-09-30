#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include"Terminal.h"
#include"../Card/card.c"

EN_terminalError_t getTransactionDate(ST_terminalData_t *termData)
{
    bool flag_TransactionDate = 0;
    printf("Please enter card Transaction Date (DD/MM/YYYY): ");
    scanf(" %s" , &termData->transactionDate);

    if( (strlen(termData->transactionDate) == 10) && (termData->transactionDate[2] == '/') && (termData->transactionDate[5] == '/') )
    {
        for(int i = 0 ; i <= 9 ; i++)
        {
            if( (i == 2) || (i == 5) )
                continue;
            if( (termData->transactionDate[i] >= '0') && (termData->transactionDate[i] <= '9') )
                continue;
            else
            {
                flag_TransactionDate = 1;
                break;
            }
        }
        if(flag_TransactionDate == 1)
            {return WRONG_DATE;}
        else
            {return OK_terminal;}
    }
    else
        {return WRONG_DATE;}
}

EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
    // transaction Date in format of DD/MM/YYYY
    uint8_t *ptr_transactionDate_month = &termData.transactionDate[3];
    uint8_t *ptr_transactionDate_year = &termData.transactionDate[8];

    // card expiration Date in format of MM/YY
    uint8_t *ptr_cardExpirationDate_month = &cardData.cardExpirationDate[0];
    uint8_t *ptr_cardExpirationDate_year = &cardData.cardExpirationDate[3];

    if( atoi(ptr_cardExpirationDate_year) > atoi(ptr_transactionDate_year) )
        {return OK_terminal;}
    else if( atoi(ptr_cardExpirationDate_year) == atoi(ptr_transactionDate_year) )
    {
        if( atoi(ptr_cardExpirationDate_month) > atoi(ptr_transactionDate_month) )
            {return OK_terminal;}
        else
            {return EXPIRED_CARD;}
    }
    else
        {return EXPIRED_CARD;}
}

EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData)
{
    uint8_t num = 0 , sum = 0;

    if(strlen(cardData->primaryAccountNumber) % 2 == 0) //num of digits is even->odd
    {
        for(uint8_t i = 0 ; i < strlen(cardData->primaryAccountNumber) ; i++)
        {
            if(i % 2 == 0)
            {
                num = (cardData->primaryAccountNumber[i] - '0') * 2;
                if(num > 9)
                {
                    num = (num % 10) + 1;// (16%10) + 1 = 6  + 1 = 7
                }
                sum += num;
            }
            else
            {
                sum += (cardData->primaryAccountNumber[i] - '0');
            }
        }

        if(sum % 10 == 0)
            {return OK_terminal;}
        else
            {return INVALID_CARD;}
    }

    else if(strlen(cardData->primaryAccountNumber) % 2 != 0) //num of digits is odd->even
    {
        for(uint8_t j = 0 ; j < strlen(cardData->primaryAccountNumber) ; j++)
        {
            if(j % 2 != 0)
            {
                num = (cardData->primaryAccountNumber[j] - '0') * 2;
                if(num > 9)
                {
                    num = (num % 10) + 1;// (16%10) + 1 = 6  + 1 = 7
                }
                sum += num;
            }
            else
            {
                sum += (cardData->primaryAccountNumber[j] - '0');
            }
        }

        if(sum % 10 == 0)
            {return OK_terminal;}
        else
            {return INVALID_CARD;}
    }

}

EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData)
{
    printf("Please Enter the transaction Amount: ");
    scanf("%lf", &termData->transAmount);

    if(termData->transAmount > 0)
        {return OK_terminal;}
    else
        {return INVALID_AMOUNT;}
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData)
{
    termData->maxTransAmount = 6000.00;
    /*there is no need to check If transaction max amount less than or equal to 0
      because This function sets the maximum allowed amount or even have setMaxAmount function */

    if( (termData->transAmount) <= (termData->maxTransAmount) )
        return OK_terminal;
    else
        return EXCEED_MAX_AMOUNT;
}

