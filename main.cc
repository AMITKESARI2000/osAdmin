#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>

const int BUFFSIZE = 512;
const std::string ADMIN = "admin";
const std::string HEAD = "head";
const std::string EDUCATOR = "educator";
const std::string STUDENT = "student";

void listUsers(std::string msg = "Available users: "){

    std::cout << msg << std::endl;
    // lists users having UIDs from 1000 to 1999 only since its user added
    system("cat /etc/passwd | grep -E \"1[0-9]{3}\"");
}

/**
 * @brief used for taking in command's output to pipe through popen() makeing use of IPC.
 *          command is used for coomand to excute and get back output to user.
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
    fgets(data, BUFFSIZE , pf);

    // the data is now in 'data'

    // Error handling
    if (pclose(pf) != 0)
        fprintf(stderr," Error: Failed to close command stream \n");

    result = data;
    return result;
}

/**
 * @brief used for passing commands to pipe through popen() makeing use of IPC.
 *          command is used for commands to be executed and output passed.
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


    // Set the data from the process execution
    fputs(data , pf);

    // the data is now written to file

    // Error handling
    if (pclose(pf) != 0)
        fprintf(stderr," Error: Failed to close command stream \n");

    return;
}


int main(int argc, char *argv[]){

    listUsers();
    std::cout << std::endl;

    std::string user = get_popen("id -u -n");
    std::cout << "Current user:  "<<user << "\n------------------------\n" << std::endl;

    // Different types of users have differnt commands
    std::string X = "X", Y = "Y", Z = "Z";
    if(user.compare(ADMIN)){

        std::string cmnd;
        std::cout << "I am currently " << user <<"!" << std::endl;
        std::cout << "Please Enter name X" << std::endl;
        std::cin >> X;
        std::cout << "Please Enter name Y" << std::endl;
        std::cin >> Y;
        std::cout << "Please Enter name Z" << std::endl;
        std::cin >> Z;

        cmnd = "echo " + X + Y + Z + " > file.txt";
        set_popen(cmnd.c_str(), cmnd.c_str());
    }
    else if(user.compare(HEAD)){

        std::cout << "I am currently " << user <<"! Please access: " << X << " " << Y << " " << Z<< std::endl;
    }
    else if(user.compare(EDUCATOR)){

        std::cout << "I am currently " << user <<"! Please access: " << Y << std::endl;
    }
    else if(user.compare(STUDENT)){

        std::cout << "I am currently " << user <<"! Please access" << X << std::endl;
    }




    std::string cmnd;
    // build a command string and then pass it to system(const char* command)
    std::string name = ADMIN;
    std::string adduser = "sudo adduser ";
    cmnd = adduser + name;
    system(cmnd.c_str());           // i.e system("sudo adduser name")
    // system("sudo useradd -m admin_os"); system(""sudo passwd admin_os");
    std::cout << "=== Create done ===" << std::endl;
    listUsers();    


    std::string groupadd = "sudo groupadd groupname";
    std::string groupdel = "sudo groupdel groupname";
    // std::string su = "sudo su ";
    // cmnd = su + name;
    // system(cmnd.c_str());
    std::string whoami = "whoami";
    cmnd = whoami; // TODO : opens a new shell and commands not executing in it
    system(cmnd.c_str());


    // cleanup and deleting of users
    char y = 'y';

    std::cout << "Delete user?[Y/n]" << std::endl;
    y = getchar();
    if(y == 'y' || y == '\n'){
        std::cout << "Deleting..." << name << std::endl;

        std::string userdel = "sudo userdel -r ";
        cmnd = userdel + name + " >/dev/null 2>&1";
        system(cmnd.c_str());     // system("sudo userdel -r name");
        std::cout << "=== Delete done ===" << std::endl;
    }

    // execl("/bin/sh", "sh", "-c", command, (char *) 0);
}
