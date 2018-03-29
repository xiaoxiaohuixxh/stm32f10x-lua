#include "stm32f10x.h"
#include "leddemo.h"
#include "rtcdemo.h"
#include "usartdemo.h"
#include "pwmdemo.h"
#include "lcddemo.h"

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "stdlib.h"
#include "malloc.h"//内存管理

static int lua_led_on(lua_State *L)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_All); 
	return 1;
}
static int lua_led_off(lua_State *L)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_All);
	return 1;
}
static int lua_delay(lua_State *L)
{
	int num;
	uint16_t i=0;
	num= lua_tointeger(L, 1);
   while(num--)
   {
      i=12;
      while(i--);
   }
	return 1;
}
static const struct luaL_Reg mylib[] =
{
	{"led_on",lua_led_on},
	{"led_off",lua_led_off},
	{"delay",lua_delay},
	{NULL,NULL}
};
const char LUA_SCRIPT_GLOBAL[] ="  \
while 1 do \
 print(\"Hello,I am lua!\\n--this is newline\\n\") \
end";
#include "time.h"
time_t time(time_t * time) {  
    return 0;  
}  
  
void exit(int status)  {
	(void)status;
}  
  
int system(const char * string) {  
    return 0;  
}
typedef int FILEHANDLE;

FILEHANDLE _sys_open(const char *name, int openmode) {
	return 0;
}

int _sys_close(FILEHANDLE fh) {
	return 0;
}

int _sys_write(FILEHANDLE fh, const unsigned char *buf, unsigned len, int mode) {
	return 0;
}

int _sys_read(FILEHANDLE fh, unsigned char*buf, unsigned len, int mode) {
	return 0;
}

//检查句柄是否为终端
int _sys_istty(FILEHANDLE fh) {
	return 0;
}

int _sys_seek(FILEHANDLE fh, long pos) {
	return 0;
}

//刷新句柄关联的缓冲区
int _sys_ensure(FILEHANDLE fh) {
	return 0;
}

 

//返回文件当前长度
long _sys_flen(FILEHANDLE fh) {
	return 0;
}

void _sys_exit(int status) {
	//while(1);
}

int _sys_tmpnam(char *name, int fileno, unsigned maxlength) {
	return 0;
}

//将一个字符写入控制台
void _ttywrch(int ch) {
	
}

int remove(const char *filename) {
	return 0;
}

char *_sys_command_string(char *cmd, int len) {
	return NULL;
}

int fputc(int ch, FILE *f)  
{
    usartdemo_Putchar(ch);
    return ch;  
}

int main()
{
	lua_State *L =NULL;
	mem_init(SRAMIN);		//初始化内部内存池
	leddemo_main();
	usartdemo_main();
	GPIO_SetBits(GPIOA,GPIO_Pin_All);
	L = luaL_newstate();  /* create state */
	//realloc(NULL, sizeof(LG));
	GPIO_ResetBits(GPIOA,GPIO_Pin_All);
  if(L == NULL)
		GPIO_ResetBits(GPIOA,GPIO_Pin_All); 
	
	luaopen_base(L);
	luaL_setfuncs(L,mylib, 0);
	int ret= luaL_dostring(L, LUA_SCRIPT_GLOBAL);
	if (ret != 0)
		return 0;
	lua_close(L);
	//rtcdemo_main();
	//pwmdemo_main();
	//LcdDemo_main();
	while(1);
}