#ifdef CONFIG_HELLO
extern int hello(void);
#endif

#ifdef CONFIG_UPTIME
extern unsigned long uptime_seconds;
#endif

//static int __ref kernel_init(void *unused)
//{
//int ret;
unsigned long uptime;

#if defined(CONFIG_HELLO)
ret = hello();
#endif
#if defined(CONFIG_UPTIME)
uptime = uptime_seconds;
#endif
