// static int __ref kernel_init(void *unused) ~ Line 1462
int link_hello(void);

    #if defined(CONFIG_HELLO) 
        ret = link_hello();
    #endif
