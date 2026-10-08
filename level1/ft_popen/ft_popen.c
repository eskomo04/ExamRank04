
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>


//pipe give fd[0](read), fd[1](write)
// if pid == 0 then child process otherwise parent

int ft_popen(const char *file, char *const argv[], char type)
{
    int fd[2];
    pid_t pid;
    if(!file || !argv || (type != 'r' && type != 'w'))
        return(-1);

    if(pipe(fd)== -1)
        return(-1);

    pid = fork();
    if(pid == -1)
    {
        close(fd[0]);
        close(fd[1]);
        return(-1);
    }

////////////////CHILD-PART///////////
    if(pid == 0)
    {
        if(type == 'r')
        {
            if(dup2(fd[1], STDOUT_FILENO) == -1)
                exit(1);
        }
        else
        {
            if(dup2(fd[0], STDIN_FILENO) == -1)
                exit(1);
        }
        close(fd[0]);
        close(fd[1]);
        execvp(file, argv);
        exit(1);
    }
//////////PARENT-PART///////////////
    else
    {
        if(type == 'r')         //The parent will read the child's output,
        {                      // so it keeps its fd[0](read end),
            close(fd[1]);     // and close the fd[1](write end -
            return(fd[0]);   //since it doesn't write anything to the child process).
        }
        else                    //The parent will write input for the child,
        {                      //so it keeps fd[1] and closes fd[0].
            close(fd[0]);
            return(fd[1]);
        }
    }
}
