#include "ft_ssl_md5.h"

unsigned int A = 0x67452301;
unsigned int B = 0xefcdab89;
unsigned int C = 0x98badcfe;
unsigned int D = 0x10325476;

unsigned int F(unsigned int b, unsigned int c, unsigned int d)
{
	return ((b & c) | ((~b) & d));
}

unsigned int G(unsigned int b, unsigned int c, unsigned int d)
{
	return ((b & d) | (c & (~d)));
}

unsigned int H(unsigned int b, unsigned int c, unsigned int d)
{
	return (b ^ c ^ d);
}

unsigned int I(unsigned int b, unsigned int c, unsigned int d)
{
	return (c ^ (b | (~d)));
}

unsigned int leftrotate(unsigned int x, unsigned int c) // wikipedia def
{
	return ((x << c) | (x >> (32 - c)));
}

char *md5(const char *input, size_t len, t_options *options) {
	(void)options;
	unsigned int r[64] = { 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
	5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
	6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21 };
	unsigned int k[64];
	for (int i = 0; i < 64; i++)
		k[i] = (unsigned int)(fabs(sin(i + 1)) * pow(2, 32));
	size_t new_len = len + 1;
	while (new_len % 64 != 56)
		new_len++;
	new_len += 8;  // pour les 64 bits de la taille du message
	char *msg = (char *)malloc(new_len);
	ft_memcpy(msg, input, len);
	msg[len] = 0x80;  // ajout du bit 1 à la fin du message
	for (size_t i = len + 1; i < new_len - 8; i++)
		msg[i] = 0;  // ajout de 0 après le bit 1
	unsigned long long bit_len = len * 8;
	ft_memcpy(msg + new_len - 8, &bit_len, 8);
	for (size_t offset = 0; offset < new_len; offset += 64) {
		unsigned int *w = (unsigned int *)(msg + offset);  // actual block
		unsigned int a = A;
		unsigned int b = B;
		unsigned int c = C;
		unsigned int d = D;

		for (int i = 0; i < 64; i++) {
			unsigned int f, g;
			if (i < 16) {
				f = F(b, c, d);
				g = i;
			} else if (i < 32) {
				f = G(b, c, d);
				g = (5 * i + 1) % 16;
			} else if (i < 48) {
				f = H(b, c, d);
				g = (3 * i + 5) % 16;
			} else {
				f = I(b, c, d);
				g = (7 * i) % 16;
			}

			f += a + k[i] + w[g];
			unsigned int temp = d;
			d = c;
			c = b;
			b += leftrotate(f, r[i]);
			a = temp;
		}
		A += a;
		B += b;
		C += c;
		D += d;
	}
	char *digest = malloc(16);
	ft_memcpy(digest, &A, 4);
	ft_memcpy(digest + 4, &B, 4);
	ft_memcpy(digest + 8, &C, 4);
	ft_memcpy(digest + 12, &D, 4);
	free(msg);
	return (digest);
}

void md5_file(const char *filename, t_options *options)
{
	int fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		return;
	}
	char buffer[1024];
	char *hash = NULL;
	while (1)
	{
		ssize_t bytes_read = read(fd, buffer, 1024);
		if (bytes_read < 0)
		{
			perror("read");
			close(fd);
			return;
		}
		if (bytes_read == 0)
			break;
		if (hash)
			free(hash);
		if (bytes_read < 512)
			hash = md5(buffer, bytes_read, options);
		else
			hash = md5(buffer, bytes_read, options);
	}
	print_hexa(hash, 16);
	A = 0x67452301;
	B = 0xefcdab89;
	C = 0x98badcfe;
	D = 0x10325476;
	close(fd);
}

void md5_helper(t_options *options)
{
	t_string_list *current_string;

	current_string = options->strings;
	while (current_string)
	{
		if (current_string->from_file)
			md5_file(current_string->str, options);
		else
		{
			char *hash = md5(current_string->str, ft_strlen(current_string->str), options);
			print_hexa(hash, 16);
			free(hash);
		}
		current_string = current_string->next;
	}
}
