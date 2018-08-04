#include <iostream>
#include "sqlite3.h"
#include<bits/stdc++.h>


using namespace std;
class User{
	public:
		int  id;
		char *username;
		char *password;
		char *birthday;
		char *gender;
		char *fullname;
		char *address;
		int flag ;

	public :
        User(){};
		void Register();
        void LogIn();
        bool isLogIn(); 
        void LogOut();
        bool Check(User u);
        void ShowMess(int);
        void ShowMessDetail(int);
        void SendMess(int);
        void AddFriend(int);
        void ShowFriend(int);
        void BlockFriend(int);

};

class ConnectSQlite{
    public:
        sqlite3 *db;
        char *zErrMsg = 0;
        int rc;
        const char *szSQL;
        const char *pzTest;
        sqlite3_stmt *stmt;
        list<string> answer;
    public :
        static int callback(void *,int,char **,char **);
        void Open();
        //MODULE1
        void RegisterSQL(sqlite3 *db,char *, char *,char *,char *,char *,char *);
        int LoginSQL(sqlite3 *db, char *username, char *password);
        int TransNameToId(char *fr);
        bool CheckAccount(sqlite3 *db,char *username);
        //MODULE 2
        void SelectMessenger(sqlite3 *db, int id);
        void ShowMessDetail(sqlite3 *db,int id1,int id2);
        void WriteToMess(sqlite3 *db,int id1, int id2,char *content,char *time,char *title);
         //MODULE 3
        bool CheckFriend(sqlite3 *db, int id1,int id2);
        void WriteToFriend(sqlite3 *db,int id1, int id2);
        void ShowFriend(sqlite3 *db,int id1);
        void BlockFriend(sqlite3 *db,int id1, int id2);
        bool CheckBlock(sqlite3 *db,int id1, int id2);

};

