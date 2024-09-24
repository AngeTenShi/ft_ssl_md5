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

char *process_block_md5(size_t len, const char *input, unsigned int *r, unsigned int *k)
{
	char *msg = (char *)malloc(len);
	ft_memcpy(msg, input, len);
	unsigned int *w = (unsigned int *)(msg);
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
	char *digest = malloc(16);
	ft_memcpy(digest, &A, 4);
	ft_memcpy(digest + 4, &B, 4);
	ft_memcpy(digest + 8, &C, 4);
	ft_memcpy(digest + 12, &D, 4);
	free(msg);
	return (digest);
}

char *md5(const char *input, size_t len, size_t total_size) {
	unsigned int r[64] = { 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
							5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
							4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
							6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21 };
	unsigned int k[64];
	char *hash = NULL;
	for (int i = 0; i < 64; i++)
		k[i] = (unsigned int)(fabs(sin(i + 1)) * pow(2, 32));
	if (len == 0)
		return (NULL);
	if (len % 64 != 0)  // padding
	{
		size_t new_len = len + 1;
		while (new_len % 64 != 56)
			new_len++;
		new_len += 8;  // pour les 64 bits de la taille du message
		char *msg = (char *)malloc(new_len);
		ft_memcpy(msg, input, len);
		msg[len] = 0x80;  // ajout du bit 1 à la fin du message
		for (size_t i = len + 1; i < new_len - 8; i++)
			msg[i] = 0;  // ajout de 0 après le bit 1
		unsigned long long bit_len = total_size * 8;
		ft_memcpy(msg + new_len - 8, &bit_len, 8);
		char *orig_msg = msg;
		while (new_len >= 64)
		{
			if (hash)
				free(hash);
			hash = process_block_md5(64, msg, r, k);
			msg += 64;
			new_len -= 64;
		}
		free(orig_msg);
		return (hash);
	}
	while (len >= 64)
	{
		if (hash)
			free(hash);
		hash = process_block_md5(64, input, r, k);
		input += 64;
		len -= 64;
	}
	return (hash);
}

void md5_file(const char *filename, t_options *options)
{
	int fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror(filename);
		return;
	}
	char buffer[1024];
	char *hash = NULL;
	A = 0x67452301;
	B = 0xefcdab89;
	C = 0x98badcfe;
	D = 0x10325476;
	size_t total_size = 0;
	while (1)
	{
		ssize_t bytes_read = read(fd, buffer, 64);
		if (bytes_read < 0)
		{
			perror("read");
			close(fd);
			return;
		}
		if (bytes_read == 0)
			break;
		total_size += bytes_read; // for the padding at the end
		if (hash)
			free(hash);
		hash = md5(buffer, bytes_read, total_size);
	}
	if (options->q == 0)
	{
		if (options->r == 0)
		{
			print_hexa(hash, 16);
			write(1, "  ", 2);
			if (ft_strncmp(filename, "/dev/stdin", 10) == 0)
				write(1, "-", 1);
			else
				write(1, filename, ft_strlen(filename));
			write(1, "\n", 1);
		}
		else
		{
			if (ft_strncmp(filename, "/dev/stdin", 10) == 0)
				write(1, "-", 1);
			else
				write(1, filename, ft_strlen(filename));
			write(1, "  ", 2);
			print_hexa(hash, 16);
			write(1, "\n", 1);
		}
	}
	else
		print_hexa(hash, 16);
	free(hash);
	close(fd);
}

void md5_helper(t_options *options)
{
	t_string_list *current_string;

	if (options->p)
		md5_file("/dev/stdin", options);
	current_string = options->strings;
	while (current_string)
	{
		if (current_string->from_file)
			md5_file(current_string->str, options);
		else
		{
			size_t len = ft_strlen(current_string->str);
			char *hash = md5(current_string->str, len, len);
			if (options->q == 0)
			{
				if (options->r == 0)
				{
					print_hexa(hash, 16);
					write(1, "  ", 3);
					write(1, "-", 1);
					write(1, "\n", 1);
				}
				else
				{
					write(1, "-", 1);
					write(1, "  ", 3);
					print_hexa(hash, 16);
					write(1, "\n", 1);
				}
			}
			else
				print_hexa(hash, 16);
			free(hash);
		}
		A = 0x67452301;
		B = 0xefcdab89;
		C = 0x98badcfe;
		D = 0x10325476;
		current_string = current_string->next;
	}
}
