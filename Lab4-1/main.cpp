#include<iostream>
#include<thread>
#include<condition_variable>
#include<mutex>

using namespace std;

int ctrA, ctrB;
int roundsA, roundsB;
const int MAX = 5;
mutex m;
condition_variable cv;
bool readyA = true;
bool readyB = false;

void printA();
void printB();

int main()
{
    thread t1(printA);
    thread t2(printB);


    t1.join();
    t2.join();


    return 0;
}

void printA()
{
    unique_lock<mutex> lockA(m);
    while (ctrA != 6)
    {
        while(readyA == false)
        {
            cv.wait(lockA);
        }
        for (int i = 0; i < MAX; i++)
        {
            cout << "A ROUND: " << roundsA << endl;
            this_thread::sleep_for(chrono::seconds(1));
        }
        roundsA++;
        ctrA++;
        readyA = false;
        cv.notify_all();
        readyB = true;
    }
}

void printB()
{
    unique_lock<mutex> lockB(m);
    while (ctrB != 6)
    {
        while(readyB == false)
        {
            cv.wait(lockB);
        }
        for (int i = 0; i < MAX; i++)
        {
            cout << "B ROUND: " << roundsB << endl;
            this_thread::sleep_for(chrono::seconds(1));
        }
        roundsB++;
        ctrB++;
        readyA = true;
        cv.notify_all();
        readyB = false;

    }
}