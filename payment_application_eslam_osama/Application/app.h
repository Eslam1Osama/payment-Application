#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

#include"../Server/server.h"

void appStart(void);
void printArray(ST_accountsDB_t *dataBase, uint8_t size);
void Do_Trans(void);
void Show_DB(void);
void Trans_Elem(void);
void trans_list(void);

#endif // APP_H_INCLUDED
