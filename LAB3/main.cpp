/*NOTE: I didn't use mutex for this program, as I determined there was no need.
        Both concurrent functions work on different variables, so there would
        be no race condition.

        Used vector threads in this program for scalability.
        There are a total of FOUR threads being used in this program.

        The original program took about 10297 milliseconds to compute.
*/
#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <thread>

using namespace std;

const long MAX = 500000000;
const int RANGE = 6;

vector<int> num(MAX);

struct tallyHistogram {
    int h1 = 0, h2 = 0, h3 = 0, h4 = 0, h5 = 0;
};

void computeChunkSum(const vector<int> &num, int start, int end, unsigned long long &chunkSum) {
    for (int i = start; i < end; ++i) {
        chunkSum += num[i];
    }
}

void computeChunkHistogram(const vector<int> &num, int start, int end, tallyHistogram &result) {
    for (int i = start; i < end; ++i) {
        switch (num[i]) {
            case 1: result.h1++; break;
            case 2: result.h2++; break;
            case 3: result.h3++; break;
            case 4: result.h4++; break;
            case 5: result.h5++; break;
        }
    }
}

int main() {
    vector<thread> sumThreads, histogramThreads;
    vector<unsigned long long> chunkSums(4, 0);
    vector<tallyHistogram> chunkHistograms(4);

    int chunkSize = MAX / 4;

    for (int i = 0; i < MAX; ++i) {
        num[i] = (rand() % RANGE);
        //cout << num[i] << endl;
    }

    auto start = chrono::high_resolution_clock::now();

    //Computing of chunks for each thread
    for (int i = 0; i < 4; ++i) {
        int begin = i * chunkSize;
        int end = (i == 4 - 1) ? MAX : begin + chunkSize;
        sumThreads.emplace_back(computeChunkSum, cref(num), begin, end, ref(chunkSums[i]));
    }

    //Computing of chunks for each thread
    for (int i = 0; i < 4; ++i) {
        int begin = i * chunkSize;
        int end = (i == 4 - 1) ? MAX : begin + chunkSize;
        histogramThreads.emplace_back(computeChunkHistogram, cref(num), begin, end, ref(chunkHistograms[i]));
    }

    for (auto &t : sumThreads) t.join();
    for (auto &t : histogramThreads) t.join();

    unsigned long long totalSum = 0;
    tallyHistogram finalHist;

    for (int i = 0; i < 4; ++i) {
        totalSum += chunkSums[i];
        finalHist.h1 += chunkHistograms[i].h1;
        finalHist.h2 += chunkHistograms[i].h2;
        finalHist.h3 += chunkHistograms[i].h3;
        finalHist.h4 += chunkHistograms[i].h4;
        finalHist.h5 += chunkHistograms[i].h5;
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "Histogram Result:\n";
    cout << "1  ----> " << finalHist.h1 << " SUM : " << finalHist.h1 * 1 << endl;
    cout << "2  ----> " << finalHist.h2 << " SUM : " << finalHist.h2 * 2 << endl;
    cout << "3  ----> " << finalHist.h3 << " SUM : " << finalHist.h3 * 3 << endl;
    cout << "4  ----> " << finalHist.h4 << " SUM : " << finalHist.h4 * 4 << endl;
    cout << "5  ----> " << finalHist.h5 << " SUM : " << finalHist.h5 * 5 << endl;

    unsigned long long histogramSum = finalHist.h1 * 1 + finalHist.h2 * 2 + finalHist.h3 * 3 + finalHist.h4 * 4 + finalHist.h5 * 5;

    cout << "Sum via threads       : " << totalSum << endl;
    cout << "Sum via histogram     : " << histogramSum << endl;
    cout << "TOTAL RUNNING TIME IS : " << duration.count() << " milliseconds." << endl;
    float percent = ((10297.0f - duration.count()) / 10297.0f) * 100.0f;
    cout << "TOTAL PERCENT IMPROVED FROM ORIGINAL DURATION : " << percent << "%" << endl;

    return 0;
}

