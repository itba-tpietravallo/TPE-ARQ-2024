#include <time.h>
#include <sys.h>

// this is my localtime(). it has no arguments because only returns the current time
struct tm *localtime(){
    struct tm * current_time; 
    sys_time(0, &(current_time->tm_sec));
    sys_time(1, &(current_time->tm_min));
    sys_time(2, &(current_time->tm_hour));
    return current_time;
}
