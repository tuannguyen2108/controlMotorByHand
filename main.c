#include <msp430g2553.h>
#include <stdio.h>
#include <stdlib.h>

char data[3];
int i = 0;
unsigned long num = 0;
float value; // Luu tru gi� tri d�ng cho dat gi� tri dau ra xung PWM.

float map(float x, float in_min, float in_max,float out_min, float out_max)
{
 return(x - in_min)*(out_max - out_min)/(in_max - in_min)+out_min;
}

void main(void)
{ 
  WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
  BCSCTL1 = CALBC1_1MHZ; // Thiet lap tan so nap DCO l�n 1MHz
  DCOCTL = CALDCO_1MHZ;  // Thiet lap tan so nap DCO l�n 1MHz
 
  P1DIR |= BIT6;  // Dat chan p1.6 la dau ra
  P1SEL |= BIT1 + BIT2 + BIT6; // Su dung chuc nang Timer/Counter (TA0.1)

  TACTL = TASSEL_2 + MC_1; // Cau hinh xung va timer A1.1 
  TACCTL1 = OUTMOD_7; // Che do dau ra xung PWM (Reset/Set)
  TACCR0 = 10000; // Dat gia tri gioi han cua Timer A
  TACCR1 = 0;  // Dat gia tri dau ra ban dau cua Timer A1.1 la 0

  // Cau hinh chan UART 1 2
  //P1SEL = BIT1 + BIT2 ; // P1.1 = RXD, P1.2=TXD
  P1SEL2 = BIT1 + BIT2 ; // P1.1 = RXD, P1.2=TXD

  UCA0CTL1 |= UCSSEL_2; // Thiet lap nguon xung cho UART la SMCLK
  UCA0BR0 = 104; // 1MHz 9600
  UCA0BR1 = 0; // 1MHz 9600
  UCA0MCTL = UCBRS_1; // Thiet lap gia tri cua thanh ghi kiem saot toc do bit UART
  UCA0CTL1 &= ~UCSWRST; // Xoa co SWRST de cho phep UART hoat dong
  IE2 |= UCA0RXIE; // Kich hoat ngat nhan du lieu UART
  _BIS_SR(GIE);
}

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void)
{
  data[i] = UCA0RXBUF; // Nhan du lieu PC truyen qua MSP duoc luu trong mang data
  i++;
  if(i>2){i=0;}
  num = atoi(data);// Chuyen ky tu luu trong data thanh so nguyen luu trong num
  value = map(num,20,120,0,10000); //H�m map() de anh xa gia tri num tu khoang 20-120 sang khoang 0-10000.
  TACCR1 = (int)(value);
}

