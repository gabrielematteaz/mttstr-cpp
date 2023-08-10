#include "mttstr.hpp"

mttstr_Converter::mttstr_Converter()
{

}

mttstr_Converter::mttstr_Converter(int base, char plus, char minus, char fill, mttstr_LetterCase letterCase, mttstr_FillMode fillMode, bool nullTerminate, std::size_t width)
	: base(2 <= base && base <= 36 ? base : 10), plus(plus), minus(minus), fill(fill), letterCase(letterCase), fillMode(fillMode), nullTerminate(nullTerminate), width(width)
{

}

int mttstr_Converter::GetBase()
{
	return base;
}

void mttstr_Converter::SetBase(int base)
{
	this->base = 2 <= base && base <= 36 ? base : 10;
}

std::size_t mttstr_Converter::ValueToString(char *string, std::size_t value)
{
	size_t valueCopy, length;
	char *originalString, letterA, reminder, *minimumStringLength;

	if (string != nullptr)
	{
		if (minus && IsNegativeValue(value))
		{
			valueCopy = value;
			value = -value;
		}
		else valueCopy = 0;

		originalString = string;

		if (base > 10)
		{
			letterA = letterCase == LOWERCASE ? 87 : 55;

			do
			{
				reminder = value % base;
				value /= base;
				*string = (reminder < 10 ? '0' : letterA) + reminder;
				string++;
			} while (value);
		}
		else
		{
			do
			{
				*string = '0' + value % base;
				value /= base;
				string++;
			} while (value);
		}

		switch (fillMode)
		{
		case INTERNAL:
			minimumStringLength = originalString + width - 1;

			while (string < minimumStringLength)
			{
				*string = fill;
				string++;
			}

			if (IsNegativeValue(valueCopy))
			{
				*string = minus;
				string++;
			}
			else if (plus)
			{
				*string = plus;
				string++;
			}
			else if (string == minimumStringLength)
			{
				*string = fill;
				string++;
			}

			length = string - originalString;
			ReverseMemory(originalString, length);

			break;
		case RIGHT:
			if (IsNegativeValue(valueCopy))
			{
				*string = minus;
				string++;
			}
			else if (plus)
			{
				*string = plus;
				string++;
			}

			ReverseMemory(originalString, string - originalString);
			minimumStringLength = originalString + width;

			while (string < minimumStringLength)
			{
				*string = fill;
				string++;
			}

			length = string - originalString;
			
			break;
		default:
			if (IsNegativeValue(valueCopy))
			{
				*string = minus;
				string++;
			}
			else if (plus)
			{
				*string = plus;
				string++;
			}

			minimumStringLength = originalString + width;

			while (string < minimumStringLength)
			{
				*string = fill;
				string++;
			}

			length = string - originalString;
			ReverseMemory(originalString, length);

			break;
		}

		if (nullTerminate) *string = 0;
	}
	else
	{
		if (minus && IsNegativeValue(value))
		{
			value = -value;
			length = 1;
		}
		else length = plus ? 1 : 0;

		do
		{
			value /= base;
			length++;
		} while (value);

		if (length < width) length = width;
	}

	return length;
}

std::size_t mttstr_Converter::StringToValue(char *string, char **lastProcessedChar)
{
	char stringChar, maximumUppercaseLetter, maximumLowercaseLetter, minimumLetter, maximumLetter;
	size_t sign, value;

	if (string != nullptr)
	{
		stringChar = *string;

		switch (fillMode)
		{
		case INTERNAL:
			if (stringChar == minus)
			{
				string++;
				stringChar = *string;
				sign = -1;
			}
			else
			{
				if (stringChar == plus)
				{
					string++;
					stringChar = *string;
				}

				sign = 1;
			}

			while (stringChar == fill)
			{
				string++;
				stringChar = *string;
			}

			break;
		default:
			while (stringChar == fill)
			{
				string++;
				stringChar = *string;
			}

		case RIGHT:
			if (stringChar == minus)
			{
				string++;
				stringChar = *string;
				sign = -1;
			}
			else
			{
				if (stringChar == plus)
				{
					string++;
					stringChar = *string;
				}

				sign = 1;
			}

			break;
		}

		value = 0;

		if (base > 10)
		{
			if (letterCase & MIXEDCASE)
			{
				maximumUppercaseLetter = 'A' + base - 10;
				maximumLowercaseLetter = maximumUppercaseLetter + 32;

				while (1)
				{
					if ('0' <= stringChar && stringChar <= '9') value = value * base + stringChar - '0';
					else if ('A' <= stringChar && stringChar < maximumUppercaseLetter) value = value * base + stringChar - 55;
					else if ('a' <= stringChar && stringChar < maximumLowercaseLetter) value = value * base + stringChar - 87;
					else break;

					string++;
					stringChar = *string;
				}
			}
			else
			{
				minimumLetter = letterCase & LOWERCASE ? 'a' : 'A';
				maximumLetter = minimumLetter + base - 10;

				while (1)
				{
					if ('0' <= stringChar && stringChar <= '9') value = value * base + stringChar - '0';
					else if (minimumLetter <= stringChar && stringChar < maximumLetter) value = value * base + stringChar - minimumLetter + 10;
					else break;

					string++;
					stringChar = *string;
				}
			}
		}
		else
		{
			maximumLetter = '0' + base;

			while ('0' <= stringChar && stringChar < maximumLetter)
			{
				string++;
				value = value * base + stringChar - '0';
				stringChar = *string;
			}
		}

		if (lastProcessedChar != nullptr) *lastProcessedChar = string;

		return sign * value;
	}

	if (lastProcessedChar != nullptr) *lastProcessedChar = nullptr;

	return 0;
}

void *mttstr_Converter::ReverseMemory(void *memory, std::size_t size)
{
	char *charMemory, *reverseCharMemory, memoryChar;

	if (memory != nullptr)
	{
		charMemory = (char *)memory;
		reverseCharMemory = charMemory + size;

		while (charMemory < reverseCharMemory)
		{
			reverseCharMemory--;
			memoryChar = *charMemory;
			*charMemory = *reverseCharMemory;
			charMemory++;
			*reverseCharMemory = memoryChar;
		}
	}

	return memory;
}