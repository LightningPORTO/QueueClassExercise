# Multi-threaded element queue in C++

This repository has been created to answer an exercise requested by Bosch. The exercise is detailed [here](Offline-Exercise-Queue_v2.pdf).

I consider that the main goal of the exercise has been reached as I have developed:

- The Queue class with the given interface;
- A main file to serve as an example implementation;
- Unit tests using GoogleTest framework;
- A CMake file to ease the build process.


## Folder structure
In this repo you will find the following folder structure:

```
📦 
├─ include
│  └─ problem_queue.hpp (The core file of the exercise, containing the Queue Class)
├─ src
│  └─  main.cpp (The main file that will implement the test case of the exercise)
├─ tests
│  └─ main_tests.cpp (File to run unit tests using Google Test)
├─ .gitignore
├─ CMakeLists.txt
├─ Offline-Exercise-Queue_v2.pdf
└─ README.md
```

## How to run

1) Clone this repository and move inside the root folder
2) Create a build folder and move inside it
```
$ mkdir build
$ cd build
```
3) Configure the CMake
```
$ cmake ..
```
4) Build the project
```
$ cmake --build .
```
5) To run the Unit tests, move inside the build folder and:
```
$ ctest
```

## Sources
[std::queue - Examples](https://cplusplus.com/reference/queue/queue/)

[std::unique_lock - Examples](https://cplusplus.com/reference/mutex/unique_lock/)

[std::condition_variable - Examples](https://cplusplus.com/reference/condition_variable/condition_variable/)

[Unit tests (Google Test) - Quick start guide](https://google.github.io/googletest/quickstart-cmake.html)


## Known problems
- The extra : "The reading/writing threads should optionally block for a certain period of time. If the
action is successful within this time, true is returned, otherwise false." - Has not been implemented as it would have required changes on the given class interface. However, only slight changes are required to implement this functionality. Preferably using `wait_for` method from `std::condition_variable`.