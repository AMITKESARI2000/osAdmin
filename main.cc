#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

const int BUFFSIZE = 512;
const std::string ADMIN = "admin";
const std::string HEAD = "head";
const std::string EDUCATOR = "educator";
const std::string STUDENT = "student";

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
        std::string adduser = "sudo adduser ";
        std::string cmnd = adduser + name;
        system(cmnd.c_str());  // i.e system("sudo adduser name")
        // system("sudo useradd -m admin_os"); system(""sudo passwd admin_os");
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
        std::cout << "I am currently " << user << "!" << std::endl;
        std::cout << "Please Enter name X" << std::endl;
        getline(std::cin, X);

        std::cout << "Please Enter name Y" << std::endl;
        getline(std::cin, Y);

        std::cout << "Please Enter name Z" << std::endl;
        getline(std::cin, Z);

        std::string filename = "file.txt";
        cmnd = "echo \"" + X + ";" + Y + ";" + Z + "\"  > " + filename;
        // std::string output=system(cmnd.c_str());
        utilities.set_popen(cmnd.c_str(), cmnd.c_str());

        // std::string groupadd = "sudo usermod -a -G sudo admin";
        // cmnd = groupadd;
        // system(cmnd.c_str());
        our_user.add_user(HEAD, display_info);
        our_user.add_user(STUDENT, display_info);
        our_user.add_user(EDUCATOR, display_info);
    } else if (user == HEAD) {
        std::string filedata = utilities.get_popen("cat file.txt");

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
        std::string filedata = utilities.get_popen("cat file.txt");
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
        std::string filedata = utilities.get_popen("cat file.txt");

        X = "";

        for (int i = 0; i < filedata.size(); i++) {
            if (filedata[i] == ';') {
                break;
            }
            X += filedata[i];
        }
        std::cout << "I am currently " << user << "! Please access: " << X
                  << std::endl;
    } else {
        // the starting user (eg:amit) that initiates the program and adds admin
        our_user.add_user(name, display_info);
    }

    std::string groupadd = "sudo groupadd groupname";
    std::string groupdel = "sudo groupdel groupname";
    // std::string su = "sudo su ";
    // cmnd = su + name;
    // system(cmnd.c_str());

    std::cout << "\n-------\n";

    our_user.delete_user(name);
}
