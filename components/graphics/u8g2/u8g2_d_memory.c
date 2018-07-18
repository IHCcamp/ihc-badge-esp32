/* u8g2_d_memory.c */

#include "u8g2.h"

uint8_t *u8g2_m_11_6_1(uint8_t *page_cnt)
{
  static uint8_t buf[88];
  *page_cnt = 1;
  return buf;
}
uint8_t *u8g2_m_11_6_2(uint8_t *page_cnt)
{
  static uint8_t buf[176];
  *page_cnt = 2;
  return buf;
}
uint8_t *u8g2_m_11_6_f(uint8_t *page_cnt)
{
  static uint8_t buf[528];
  *page_cnt = 6;
  return buf;
}
