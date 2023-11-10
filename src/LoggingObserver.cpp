#include <iostream>
#include <fstream>
#include "LoggingObserver.h"

ILoggable::ILoggable()
{
}

ILoggable::ILoggable(ILoggable &ILoggable)
{
}

ILoggable::~ILoggable()
{
}

ILoggable &ILoggable::operator=(const ILoggable &)
{
    return *this;
}

ostream &operator<<(ostream &outs, const ILoggable &ILoggable)
{
    return outs << "ILoggable";
}

Observer::Observer()
{
}

Observer::Observer(Observer &observer)
{
}

Observer::~Observer()
{
}

Observer &Observer::operator=(const Observer &)
{
    return *this;
}

ostream &operator<<(ostream &outs, const Observer &observer)
{
    return outs << "Observer";
}

Subject::Subject()
{
    _observers = new list<Observer *>;
}

Subject::Subject(Subject &subject)
{
    _observers = subject._observers;
}

Subject::~Subject()
{
    delete _observers;
}

Subject &Subject::operator=(const Subject &subject)
{
    _observers = subject._observers;
    return *this;
}

void Subject::Attach(Observer *o)
{
    _observers->push_back(o);
}

void Subject::Detach(Observer *o)
{
    _observers->remove(o);
}

void Subject::notify(ILoggable *log)
{
    for (Observer *observer : *_observers)
    {
        observer->Update(log);
    }
}

ostream &operator<<(ostream &outs, const Subject &observer)
{
    return outs << "Subject";
}

LogObserver::LogObserver()
{
}

LogObserver::LogObserver(LogObserver &logObserver)
{
}

LogObserver::~LogObserver()
{
}

void LogObserver::Update(ILoggable *log)
{
    std::ofstream logFile("gamelog.txt");

    if (logFile.is_open())
    {
        logFile << log->stringToLog() << std::endl;

        logFile.close();
    }
    else
    {
        std::cout << "Unable to open file." << std::endl;
    }
}

LogObserver &LogObserver::operator=(const LogObserver &)
{
    return *this;
}

ostream &operator<<(ostream &outs, const LogObserver &observer)
{
    return outs << "LogObserver";
}