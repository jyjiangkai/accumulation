static inline void USLEEP(INT32 num)
{                                                               
    struct timespec t_tv;                    
    t_tv.tv_sec = (num) / 1000000;           
    t_tv.tv_nsec = ((num) % 1000000) * 1000; 
    (VOID) nanosleep(&t_tv, NULL);           
}
