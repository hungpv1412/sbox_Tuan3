#include <bits/stdc++.h>
#include "user.h"

using namespace std;

 main(){
    while(1){
        cout << "\nWelcome to Messenger\n"
				+ "1->Register\n" + 
				+ "2->Login\n" + 
				+ "3->Inbox\n" + 
				+ "4->View Detail Message\n" + 
				+ "5->Send Message\n" + 
                + "6.Add Friend\n" + 
                + "7.List Friend\n" +
                + "8.Block\n" + 
                + "9.Logout\n" + 
				+ "INPUT NUMBER : ";
		char action;
        cin >> action;
        User user = User();
        switch(action){
            case '1':
                user.Register();
                break;
            case '2':
                user.LogIn();
                break;
            case '3':
				user.ShowMess(user.id);
                break;
            case '4':
                user.ShowMessDetail(user.id);
                break;
            case '5':
                user.SendMess(user.id);
                break;
            case '6':
                user.AddFriend(u.id);
                break;
            case '7':
                user.ShowFriend(u.id);
                break;
            case '8':
                user.BlockFriend(u.id);
                break;
            case '9':
                user.LogOut();
                break;
            default:
                cout << "\nIncorrect\n";
                break;
        }

    }

	return 0;
}
