#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

/*PAX*/
#include "osal.h"
#include "ui.h"
#include <xui.h>
// #include "emvlib_Prolin.h"
#include "MID/structDefine.h"
#include "keyboard.h"

/* Include the mruby header */
#include "mruby.h"
#include "mruby/value.h"
#include "mruby/compile.h"
#include "mruby/proc.h"

ST_EVENT_MSG   *glEdcMsgPtr;					// manager event

static int main_sub(const ST_EVENT_MSG *pstEventMsg);

extern int event_main(ST_EVENT_MSG *pstEventMsg);

const uint8_t start[] = {
  0x52,0x49,0x54,0x45,0x30,0x30,0x30,0x32,0x28,0x53,0x00,0x00,0x00,0xa2,0x4d,0x41,
  0x54,0x5a,0x30,0x30,0x30,0x30,0x49,0x52,0x45,0x50,0x00,0x00,0x00,0x55,0x30,0x30,
  0x30,0x30,0x00,0x00,0x00,0x49,0x00,0x01,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x05,
  0x00,0x80,0x00,0x11,0x01,0x00,0x00,0x3d,0x01,0x80,0x00,0xbd,0x00,0x80,0x41,0x20,
  0x00,0x00,0x00,0x4a,0x00,0x00,0x00,0x02,0x00,0x00,0x04,0x6d,0x61,0x69,0x6e,0x00,
  0x00,0x03,0x50,0x41,0x58,0x00,0x00,0x00,0x02,0x00,0x07,0x43,0x6f,0x6e,0x74,0x65,
  0x78,0x74,0x00,0x00,0x05,0x73,0x74,0x61,0x72,0x74,0x00,0x44,0x42,0x47,0x00,0x00,
  0x00,0x00,0x2f,0x00,0x01,0x00,0x08,0x73,0x74,0x61,0x72,0x74,0x2e,0x72,0x62,0x00,
  0x00,0x00,0x1b,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x00,
  0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x45,0x4e,0x44,0x00,0x00,0x00,
  0x00,0x08,
};

mrb_state *mrb;

mrb_state *mrb_robot(void)
{
  return mrb;
}

int robot_rock_execute(void)
{
  mrbc_context *c;
  mrb = mrb_open();

  c = mrbc_context_new(mrb);

  mrb_load_irep_cxt(mrb, start, c);

  mrbc_context_free(mrb, c);

  mrb_close(mrb);

  return 0;
}

static int GuiInit(void)
{
  int ret;
  int  xui_argc=2;
  char *xui_argv[] = {"ROTATE=0","STATUSBAR=22"};

  ret = XuiOpen(xui_argc, xui_argv);
  if (ret == XUI_RET_OK) {
    return RET_OK;
  }
  else {
    return -1;
  }

}

static void GuiDeinit(void)
{
  XuiClose();
}

static void CrashReportInit(void)
{
  signal(SIGILL,    OsSaveCrashReport);
  signal(SIGABRT,   OsSaveCrashReport);
  signal(SIGBUS,    OsSaveCrashReport);
  signal(SIGFPE,    OsSaveCrashReport);
  signal(SIGSEGV,   OsSaveCrashReport);
  signal(SIGSTKFLT, OsSaveCrashReport);
  signal(SIGPIPE,   OsSaveCrashReport);
}

void StatusbarInit(void)
{
  XuiSetStatusbarIcon(0,"./res/mobile100.png");
  XuiSetStatusbarIcon(1,"./res/modem.png");
  XuiSetStatusbarIcon(2,"./res/ethernet.png");
  XuiSetStatusbarIcon(3,"./res/wifi100.png");
  XuiSetStatusbarIcon(4,"./res/lock.png");
  XuiSetStatusbarIcon(5,"./res/card.png");
  XuiSetStatusbarIcon(6,"./res/print.png");
  XuiSetStatusbarIcon(7,"./res/battery0c.png");
}

int ScreenInit(void)
{
  XuiColor colorTitleBg;

  colorTitleBg.r = 0xff;
  colorTitleBg.g = 0xff;
  colorTitleBg.b = 0xff;
  colorTitleBg.a = 0x00;

  XuiCanvasSetBackground(XuiRootCanvas(), XUI_BG_NORMAL, NULL, colorTitleBg);

  StatusbarInit();

  return 0;
}

void Init(void)
{
  CrashReportInit();
  GuiInit();
  ScreenInit();
  OpenFont();

  return;
}

int DeInit()
{
  CloseFont();
  GuiDeinit();

  return 0;
}

void emv_test_open(void)
{
  /*Should return 0*/
  // display("EMVCore Init %d", EMVCoreInit());
}

int event_main(ST_EVENT_MSG *pstEventMsg)
{
	glEdcMsgPtr = pstEventMsg;	// save the event pointer
	return main_sub(pstEventMsg);
}

int main(int argc, char **argv)
{
  ST_EVENT_MSG	stEventMsg;
  event_main(&stEventMsg);
  return 0;
}

int main_sub(const ST_EVENT_MSG *pstEventMsg)
{
  Init();
  SystemInit();
  robot_rock_execute();
  DeInit();

  return 0;
}
