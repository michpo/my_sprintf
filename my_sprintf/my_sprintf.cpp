
#define _CRT_NONSTDC_NO_WARNINGS	1

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define INT_STR_SIZE	(sizeof(int)* 8 / 3 + 1)

//#define va_start(list, param) (list = (((char*)&param) + sizeof(param)))
//#define va_arg(list, type)    (*(type *)((list += sizeof(type)) - sizeof(type)))

char* convert(char* buff, int size, unsigned int num, int base);
int my_sprintf(char* buff, int size, const char *str, ...);

void itox(unsigned int val, char* buff);
int GetFloatPoint(float, int);
char* itoa(char* buff, int size, int val);
char* ftoa(char* buff, int size, float val, int digits);
int GetNumBefore(char** str);
char* FractionalPartToStr(char* , int , int , int );

int main()
{
	char buff[200];
	int num;
	//char* p;
	memset(buff, 0, sizeof(buff));
		
	num = my_sprintf(buff, sizeof(buff), " String1: %s Hex=%04x Float = %3.4f Hex=%8.4x Val = %8d Float1 = %3.5f ", "qwerty",123, -123.0783, 300, 123, -0.01234);
	//num = my_sprintf(buff, sizeof(buff), "Float = %3.4f Float1 = %3.3f ", -123.0783, -0.01234);

	return 0;
}
 
/**
* @brief		 : Simple analog of sprintf
* @param[out]    : pointer to output formatted string that contains result.
* @param[in]     : buffer size.
* @param[in]	 : Pointer to parametrs string
* @retval        : None
*/
int my_sprintf(char* buff, int size, const char *str, ...)
{
	char* p;
	bool flag = false;
	int i = 0;
	char temp[INT_STR_SIZE*2];
	va_list list;
	va_start (list, str); 

	int num1, num2;
	unsigned char len;
	//void**  argAddr = (void**)((unsigned int)&str + sizeof(unsigned int));

	while (*str != 0 && i < size)
	{
		if (*str++ == '%')
		{		
			if (*str == '0')
				flag = true;
			num1 = GetNumBefore((char**)&str);
			if (*str == '.')
				str++;
			num2 = GetNumBefore((char**)&str);
			  
			switch (*str++)
			{
			case 'X':
			case 'x':

				//val = *(unsigned int*)argAddr;			
				//p = convert(temp, sizeof(temp) , *(unsigned int*)argAddr, 16);
				p = convert(temp, sizeof(temp), va_arg(list, unsigned int), 16);
			
				len = (unsigned char)strlen(p);

				if (!flag)
				{
					num1 -= num2;
					if (num1 < 0) num1 = 0;
					memset(&buff[i], ' ', num1);
				}
				else
				{
					flag = false;
					num2 = num1;
					num1 = 0;
				}

				num2 -= len;
				if (num2 < 0) num2 = 0;
				memset(&buff[i + num1], '0', num2);
			
				strcat(&buff[i + num1], p);
				i += (num1 + num2 +len);
				//i = strlen(buff);
				//argAddr++;
				break;

			case 'D':
			case 'd':

				//p = itoa(temp, sizeof(temp), *(unsigned int*)argAddr);
				p = itoa(temp, sizeof(temp), va_arg(list, unsigned int));
				len = (unsigned char)strlen(p);

				num1 -= len;
				if (num1 < 0) num1 = 0;
				memset(&buff[i], ' ', num1);

				strcat(&buff[i + num1],p);
				i += (num1 + len);
				//i = strlen(buff);

				//argAddr++;
				break;

			case 'S':
			case 's':
				
				//p = (char*)*argAddr;
				p = va_arg(list,char*);
				len = (unsigned char)strlen(p);
				
				num1 -= len;
				if (num1 < 0) num1 = 0;

				memset(&buff[i], ' ', num1);

				strncat(&buff[i + num1], p,len - num2);
				i += (num1 + len - num2);
				//i = strlen(buff);
				//argAddr++;
				break;

			case 'F':
			case 'f':
				
				if (num2 < 0) num2 = 0;
				
				//float v = *(double*)argAddr;

				float v = (float)va_arg(list, double);
				p = ftoa(temp, sizeof(temp), v, num2);
				len = (unsigned char)strlen(p);

				num1 -= len;
				if (num1 < 0) num1 = 0;
				memset(&buff[i], ' ', num1);

				strcat(&buff[i + num1], p);
				i += (num1 + len);
				//i = strlen(buff);
				//argAddr++;
				//argAddr++;
				
				break;
			}

		}
		else
		{
			buff[i++] = *(str - 1);
			buff[i] = 0;
		}
		
	}

	return i-1;
}

/**
* @brief		 : Convert int to ASCII relative selected  base. 
* @param[in]     : pointer to working buffer
* @param[in]     : Buffer size
* @param[in]     : int value
* @param[in]     : Base. if Base = 2 then convert valeu to binary string.
*					If base = 16 then convert value to hex string
* @retval        : String of the value
*/
char* convert(char* buff, int size, unsigned int num, int base)
{
	char *p;

	if (size >= INT_STR_SIZE)
	{
		p = &buff[size - 1];
		*p = '\0';

		if(num != 0)
		{
            while (num != 0)
            {
                *--p = "0123456789ABCDEF"[num%base];
                num /= base;
            }
		}
		else
		{
		    *--p  = '0';
		}

		return p;
	}
	else
		return 0;
}

/**
* @brief		 : Convert string int to int
* @param[in/out] : Input string and output current position in the string
* @retval        : value before point in the float value. 
*/

int GetNumBefore(char** str)
{
	char ch;
	char temp[10];
	char *p = temp;
	char *q = *str;

	while ((ch = *q) != '.' && ch < ':')
	{
		*p++ = ch;
		q++;
	}

	*str = q;
	*p = 0;
	return atoi(temp);
}

/**
* @brief		 : Convert int to string
* @param[in]     : Pointer to working buffer
* @param[in]     : Buffer size
* @param[in]     : int value
* @retval        : Decimal String of the value
*/

char* itoa(char* buff, int size, int val)
{
	char *p;

	if (val < 0)
	{
		val = val*(-1);
		p = convert(buff, size, val, 10);
		*--p = '-';
	}
	else
		p = convert(buff, size, val, 10);

	return p;
}

char* utoa(char* buff, int size, unsigned int val)
{
    char *p;

    p = convert(buff, size, val, 10);
    return p;
}

/**
* @brief		 : Get part of float before point
* @param[in]     : Float value
* @param[in]     : Float value
* @retval        : Decimal String of the value
*/

char* FractionalPartToStr(char* buff, int size, float val, int digits)
{
    int i =0, factor = 10;
	int  num = (int)val;

	int sub;

	char* p;

	 p = &buff[size - 1]; // Get end of buffer
	*p = '\0';		      // Set End of string

	// convert negartive val to positive
	if (val < 0)
	{
		num *= -1;
		val = val *(-1);
	}

	// set default numbers of fractinal part  after point if it is not defined
	if(digits == 0)
	    digits = 6;
	
	// convert fractional part of float to string
	
	while (i++ < digits)
	{
		val = val * factor; // val = 1230.783 -> 1230.783
		num = num * factor; // val = 1230	  -> 1230

		sub = val - num;	// 12307.83 - 12300 = 7

		*(p - digits)= sub + '0'; // 0x37 '7' -> buff from end of the buffer
		p++;
		num += sub;	// add 7 for next cycle	
		
	}
	
	// return string of the buffer from end ".0783"
	return  &buff[size - 1] - digits;
}

char* IntToStrLeadZero(char* buff,int size, int val, int digits)
{
    char *p;

    p = &buff[size - 1];
   *p = '\0';

    if(digits == 0)
        digits = 7;

    while (val && digits-- > 0 )
    {
        *--p = (val%10) + '0';
        val = val/10;
    }

    return p;
}

/**
* @brief		 : Convert float value to string
* @param[in]     : float value
* @param[in]     : float value
* @retval        : Decimal String of the value
*/

char* ftoa(char* buff, int size, float val, int digits)
{
	char* p;
	char* q;

	p = FractionalPartToStr(buff, size, val, digits);
	
  	*--p = '.';
	q = itoa(buff, p - &buff[0], (int)val);

	if (val > -1 && val < 0 )
	{
		*--q = '-';
	}

	strcat(q, p);
	return q;
}



