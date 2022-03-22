#include "timer.h"
//struct sensor
//{
//   std::string name;
//   std::string rom;
//};

sensor sensors[10];

//char rom[20];
char buf[100];
DIR *dirp;
struct dirent *direntp;
int fd =-1;
char *temp;
double value;
void DsInit()
{
    system("sudo modprobe w1-gpio");
    system("sudo modprobe w1-therm");
}

//-------------------------------------
//наполняет структуру sensors адресами датчиков
void get_roms (){
    char path[50] = "/sys/bus/w1/devices/";
// Check if /sys/bus/w1/devices/ exists.
if((dirp = opendir(path)) == nullptr) { printf("opendir error0\n");
    //return 1;
}
// Reads the directories or files in the current directory.
int i{0};
while((direntp = readdir(dirp)) != nullptr) {
    // If 28-00000 is the substring of d_name, then copy d_name to 
    // rom and print rom.
    if(strstr(direntp->d_name,"28-00000")) {

       strcpy(sensors[i].rom,direntp->d_name);
        std::cout<<" rom: "<< sensors[i].rom;
        ++i;
    }
}
closedir(dirp);
}

//принимает номер датчика, возвращает температуру
double get_temp(int num)
{
    

    char path[50] = "/sys/bus/w1/devices/";
    // Append the String rom and "/w1_slave" to path path becomes to 
    // "/sys/bus/w1/devices/28-00000xxxx/w1_slave"
    strcat(path, sensors[num].rom);
strcat(path, "/w1_slave");
    
        // Open the file in the path.
        if((fd = open(path,O_RDONLY)) < 0) { printf("open error1\n");
            return 1;
        }
        // Read the file
        if(read(fd,buf,sizeof(buf)) < 0) { printf("read error2\n");
            return 1;
        }
        // Returns the first index of 't'.
        temp = strchr(buf,'t');
        // Read the string following "t=".
        sscanf(temp,"t=%s",temp);
        // atof: changes string to float.
        value = atof(temp)/1000.0; //printf(" temp : %3.3f °C\n",value);
 
        //sleep(1);

    return value;
}
