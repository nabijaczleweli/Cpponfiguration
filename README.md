Cpponfiguration
===============

A pretty simple configuration library, inspired by MCF's one.

### Example

```c++
#include "cpponfiguration/configuration.hpp"

int main() {
  cpponfiguration::configuration config("example.cfg");
  if(!config.load())
    throw runtime_error("Couldn\'t load configuration file!"); // Or ignore it.
                                                               // Will always
                                                               // happen upon
                                                               // file creation.

  cpponfig::property & test0(config.get("test0"));
  //      ^ `cpponfiguration` and `cpponfig` are equivalent
  cout << "0:integer()=" << test0.integer() << "; "
          "textual()=" << test0.textual() << '\n';
  test0.integer() += 1;
  cout << "1:integer()=" << test0.integer() << "; "
          "textual()=" << test0.textual() << '\n';
  test0.update_from_integer();
  cout << "2:integer()=" << test0.integer() << "; "
          "textual()=" << test0.textual() << '\n';

  if(!config.save())
    throw runtime_error("Couldn\'t save configuration file!"); // Or ignore it.
}
```

Would create a config file "example.cfg" containing
```
{
  test0=1
}
```

Cpponfiguration also supports categories, which makes complex configuration systems simple to implement and
integrates seamlessly with current and preudo-categorized systems.

### Notes
* Contrary to GitHub's belief, there is no JavaScript involved in this project.
