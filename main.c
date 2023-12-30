#include <Windows.h>
#include <wchar.h>

INT64 StringToDecimalValue(LPCWSTR string)
{
	if (string == NULL) return 0;

	BOOL negative;

	if (*string == L'-')
	{
		string++;
		negative = 1;
	}
	else
	{
		if (*string == L'+') string++;

		negative = 0;
	}

	INT64 value = 0;

	while ('0' <= *string && *string <= '9')
	{
		value = value * 10 + *string - '0';
		string++;
	}

	if (negative) value = -value;

	return value;
}

DWORD GetUnsignedValueLength(UINT64 value, int base)
{
	if (base < 2 || base > 36) return 0;

	DWORD length = 0;

	do
	{
		value = value / base;
		length++;
	} while (value);

	return length;
}

void UnsignedValueToFormattedString(LPWSTR string, UINT64 value, int base, DWORD width)
{
	if (string == NULL || base < 2 || base > 36) return;

	LPWSTR stringIterator = string;

	if (base > 10)
	{
		do
		{
			WCHAR remainder = (WCHAR)(value % base);

			value = value / base;
			*stringIterator = (remainder < 10 ? L'0' : 55) + remainder;
			stringIterator++;
		} while (value);
	}
	else
	{
		do
		{
			*stringIterator = L'0' + (WCHAR)(value % base);
			value = value / base;
			stringIterator++;
		} while (value);
	}

	LPWSTR stringIteratorEnd = string + width;

	while (stringIterator < stringIteratorEnd)
	{
		*stringIterator = '0';
		stringIterator++;
	}

	while (string < stringIterator)
	{
		WCHAR character = *string;

		stringIterator--;
		*string = *stringIterator;
		*stringIterator = character;
		string++;
	}
}

int wmain(int argumentCount, wchar_t* argumentVector[])
{
	LONGLONG bytesToRead = 0;
	LARGE_INTEGER offset;
	DWORD columns = 16;
	int offsetBase = 16;
	int byteBase = 16;

	offset.QuadPart = 0;

	/* ... */

	LPWSTR* argument = argumentVector + 1;
	LPWSTR* lastArgument = argumentVector + argumentCount;
	int exitCode = 0;

	while (argument && argument < lastArgument)
	{
		LPWSTR argumentString = *argument;

		if (*argumentString == '/')
		{
			argumentString++;

			if (wcscmp(argumentString, L"bytesToRead") == 0)
			{
				argument++;

				if (argument == lastArgument)
				{
					exitCode = 1;

					break;
				}

				bytesToRead = StringToDecimalValue(*argument);
			}
			else if (wcscmp(argumentString, L"offset") == 0)
			{
				argument++;

				if (argument == lastArgument)
				{
					exitCode = 1;

					break;
				}

				offset.QuadPart = StringToDecimalValue(*argument);
			}
			else if (wcscmp(argumentString, L"columns") == 0)
			{
				argument++;

				if (argument == lastArgument)
				{
					exitCode = 1;

					break;
				}

				columns = (DWORD)StringToDecimalValue(*argument);
			}
			else if (wcscmp(argumentString, L"offsetBase") == 0)
			{
				argument++;

				if (argument == lastArgument)
				{
					exitCode = 1;

					break;
				}

				offsetBase = (int)StringToDecimalValue(*argument);
			}
			else if (wcscmp(argumentString, L"byteBase") == 0)
			{
				argument++;

				if (argument == lastArgument)
				{
					exitCode = 1;

					break;
				}

				byteBase = (int)StringToDecimalValue(*argument);
			}
			else
			{
				argument++;

				break;
			}
		}
		else break;

		argument++;
	}

	if (exitCode == 0)
	{
		HANDLE fileHandle = CreateFileW(*argument, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		LARGE_INTEGER fileSize;

		if (GetFileSizeEx(fileHandle, &fileSize) == 0) exitCode = 2;
		else
		{
			if (bytesToRead == 0) bytesToRead = fileSize.QuadPart;

			if (offset.QuadPart < 0) offset.QuadPart = 0;

			LONGLONG finalOffset = offset.QuadPart + bytesToRead;

			if (finalOffset > fileSize.QuadPart) bytesToRead = bytesToRead - (finalOffset - fileSize.QuadPart);

			bytesToRead = bytesToRead - offset.QuadPart;

			if (bytesToRead > 0)
			{
				HANDLE processHeap = GetProcessHeap();

				if (processHeap == NULL) exitCode = 3;
				else
				{
					if (columns == 0) columns = 16;

					DWORD bytesSize = min((DWORD)bytesToRead, columns * 256);
					LPBYTE bytes = HeapAlloc(processHeap, 0, bytesSize);

					if (offsetBase < 2 || offsetBase > 36) offsetBase = 16;

					DWORD offsetWidth = GetUnsignedValueLength(bytesToRead - bytesToRead % columns, offsetBase);

					if (byteBase < 2 || byteBase > 36) byteBase = 16;

					DWORD byteWidth = GetUnsignedValueLength((BYTE)(-1), byteBase);
					DWORD bytesSectionOffset = offsetWidth + 3;
					DWORD textSectionOffset = bytesSectionOffset + columns * byteWidth + columns + 2;
					DWORD lineSize = textSectionOffset + columns + 1;
					LPWSTR line = HeapAlloc(processHeap, 0, lineSize * sizeof(WCHAR));

					if (bytes == NULL || line == NULL) exitCode = 4;
					else
					{
						LPWSTR bytesSectionFirstCharacter = line + bytesSectionOffset;
						LPWSTR textSectionFirstCharacter = line + textSectionOffset;
						LPWSTR lineIterator = line + offsetWidth;

						*lineIterator = L' ';
						lineIterator++;
						*lineIterator = L' ';
						lineIterator++;
						*lineIterator = L' ';

						for (DWORD column = 0; column < columns; column++)
						{
							lineIterator = lineIterator + byteWidth + 1;
							*lineIterator = L' ';
						}

						*lineIterator = L' ';
						lineIterator++;
						*lineIterator = L' ';
						lineIterator++;
						*lineIterator = L' ';
						lineIterator = lineIterator + columns + 1;
						*lineIterator = L'\n';

						HANDLE consoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

						if (consoleOutput == NULL || consoleOutput == INVALID_HANDLE_VALUE) exitCode = 5;
						else
						{
							SetFilePointerEx(fileHandle, offset, NULL, FILE_BEGIN);

							// OTTIMIZZARE (DIVIDERE CASO "bytesRead == bytesSize")
							do
							{
								DWORD bytesRead;

								if (ReadFile(fileHandle, bytes, bytesSize, &bytesRead, NULL) == FALSE || bytesRead == 0)
								{
									exitCode = 6;

									break;
								}

								bytesToRead = bytesToRead - bytesRead;

								UnsignedValueToFormattedString(line, offset.QuadPart, offsetBase, offsetWidth);

								LPBYTE bytesIterator = bytes;
								DWORD column = 0;
								LPWSTR bytesSectionIterator = bytesSectionFirstCharacter;
								LPWSTR textSectionIterator = textSectionFirstCharacter;

								while (1)
								{
									bytesRead--;

									if (column == columns)
									{
										offset.QuadPart = offset.QuadPart + columns;
										WriteConsoleW(consoleOutput, line, lineSize, NULL, NULL);
										UnsignedValueToFormattedString(line, offset.QuadPart, offsetBase, offsetWidth);
										column = 0;
										bytesSectionIterator = bytesSectionFirstCharacter;
										textSectionIterator = textSectionFirstCharacter;
									}

									UnsignedValueToFormattedString(bytesSectionIterator, *bytesIterator, byteBase, byteWidth);
									*textSectionIterator = *bytesIterator < 32 || *bytesIterator > 126 ? L'.' : *bytesIterator;
									bytesIterator++;
									column++;
									bytesSectionIterator = bytesSectionIterator + byteWidth + 1;
									textSectionIterator++;

									if (bytesRead == 0)
									{
										while (column < columns)
										{
											column++;
											wmemset(bytesSectionIterator, L' ', byteWidth);
											bytesSectionIterator = bytesSectionIterator + byteWidth + 1;
											*textSectionIterator = L' ';
											textSectionIterator++;
										}

										WriteConsoleW(consoleOutput, line, lineSize, NULL, NULL);

										break;
									}
								}
							} while (bytesToRead > 0);
							//
						}
					}

					HeapFree(processHeap, 0, line);
					HeapFree(processHeap, 0, bytes);
				}
			}
		}

		CloseHandle(fileHandle);
	}

	return exitCode;
}