#include<iostream>
#include<thread>
#include<condition_variable>
#include<mutex>

using namespace std;

int ctrA = 0;
int ctrB = 0;
const int MAX = 5;
mutex m;
condition_variable cv;

void printA()
{
    while (true)
    {
        unique_lock<mutex> ul(m);
        if (ctrA == MAX)
        {
            cout << "Producer waiting..." << endl;
            cv.wait(ul);
        }
        else
        {
            ctrA++;
            cout << ctrA << endl;
            this_thread::sleep_for(chrono::seconds(2));
        }
        ul.unlock();
        cv.notify_all();
    }
}

void printBr()
{
    while (true)
    {
        unique_lock<mutex> ul(m);
        if (ctrB == MAX)
        {
            cout << "Consumer waiting..." << endl;
            cv.wait(ul);
        }
        else
        {
            ctrB++;
            cout << ctrB << endl;
            this_thread::sleep_for(chrono::seconds(2));
        }
        ul.unlock();
        cv.notify_all();
    }
}

int main()
{
    thread t1(printA);
    thread t2(printB);


    t1.join();
    t2.join();


    return 0;
}