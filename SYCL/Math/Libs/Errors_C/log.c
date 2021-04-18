#include "log.h"

#define ERROR_OUT -1
#define OK 0

#define BUFSZ_ERR 4096
static char BUF[BUFSZ_ERR] = {};
static int logfd = STDOUT_FILENO;
static int pos = 0;

enum { NOT_COLOR = 0 , COLOR };
static int color = COLOR;

int CheckBytes (int bytes)
{
    if (bytes <= 0)
        return ERROR_OUT;

    pos += bytes;
    return OK;
}

int GetData ()
{
    long int ttime = time (NULL);
    int bytes = 0;
    if (color)
        bytes = sprintf (BUF + pos , ANSI_COLOR_MAGENTA" %s" , ctime (&ttime));
    else
        bytes = sprintf (BUF + pos , " %s" , ctime (&ttime));

    bytes -= 1;
    return CheckBytes (bytes);
}

int GetPid ()
{
    if (color)
        return CheckBytes (
                sprintf (BUF + pos , ANSI_COLOR_CYAN " PID: %d " , getpid ()));

    //else
    return CheckBytes (
                sprintf (BUF + pos , " PID: %d " , getpid ()));

}

int GetLevel (int level)
{
    int bytes = 0;
    if (color)
    {
        switch (level)
        {
        case LOG_INFO:
            bytes = sprintf (BUF + pos , ANSI_BOLD_BLUE  "Info:    " ANSI_UNDER_GREEN);
            break;
        case LOG_ERROR:
            bytes = sprintf (BUF + pos , ANSI_BOLD_RED "ERROR:   " ANSI_UNDER_GREEN);
            break;
        case LOG_WARNING:
            bytes = sprintf (BUF + pos , ANSI_BOLD_YELLOW "Warning: "ANSI_UNDER_GREEN);
            break;
        default:
            bytes = -1;
        }
    }
    else
    {
        switch (level)
        {
        case LOG_INFO:
            bytes = sprintf (BUF + pos , "Info: ");
            break;
        case LOG_ERROR:
            bytes = sprintf (BUF + pos , "ERROR: ");
            break;
        case LOG_WARNING:
            bytes = sprintf (BUF + pos , "Warning: ");
            break;
        default:
            bytes = -1;
        }
    }

    return CheckBytes (bytes);
}

int SetLogFile (int fd)
{
    if (fd < 0)
    {
        pr_strerr ("Can't change directory for logging to %d!", fd);
        return -1;
    }
    if (fd != STDOUT_FILENO)
        color = NOT_COLOR;

    logfd = fd;
    return OK;
}
int UnSetLogFile ()
{
    int err = close (logfd);
    if (err == -1)
        pr_strerr("Can't close file %d", logfd);
    
    logfd = -1;
    return logfd;
}

int WriteLoggingFile (int level , char* message , ...)
{
    if (logfd < 0)
        return ERROR_OUT;

    va_list param;
    va_start (param , message);

    GetLevel (level);

    int bytes = vsnprintf (BUF + pos , BUFSZ_ERR - pos , message , param);
    if (CheckBytes (bytes) == ERROR_OUT)
        return ERROR_OUT;

    //GetData ();
    //GetPid ();
    BUF[pos] ='\n';
    pos++;

    int out = ERROR_OUT;
    if (write (logfd , BUF , pos) > 0)
        out = OK;

    pos = 0;
    return out;
}