#include <iostream>
#include <fstream>

class LogCommand {
public:
    virtual ~LogCommand() = default;
    virtual void print(const std::string& message) = 0;
};

class ConsoleLogCommand : public LogCommand
{
public:
    void print(const std::string& message) override
    {
        std::cout << message << std::endl;
    }
};

class FileLogCommand : public LogCommand
{
public:
    FileLogCommand(const std::string& path) : path(path)
    {
        file.open(path, std::ios::app);
        if (!file.is_open())
            std::cerr << "Error! Failed to open file: \"" << path << "\"" << std::endl;
    }

    void print(const std::string& message) override
    {
        file << message << std::endl;
    }
    
    ~FileLogCommand()
    {
        file.close();
    }

private:
    std::string path;
    std::ofstream file;
};

void print(LogCommand& command, std::string&& msg)
{
    command.print(msg);
}

int main()
{
    ConsoleLogCommand consoleComm;
    print(consoleComm, "Message to console");

    FileLogCommand fileComm("out.txt");
    print(fileComm, "Message to file");

    return 0;
}