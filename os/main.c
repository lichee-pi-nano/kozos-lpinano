#include "defines.h"
#include "kozos.h"
#include "interrupt.h"
#include "intr.h"
#include "lib.h"

/* システム・タスクとユーザ・タスクの起動 */
static int start_threads(int argc, char *argv[])
{
  int dummy;

  kz_run(consdrv_main, "consdrv",  1, 0x200, 0, NULL);
  kz_run(command_main, "command",  8, 0x200, 0, NULL);

  kz_chpri(15); /* 優先順位を下げて，アイドルスレッドに移行する */
  INTR_ENABLE; /* 割込み有効にする */
  while (1) {
    /* wait for interrupt */
    asm volatile ("mcr p15, 0, %0, c7, c0, 4" : "=r"(dummy));
  }

  return 0;
}

int main(void)
{
  INTR_DISABLE; /* 割込み無効にする */
  intc_init();	/* 割込みコントローラを初期化する */

  puts("kozos boot succeed!\n");

  /* OSの動作開始 */
  kz_start(start_threads, "idle", 0, 0x100, 0, NULL);
  /* ここには戻ってこない */

  return 0;
}
