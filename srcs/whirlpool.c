// #include "ft_ssl_md5.h"

// unsigned long long CState[8][8]; //the plaintext
// unsigned long long KState[8][8]; //the key
// unsigned long long HState[8][8]; // the hash

// unsigned long long sbox(unsigned long long x)
// {
// 	unsigned long long ebox[16] = {0x1,0xb,0x9,0xc,0xd,0x6,0xf,0x3,0xe,0x8,0x7,0x4,0xa,0x2,0x5,0x0};
// 	unsigned long long eboxinv[16] = {0xf,0x0,0xd,0x7,0xb,0xe, 0x5, 0xa, 0x9, 0x2, 0xc, 0x1, 0x3, 0x4, 0x8, 0x6};
// 	unsigned long long rbox[16]={0x7, 0xc, 0xb, 0xd, 0xe, 0x4, 0x9, 0xf, 0x6, 0x3, 0x8, 0xa, 0x2, 0x5, 0x1, 0x0};
// 	unsigned long long result;

// 	result = ebox[rbox[ebox[x >> 4] ^ eboxinv[x & 0xF]] ^ ebox[x >> 4]] << 4;
// 	result ^= eboxinv[rbox[ebox[x >> 4] ^ eboxinv[x & 0xF]] ^ eboxinv[x & 0xF]];
// 	return (result);
// }

// void sub_bytes() {
// 	for(int i=0; i<64; i++){
// 		CState[i/8][i%8]=sbox(CState[i/8][i%8]);
// 		KState[i/8][i%8]=sbox(KState[i/8][i%8]);
//   }
// }

// void shift_columns()
// {
// 	unsigned long long tempCState[64], tempKState[64];
// 	for (int i=0; i<64; i++)
// 	{
// 		tempCState[i]=0;
// 		tempKState[i]=0;
// 	}
// 	tempCState[0] = CState[0][0];
// 	tempKState[0] = KState[0][0];
// 	for (int i=1; i<64; i++){
// 		tempCState[i] = CState[(((64-i)*7)%64)/8][(((64-i)*7)%64)%8];
// 		tempKState[i] = KState[(((64-i)*7)%64)/8][(((64-i)*7)%64)%8]; // weird but in formula
// 	}
// 	for (int i=0; i<64; i++)
// 	{
// 		CState[i/8][i%8] = tempCState[i];
// 		KState[i/8][i%8] = tempKState[i];
// 	}

// }

// void mix_rows() {
// 	const unsigned char M[8][8] = {
// 		{0x01, 0x01, 0x04, 0x01, 0x08, 0x05, 0x02, 0x09},
// 		{0x09, 0x01, 0x01, 0x04, 0x01, 0x08, 0x05, 0x02},
// 		{0x02, 0x09, 0x01, 0x01, 0x04, 0x01, 0x08, 0x05},
// 		{0x05, 0x02, 0x09, 0x01, 0x01, 0x04, 0x01, 0x08},
// 		{0x08, 0x05, 0x02, 0x09, 0x01, 0x01, 0x04, 0x01},
// 		{0x01, 0x08, 0x05, 0x02, 0x09, 0x01, 0x01, 0x04},
// 		{0x04, 0x01, 0x08, 0x05, 0x02, 0x09, 0x01, 0x01},
// 		{0x01, 0x04, 0x01, 0x08, 0x05, 0x02, 0x09, 0x01}
// 	};
// 	unsigned int tempCState[8][8];
// 	unsigned int tempKState[8][8];
// 	for (int i = 0; i < 64; i++)
// 	{
// 		tempCState[i / 8][i % 8] = 0;
// 		tempKState[i / 8][i % 8] = 0;
// 	}
// 	for (int i = 0; i < 64; i++)
// 	{
// 		for (int j = 0; j < 8; j++)
// 		{
// 			tempCState[i / 8][i % 8] ^= M[j][i % 8] * CState[i / 8][j];
// 			tempKState[i / 8][i % 8] ^= M[j][i % 8] * KState[i / 8][j];
// 		}
// 	}
// 	for (int i = 0; i < 64; i++)
// 	{
// 		CState[i / 8][i % 8] = tempCState[i / 8][i % 8];
// 		KState[i / 8][i % 8] = tempKState[i / 8][i % 8];
// 	}
// }



// void add_round_const(int rc)
// {
// 	unsigned long long RC[8][8];
// 	for (int i = 0; i < 8; i++)
// 	{
// 		for (int j = 0; j < 8; j++)
// 			RC[i][j] = 0;
// 	}
// 	for (int i = 0; i < 8; i++)
// 	{
// 		RC[0][i] = sbox((8 * rc + i));
// 	}
// 	for (int i = 0; i < 64; i++)
// 	{
// 		KState[i / 8][i % 8] ^= RC[i / 8][i % 8];
// 	}
// }

// void add_round_key()
// {
// 	for (int i = 0; i < 64; i++)
// 	{
// 		CState[i / 8][i % 8] ^= KState[i / 8][i % 8];
// 	}
// }

// char *process_block_whirlpool(const char *input)
// {
// 	const unsigned long long RC[10] = {
// 		0x1823C6E887B8014F, 0x36A6D2F5796F9152, 0x60BC9B8EA30C7B35, 0x1DE0D7C22E4BFE57,
// 		0x157737E59FF04ADA, 0x58C9290AB1A06B85, 0xBD5D10F4CB3E0567, 0xE427418BA77D95D8,
// 		0xFBEE7C66DD17479E, 0xCA2DBF07AD5A8333
// 	};


// 	add_round_key();

// 	for (int i = 0; i < 10; i++) {
// 		sub_bytes();
// 		shift_columns();
// 		mix_rows();
// 		add_round_const(RC[i]);
// 		add_round_key();
// 	}

// 	char *digest = (char *)malloc(64);
// 	for (int i = 0; i < 8; i++)
// 	{
// 		digest[i] = state[0] >> (56 - (i * 8)) & 0xFF;
// 		digest[i + 8] = state[1] >> (56 - (i * 8)) & 0xFF;
// 		digest[i + 16] = state[2] >> (56 - (i * 8)) & 0xFF;
// 		digest[i + 24] = state[3] >> (56 - (i * 8)) & 0xFF;
// 		digest[i + 32] = state[4] >> (56 - (i * 8)) & 0xFF;
// 		digest[i + 40] = state[5] >> (56 - (i * 8)) & 0xFF;
// 		digest[i + 48] = state[6] >> (56 - (i * 8)) & 0xFF;
// 		digest[i + 56] = state[7] >> (56 - (i * 8)) & 0xFF;

// 	}
// 	return (digest);
// }

// char *whirlpool(const char *input, size_t len, size_t total_size) {
// 	char *hash = NULL;
// 	for (int i = 0; i < 8; i++) {
// 		for (int j = 0; j < 8; j++) {
// 			K[i][j] = 0;
// 		}
// 	}
// 	if (len % 64 != 0) {
// 		size_t new_len = len + 1;
// 		while (new_len % 64 != 32)
// 			new_len++;
// 		new_len += 32;  // 256 bits
// 		char *msg = (char *)malloc(new_len);
// 		ft_memcpy(msg, input, len);
// 		msg[len] = 0x80;  // ajout du bit 1 à la fin du message
// 		for (size_t i = len + 1; i < new_len - 32; i++)
// 			msg[i] = 0;
// 		unsigned char bit_len[32] = {0};
// 		unsigned long long bit_len_low = total_size * 8;
// 		unsigned long long bit_len_high = total_size >> 61;
// 		for (int i = 0; i < 8; i++) {
// 			bit_len[31 - i] = (bit_len_low >> (8 * i)) & 0xFF;
// 		}
// 		for (int i = 0; i < 8; i++) {
// 			bit_len[23 - i] = (bit_len_high >> (8 * i)) & 0xFF;
// 		}
// 		ft_memcpy(msg + new_len - 32, bit_len, 32);
// 		char *orig_msg = msg;
// 		while (new_len >= 64) {
// 			if (hash)
// 				free(hash);
// 			hash = process_block_whirlpool(msg);
// 			msg += 64;
// 			new_len -= 64;
// 		}
// 		free(orig_msg);
// 	}
// 	while (len >= 64) {
// 		if (hash)
// 			free(hash);
// 		hash = process_block_whirlpool(input);
// 		input += 64;
// 		len -= 64;
// 	}
// 	return (hash);
// }

// void whirlpool_file(const char *filename, t_options *options) {
// 	int fd = open(filename, O_RDONLY);
// 	if (fd < 0) {
// 		perror(filename);
// 		return;
// 	}
// 	char buffer[1024];
// 	char *hash = NULL;
// 	size_t total_size = 0;
// 	while (1) {
// 		ssize_t bytes_read = read(fd, buffer, 128);
// 		if (bytes_read < 0) {
// 			perror("read");
// 			close(fd);
// 			return;
// 		}
// 		if (bytes_read == 0)
// 			break;
// 		total_size += bytes_read; // for the padding at the end
// 		if (hash)
// 			free(hash);
// 		hash = whirlpool(buffer, bytes_read, total_size);
// 	}
// 	if (options->q == 0) {
// 		if (options->r == 0) {
// 			print_hexa(hash, 64);
// 			write(1, "  ", 2);
// 			if (ft_strncmp(filename, "/dev/stdin", 10) == 0)
// 				write(1, "-", 1);
// 			else
// 				write(1, filename, ft_strlen(filename));
// 		} else {
// 			if (ft_strncmp(filename, "/dev/stdin", 10) == 0)
// 				write(1, "-", 1);
// 			else
// 				write(1, filename, ft_strlen(filename));
// 			write(1, "  ", 2);
// 			print_hexa(hash, 64);
// 		}
// 	} else {
// 		print_hexa(hash, 64);
// 	}
// 	free(hash);
// }

// void whirlpool_helper(t_options *options) {
// 	t_string_list *current = options->strings;
// 	while (current) {
// 		if (current->from_file)
// 			whirlpool_file(current->str, options);
// 		else {
// 			size_t len = ft_strlen(current->str);
// 			char *hash = whirlpool(current->str, len, len);
// 			if (options->q == 0) {
// 				if (options->r == 0) {
// 					print_hexa(hash, 64);
// 					write(1, "  ", 2);
// 					write(1, "-", 1);
// 				} else {
// 					write(1, "-", 1);
// 					write(1, "  ", 2);
// 					print_hexa(hash, 64);
// 				}
// 			} else {
// 				print_hexa(hash, 64);
// 			}
// 			free(hash);
// 		}
// 		write(1, "\n", 1);
// 		current = current->next;
// 	}
// }
