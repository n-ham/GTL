#ifndef PROCESS_H_
#define PROCESS_H_

#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>

#ifdef _WIN32
    #include <io.h>
    #include <windows.h>
#endif

#include "../Const.h"

#define BUFFER 80

/*
    Special thanks to SmkMnstr for writing the start function.
*/

/*
    Instructions:
        Process p;
        p.start("test.exe");                                        //starts the process
        p << "hello, world! " << 2.0 << " " << '%' << endl;         //outputting to the process
        p >> myString >> myInt;                                     //inputting from the process
*/
namespace GTL
{
    struct Process
    {
        FILE *in, *out;
        std::queue<std::string> input;
        std::string output, word;
        char line[BUFFER];

        Process() : in(0), out(0)
        {
            signal(SIGCHLD,SIG_IGN);
        };

        void stop()
        {
            fclose(in);
            fclose(out);
        };

        int start(const std::string &filename)
        {
            #ifdef _WIN32
                SECURITY_ATTRIBUTES sa;
                memset(&sa, 0, sizeof(sa));
                sa.nLength = sizeof(sa);
                sa.bInheritHandle = 1;
                HANDLE pipeinR, pipeinW, pipeoutR, pipeoutW;
                if(!CreatePipe(&pipeinR, &pipeinW, &sa, 0))
                    return -1;
                if(!CreatePipe(&pipeoutR, &pipeoutW, &sa, 0))
                {
                    CloseHandle(pipeinR);
                    CloseHandle(pipeinW);
                    return -1;
                }

                STARTUPINFO si;
                memset(&si, 0, sizeof(si));
                si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
                si.wShowWindow = SW_HIDE;
                si.hStdInput = pipeoutR;
                si.hStdOutput = pipeinW;
                si.hStdError = pipeinW;

                PROCESS_INFORMATION pi;
                if(!CreateProcess(0, (LPSTR)filename.c_str(), 0, 0, 1, CREATE_NEW_PROCESS_GROUP, 0, 0, &si, &pi))
                    return -1;
                CloseHandle(pi.hThread);
                CloseHandle(pipeinW);
                CloseHandle(pipeoutR);

                in = fdopen(_open_osfhandle((long)pipeinR, O_TEXT), "r");
                out = fdopen(_open_osfhandle((long)pipeoutW, O_TEXT), "w");
            #else
                int pipein[2], pipeout[2], ret;
                if(pipe(pipein) < 0)
                    return -1;
                if(pipe(pipeout) < 0)
                {
                    close(pipein[0]);
                    close(pipein[1]);
                    return -1;
                }

                if((ret = fork()))
                {
                    close(pipein[1]);
                    close(pipeout[0]);
                    if (ret<0)
                    {
                        close(pipein[0]);
                        close(pipeout[1]);
                        return -1;
                    }
                    in = fdopen(pipein[0], "r");
                    out = fdopen(pipeout[1], "w");
                }
                else
                {
                    close(pipein[0]);
                    close(pipeout[1]);
                    dup2(pipein[1], 1);
                    dup2(pipeout[0], 0);
                    execl(filename.c_str(), filename.c_str(), NULL);
                }
            #endif

            return 0;
        };
    };

    //template output function
    template <class T>
    Process& operator<<(Process &p, T &t)
    {
        p.output += toString(t);

        return p;
    };

    //output function for endl
    Process& operator<<(Process &process, std::ostream& (*manipulator)(std::ostream&))
    {
        fputs((process.output + " ").c_str(), process.out);
        fflush(process.out);
        process.output = "";

        return process;
    };

    //template input function
    template <class T>
    Process& operator>>(Process &p, T &t)
    {
        std::stringstream ss;

        if(p.input.size() == 0)
        {
            fgets(p.line, BUFFER, p.in);
            ss << p.line;

            while(ss >> p.word)
                p.input.push(p.word);
            ss.clear();
        }

        ss << p.input.front();
        p.input.pop();
        ss >> t;

        return p;
    };
}

#endif //PROCESS_H_
