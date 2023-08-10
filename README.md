# mttstrcpp
C++ version of "mttstr"

# Example
```cpp
#include "mttstr.hpp"
#include <iostream>

int main()
{
	std::string number;
	std::size_t value;
	mttstr_Converter from, to(16, 0, 0, '0', UPPERCASE, LEFT, true, 0);
	char string[32];

	std::cin >> number;
	value = from.StringToValue(number.data(), nullptr);
	std::cout << to.ValueToString(string, value) << " \'" << string << '\'' << std::endl;

	return 0;
}
```
