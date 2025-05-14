#include "renderer.h"

int main()
{
    Renderer r;
    r.set_fps_limit(240);
    r.run();

    // Thread testing
    // vector<int> a;
    // void func(int n) {
    //     // cout << n << endl;
    //     a[n] = n;
    // }
    // int n = 100;
    // a.resize(n);
    // cout << "start" << endl;
    // threadManager::multithread_range(&func, 0, n);
    // cout << "done" << endl;
    // // for (int& i : a) {
    // //     i = i + 1;
    // // }
    // cout << "done 2" << endl;
}
