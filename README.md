Cpponfiguration
===============

A pretty simple configuration library, inspired by MCF's one.

<h3>Example</h3>

```c++
#include "cpponfiguration/configuration.hpp"

int main() {
  configuration config("example.cfg");
  if(!config.load())
    throw runtime_error("Couldn\'t load configuration file!"); // Or ignore it

  cout << boolalpha;
  property & test0(config.get("test0"));
  cout << "0:boolean()=" << test0.boolean() << ";"
          "textual()=" << test0.textual() << '\n';
  test0.boolean() ^= 1;
  cout << "1:boolean()=" << test0.boolean() << ";"
          "textual()=" << test0.textual() << '\n';
  test0.update_from_boolean();
  cout << "2:boolean()=" << test0.boolean() << ";"
          "textual()=" << test0.textual() << '\n';

  if(!config.save())
    throw runtime_error("Couldn\'t save configuration file!"); // Or ignore it
}
```
