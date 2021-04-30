
typedef void (*Constructor)(void);
typedef void (*Destructor)(void);

extern "C" Constructor start_ctors;
extern "C" Constructor end_ctors;

extern "C" Destructor start_dtors;
extern "C" Destructor end_dtors;

extern "C" void init(void) 
{
    for (Constructor* constructor = &start_ctors; constructor != &end_ctors; constructor++) {

        (*constructor)();

    }
}

extern "C" void fini(void) 
{
    for (Destructor* destructor = &start_dtors; destructor != &end_dtors; destructor++) {

        (*destructor)();

    }
}