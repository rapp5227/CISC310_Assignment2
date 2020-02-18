#include <vector>
#include <string>

std::vector<std::string> splitString(std::string text, char d)
{
    std::vector<std::string> result;

    int start = 0;
    int size = 0;

    text = "test_test_shenanigans_test__";
    d = '_';

    for(int i = 0;i < text.length();i++)
    {
        ++size;

        if(text[i] == d)
        {
            result.push_back(text.substr(start,size-1));

            start = i + 1;
            size = 0;
        }
    }
    
    result.push_back(text.substr(start,size));

    return result;
}

int main(int argc, char** argv)
{
    std::vector<std::string>    v = splitString("",'c');

    for(int i = 0;i < v.size();i++)
    {
        printf(">%s<\n",v[i].data());
    }
}
