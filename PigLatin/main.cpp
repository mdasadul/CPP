
#include <iostream>
#include<stdio.h>

using namespace std;

class PigLatin
{
    string inputText;
    string outputText;
    bool isvowel(char);
public:
    void getText();
    void convertToPigLation();
    void displayText();
};
bool PigLatin::isvowel( char c)
{
    c=tolower(c);
    if(c == 'a'|| c == 'e' || c == 'i' || c == 'o' || c == 'u')
    {
        return true;
    }
    else
    {
        return false;
    }
}

void PigLatin::getText()
{
    freopen("inputText.txt","r",stdin);
    freopen("outputText.txt","w",stdout);
    char tempInput;
    int i;
    string originalWord,pigLatinWord;
    while(scanf("%c",&tempInput)!=EOF)
    {

        //printf("%c",tempInput);
        if(isalpha(tempInput))
           {
                originalWord += tempInput;
           }
           else
           {
               if(isvowel(originalWord[0]))
               {
                    pigLatinWord = originalWord+"yay";
               }
               else
               {
                    for(i=0;i<originalWord.length();i++)
                    {

                        if(isvowel(originalWord[i]))
                        {
                            pigLatinWord = originalWord.substr(i);
                            pigLatinWord += originalWord.substr(0,i)+"ay";
                            break;
                        }
                    }

               }
                outputText += pigLatinWord +tempInput;
                pigLatinWord = "";
                originalWord = "";
           }
    }
    printf("%s",outputText.c_str());
}
void PigLatin::convertToPigLation()
{

}
void PigLatin::displayText()
{


}

int main()
{
    PigLatin pigLationObj;
    pigLationObj.getText();
    return 0;
}
