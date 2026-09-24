#include <cstdio>
#include <iostream>
#include <string>
#include <memory>

class PipeOwner
{
    public:
       PipeOwner()
       : pipe( popen("find $HOME -name 'tones3.cpp' -print", "r") )
       {
       }

       ~PipeOwner()
       {
          pclose( pipe );
       }

    private:
        FILE* pipe = nullptr;
};

int main()
{
    std::unique_ptr<PipeOwner> uptr = std::make_unique<PipeOwner>();

    if ( !uptr.get() )
        return 1;

    char buffer[256];
    std::string result;

    //while (fgets(buffer, sizeof(buffer), pipe))
    //    result += buffer;

    //pclose(pipe);

    std::cout << result;
}
