#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

const int BUFFSIZE = 512;
const std::string ADMIN = "admin";
const std::string HEAD = "head";
const std::string EDUCATOR = "educator";
const std::string STUDENT = "student";

void listUsers(std::string msg = "Available users: ") {
    const std::string red("\033[0;31m");
    const std::string reset("\033[0m");
    std::cout << red << std::endl;
    std::cout << msg << std::endl;
    // lists users having UIDs from 1000 to 1999 only since its user added
    system("cat /etc/passwd | grep -E \"1[0-9]{3}\"");

    std::cout << reset << std::endl;
}

/**
 * @brief used for taking in command's output to pipe through popen() makeing
 * use of IPC. command is used for coomand to excute and get back output to
 * user.
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
    fputs(data, pf);

    // the data is now written to file

    // Error handling
    if (pclose(pf) != 0)
        fprintf(stderr, " Error: Failed to close command stream \n");

    return;
}

void add_user(std::string name) {
    std::string adduser = "sudo adduser ";
    std::string cmnd = adduser + name;
    system(cmnd.c_str());  // i.e system("sudo adduser name")
    // system("sudo useradd -m admin_os"); system(""sudo passwd admin_os");
    std::cout << "=== Create done ===" << std::endl;
    listUsers();
}

void delete_user(std::string name) {
    // cleanup and deleting of users
    char y = 'y';

    std::cout << "Delete user " << name <<" ?[Y/n]" << std::endl;
    y = getchar();
    if (y == 'y' || y=='Y' || y == '\n') {
        std::string cmnd;

        std::cout << "Deleting..." << name << std::endl;

        std::string userdel = "sudo userdel -r ";
        cmnd = userdel + name + " >/dev/null 2>&1";
        system(cmnd.c_str());  // system("sudo userdel -r name");
        std::cout << "=== Delete done ===" << std::endl;
    }
}

int main(int argc, char *argv[]) {
    listUsers();

    std::string user = get_popen("id -u -n");
    user.erase(user.end() - 1);
    std::cout << "Current user:  " << user << "\n------------------------\n"
              << std::endl;

    std::string cmnd;
    // build a command string and then pass it to system(const char* command)
    std::string name = EDUCATOR;

    // Different types of users have different commands
    std::string X = "X", Y = "Y", Z = "Z";

    if (user == ADMIN) {
        std::cout << "I am currently " << user << "!" << std::endl;
        std::cout << "Please Enter name X" << std::endl;
        std::cin >> X;
        std::cout << "Please Enter name Y" << std::endl;
        std::cin >> Y;
        std::cout << "Please Enter name Z" << std::endl;
        std::cin >> Z;

        std::string filename = "file.txt";
        cmnd = "echo " + X + ";" + Y + ";" + Z + " > " + filename;
        set_popen(cmnd.c_str(), cmnd.c_str());

        std::string groupadd = "sudo usermod -a -G sudo admin";
        cmnd = groupadd;
        system(cmnd.c_str());
        add_user("head");
        add_user("student");
        add_user("educator");
    } else if (user == HEAD) {
        std::string filedata = get_popen("cat file.txt");

        X = "";
        Y = "";
        Z = "";

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

        std::cout << "I am currently " << user << "! Please access: " << X
                  << " " << Y << " " << Z << std::endl;
    } else if (user == EDUCATOR) {
        std::string filedata = get_popen("cat file.txt");
        Y = "";

        int cnt = 0;

        for (int i = 0; i < filedata.size(); i++) {
            if (filedata[i] == ';') {
                cnt++;
                continue;
            }
            if (cnt == 1) {
                Y += filedata[i];
            }
        }
        std::cout << "I am currently " << user << "! Please access: " << Y
                  << std::endl;
    } else if (user == STUDENT) {
        std::string filedata = get_popen("cat file.txt");

        X = "";

        for (int i = 0; i < filedata.size(); i++) {
            if (filedata[i] == ';') {
                break;
            }
            X += filedata[i];
        }
        std::cout << "I am currently " << user << "! Please access: " << X
                  << std::endl;
    }

    else {
        // the starting user (eg:amit) that initiates the program and adds admin
        add_user(name);
    }

    std::string groupadd = "sudo groupadd groupname";
    std::string groupdel = "sudo groupdel groupname";
    // std::string su = "sudo su ";
    // cmnd = su + name;
    // system(cmnd.c_str());
    

    std::cout<<"\n-------\n";
    
    delete_user(name);
    
}
