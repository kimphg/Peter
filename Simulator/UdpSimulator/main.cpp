#include <chrono>
int main()
{
    using milli = std::chrono::milliseconds;
    auto start = std::chrono::high_resolution_clock::now();
    myFunction("param1");
    auto finish = std::chrono::high_resolution_clock::now();
    std::cout << "myFunction() took "
              << std::chrono::duration_cast<milli>(finish - start).count()
              << " milliseconds\n";
}
