# mttstrcpp
C++ version of "mttstr"

# Example
```cpp
#include "mttlib/mttstr/mttstr.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
	char **av = argv + 1, **avc = argv + argc, fstr[32];
	mttstr_fmt_t from = FMT_DEF_DEC, to = FMT_DEF_HEX;
	size_t ival;

	while (av < avc)
	{
		ival = from.fstr_to_ival(*av, nullptr);
		std::cout << to.ival_to_fstr(fstr, ival) << " '" << fstr << '\'' << std::endl;
		av++;
	}

	return 0;
}
```
