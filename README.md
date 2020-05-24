# Visual Profiler CPU

## Introduce
Visual Profiler is a lightweight/header-only/singleton-pattern file which generate a json file to profile your CPU code. The backend is using chrome::tracing. Therefore, the json file needs to be complied with the chrome::tracing format

## How to use
1. Download/copy-paste the **VisualProfiler.hpp**
2. Insert PROFILE_FUNCTION() Marco into the functions you want to profile
e.g.
```
void dummyFunction()
{
    PROFILE_FUNCTION(); // place marco at the topest of the function

    . . .  

}
```
3. Insert Instrumentor::get().beginSession("name")
```
int main()
{
    Instrumentor::get().beginSession("Profile");  // insert beginSession
    dummyPrintOut();
    dummySum();
    dummyFactorial();
    Instrumentor::get().endSession();
    
}
```
4. Open Chrome tracing and type [Chrome:://tracing](chrome://tracing/) in your Chrome
5. Drag and drop the ProfileResult.json


## Example
- Check the code in main.cpp
- you can git clone this repo and try it out
```
mkdir build && cd build
cmake ..
make 
./VisualProfiler
```

## Reference
- The Cherno [[Link](https://www.youtube.com/watch?v=xlAH4dbMVnU)]
- davechurchill's code [[Link](https://pastebin.com/qw5Neq4U)]