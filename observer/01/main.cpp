
#include <iostream>
#include <string>
#include <vector>

using namespace std;


// Observer interface
class Observer
{
public:
    virtual void update (const string& message) = 0;
};

// Concrete Observer
class ConcreteObserver : public Observer
{
public:
    void update (const string& message) override
    {
        cout << "Received message: " << message << endl;
    }
};

// Publisher (Subject) class
class Publisher
{
private:
    vector<Observer*> observers;

public:
    // Attach an observer
    void subscribe (Observer* observer)
    {
        observers.push_back (observer);
    }

    // Detach an observer
    void unsubscribe (Observer* observer)
    {
        // remove observer from the vector
    }

    // Notify all observers
    void notify (const string& message)
    {
        for (Observer* observer : observers)
        {
            observer->update (message);
        }
    }
};

int main()
{
    Publisher publisher;

    // Create subscribers
    ConcreteSubscriber subscriber1;
    ConcreteSubscriber subscriber2;

    // Attach subscribers to the publisher
    publisher.subscribe (&subscriber1);
    publisher.subscribe (&subscriber2);

    // Publish messages
    publisher.publish ("Message 1");
    publisher.publish ("Message 2");

    return 0;
}
