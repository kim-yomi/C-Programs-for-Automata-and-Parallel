#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>


using namespace std;
shared_mutex m;
int ctr = 0;

void reader()
{
    while (true)
    {
        {
            shared_lock<shared_mutex> sl(m);
            cout << "Reader Thread : " << ctr << " ID: " << this_thread::get_id() << endl;
        }
        this_thread::sleep_for(1000ms);
    }
}

void writer()
{
    while (true)
    {
        {
            unique_lock<shared_mutex> ul(m);
            ctr++;
            cout << "Writer Thread : " << ctr << " ID: " << this_thread::get_id() << endl;
        }

        this_thread::sleep_for(5000ms);
    }
}

int main()
{
    thread t1(writer);
    thread t2(reader);
    thread t3(reader);
    thread t4(reader);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}