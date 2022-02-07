#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>

void listUsers(std::string msg = "Available users: "){

    std::cout << msg << std::endl;
    // lists users having UIDs from 1000 to 1999 only since its user added
    system("cat /etc/passwd | grep -E \"1[0-9]{3}\"");
}

int main(int argc, char *argv[]){

    listUsers();
    std::cout << "Current user: " << std::endl;
    system("id -u -n");
    std::cout << std::endl;

    system("ls -halt");


    std::string cmnd;
    // build a command string and then pass it to system(const char* command)
    std::string name = "admin";
    std::string adduser = "sudo adduser ";
    cmnd = adduser + name;
    system(cmnd.c_str());           // i.e system("sudo adduser name")
    // system("sudo useradd -m admin_os"); system(""sudo passwd admin_os");
    std::cout << "=== Create done ===" << std::endl;
    listUsers();    


    std::string su = "sudo su ";
    cmnd = su + name;
    system(cmnd.c_str());
    std::string whoami = "whoami";
    cmnd = whoami; // TODO : opens a new shell and commands not executing in it
    system(cmnd.c_str());


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
