#include "ft_ssl_md5.h"

void sub_bytes(unsigned char state[8][8]) {
	const unsigned int S[16][16] = {
		{0x18, 0x23, 0xc6, 0xe8, 0x87, 0xb8, 0x01, 0x4f, 0x36, 0xa6, 0xd2, 0xf5, 0x79, 0x6f, 0x91, 0x52},
		{0x60, 0xbc, 0x9b, 0x8e, 0xa3, 0x0c, 0x7b, 0x35, 0x1d, 0xe0, 0xd7, 0xc2, 0x2e, 0x4b, 0xfe, 0x57},
		{0x15, 0x77, 0x37, 0xe5, 0x9f, 0xf0, 0x4a, 0xda, 0x58, 0xc9, 0x29, 0x0a, 0xb1, 0xa0, 0x6b, 0x85},
		{0xbd, 0x5d, 0x10, 0xf4, 0xcb, 0x3e, 0x05, 0x67, 0xe4, 0x27, 0x41, 0x8b, 0xa7, 0x7d, 0x95, 0xd8},
		{0xfb, 0xee, 0x7c, 0x66, 0xdd, 0x17, 0x47, 0x9e, 0xca, 0x2d, 0xbf, 0x07, 0xad, 0x5a, 0x83, 0x33},
		{0x63, 0x02, 0xaa, 0x71, 0xc8, 0x19, 0x49, 0xd9, 0xf2, 0xe3, 0x5b, 0x88, 0x9a, 0x26, 0x32, 0xb0},
		{0xe9, 0x0f, 0xd5, 0x80, 0xbe, 0xcd, 0x34, 0x48, 0xff, 0x7a, 0x90, 0x5f, 0x20, 0x68, 0x1a, 0xae},
		{0xb4, 0x54, 0x93, 0x22, 0x64, 0xf1, 0x73, 0x12, 0x40, 0x08, 0xc3, 0xec, 0xdb, 0xa1, 0x8d, 0x3d},
		{0x97, 0x00, 0xcf, 0x2b, 0x76, 0x82, 0xd6, 0x1b, 0xb5, 0xaf, 0x6a, 0x50, 0x45, 0xf3, 0x30, 0xef},
		{0x3f, 0x55, 0xa2, 0xea, 0x65, 0xba, 0x2f, 0xc0, 0xde, 0x1c, 0xfd, 0x4d, 0x92, 0x75, 0x06, 0x8a},
		{0xb2, 0xe6, 0x0e, 0x1f, 0x62, 0xd4, 0xa8, 0x96, 0xf9, 0xc5, 0x25, 0x59, 0x84, 0x72, 0x39, 0x4c},
		{0x5e, 0x78, 0x38, 0x8c, 0xd1, 0xa5, 0xe2, 0x61, 0xb3, 0x21, 0x9c, 0x1e, 0x43, 0xc7, 0xfc, 0x04},
		{0x51, 0x99, 0x6d, 0x0d, 0xfa, 0xdf, 0x7e, 0x24, 0x3b, 0xab, 0xce, 0x11, 0x8f, 0x4e, 0xb7, 0xeb},
		{0x3c, 0x81, 0x94, 0xf7, 0xb9, 0x13, 0x2c, 0xd3, 0xe7, 0x6e, 0xc4, 0x03, 0x56, 0x44, 0x7f, 0xa9},
		{0x2a, 0xbb, 0xc1, 0x53, 0xdc, 0x0b, 0x9d, 0x6c, 0x31, 0x74, 0xf6, 0x46, 0xac, 0x89, 0x14, 0xe1},
		{0x16, 0x3a, 0x69, 0x09, 0x70, 0xb6, 0xd0, 0xed, 0xcc, 0x42, 0x98, 0xa4, 0x28, 0x5c, 0xf8, 0x86}
	};
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			state[i][j] = S[(state[i][j] >> 4) & 0xF][state[i][j] & 0xF];
		}
	}
}

void shift_columns(unsigned char state[8][8]) {
	unsigned long long temp[8][8];
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			temp[i][j] = state[i][j];
		}
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			state[i][j] = temp[(i + j) % 8][j];
		}
	}
}

unsigned char gf_mult(unsigned char a, unsigned char b) {
	unsigned char p = 0;
	unsigned char hi_bit_set;
	for (int i = 0; i < 8; i++) {
		if (b & 1)
			p ^= a; // Ajoute a à p si le bit le moins significatif de b est 1
		hi_bit_set = a & 0x80; // Vérifie si le bit le plus significatif de a est défini
		a <<= 1; // Décale a à gauche d'un bit
		if (hi_bit_set)
			a ^= 0x1B; // Applique le polynôme de réduction si nécessaire
		b >>= 1; // Décale b à droite d'un bit
	}
	return p;
}

void mix_rows(unsigned char state[8][8]) {
	const unsigned char M[8][8] = {
		{0x01, 0x01, 0x04, 0x01, 0x08, 0x05, 0x02, 0x09},
		{0x09, 0x01, 0x01, 0x04, 0x01, 0x08, 0x05, 0x02},
		{0x02, 0x09, 0x01, 0x01, 0x04, 0x01, 0x08, 0x05},
		{0x05, 0x02, 0x09, 0x01, 0x01, 0x04, 0x01, 0x08},
		{0x08, 0x05, 0x02, 0x09, 0x01, 0x01, 0x04, 0x01},
		{0x01, 0x08, 0x05, 0x02, 0x09, 0x01, 0x01, 0x04},
		{0x04, 0x01, 0x08, 0x05, 0x02, 0x09, 0x01, 0x01},
		{0x01, 0x04, 0x01, 0x08, 0x05, 0x02, 0x09, 0x01}
	};
	unsigned char temp[8][8];
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			temp[i][j] = 0;
			for (int k = 0; k < 8; k++) {
				temp[i][j] ^= gf_mult(M[j][k], state[i][k]); // Multiplication dans GF(2^8)
			}
		}
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			state[i][j] = temp[i][j]; // Mise à jour de l'état
		}
	}
}

void add_round_key(unsigned char state[8][8], unsigned char K[8][8]) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++)
			state[i][j] ^= K[i][j];
	}
}

void expand_key(unsigned char K[8][8], unsigned long long RC)
{
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			K[i][j] ^= (RC >> (8 * (7 - i))) & 0xFF;
		}
	}
}

char *process_block_whirlpool(const char *input)
{
	unsigned char state[8][8];
	unsigned char K[8][8];
	const unsigned long long RC[10] = {
		0x1823C6E887B8014F, 0x36A6D2F5796F9152, 0x60BC9B8EA30C7B35, 0x1DE0D7C22E4BFE57,
		0x157737E59FF04ADA, 0x58C9290AB1A06B85, 0xBD5D10F4CB3E0567, 0xE427418BA77D95D8,
		0xFBEE7C66DD17479E, 0xCA2DBF07AD5A8333
	};
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			state[i][j] = 0;
		}
	}
	for (int i = 0; i < 64; i++) {
		state[i / 8][i % 8] = input[i];
	}

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			K[i][j] = 0;
		}
	}
	add_round_key(state, K);

	for (int i = 0; i < 10; i++) {
		sub_bytes(state);
		shift_columns(state);
		mix_rows(state);
		expand_key(K, RC[i]);
		add_round_key(state, K);
	}

	char *hash = (char *)malloc(64);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			hash[8 * i + j] = state[j][i];
		}
	}
	return (hash);
}

char *whirlpool(const char *input, size_t len, size_t total_size) {
	char *hash = NULL;
	if (len % 64 != 0) {
		size_t new_len = len + 1;
		while (new_len % 64 != 32)
			new_len++;
		new_len += 32;  // 256 bits
		char *msg = (char *)malloc(new_len);
		ft_memcpy(msg, input, len);
		msg[len] = 0x80;  // ajout du bit 1 à la fin du message
		for (size_t i = len + 1; i < new_len - 32; i++)
			msg[i] = 0;
		unsigned char bit_len[32] = {0};
		unsigned long long bit_len_low = total_size * 8;
		unsigned long long bit_len_high = total_size >> 61;
		for (int i = 0; i < 8; i++) {
			bit_len[31 - i] = (bit_len_low >> (8 * i)) & 0xFF;
		}
		for (int i = 0; i < 8; i++) {
			bit_len[23 - i] = (bit_len_high >> (8 * i)) & 0xFF;
		}
		ft_memcpy(msg + new_len - 32, bit_len, 32);
		char *orig_msg = msg;
		while (new_len >= 64) {
			if (hash)
				free(hash);
			hash = process_block_whirlpool(msg);
			msg += 64;
			new_len -= 64;
		}
		free(orig_msg);
	}
	while (len >= 64) {
		if (hash)
			free(hash);
		hash = process_block_whirlpool(input);
		input += 64;
		len -= 64;
	}
	return (hash);
}

void whirlpool_file(const char *filename, t_options *options) {
	int fd = open(filename, O_RDONLY);
	if (fd < 0) {
		perror(filename);
		return;
	}
	char buffer[1024];
	char *hash = NULL;
	size_t total_size = 0;
	while (1) {
		ssize_t bytes_read = read(fd, buffer, 128);
		if (bytes_read < 0) {
			perror("read");
			close(fd);
			return;
		}
		if (bytes_read == 0)
			break;
		total_size += bytes_read; // for the padding at the end
		if (hash)
			free(hash);
		hash = whirlpool(buffer, bytes_read, total_size);
	}
	if (options->q == 0) {
		if (options->r == 0) {
			print_hexa(hash, 64);
			write(1, "  ", 2);
			if (ft_strncmp(filename, "/dev/stdin", 10) == 0)
				write(1, "-", 1);
			else
				write(1, filename, ft_strlen(filename));
		} else {
			if (ft_strncmp(filename, "/dev/stdin", 10) == 0)
				write(1, "-", 1);
			else
				write(1, filename, ft_strlen(filename));
			write(1, "  ", 2);
			print_hexa(hash, 64);
		}
	} else {
		print_hexa(hash, 64);
	}
	free(hash);
}

void whirlpool_helper(t_options *options) {
	t_string_list *current = options->strings;
	while (current) {
		if (current->from_file)
			whirlpool_file(current->str, options);
		else {
			size_t len = ft_strlen(current->str);
			char *hash = whirlpool(current->str, len, len);
			if (options->q == 0) {
				if (options->r == 0) {
					print_hexa(hash, 64);
					write(1, "  ", 2);
					write(1, "-", 1);
				} else {
					write(1, "-", 1);
					write(1, "  ", 2);
					print_hexa(hash, 64);
				}
			} else {
				print_hexa(hash, 64);
			}
			free(hash);
		}
		write(1, "\n", 1);
		current = current->next;
	}
}
