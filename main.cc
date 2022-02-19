#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
using namespace std;

const int BUFFSIZE = 512;
const std::string ADMIN = "admin";
const std::string HEAD = "head";
const std::string EDUCATOR = "educator";
const std::string STUDENT = "student";

void setPermission(string permission,string filename,string group);

class DisplayInfo {
   public:
    DisplayInfo() {}

    void listUsers(std::string msg = "Available users: ") {
        const std::string red("\033[0;31m");
        const std::string reset("\033[0m");
        std::cout << red << std::endl;
        std::cout << msg << std::endl;
        // lists users having UIDs from 1000 to 1999 only since its user added
        system("cat /etc/passwd | grep -E \"1[0-9]{3}\"");

        std::cout << reset << std::endl;
    }
};

class Utilities {
   public:
    /**
     * @brief used for taking in command's output to pipe through popen()
     * makeing use of IPC. command is used for coomand to excute and get back
     * output to user.
     *
     * @param command
     * @return string
     *
     */
    std::string get_popen(const char *command) {
        FILE *pf;
        char data[BUFFSIZE];
        std::string result;

        // Setup our pipe for reading and execute our command.
        pf = popen(command, "r");

        // Get the data from the process execution
        fgets(data, BUFFSIZE, pf);

        // the data is now in 'data'

        // Error handling
        if (pclose(pf) != 0)
            fprintf(stderr, " Error: Failed to close command stream \n");

        result = data;
        return result;
    }

    /**
     * @brief used for passing commands to pipe through popen() makeing use of
     * IPC. command is used for commands to be executed and output passed.
     *
     * @param command
     * @param data
     *
     */
    void set_popen(const char *command, const char *data) {
        FILE *pf;
        // char data[BUFFSIZE];

        // Setup our pipe for writing to file according to our command.
        pf = popen(command, "w");
        // std::cout<<data<<std::endl;
        // Set the data from the process execution
        fputs(data, pf);

        // the data is now written to file

        // Error handling
        if (pclose(pf) != 0)
            fprintf(stderr, " Error: Failed to close command stream \n");

        return;
    }
};

class User {
   public:
    void add_user(std::string name, DisplayInfo display_info) {

       /*
		std::string adduser = "sudo adduser ";
		std::string cmnd = adduser + name;
		system(cmnd.c_str());  // i.e system("sudo adduser name")
		// system("sudo useradd -m admin_os"); system(""sudo passwd admin_os");
	*/
                string username;
                cout<<"Enter the username: ";
		getline(cin,username);
		std::string adduser = "sudo pw user add ";
		std::string cmnd = adduser + username;
		system(cmnd.c_str());  // i.e system("sudo pw user add name")
		cout<<"Create a password"<<endl;
                std::string password = "sudo passwd "+username;
		system(password.c_str());  // i.e system("sudo pw user add name")
        std::cout << "=== Create done ===" << std::endl;
        display_info.listUsers();
    }

    void delete_user(std::string name) {
        // cleanup and deleting of users
        char y = 'y';
        std::cout << "Delete user " << name << " ?[Y/n]" << std::endl;
        y = getchar();
        if (y == 'y' || y == 'Y' || y == '\n') {
            std::string cmnd;

            std::cout << "Deleting..." << name << std::endl;
            // std::cout<<name.size()<<std::endl;
            std::string userdel = "sudo userdel -r ";
            // cmnd = userdel + name + " >/dev/null 2>&1";
            std::string rmuser = "sudo rmuser -y " + name;
            system(rmuser.c_str());  // system("sudo rmuser -y  name");
            std::cout << "=== Delete done ===" << std::endl;
        }
    }
};

void addGroup(string types)
{
	string cmnd;
        cmnd = "pw groupadd "+types;
        system(cmnd.c_str());
	setPermission("","xdata","everyone@");
        setPermission("","xdata","group@");
	setPermission("","ydata","everyone@");
        setPermission("","ydata","group@");
	setPermission("","zdata","everyone@");
        setPermission("","zdata","group@");
        
	if(types == "students")
	{
	        setPermission("rca","xdata","g:"+types);
		setPermission("","ydata","g:"+types);
		setPermission("","zdata","g:"+types);
  	}
        
	if(types == "heads")
	{
	        setPermission("rca","xdata","g:"+types);
		setPermission("rca","ydata","g:"+types);
		setPermission("rca","zdata","g:"+types);
  	}
        
	if(types == "educators")
	{
	        setPermission("","xdata","g:"+types);
		setPermission("rca","ydata","g:"+types);
		setPermission("","zdata","g:"+types);
  	}
        
	if(types == "admins")
	{
	        setPermission("rwpaRc","xdata","g:"+types);
		setPermission("rwpaRc","ydata","g:"+types);
		setPermission("rwpaRc","zdata","g:"+types);
  	}

} 

void setPermission(string permission,string filename,string group)
{
	string cmnd;
        cmnd = "setfacl -m "+group+":"+permission+"::allow "+filename;
        //cout<<cmnd<<endl;
        system(cmnd.c_str());
}

void toGroup(string group,string user)
{
	string cmnd;
	cmnd = "sudo pw groupmod "+group+" -m "+user;
	system(cmnd.c_str());

}


int main(int argc, char *argv[]) {
    DisplayInfo display_info;
    Utilities utilities;
    User our_user;  // TODO:Have to check the user, how it will work as we
                    // already have "user"

    display_info.listUsers();

    std::string user = utilities.get_popen("id -u -n");
    user.erase(user.end() - 1);
    std::cout << "Current user:  " << user << "\n------------------------\n"
              << std::endl;

    std::string cmnd;
    // build a command string and then pass it to system(const char* command)
    std::string name = ADMIN;

    // Different types of users have different commands
    std::string X = "X", Y = "Y", Z = "Z";

    if (user == ADMIN) {
        std::cout << "Hello Admin\n";
        std::cout << "You have the permission to:\n";
        std::cout << "\tView and Edit data in File X, File Y, File Z.Enter 1 to do it.\n";
        std::cout << "\tAdd students, educators and heads in their respective groups.Enter 2 for it\n";
        std::cout << "\tRestrict and modify permissions of any file or users.\n\n";
     
        cout<<"Enter which operation you want to do:  ";
       char y = 'y';
       y=getchar();
        if(y=='1')
     	{
		cin.ignore();

		std::cout << "Please Enter X" << std::endl;
		getline(std::cin, X);

		std::cout << "Please Enter Y" << std::endl;
		getline(std::cin, Y);

		std::cout << "Please Enter Z" << std::endl;
		getline(std::cin, Z);

		cmnd = "echo \"" + X  + "\"  > " + "xdata";
		utilities.set_popen(cmnd.c_str(), cmnd.c_str());
		cmnd = "echo \"" + Y  + "\"  > " + "ydata";
		utilities.set_popen(cmnd.c_str(), cmnd.c_str());
		cmnd = "echo \"" + Z  + "\"  > " + "zdata";
		utilities.set_popen(cmnd.c_str(), cmnd.c_str());
    	}

	else if(y=='2')
	{
		// std::string groupadd = "sudo usermod -a -G sudo admin";
		// cmnd = groupadd;
		// system(cmnd.c_str());
		bool flag = true;
           		
		while(flag)
		{
		        std::cout << "\nEnter 1 if you want to add student\n";
	       		std::cout << "Enter 2 if you want to add head\n";
	       		std::cout << "Enter 3 if you want to add educator\n";
			
			cout<<"Enter which operation you want to do:  \n";
			cin.ignore();
			string type ;
			getline(cin,type);
			if(type=="1")
			{
				our_user.add_user(STUDENT, display_info);
				toGroup("wheel","student");
	       			 toGroup("students","student");
			}
			else if(type=="2")
			{
				our_user.add_user(HEAD, display_info);
				toGroup("wheel","head");
	       			 toGroup("heads","head");
			}
			else if(type=="3")
			{
				our_user.add_user(EDUCATOR, display_info);
				toGroup("wheel","educator");
	       			 toGroup("educators","educator");
			}

		        std::cout << "\nEnter 1 if you want to add another user\n";
	       		std::cout << "Enter 2 if you want to exit\n";		
			char again = 'y';
			again = getchar();
			if(again=='2')
				flag=false;
		}
	}
    } 
    else if (user == HEAD) {
        std::cout << "Hello Head\n";
        std::cout << "You have the permission to:\n";
        std::cout << "\tView the data in File X, File Y, File Z.\n";

        std::string filedata = utilities.get_popen("cat file.txt");

        X = utilities.get_popen("cat xdata");
        Y = utilities.get_popen("cat ydata");
        Z = utilities.get_popen("cat zdata");
        /*
		int cnt = 0;

		for (int i = 0; i < filedata.size(); i++) {
		    if (filedata[i] == ';') {
		        cnt++;
		        continue;
		    }
		    if (cnt == 0) {
		        X += filedata[i];
		    } else if (cnt == 1) {
		        Y += filedata[i];
		    } else {
		        Z += filedata[i];
		    }
		}
	*/
        std::cout << "Data in File X: " << X << std::endl;
        std::cout << "Data in File Y: " << Y << std::endl;
        std::cout << "Data in File Z: " << Z << std::endl;
    } else if (user == EDUCATOR) {

        std::cout << "Hello Educator\n";
        std::cout << "You have the permission to:\n";
        std::cout << "\tView the data in File Y.\n";

        Y = utilities.get_popen("cat ydata");

        std::cout << "Data in File Y: " << Y << std::endl;
    } else if (user == STUDENT) {

        std::cout << "Hello Student\n";
        std::cout << "You have the permission to:\n";
        std::cout << "\tView the data in File X.\n";


        X =  utilities.get_popen("cat xdata");

        std::cout << "Data in File X: " << X << std::endl;
    } else {
        // the starting user (eg:amit) that initiates the program and adds admin
        addGroup("students");
        addGroup("admins");
        addGroup("educators");
        addGroup("heads");

        cout<<endl<<"If admin not found please add the admin."<<endl;
        our_user.add_user(name, display_info);
        toGroup("wheel","admin");
        toGroup("admins","admin");
    }


    std::cout << "\n---------------------------\n";

//    our_user.delete_user(name);
}
