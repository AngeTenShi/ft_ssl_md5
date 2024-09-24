#include "ft_ssl_md5.h"

unsigned int h0 = 0x6a09e667;
unsigned int h1 = 0xbb67ae85;
unsigned int h2 = 0x3c6ef372;
unsigned int h3 = 0xa54ff53a;
unsigned int h4 = 0x510e527f;
unsigned int h5 = 0x9b05688c;
unsigned int h6 = 0x1f83d9ab;
unsigned int h7 = 0x5be0cd19;

unsigned int rightrotate(unsigned int value, unsigned int count)
{
	return (value >> count) | (value << (32 - count));
}

char *process_block_sha256(const char *input)
{
	unsigned int a = h0;
	unsigned int b = h1;
	unsigned int c = h2;
	unsigned int d = h3;
	unsigned int e = h4;
	unsigned int f = h5;
	unsigned int g = h6;
	unsigned int h = h7;
	unsigned int w[64] = {0};
	unsigned int k[64] = {	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
							0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
							0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
							0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
							0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
							0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
							0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
							0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
							0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
							0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
							0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
							0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
							0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
							0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
							0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
							0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

	for (size_t i = 0; i < 64; i++)
	{
		if (i < 16)
			w[i] = ((unsigned int)(unsigned char)input[i * 4] << 24) | ((unsigned int)(unsigned char)input[i * 4 + 1] << 16) | ((unsigned int)(unsigned char)input[i * 4 + 2] << 8) | (unsigned int)(unsigned char)input[i * 4 + 3];
		else
		{
			unsigned int s0 = (rightrotate(w[i - 15], 7) ^ rightrotate(w[i - 15], 18) ^ (w[i - 15] >> 3));
			unsigned int s1 = (rightrotate(w[i - 2], 17) ^ rightrotate(w[i - 2], 19) ^ (w[i - 2] >> 10));
			w[i] = w[i - 16] + s0 + w[i - 7] + s1;
		}
	}
	for (size_t i = 0; i < 64; i++)
	{
		unsigned int s1 = rightrotate(e, 6) ^ rightrotate(e, 11) ^ rightrotate(e, 25);
		unsigned int ch = (e & f) ^ ((~e) & g);
		unsigned int temp1 = h + s1 + ch + k[i] + w[i];
		unsigned int s0 = rightrotate(a, 2) ^ rightrotate(a, 13) ^ rightrotate(a, 22);
		unsigned int maj = (a & b) ^ (a & c) ^ (b & c);
		unsigned int temp2 = s0 + maj;
		h = g;
		g = f;
		f = e;
		e = d + temp1;
		d = c;
		c = b;
		b = a;
		a = temp1 + temp2;
	}
	h0 += a;
	h1 += b;
	h2 += c;
	h3 += d;
	h4 += e;
	h5 += f;
	h6 += g;
	h7 += h;
	char *hash = (char *)malloc(32);
	for (int i = 0; i < 4; i++) {
		hash[i] = (h0 >> (24 - i * 8)) & 0xFF;
		hash[i + 4] = (h1 >> (24 - i * 8)) & 0xFF;
		hash[i + 8] = (h2 >> (24 - i * 8)) & 0xFF;
		hash[i + 12] = (h3 >> (24 - i * 8)) & 0xFF;
		hash[i + 16] = (h4 >> (24 - i * 8)) & 0xFF;
		hash[i + 20] = (h5 >> (24 - i * 8)) & 0xFF;
		hash[i + 24] = (h6 >> (24 - i * 8)) & 0xFF;
		hash[i + 28] = (h7 >> (24 - i * 8)) & 0xFF; // BIG ENDIAN
	}
	return (hash);
}

char *sha256(const char *input, size_t len, size_t total_size)
{
	char *hash = NULL;
	if (len % 64 != 0)  // padding
	{
		size_t new_len = len + 1;
		while (new_len % 64 != 56)
			new_len++;
		new_len += 8;  // pour les 64 bits de la taille du message
		char *msg = (char *)malloc(new_len);
		ft_memcpy(msg, input, len);
		msg[len] = 0x80;  // ajout du bit 1 à la fin du message
		unsigned long long bit_len = total_size * 8;
		for (size_t i = 0; i < 8; i++)
			msg[new_len - 8 + i] = (bit_len >> (56 - i * 8)) & 0xFF;
		char *orig_msg = msg;
		while (new_len >= 64)
		{
			if (hash)
				free(hash);
			hash = process_block_sha256(msg);
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
		hash = process_block_sha256(input);
		input += 64;
		len -= 64;
	}
	return (hash);
}

void sha256_file(const char *filename, t_options *options)
{
	int fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror(filename);
		return;
	}
	char buffer[1024];
	char *hash = NULL;
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
		hash = sha256(buffer, bytes_read, total_size);
	}
	if (options->q == 0)
	{
		if (options->r == 0)
		{
			print_hexa(hash, 32);
			write(1, "  ", 2);
			if (ft_strncmp(filename, "/dev/stdin", 10) == 0)
				write(1, "-", 1);
			else
				write(1, filename, ft_strlen(filename));
		}
		else
		{
			if (ft_strncmp(filename, "/dev/stdin", 10) == 0)
				write(1, "-", 1);
			else
				write(1, filename, ft_strlen(filename));
			write(1, "  ", 2);
			print_hexa(hash, 32);
		}
	}
	else
		print_hexa(hash, 32);
	free(hash);
}

void sha256_helper(t_options *options)
{
	t_string_list *current = options->strings;
	while (current)
	{
		if (current->from_file)
			sha256_file(current->str, options);
		else
		{
			size_t len = ft_strlen(current->str);
			char *hash = sha256(current->str, len, len);
			if (options->q == 0)
			{
				if (options->r == 0)
				{
					print_hexa(hash, 32);
					write(1, "  ", 2);
					write(1, "-", 1);
				}
				else
				{
					write(1, "-", 1);
					write(1, "  ", 2);
					print_hexa(hash, 32);
				}
			}
			else
				print_hexa(hash, 32);
			free(hash);
		}
		write(1, "\n", 1);
		current = current->next;
	}
}

