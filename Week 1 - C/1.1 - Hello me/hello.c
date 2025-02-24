#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Get user name
    string name = get_string("What is your name?\n");

    // Print hello + user name
    printf("hello, %s\n", name);
}
