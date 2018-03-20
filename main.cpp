#include <iostream>
#include <fstream>
#include <vector>

//first index is the table
//last index is the resulting query
//the rest are the fields
std::string to_lower(std::string str)
{
    for ( std::size_t a = 0; a < str.size(); a++ )
    {
        str[a] += ( str[a] >= 'A' && str[a] <= 'Z' ) ? 32 : 0;
    }
    return str;
}
std::vector<std::string> convertFromRegularQuery(const std::string& query)
{
    std::vector<std::string> fields;
    std::string temp = "\"";
    std::string field = "";
    bool found = false;
    for ( std::size_t a = 0; a < query.size(); a++ )
    {
        if ( query[a] != '`' && query[a] != '"' )
        {
            if ( !found )
                temp += query[a];
            else
                field += (query[a] - (query[a] >= 'a' && query[a] <= 'z' ? 32 : 0));
        }
        else
        {
            found = !found;
            if(!found)
            {
                found = false;
                temp += "\" + " + field + " + \"";
                fields.push_back(field);
                field = "";
            }
        }
    }
    fields.push_back(temp + "\"");
    return fields;

}
void createJavaClass(const std::vector<std::string>& schema)
{
    std::ofstream of((schema[0] + ".txt").c_str());
    if ( of.is_open() )
    {
        of << "public class Schema_" + schema[0] + " {" << std::endl;
        of << "public static final String TABLE_NAME = \"" + to_lower(schema[0]) + "\";" << std::endl;
        for ( std::size_t a = 1; a < schema.size() - 1; a++ )
        {
            of << "public static final String " << schema[a] << " = \"" << to_lower(schema[a]) << "\";" << std::endl;
        }
        of << "public static final String SCHEMA =\"" << schema[schema.size() - 1] << "\";" << std::endl;
        of << "}";
        of.close();
    }
}
int main(int argc, char* argv[])
{
    if ( argc > 1 )
    {
        std::vector<std::string> result = convertFromRegularQuery(argv[1]);
        createJavaClass(result);
        std::cout << "Success!" << std::endl;
    }
    else
    {

        std::cout << "Help: " << std::endl;
        std::cout << "exe [\"Create statement\"]";
        std::cin.get();
    }


}
