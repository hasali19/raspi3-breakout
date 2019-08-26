#pragma once

/**
 * Initialises the mini uart peripheral.
 */
void mini_uart_init(void);

/**
 * Reads a character from the mini uart.
 */
char mini_uart_read(void);

/**
 * Reads a character from the mini uart, returning immediately if there is no
 * data.
 */
char mini_uart_poll(void);

/**
 * Writes a character to the mini uart.
 */
void mini_uart_write(char c);

/**
 * Writes a string to the mini uart.
 */
void mini_uart_write_string(const char* str);
