#ifndef LOGGING_OBSERVER_H
#define LOGGING_OBSERVER_H

#pragma once

#include <list>
#include <string>
using namespace std;

/**
 * \class   ILoggable
 * \brief   Abstract class to be inherited by classes to log info
 */
class ILoggable
{
public:
    /**
     * \brief   Virtual destructor that deallocates dynamically declared memory during runtime
     */
    virtual ~ILoggable();

    /**
     * \brief   Pure virtual method that creates a string of the class, has to be implemented by child classes
     * \return  String representing the class action
     */
    virtual string stringToLog() = 0;

    /**
     * \brief   Assigns new values to member variables of the ILoggable object
     * \param   ILoggable ILoggable object from which new values are to be taken
     */
    ILoggable &operator=(const ILoggable &);

    /**
     * \brief   Stream insertion override
     */
    friend ostream &operator<<(ostream &outs, const ILoggable &iLoggable);

protected:
    ILoggable();
    ILoggable(ILoggable &iLoggable);
};

/**
 * \class   Observer
 * \brief   Abstract class to be inherited by LogObserver
 */
class Observer
{
public:
    /**
     * \brief   Virtual destructor that deallocates dynamically declared memory during runtime
     */
    virtual ~Observer();
    virtual void Update(ILoggable *log) = 0;

    /**
     * \brief   Assigns new values to member variables of the Observer object
     * \param   Observer Observer object from which new values are to be taken
     */
    Observer &operator=(const Observer &);

    /**
     * \brief   Stream insertion override
     */
    friend ostream &operator<<(ostream &outs, const Observer &observer);

protected:
    Observer();
    Observer(Observer &observer);
};

/**
 * \class   Subject
 * \brief   Class to be inherited by all classes that uses observation
 */
class Subject
{
public:
    /**
     * \brief   Attach to the param observer
     * \param   Observer observer object
     */
    virtual void Attach(Observer *o);

    /**
     * \brief   Detach to the param observer
     * \param   Observer observer object
     */
    virtual void Detach(Observer *o);

    /**
     * \brief   Notify changes for logging
     * \param   Observer ILoggable object
     */
    virtual void Notify(ILoggable *log);

    /**
     * \brief   Default constructor
     */
    Subject();

    /**
     * \brief   Destructor
     */
    ~Subject();

    /**
     * \brief   Copy constructor
     */
    Subject(Subject &subject);

    /**
     * \brief   Assigns new values to member variables of the Subject object
     * \param   Subject subject object from which new values are to be taken
     */
    Subject &operator=(const Subject &);

    /**
     * \brief   Stream insertion override
     */
    friend ostream &operator<<(ostream &outs, const Subject &observer);

private:
    // list of observers
    list<Observer *> *_observers;
};

/**
 * \class   LogObserver
 * \brief   Class that inherits Observer interface that writes the logs to file
 */
class LogObserver : public Observer
{
public:
    /**
     * \brief  Default constructor
     */
    LogObserver();

    /**
     * \brief  Copy constructor
     */
    LogObserver(LogObserver &logObserver);

    /**
     * \brief  Destructor
     */
    ~LogObserver();

    /**
     * \brief   Takes the string from ILoggable and write it to a file
     */
    void Update(ILoggable *log);

    /**
     * \brief   Assigns new values to member variables of the ILoggable object
     * \param   ILoggable ILoggable object from which new values are to be taken
     */
    LogObserver &operator=(const LogObserver &);

    /**
     * \brief   Stream insertion override
     */
    friend ostream &operator<<(ostream &outs, const LogObserver &observer);
};

#endif // LOGGING_OBSERVER_H
