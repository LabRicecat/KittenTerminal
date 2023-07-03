# KittenTerminal
A simple cat themed command line parser tool

## Installation
To install this project as dependencies simply install the [CatCaretaker](https://github.com/labricecat/catcaretaker) and run following command inside your project folder:
```
$ catcare get labricecat/kittenterminal@main
```
After that you can simply include the file `catpkgs/kittenterminal/kittenterminal.hpp`

## Example
```cpp
#include "catpkgs/kittenterminal/kittenterminal.hpp"

int main() {
    std::string promt_name = "meow";

    MeowingPromt promt = MeowingPromt()
        .set_look("{name}> ") // the "{name}" will get replaced with the value of "promt_name"
        .learn_value("name",&promt_name) // here we register the variable
        .employ_worker(KittenWorker() // a new worker
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
            }))
        .plan_work({ // add a work cycle the promt will work down
            Cat_PrintLayout, // print our layout
            Cat_GetUserInput, // get the user input
            Cat_ParseLine, // parse the input
            Cat_Repeat, // repeat the cycle
        })
        .on_panic([](worried_kitten code, std::string message) { 
            // whenever an error occurs, this function gets called
            std::cout << "An error occured: " << message << "\n";
        });
    
    // this starts the working plan
    promt.do_your_thing();
}
```
