#include "renderer.h"

// #include "threadManager.h"
// vector<int> a;
// void func(int n) {
//     // cout << n << endl;
//     a[n] = n;
// }

int main()
{
    Renderer r;
    r.set_fps_limit(240);
    r.run();



    // int n = 100;
    // a.resize(n);
    // threadManager::multithread_range(&func, 0, n);
    // for (auto i : a) {
    //     cout << i << endl;
    // }
}
