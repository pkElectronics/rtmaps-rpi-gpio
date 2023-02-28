#pragma once

extern "C" {


#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>

#define DRIVE_UNSET -1
#define DRIVE_LOW    0
#define DRIVE_HIGH   1

#define PULL_UNSET  -1
#define PULL_NONE    0
#define PULL_DOWN    1
#define PULL_UP      2

#define FUNC_UNSET  -1
#define FUNC_IP      0
#define FUNC_OP      1

struct gpio_chip
{
	const char *name;
	uint32_t reg_base;
	uint32_t reg_size;
	unsigned int gpio_count;
	unsigned int fsel_count;
	const char *info_header;
	const char **alt_names;
	const int *default_pulls;

	int(*get_level)(struct gpio_chip *chip, unsigned int gpio);
	int(*get_fsel)(struct gpio_chip *chip, unsigned int gpio);
	int(*get_pull)(struct gpio_chip *chip, unsigned int gpio);
	int(*set_level)(struct gpio_chip *chip, unsigned int gpio, int level);
	int(*set_fsel)(struct gpio_chip *chip, unsigned int gpio, int fsel);
	int(*set_pull)(struct gpio_chip *chip, unsigned int gpio, int pull);
	int(*next_reg)(int reg);

	volatile uint32_t *base;
};

void gpio_initialize(void);
int gpio_get(unsigned int gpio);

int gpio_set(unsigned int gpio, int fsparam, int drive, int pull);

extern struct gpio_chip *get_gpio_chip(void);
}
