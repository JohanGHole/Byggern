
// three pins from the RS-232 interface; RX, TX and GRD

// 1) The application tries to send a new character while the UART is busy transmitting the previous one
// 2) The application wants to be notified when a new character is received

// set the baud rate, set frame format, enable transmitter for the receiver depending on the usage. 
// For interrupt driven USART operation, the global interrupt flag should be cleared.
// The TXC Flag can be used to check that the transmitter has completed all transfers
// The RXC flag can be used to check that there are no unread data in the receive buffer. 
// NB, the TXC flag must be cleared before each transmission (Before UDR is written) if it is used for this purpose.
// simplest: fixed frame format, asynchronous polling. 

void uart_init();

void uart_send(unsigned char letter);

unsigned char uart_read(void);

__attribute__((format(printf, 1, 2))) void uart_printf(const char* fmt, ...);