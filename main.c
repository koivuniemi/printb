/* MIT License
 * 
 * Copyright (c) 2024 Lauri Koivuniemi
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


const char HELP[] =
	"usage: ./<bin> double|int\n"
	"prefix: [0x] hexadecimal, [0b] binary, [0] octal\n"
        "suffix: [f] float, [l] long, [l] long long\n";

struct num {
	size_t size;
	union {
		unsigned char u8;
		unsigned short u16;
		unsigned int u32;
		unsigned long long u64;
		float f;
		double d;
	} val;
};


void print_bin(struct num n)
{
	int bit_count = n.size * 8;
	char str[bit_count + 1];
	str[bit_count] = '\0';
	for (int i = 0; i < bit_count; i++) {
		if (n.val.u64 & ((unsigned long long)1 << (bit_count - 1 - i)))
			str[i] = '1';
		else
			str[i] = '0';
	}
	puts(str);
}

struct num parse(char* s)
{
	int base = 10;
	if (s[0] != '\000' && s[1] != '\000' && s[0] == '0') {
		base = 8;
		if      (s[1] == 'x' || s[1] == 'X') base = 16;
		else if (s[1] == 'b' || s[1] == 'B') base = 2;
	}
	char last_char = s[strlen(s) - 1];
	struct num n;
	if (strchr(s, '.')) {
		if (last_char == 'f' || last_char == 'F') {
			n.size = 4;
			n.val.f = strtod(s, NULL);
		} else {
			n.size = 8;
			n.val.d = strtod(s, NULL);
		}
	} else {
		if (base == 2) n.val.u64 = strtol(s + 2, NULL, base);
		else           n.val.u64 = strtol(s,     NULL, base);
		n.size = 4;
		if      (last_char == 'b' || last_char == 'B') n.size = 1;
		else if (last_char == 's' || last_char == 'S') n.size = 2;
		else if (last_char == 'l' || last_char == 'L') n.size = 8;
	}
	return n;
}

int main(int argc, char** argv)
{
	if (argc < 2) {
		fprintf(stderr, HELP);
		return 1;
	}

	struct num n = parse(argv[1]);
	print_bin(n);

	return 0;
}
