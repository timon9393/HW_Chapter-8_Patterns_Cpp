#include <iostream>
#include <vector>
#include <fstream>

class Observer  // Интерфейс наблюдателя
{
public:
    virtual void onWarning(const std::string& message) {}
    virtual void onError(const std::string& message) {}
    virtual void onFatalError(const std::string& message) {}
};

class Log  // наблюдаемый класс
{
private:
    std::vector<Observer*> observers;

public:
    void addObserver(Observer* obs)
    {
        observers.push_back(obs);
    }

    void removeObserver(Observer* obs)
    {
        auto it = std::remove(observers.begin(), observers.end(), obs);
        observers.erase(it, observers.end());
    }

    void warning(const std::string& msg) const
    {
        for (auto observer : observers)
            observer->onWarning(msg);
    }

    void error(const std::string& msg) const
    {
        for (auto observer : observers)
            observer->onError(msg);
    }

    void fatalError(const std::string& msg) const
    {
        for (auto observer : observers)
            observer->onFatalError(msg);
    }
};

class ConsoleWarningObs : public Observer  // класс-наблюдатель для работы с предупреждениями, который будет печатать сообщение в консоль
{
public:
    void onWarning(const std::string& message) override
    {
        std::cout << "Console warning: " << message << std::endl;
    }
};

class FileErrorObs : public Observer  // класс-наблюдатель для работы с ошибками, который будет печатать сообщение в файл
{
private:
    std::string path;

public:
    FileErrorObs(const std::string& path) : path(path) {}
    void onError(const std::string& message) override
    {
        std::ofstream file(path, std::ios::app);
        if (file.is_open())
        {
            file << "Error with message: " << message << std::endl;
            file.close();
        }
        else
        {
            std::cerr << "Failed to open file: \"" << path << "\"" << std::endl;
        }
    }
};

class FileAndConsoleFatalErrorObs : public Observer  // класс-наблюдатель для работы с фатальными ошибками, который будет печатать сообщение в консоль и в файл
{
private:
    std::string path;

public:
    FileAndConsoleFatalErrorObs(const std::string& path) : path(path) {}
    void onFatalError(const std::string& message) override
    {
        std::cout << "Fatal error with message: " << message << std::endl;
        std::ofstream file(path, std::ios::app);
        if (file.is_open())
        {
            file << "Fatal error with message: " << message << std::endl;
            file.close();
        }
        else
        {
            std::cerr << "Failed to open file: \"" << path << "\"" << std::endl;
        }
    }
};

int main()
{
    Log log;
    ConsoleWarningObs consWarn;
    FileErrorObs fileErr("Errors.txt");
    FileAndConsoleFatalErrorObs fileConsFatErr("Fatal_Errors.txt");

    log.addObserver(&consWarn);
    log.addObserver(&fileErr);
    log.addObserver(&fileConsFatErr);

    log.warning("Test warning");
    log.error("Test error");
    log.fatalError("Test fatal error");

    log.removeObserver(&fileConsFatErr);

    log.fatalError("Test 2 fatal error");  // не напечатается, поскольку удалили наблюдателя

    return 0;
}