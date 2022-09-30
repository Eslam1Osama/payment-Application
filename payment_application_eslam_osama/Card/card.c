#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include"card.h"

EN_cardError_t getCardHolderName(ST_cardData_t *cardData)
{
    printf("Please enter card holder name: ");
    scanf(" %[^\n]%*c" , cardData->cardHolderName);

    if( (strlen(cardData->cardHolderName) >= 20) && (strlen(cardData->cardHolderName) <=  24) )
        return OK_card;
    else
        return WRONG_NAME;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData)
{
    bool flag_ExpiryDate = 0;
    printf("Please enter card Expiry Date (MM/YY): ");
    scanf(" %s" , &cardData->cardExpirationDate);

    if( (strlen(cardData->cardExpirationDate) == 5) && (cardData->cardExpirationDate[2] == '/') )
    {
        for(int i = 0 ; i <= 4 ; i++)
        {
            if(i == 2)
                continue;

            if( (cardData->cardExpirationDate[i] >= '0') && (cardData->cardExpirationDate[i] <= '9') )
                continue;
            else
            {
                flag_ExpiryDate = 1;
                break;
            }
        }
        if(flag_ExpiryDate == 1)
            return WRONG_EXP_DATE;
        else
            return OK_card;
    }
    else
        return WRONG_EXP_DATE;
}

EN_cardError_t getCardPAN(ST_cardData_t *cardData)
{
    printf("Please enter account number: ");
    scanf(" %s" , &cardData->primaryAccountNumber);

    if( (strlen(cardData->primaryAccountNumber) >= 16) && (strlen(cardData->primaryAccountNumber) <= 19) )
        return OK_card;
    else
        return WRONG_PAN;
}

