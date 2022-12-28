# KittenTerminal
A simple cat themed command line parser tool

## Installation
To install this project and its dependencies simply install the [CatCaretaker]() and run following command inside your project folder:
```
$ catcare sync
```
After that you can simply include the file `catmods/kittenterminal/kittenterminal.hpp`

## Example
```cpp
#include "catmods/kittenterminal/kittenterminal.hpp"

int main() {
    KittenWorker worker = KittenWorker() // a new worker
        .learn_skill("print",{ // add a command to the commandline
            kittenknowledge() // options for the arguments
                .setbin()
            ,[](kittens_howto pargs)->std::string { // the body that gets executed, returns an error message
                for(auto i : pargs.get_bin()) {
                    std::cout << i << " ";
                }
                std::cout << "\n";
                return ""; // no error is an empty string
            }
        });
    
    worried_kitten error = worker.playwith("print this looks really cool!!!"); // we can run now a string

    if(error != worried_kitten::OK) { // an error occured
        std::cout << worker.current_error << "\n"; // closer description of the error
    }
}
```
