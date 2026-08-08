#include "Core/Application.h"

int main()
{
    Pelvis::Application application;

    if (!application.initialize())
    {
        return 1;
    }

    return application.run();
}
