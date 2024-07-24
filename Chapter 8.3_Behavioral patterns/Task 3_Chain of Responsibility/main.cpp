#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>

class LogMessage
{
public:
    enum class Type
    {
        WARNING,
        ERROR,
        FATAL_ERROR,
        UNKNOWN
    };

    LogMessage(const std::string& message, Type type = Type::UNKNOWN) : type_(type), message_(message) {}
    Type type() const { return type_; };
    virtual const std::string& message() const { return message_; };
private:
    Type type_;
    std::string message_;
};

class LogHandler
{
protected:
    LogHandler* nextHandler = nullptr;
    void NextHandler(const LogMessage* msg)
    {
        if (nextHandler)
            nextHandler->handle(msg);
        else
            throw std::runtime_error("Error: Message should be handled!");
    }

public:
    virtual void handle(const LogMessage* message) = 0;
    void SetNextHandler(LogHandler* next)
    {
        nextHandler = next;
    }
};

class FatalErrorHandler : public LogHandler
{
public:
    void handle(const LogMessage* message) override 
    {
        if (message->type() == LogMessage::Type::FATAL_ERROR) {
            throw std::runtime_error("Fatal Error: " + message->message());
        }
        else {
            NextHandler(message);
        }
    }
};

class ErrorHandler : public LogHandler
{
private:
    std::string filePath;
public:
    explicit ErrorHandler(const std::string& path) : filePath(path) {}
    void handle(const LogMessage* message) override
    {
        if (message->type() == LogMessage::Type::ERROR) 
        {
            std::ofstream file(filePath, std::ios::app);
            if (file.is_open())
            {
                file << "Error with message: " << message->message() << std::endl;
                file.close();
            }
            else
            {
                std::cerr << "Failed to open file: \"" << filePath << "\"" << std::endl;
            }
        }
        else {
            NextHandler(message);
        }
    }
};

class WarningHandler : public LogHandler
{
public:
    void handle(const LogMessage* message) override
    {
        if (message->type() == LogMessage::Type::WARNING) {
            std::cerr << "Warning with message: " << message->message() << std::endl;
        }
        else {
            NextHandler(message);
        }
    }
};

class UnknownHandler : public LogHandler
{
public:
    void handle(const LogMessage* message) override
    {
        if (message->type() == LogMessage::Type::UNKNOWN) {
            throw std::runtime_error("Unknown error with message: " + message->message());
        }
        else {
            NextHandler(message);
        }
    }
};

int main()
{
    LogHandler* handler1 = new FatalErrorHandler();
    LogHandler* handler2 = new ErrorHandler("error_log.txt");
    LogHandler* handler3 = new WarningHandler();
    LogHandler* handler4 = new UnknownHandler();

    handler1->SetNextHandler(handler2);
    handler2->SetNextHandler(handler3);
    handler3->SetNextHandler(handler4);

    LogMessage fatal("Critical system error!", LogMessage::Type::FATAL_ERROR);
    LogMessage error("Wrong connection!", LogMessage::Type::ERROR);
    LogMessage warning("Low disk space!", LogMessage::Type::WARNING);
    LogMessage unknown("Restart your system!", LogMessage::Type::UNKNOWN);

    try
    {
        handler1->handle(&warning);
        handler1->handle(&error);
        handler1->handle(&fatal);
        handler1->handle(&unknown);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    delete handler1, handler2, handler3, handler4;

    return 0;
}
