#include <ngspice/sharedspice.h>

#include <iostream>

int send_char(char* msg, int, void*)
{
    if(msg)
        std::cout << msg << '\n';

    return 0;
}

int controlled_exit(int status, bool, bool, int, void*)
{
    std::cout << "ngspice terminou com status "
              << status << '\n';

    return 0;
}

int main()
{
    ngSpice_Init(
        send_char,
        nullptr,
        controlled_exit,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    );

    char command[] = "version";

    ngSpice_Command(command);

    return 0;
}
