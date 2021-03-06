#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

const int BUFFSIZE = 512;
const std::string ADMIN = "admin";
const std::string HEAD = "head";
const std::string EDUCATOR = "educator";
const std::string STUDENT = "student";

const std::string ADMIN_GROUP = "admins";
const std::string HEAD_GROUP = "heads";
const std::string EDUCATOR_GROUP = "educators";
const std::string STUDENT_GROUP= "students";

// different colour pallete for colour printing, remember to use reset at last
const std::string red("\033[0;31m");
const std::string green("\033[1;32m");
const std::string yellow("\033[1;33m");
const std::string cyan("\033[0;36m");
const std::string magenta("\033[0;35m");
const std::string reset("\033[0m");

vector <std::string> names={"aditya","amit","anand","sowmya"};
vector <std::string> educators = {"edu1", "edu2", "edu3", "edu4"};
vector <std::string> data= {"./data/11", "./data/12", "./data/13", "./data/14", "./data/21", "./data/22", "./data/23", "./data/24", "./data/31", "./data/32", "./data/33", "./data/34", "./data/41", "./data/42", "./data/43", "./data/44"};

void set_permission(string permission,string filename,string group);

class DisplayInfo {
	public:

		DisplayInfo() {}

		void listUsers(std::string msg = "Available users: ") {
			std::cout << green << std::endl;
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
		 * makeing use of IPC. command paramter is used for coomand to excute and get back
		 * output to user.
		 *
		 * @param command
		 * @return string
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
		 * IPC. command paramter is used for commands to be executed and output passed.
		 *
		 * @param command
		 * @param data
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

		/**
		 * @brief adds user to the system, along with password.
		 *
		 * @param name
		 * @param display_info
		 * 
		 */
		void add_user(std::string username, DisplayInfo display_info) {

			/*
			   std::string adduser = "sudo adduser ";
			   std::string cmnd = adduser + name;
			   system(cmnd.c_str());  // i.e system("sudo adduser name")
			// system("sudo useradd -m admin_os"); system(""sudo passwd admin_os");
			*/
			std::cout << cyan << std::endl;
			std::string adduser = "sudo pw user add ";
			std::string cmnd = adduser + username;
			system(cmnd.c_str());  // i.e system("sudo pw user add <username>")

			std::cout << "Create a password" << std::endl;
			std::string password = "sudo passwd " + username;
			system(password.c_str());  // i.e system("sudo pw user add name")
			std::cout << "=== Create done ===" << std::endl;
			display_info.listUsers();

			std::cout << reset << std::endl;
		}

		/**
		 * @brief cleanup and deleting of users
		 *
		 * @param name
		 */
		void delete_user(std::string name) {
			std::cout << red << std::endl;
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

			std::cout << reset << std::endl;
		}
};

void add_new_group(string types) {
	string cmnd;
	cmnd = "pw groupadd "+types;
	system(cmnd.c_str());
	// set_permission("","xdata","everyone@");
	// set_permission("","xdata","group@");
	// set_permission("","ydata","everyone@");
	// set_permission("","ydata","group@");
	// set_permission("","zdata","everyone@");
	// set_permission("","zdata","group@");
	for(int i=0;i<data.size();i++)
	{
		set_permission("",data[i],"everyone@");
		set_permission("",data[i],"group@");
	}


	if(types == STUDENT_GROUP) {
		// set_permission("rca","xdata","g:"+types);
		// set_permission("","ydata","g:"+types);
		// set_permission("","zdata","g:"+types);
		for(int i=0;i<data.size();i++)
		{
			set_permission("", data[i], "g:" + types);
			set_permission("rca",data[i],"u:" + names[int(data[i][7]-'0')-1]);
		}
	}

	if(types == HEAD_GROUP) {
		// set_permission("rca","xdata","g:"+types);
		// set_permission("rca","ydata","g:"+types);
		// set_permission("rca","zdata","g:"+types);
		for(int i=0;i<data.size();i++)
		{
			set_permission("rwpaRc", data[i], "g:" + types);
		}
	}

	if(types ==EDUCATOR_GROUP) {
		// set_permission("","xdata","g:"+types);
		// set_permission("rca","ydata","g:"+types);
		// set_permission("","zdata","g:"+types);
		for(int i=0;i<data.size();i++)
		{
			set_permission("", data[i], "g:" + types);
			set_permission("rwpaRc", data[i], "u:" + educators[int(data[i][8] - '0') - 1]);
		}
	}

	if(types == ADMIN_GROUP) {
		// set_permission("rwpaRc","xdata","g:"+types);
		// set_permission("rwpaRc","ydata","g:"+types);
		// set_permission("rwpaRc","zdata","g:"+types);
		for(int i=0;i<data.size();i++)
		{
			set_permission("rwpaRc", data[i], "g:" + types);
		}
	}

} 

void set_permission(string permission, string filename, string group) {
	string cmnd;
	cmnd = "setfacl -m "+group+":"+permission+"::allow "+filename;
	//cout<<cmnd<<endl;
	system(cmnd.c_str());
}

void to_group(string group, string user) {
	string cmnd;
	cmnd = "sudo pw groupmod "+group+" -m "+user;
	system(cmnd.c_str());

}

class Mainmenu {
	public:
		DisplayInfo display_info;
		Utilities utilities;
		User our_user; 

		string studentsFilename = "students.txt";
		string educatorsFilename = "educators.txt";
		Mainmenu() {
			call_menu();
		}

		void call_menu() {

			display_info.listUsers();

			std::string cmnd;
			// build a command string and then pass it to system(const char* command)
			std::string name = ADMIN;
			//    our_user.delete_user(name);

			std::string user = utilities.get_popen("id -u -n");
			user.erase(user.end() - 1);
			std::cout <<"\n------------------------\n";
			std::cout << "Current user:  " << user ; 
			std::cout <<"\n------------------------\n";         

			std::string group = utilities.get_popen("groups");
			string group_type="";
			for(int i=group.length()-1;i>=0;i--)
			{
				if(int(group[i])==32)
					break;
				else if(int(group[i])==10)
					continue;
				else
					group_type+=group[i];
			}
			reverse(group_type.begin(),group_type.end());

			// Different types of users have different commands
			std::string X = "X", Y = "Y", Z = "Z";

			std::cout << magenta << std::endl;

			if (group_type == ADMIN_GROUP) {
				bool f = true;
				while(f) {
					std::cout << "Hello " << ADMIN << "\n";

					std::cout << "You have the permission to:\n";
					std::cout << "1. \tView data in File X, File Y, File Z.\n";
					std::cout << "2. \tEdit data in File X, File Y, File Z.\n";
					std::cout << "3. \tAdd students, educators and heads in their respective groups.\n";
					std::cout << "4. \tDelete students, educators and heads along with their respective groups and home directories.\n";
					std::cout << "5. \tRestrict and modify permissions of any file or users.\n\n";

					cout<<"Enter which operation you want to do:  ";
					char y = 'y';
					//y = getchar();
					cin>>y;
					if(y == '1') {

						X = utilities.get_popen("cat xdata");
						Y = utilities.get_popen("cat ydata");
						Z = utilities.get_popen("cat zdata");

						std::cout << "Data in File X: " << X << std::endl;
						std::cout << "Data in File Y: " << Y << std::endl;
						std::cout << "Data in File Z: " << Z << std::endl;
					}

					else if(y == '2') {
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

					else if(y == '3') {
						// std::string groupadd = "sudo usermod -a -G sudo admin";
						// cmnd = groupadd;
						// system(cmnd.c_str());
						bool flag = true;

						while(flag) {
							cout<<"\nEnter which operation you want to do:  \n";
							std::cout << "1. \tAdd head\n";
							std::cout << "2. \tAdd educator\n";
							std::cout << "3. \tAdd student\n";

							cin.ignore();
							string type;
							getline(cin, type);
							if(type=="1") {
								std::cout << cyan << std::endl;
								string username;
								std::cout << "Enter the new username: ";
								getline(cin, username);
								our_user.add_user(username, display_info);
								to_group("wheel", username);
								to_group(HEAD_GROUP, username);
							}

							else if(type=="2") {

								std::cout << cyan << std::endl;
								string username;
								std::cout << "Enter the new username: ";
								getline(cin, username);
								string cmnd = "echo " + username + " >>" +educatorsFilename;
								system(cmnd.c_str());
								our_user.add_user(username, display_info);
								string studentsFilename = "students.txt";
								string educatorsFilename = "educators.txt";

								ifstream fin(studentsFilename, ios::in);

								string stu;

								names.clear();
								educators.clear();
								while(getline(fin, stu)){        
									names.push_back(string(stu));

								}
								fin.close();

								fin.open(educatorsFilename, ios::in);
								string edu;

								while(getline(fin, edu)){
									educators.push_back(edu);

								}
								fin.close();

								for(int i = 0; i < names.size(); i++){
									string datFilename = "./data/"+ to_string(i+1) + to_string(educators.size());
									cmnd = "echo 0 >" + datFilename;
									system(cmnd.c_str());
									data.push_back(datFilename);
									set_permission("",datFilename,"everyone@");
									set_permission("",datFilename,"group@");
									set_permission("", datFilename, "g:" + STUDENT_GROUP);
									set_permission("rca",datFilename,"u:" + names[i]);

									set_permission("rwpaRc", datFilename, "g:" + ADMIN_GROUP);
									set_permission("rwpaRc", datFilename, "g:" + HEAD_GROUP);
									set_permission("", datFilename, "g:" + EDUCATOR_GROUP);
									set_permission("rwpaRc", datFilename, "u:" + username);

								}


								to_group("wheel", username);
								to_group(EDUCATOR_GROUP, username);
							}

							else if(type=="3") {
								std::cout << cyan << std::endl;
								string username;
								std::cout << "Enter the new username: ";
								getline(cin, username);
								string cmnd = "echo " + username + " >>" +studentsFilename;
								system(cmnd.c_str());
								our_user.add_user(username, display_info);

								string studentsFilename = "students.txt";
								string educatorsFilename = "educators.txt";

								ifstream fin(studentsFilename, ios::in);

								string stu;

								names.clear();
								educators.clear();
								while(getline(fin, stu)){        
									names.push_back(string(stu));

								}
								fin.close();

								fin.open(educatorsFilename, ios::in);
								string edu;

								while(getline(fin, edu)){
									educators.push_back(edu);

								}
								fin.close();

								for(int i = 0; i < educators.size(); i++){
									string datFilename = "./data/"+ to_string(names.size()) + to_string(i+1);
									cmnd = "echo 0 >" + datFilename;
									system(cmnd.c_str());
									data.push_back(datFilename);
									set_permission("",datFilename,"everyone@");
									set_permission("",datFilename,"group@");
									set_permission("", datFilename, "g:" + STUDENT_GROUP);
									set_permission("rca",datFilename,"u:" + username);
									set_permission("rwpaRc", datFilename, "g:" + ADMIN_GROUP);
									set_permission("rwpaRc", datFilename, "g:" + HEAD_GROUP);
									set_permission("", datFilename, "g:" + EDUCATOR_GROUP);
									set_permission("rwpaRc", datFilename, "u:" + educators[i]);

								}



								to_group("wheel", username);
								to_group(STUDENT_GROUP, username);
							}

							std::cout << "\nEnter 1, if you want to add another user\n";
							std::cout << "Enter 2, if you want to continue?\n";		
							cin.ignore();
							char again = 'y';
							again = getchar();
							if(again=='2')
								flag=false;
						}
					}
					else if(y == '4') {

						//delete user
						display_info.listUsers();

						std::cout << "\nEnter username of the user you wish to delete: \n";

						cin.ignore();
						std::string username;
						getline(cin, username);

						string studentsFilename = "students.txt";
						string educatorsFilename = "educators.txt";

						ifstream fin(studentsFilename, ios::in);

						string stu;

						names.clear();
						educators.clear();
						while(getline(fin, stu)){        
							names.push_back(string(stu));

						}
						fin.close();

						fin.open(educatorsFilename, ios::in);
						string edu;

						while(getline(fin, edu)){
							educators.push_back(edu);

						}
						fin.close();

						auto it = std::find (names.begin(), names.end(), username);
						if (it != names.end())
						{
							// deleting a student
							int index = it - names.begin();
							string cmnd = "rm ";
							for(int i = 0; i < educators.size(); i++){
								cmnd += "./data/" + to_string( index + 1 ) + to_string(i+1) +" ";
							}
							system(cmnd.c_str());

							names.erase(it);
							std::ofstream fout;
							fout.open(studentsFilename);
							for(int i = 0; i < names.size(); i++){
								fout << names[i]<<"\n";
							}
							fout.close();
							

						}
						else{
							it = std::find (educators.begin(), educators.end(), username);
							if(it != educators.end()){
								// deleting a educator
								int index = it - educators.begin();
								string cmnd = "rm ";
								for(int i = 0; i < names.size(); i++){
									cmnd += "./data/" + to_string(i+1) + to_string( index + 1 ) +" ";
								}
								system(cmnd.c_str());

							educators.erase(it);
							std::ofstream fout;
							fout.open(educatorsFilename);
							for(int i = 0; i < educators.size(); i++){
								fout << educators[i]<<"\n";
							}
							fout.close();

							}else	
								std::cout << "User not found.\n\n";
						}    


						our_user.delete_user(username);
					}
					std::cout << "Do you want to continue? [y/N] \n";
					char cont;
					cin>>cont;
					if (cont == 'N')
						f = false;
				}
			} 
			else if (group_type == HEAD_GROUP) {
				bool f = true;
				while( f ) {
					std::cout << "Hello " << HEAD << "\n";
					std::cout << "You have the permission to:\n";
					std::cout << "1. \tView the data in File X, File Y, File Z.\n";


					X = utilities.get_popen("cat xdata");
					Y = utilities.get_popen("cat ydata");
					Z = utilities.get_popen("cat zdata");
					std::cout << "Data in File X: " << X << std::endl;
					std::cout << "Data in File Y: " << Y << std::endl;
					std::cout << "Data in File Z: " << Z << std::endl;
					std::cout << "Do you want to continue? [y/N] \n";
					char cont = getchar();
					if (cont =='N')
						f = false;
				}
			} else if (group_type == EDUCATOR_GROUP) {

				bool f = true;
				while(f) {
					std::cout << "Hello Educator\n";
					std::cout << "You have the permission to:\n";
					std::cout << "\tView the data in File Y.\n";

					Y = utilities.get_popen("cat ydata");

					std::cout << "Data in File Y: " << Y << std::endl;
					std::cout << "Do you want to continue? [y/N] \n";
					char cont = getchar();
					if (cont == 'N')
						f = false;
				}
			} else if (group_type == STUDENT_GROUP) {
				bool f = true;
				while(f) {
					std::cout << "Hello Student\n";
					std::cout << "You have the permission to:\n";
					std::cout << "\tView the data in File X.\n";


					X =  utilities.get_popen("cat xdata");

					std::cout << "Data in File X: " << X << std::endl;
					std::cout << "Do you want to continue? [y/N] \n";
					char cont = getchar();
					if (cont == 'N')
						f = false;
				}
			} else {
				// the starting user (eg:amit) that initiates the program and adds admin
				add_new_group(STUDENT_GROUP);
				add_new_group(ADMIN_GROUP);
				add_new_group(EDUCATOR_GROUP);
				add_new_group(HEAD_GROUP);

				std::cout << "\nIf " << ADMIN << " not found in above users list, please add the user: " << ADMIN << std::endl;
				string username;
				std::cout << cyan << std::endl;
				std::cout << "Enter the new username: ";
				getline(cin, username);
				our_user.add_user(username, display_info);
				to_group("wheel", username);
				to_group(ADMIN_GROUP, username);
			}


			std::cout << "\n---------------------------\n";

			std::cout << reset << std::endl;

		}
};

int main(int argc, char *argv[]) {
	Mainmenu mainmenu;
	// TODO:Have to check the user, how it will work as we already have "user"

}
