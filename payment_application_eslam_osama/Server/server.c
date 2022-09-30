#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include"server.h"
#include"../Terminal/terminal.c"

ST_accountsDB_t dataBase[255] =
{
    {"4012888888881881", 5500.00, RUNNING},
    {"5333619503715702", 20000.00, RUNNING},
    {"4388576018410707", 4650.26, RUNNING},
    {"4305509557712365", 1750.22, RUNNING},
    {"5244666125569403", 499.00, RUNNING},
    {"6011320897384489", 2100.00, RUNNING},
    {"6011335566707075", 0.00, BLOCKED},
    {"5541979626860019", 1.00, BLOCKED},
    {"5142059195611649", 900.12, RUNNING},
    {"4662251408620870", 312.23, BLOCKED},
};

ST_transaction_t Transactions_DataBase[255] = {0};


uint8_t i = 0;
extern uint32_t SequenceNumber = 0;

EN_transState_t recieveTransactionData(ST_transaction_t *transData)
{
    EN_serverError_t check_isValidAccount = isValidAccount(&transData->cardHolderData);
    EN_serverError_t check_isAmountAvailable = isAmountAvailable(&transData->terminalData);
    EN_terminalError_t check_isCardExpired =  isCardExpired(transData->cardHolderData, transData->terminalData);
    EN_serverError_t check_isBlockedAccount = isBlockedAccount(dataBase);

        if( (check_isValidAccount == OK_server) && (check_isBlockedAccount == OK_server) && (check_isAmountAvailable == OK_server) && (check_isCardExpired == OK_terminal) )
        {
            dataBase[i].balance -= transData->terminalData.transAmount;
            return APPROVED;
        }
        else if(check_isCardExpired == EXPIRED_CARD)
            return EXPIRED_CARD_SS;

        else if(check_isAmountAvailable == EXCEED_MAX_AMOUNT_S)
            return EXCEED_MAX_AMOUNT_SS;

        else if(check_isValidAccount == ACCOUNT_NOT_FOUND)
            return FRAUD_CARD;

        else if(check_isBlockedAccount == BLOCKED_ACCOUNT)
                return DECLINED_STOLEN_CARD;

        else if(check_isAmountAvailable == LOW_BALANCE)
                return DECLINED_INSUFFECIENT_FUND;

        else
            return INTERNAL_SERVER_ERROR;
            /* as console app we do not have any parameter to control Internal Server Errors
            ,so just use it as else even thought function will never reach to else*/
}

EN_serverError_t isValidAccount(ST_cardData_t *cardData)
{
    EN_cardError_t check_CardHolderName = getCardHolderName(cardData);
    while(check_CardHolderName == WRONG_NAME)
    {
        printf("WARNING!! you exceed name size, please try again\n");
        check_CardHolderName = getCardHolderName(cardData);
    }

    EN_cardError_t check_CardExpiryDate = getCardExpiryDate(cardData);
    while(check_CardExpiryDate == WRONG_EXP_DATE)
    {
        printf("\nWARNING!! the format of ExpiryDate, please try again\n");
        check_CardExpiryDate = getCardExpiryDate(cardData);
    }

    EN_serverError_t check_cardPAN_inDB = linearSearch(dataBase , ( sizeof(dataBase) / sizeof(dataBase[0]) ) , cardData);

    if(check_cardPAN_inDB == OK_server)
        return OK_server;
    else
        return ACCOUNT_NOT_FOUND;// invalid account
}

EN_serverError_t isAmountAvailable(ST_terminalData_t *termData)
{
    EN_terminalError_t check_TransactionDate = getTransactionDate(termData);
    while (check_TransactionDate == WRONG_DATE)
        {
            printf("\nWARNING!! the format of Transaction Date, please try again\n");
            check_TransactionDate = getTransactionDate(termData);
        }
    EN_terminalError_t check_TransactionAmount = getTransactionAmount(termData);
    while (check_TransactionAmount == INVALID_AMOUNT)
        {
            printf("WARNING!! the Transaction Amount value is less than or equal zero, please try again\n");
            check_TransactionAmount = getTransactionAmount(termData);
        }

    EN_terminalError_t check_isBelowMaxAmount =  isBelowMaxAmount(termData);
    if(check_isBelowMaxAmount == EXCEED_MAX_AMOUNT)
        return EXCEED_MAX_AMOUNT_S;

    if(dataBase[i].balance >= termData->transAmount)
        return OK_server;
    else
        return LOW_BALANCE;
}

EN_serverError_t saveTransaction(ST_transaction_t *transData)
{
    EN_transState_t check_recieveTransactionData = recieveTransactionData(transData);
    if(check_recieveTransactionData == OK_server)
    {
        Transactions_DataBase[SequenceNumber].cardHolderData = transData->cardHolderData;
        Transactions_DataBase[SequenceNumber].terminalData = transData->terminalData;
        Transactions_DataBase[SequenceNumber].transactionSequenceNumber = SequenceNumber;
        Transactions_DataBase[SequenceNumber].transState = APPROVED;
        SequenceNumber++;
        return OK_server;
    }

    else if( (check_recieveTransactionData == EXPIRED_CARD_SS) || (check_recieveTransactionData == EXCEED_MAX_AMOUNT_SS) || (check_recieveTransactionData == FRAUD_CARD) || (check_recieveTransactionData == DECLINED_STOLEN_CARD) || (check_recieveTransactionData == DECLINED_INSUFFECIENT_FUND) )
    {
        Transactions_DataBase[SequenceNumber].cardHolderData = transData->cardHolderData;
        Transactions_DataBase[SequenceNumber].terminalData = transData->terminalData;
        Transactions_DataBase[SequenceNumber].transactionSequenceNumber = SequenceNumber;

        if(check_recieveTransactionData == EXPIRED_CARD_SS)
        {
            Transactions_DataBase[SequenceNumber].transState = EXPIRED_CARD_SS;
            SequenceNumber++;
            return EXPIRED_CARD_S;
        }


        else if(check_recieveTransactionData == EXCEED_MAX_AMOUNT_SS)
        {
            Transactions_DataBase[SequenceNumber].transState = EXCEED_MAX_AMOUNT_SS;
            SequenceNumber++;
            return EXCEED_MAX_AMOUNT_S;
        }


        else if(check_recieveTransactionData == FRAUD_CARD)
        {
            Transactions_DataBase[SequenceNumber].transState = FRAUD_CARD;
            SequenceNumber++;
            return ACCOUNT_NOT_FOUND;
        }


        else if(check_recieveTransactionData == DECLINED_STOLEN_CARD)
        {
            Transactions_DataBase[SequenceNumber].transState = DECLINED_STOLEN_CARD;
            SequenceNumber++;
            return BLOCKED_ACCOUNT;
        }


        else if(check_recieveTransactionData == DECLINED_INSUFFECIENT_FUND)
        {
            Transactions_DataBase[SequenceNumber].transState = DECLINED_INSUFFECIENT_FUND;
            SequenceNumber++;
            return LOW_BALANCE;
        }

    }

    else /*If the transaction can't be saved , for any reason (ex: dropped connection)
           will return SAVING_FAILED and again this is console application so it will not reach to this point*/
        return SAVING_FAILED;

}

EN_serverError_t getTransaction(uint32_t transaction_Sequence_Number, uint8_t size , ST_transaction_t *transData)
{
    bool Transactions_flag = 0;
    /*Transactions_DataBase[255]*/
    for(uint8_t j = 0;  j < size ; j++)
    {
        if(transData[j].transactionSequenceNumber == transaction_Sequence_Number)
        {
            Transactions_flag = 1;
            break;
        }
    }

    if(Transactions_flag == 1)
        return OK_server;
    else if(Transactions_flag == 0)
        return TRANSACTION_NOT_FOUND;

}

EN_serverError_t linearSearch(ST_accountsDB_t *data_base, uint8_t size, ST_cardData_t *cardData)
{
    bool account_num_flag = 0;

    EN_cardError_t check_CardPAN = getCardPAN(cardData);
    EN_terminalError_t check_isValidCardPAN = isValidCardPAN(cardData);

    /* By using Linear search algorithm, we can target string in dataBase[255] */
    for (i = 0; i < size ; i++)
    {
        if(strcmp(data_base[i].primaryAccountNumber , cardData->primaryAccountNumber) == 0)
        {
            account_num_flag = 1;
            break;
        }
    }

    if( (account_num_flag == 1) && (check_CardPAN == OK_card) && (check_isValidCardPAN == OK_terminal) )
        return OK_server;
    else
        return ACCOUNT_NOT_FOUND;
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountRefrence)
{
    if(accountRefrence[i].state == BLOCKED)
       return BLOCKED_ACCOUNT;
    else
        return OK_server;
}

