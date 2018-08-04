#include <iostream>
#include "user.h"
#include<bits/stdc++.h>

using namespace std;

// Connect to SQLite
int ConnectSQlite::callback(void *NotUsed, int argc, char **argv, char **azColName){
           int i;
           for(i=0; i<argc; i++){
              printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
           }
           printf("\n");
           return 0;
        }
void ConnectSQlite::Open(){
        rc = sqlite3_open("tinnhan.db", &db);
       if( rc ){
          fprintf(stderr, "Can't connect to SQLite: %s\n", sqlite3_errmsg(db));
          exit(0);
       }else{

       }
}
void User::Register(){
    username = new (char);
    password = new char;
    fullname = new char;
    birthday = new char;
    gender = new char;
    address = new char;

    cout << "Username: ";
	cin >> username;
	cout << "Password: ";
	cin >> password;
	cout << "Fullname: ";
	cin.ignore(1);
    cin.getline(fullname,30);
	cout << "Birthday: ";
	cin >> birthday;
	cout << "Gender: ";
	cin >> gender;
	cout << "Address: ";
	cin.ignore(1);
    cin.getline(address,30);
	ConnectSQlite conn ;
	conn.Open();
    conn.RegisterSQL(conn.db,username,password,fullname,birthday,gender,address);

}
//INSERT REGISTER
void ConnectSQlite::RegisterSQL(sqlite3 *db, char *username, char *password,char *fullname,char *birthday, char *gender, char *address){
  if (!db)return;
  szSQL = "INSERT INTO user (username, password,fullname,birthday,gender,address) values (?,?,?,?,?,?)";

  int rc = sqlite3_prepare_v2(db, szSQL, strlen(szSQL), &stmt, &pzTest);

  if( rc == SQLITE_OK ) {
    // bind the value
    sqlite3_bind_text(stmt, 1, username, strlen(username), 0);
    sqlite3_bind_text(stmt, 2, password, strlen(password), 0);
    sqlite3_bind_text(stmt, 3, fullname, strlen(fullname), 0);
    sqlite3_bind_text(stmt, 4, birthday, strlen(birthday), 0);
    sqlite3_bind_text(stmt, 5, gender, strlen(gender), 0);
    sqlite3_bind_text(stmt, 6, address, strlen(address), 0);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
  }else{

      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);

  }
}
void  User::LogIn(){
    username = new char;
    password = new char;
    cout << "username: ";
	cin >> username;
	cout << "password: ";
	cin >> password;
	ConnectSQlite conn ;
	conn.Open();

	if(conn.LoginSQL(conn.db,username,password) > 0) {
        cout << "Login successfuly!\n";
        id = conn.LoginSQL(conn.db,username,password);
        flag = 1;
	}
	else {
        cout << "Fail!\n";
        flag = 0;
	}
}

//LOGIN FROM USERNAME AND PASSWORD on SQLite
int ConnectSQlite::LoginSQL(sqlite3 *db,char *username, char *password){
        int id = 0;
        if (!db)return 0;
        szSQL = "SELECT * FROM  user WHERE username = ? and password = ? ";
        rc = sqlite3_prepare(db, szSQL, strlen(szSQL), &stmt, &pzTest);
       if( rc == SQLITE_OK ) {
            // bind the value
            sqlite3_bind_text(stmt, 1, username, strlen(username), 0);
            sqlite3_bind_text(stmt, 2, password, strlen(password), 0);
            // commit
           while(sqlite3_step(stmt) == SQLITE_ROW) {
                stringstream str;
                str << sqlite3_column_text(stmt, 0);
                str >> id ;
            }
            sqlite3_finalize(stmt);


      }else{

          fprintf(stderr, "SQL error: %s\n", zErrMsg);
          sqlite3_free(zErrMsg);
      }
    return id;

}

bool User::isLogIn(){
    ConnectSQlite conn;
    if(flag == 1) return true;
    return false;
}

void User::LogOut(){
    flag = 0;
    cout << "-----Login to Process.-------\n";
}

void User::AddFriend(int id){
    if (isLogIn()){
        cout << "Enter Username: ";
        char *newfr;
        newfr = new char;
        cin >> newfr;
        ConnectSQlite conn;
        conn.Open();
        int id2 = conn.TransNameToId(newfr);
        if(id2 > 0){
            if(conn.CheckBlock(conn.db,id,id2) && !conn.CheckFriend(conn.db,id,id2) ){
                conn.WriteToFriend(conn.db,id,id2);
                cout<<"DONE\n";
            }else {
                cout << "\t Blocked / Friendship \n";
                conn.CheckFriend(conn.db,id,id2);
            }
        }else cout <<  "\nAccount not exist \n";
    }else {
        cout << "\nPlease Login.\n";
    }

}

void ConnectSQlite::WriteToFriend(sqlite3 *db,int id1, int id2){
    if (!db)return ;
     szSQL = "INSERT INTO friend VALUES (?,?,?)";
     rc = sqlite3_prepare_v2(db, szSQL, strlen(szSQL), &stmt, &pzTest);
     if( rc == SQLITE_OK ) {
            // bind the value
        int block = 0;
        sqlite3_bind_int(stmt,1,id1);
        sqlite3_bind_int(stmt,2,id2);
        sqlite3_bind_int(stmt,3,block);

        sqlite3_step(stmt);
        sqlite3_finalize(stmt);

    }else{

          fprintf(stderr, "SQL error: %s\n", zErrMsg);
          sqlite3_free(zErrMsg);

      }
}
void User::ShowFriend(int id){
    if (isLogIn()){
        ConnectSQlite conn;
        conn.Open();
        conn.ShowFriend(conn.db,id);
    }else {
        cout << "\nPlease Login.\n";
    }
}

void User::BlockFriend(int id1){
    if (isLogIn()){
        cout <<"\nBlock user :\n";
        char *name;
        name = new char;
        cin >> name;
        ConnectSQlite conn;
        conn.Open();
        int id2 = conn.TransNameToId(name);
        if(id2 > 0){
            conn.BlockFriend(conn.db,id1,id2);
            cout <<"\nBlock Successfuly\n";
            conn.ShowFriend(conn.db,id1);
        } else cout <<  "\nAcount not Exist.\n";
    }else {
        cout << "\nPlease Login.n\n";
    }
}

int ConnectSQlite::TransNameToId(char *fr){
    int id2 = -1;
    szSQL = "SELECT id FROM user WHERE username = ?";
    rc = sqlite3_prepare_v2(db, szSQL, strlen(szSQL), &stmt, &pzTest);

    if( rc == SQLITE_OK ){
        sqlite3_bind_text(stmt,1,fr,strlen(fr),0);
        if(sqlite3_step(stmt) == SQLITE_ROW) {
            stringstream str;
            str << sqlite3_column_text(stmt, 0);
            str >> id2 ;

        }
        sqlite3_finalize(stmt);
    }else{
          fprintf(stderr, "SQL error: %s\n",zErrMsg);
          sqlite3_free(zErrMsg);
    }
    return id2;

}
bool ConnectSQlite::CheckFriend(sqlite3 *db, int id1,int id2){
    int isFriend = 0;
    szSQL = "SELECT * FROM friend WHERE ( id1 = ? AND id2 = ?) OR ( id1 = ? AND id2 = ?)";
    rc = sqlite3_prepare_v2(db, szSQL, strlen(szSQL), &stmt, &pzTest);
      if( rc == SQLITE_OK ) {

        sqlite3_bind_int(stmt,1,id1);
        sqlite3_bind_int(stmt,2,id2);
        sqlite3_bind_int(stmt,3,id2);
        sqlite3_bind_int(stmt,4,id1);

       if(sqlite3_step(stmt) == SQLITE_ROW ){
           stringstream str;
            str << sqlite3_column_text(stmt, 0);
            str >> isFriend;

            if(isFriend != 0)
                isFriend = 1;
            else isFriend = 0;
            sqlite3_finalize(stmt);
        }
    }else{
          fprintf(stderr, "SQL error: %s\n", zErrMsg);
          sqlite3_free(zErrMsg);

      }
      return isFriend;
}

void ConnectSQlite::ShowFriend(sqlite3 *db,int id1){
    cout << "List Friends\n";
    if (!db)return ;
    szSQL = "SELECT DISTINCT user.username FROM"
            "(SELECT * FROM friend where ((id1 = ? OR id2 = ?) AND block = 0)) as A "
              "LEFT JOIN user ON (A.id2 = user.id OR A.id1 = user.id)";
    rc = sqlite3_prepare_v2(db, szSQL, strlen(szSQL), &stmt, &pzTest);
    if( rc == SQLITE_OK ) {

        sqlite3_bind_int(stmt,1,id1);
        sqlite3_bind_int(stmt,1,id1);

        while(sqlite3_step(stmt) == SQLITE_ROW) {
           cout <<"- ";
           cout<< string((const char *) sqlite3_column_text(stmt, 0)) << endl;
        }
        sqlite3_finalize(stmt);
            cout <<"\n";
    }else{

          fprintf(stderr, "SQL error: %s\n", zErrMsg);
          sqlite3_free(zErrMsg);

      }
}
void ConnectSQlite::BlockFriend(sqlite3 *db,int id1, int id2){
     if (!db)return ;
     szSQL = "UPDATE friend SET block = 1 WHERE id1 = ? AND id2 = ?";
     rc = sqlite3_prepare_v2(db, szSQL, strlen(szSQL), &stmt, &pzTest);
     if( rc == SQLITE_OK ) {

        sqlite3_bind_int(stmt,1,id1);
        sqlite3_bind_int(stmt,2,id2);

        sqlite3_step(stmt);
        sqlite3_finalize(stmt);

    }else{

          fprintf(stderr, "SQL error: %s\n", zErrMsg);
          sqlite3_free(zErrMsg);

      }
}
bool ConnectSQlite::CheckBlock(sqlite3 *db,int id1, int id2){
    int isblock = 0;
     if (!db)return 0;
     szSQL = "SELECT block FROM friend WHERE (id1 = ? AND id2 = ?) OR (id1 = ? AND id2 = ?)";
     rc = sqlite3_prepare_v2(db, szSQL, strlen(szSQL), &stmt, &pzTest);
      if( rc == SQLITE_OK ) {

        sqlite3_bind_int(stmt,1,id1);
        sqlite3_bind_int(stmt,2,id2);
        sqlite3_bind_int(stmt,3,id2);
        sqlite3_bind_int(stmt,4,id1);

       if(sqlite3_step(stmt) == SQLITE_ROW ){
           stringstream str;
            str << sqlite3_column_text(stmt, 0);
            str >> isblock ;
            sqlite3_finalize(stmt);
        }
    }else{

          fprintf(stderr, "SQL error: %s\n", zErrMsg);
          sqlite3_free(zErrMsg);

      }
      return !isblock;
}
void User::SendMess(int id){
    if (isLogIn()){
        int id2;
        cout << "Send To: ";
        char *frien;
        frien = new char;
        cin >> frien;
        ConnectSQlite conn;
        conn.Open();
        id2 = conn.TransNameToId(frien);
        if(id2 > 0){
            char *mess,*dt,*title;
            mess = new char;
            dt =new char;
            title= new char;
            if(!conn.CheckBlock(conn.db,id,id2)){
                        cout << "Blocked\n";
            }else{
                cout << "Title:";
                cin.ignore(1);
                cin.getline(title,10);
                cout << "Content:\n";
                cin.getline(mess,120);
                 time_t hientai = time(0);
                 dt = ctime(&hientai);
                 conn.WriteToMess(conn.db,id,id2,mess,dt,title);
                 cout<<"SEND\n";

            }
        }else cout <<  "Account not exist\n";

    }else {
        cout << "Please Login\n\n";
    }
}
void ConnectSQlite::WriteToMess(sqlite3 *db,int id1, int id2,char *content,char *time,char *title){
         szSQL = "INSERT INTO message VALUES (?,?,?,?,?)";
         rc = sqlite3_prepare_v2(db, szSQL, strlen(szSQL), &stmt, &pzTest);
         if( rc == SQLITE_OK ) {
                // bind the value
            sqlite3_bind_int(stmt,1,id1);
            sqlite3_bind_int(stmt,2,id2);
            sqlite3_bind_text(stmt,3,content,strlen(content),0);
            sqlite3_bind_text(stmt,4,time,strlen(time),0);
            sqlite3_bind_text(stmt,5,title,strlen(title),0);
            // commit
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);

        }else{

              fprintf(stderr, "SQL error: %s\n", zErrMsg);
              sqlite3_free(zErrMsg);
              // break the loop if error occur
          }

}
void User::ShowMess(int id){
    if (isLogIn()){
        ConnectSQlite conn;
        conn.Open();
        conn.SelectMessenger(conn.db,id);
    }else {
        cout << "Please Login\n\n";
    }
}
void ConnectSQlite::SelectMessenger(sqlite3 *db, int id){
    if (!db)return ;
    szSQL ="SELECT DISTINCT title FROM"
                "(SELECT * FROM message where idsen = ? ) as A LEFT JOIN user ON A.idrec = user.id";
    rc = sqlite3_prepare_v2(db, szSQL, strlen(szSQL), &stmt, &pzTest);
    if( rc == SQLITE_OK ) {
            // bind the value
        sqlite3_bind_int(stmt,1,id);

        // commit
        while(sqlite3_step(stmt) == SQLITE_ROW) {
            answer.push_back(string((const char *) sqlite3_column_text(stmt, 0)));
        }
        sqlite3_finalize(stmt);
        cout <<"List Message SENT\n" ;
        for( list<std::string>::const_iterator it = answer.begin(); it != answer.end(); it++)

            cout << "-Title:"<<*it << endl;

    }else{

          fprintf(stderr, "SQL error: %s\n", zErrMsg);
          sqlite3_free(zErrMsg);
          // break the loop if error occur
      }
}
void ConnectSQlite::ShowMessDetail(sqlite3 *db,int id1,int id2){
    if (!db)return ;
    cout << "Detail:\n";
    szSQL = "SELECT * FROM (SELECT * FROM message WHERE (idsen = ? OR idsen = ?) AND (idrec = ? OR idrec =?) ) AS B LEFT JOIN  user ON user.id = B.idsen";
    rc = sqlite3_prepare_v2(db, szSQL, strlen(szSQL), &stmt, &pzTest);
    if( rc == SQLITE_OK ) {
            // bind the value
        sqlite3_bind_int(stmt,1,id1);
        sqlite3_bind_int(stmt,2,id2);
        sqlite3_bind_int(stmt,3,id1);
        sqlite3_bind_int(stmt,4,id2);
        // commit
        while(sqlite3_step(stmt) == SQLITE_ROW) {

                cout << " Send To: " << string((const char *) sqlite3_column_text(stmt, 5)) << "" <<  endl;
                cout << " Content: "<< string((const char *) sqlite3_column_text(stmt, 2)) << "\n" ;
                cout << " Time: "<<string((const char *) sqlite3_column_text(stmt, 3)) << endl ;

        }
        sqlite3_finalize(stmt);

    }else{

          fprintf(stderr, "SQL error: %s\n", zErrMsg);
          sqlite3_free(zErrMsg);
          // break the loop if error occur
      }
}
void User::ShowMessDetail(int id1){
    if (isLogIn()){
        int id2;
        cout << "Enter username: ";
        char *frien;
        frien = new char;
        cin >> frien;
        ConnectSQlite conn;
        conn.Open();
        id2 = conn.TransNameToId(frien);

        if(id2 > 0)
        conn.ShowMessDetail(conn.db,id1,id2);
        else cout <<  "Account not Exist\n";
    }else {
        cout << "Please Login\n";
    }

}
