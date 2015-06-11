/*
*
*PigLatin is a Language Game in which English word are altered.
* For more information please have a look on wiki(http://en.wikipedia.org/wiki/Pig_Latin)
*
* In this program I implemented two rules
* 1)For words that begin with consonant sounds,
*  the initial consonant or consonant cluster is
*  moved to the end of the word, and "ay"  is added,
   as in the following examples:
   "pig" → "igpay"
* 2) For words which begin with vowel sounds or silent letter, one just adds "yay" to the end. Examples are:
    "eat" → "eatyay"
* @author Md Asadul Islam Date: 11/06/2015
*/

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
    string convertToPigLation(string);
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

    char tempInput;
    string originalWord,pigLatinWord;

    while(scanf("%c",&tempInput)!=EOF)
    {
        if(isalpha(tempInput))
        {
            originalWord += tempInput;
        }
        else
        {
            pigLatinWord = convertToPigLation(originalWord);
            outputText += pigLatinWord +tempInput;
            pigLatinWord = "";
            originalWord = "";
        }
    }

}



string PigLatin::convertToPigLation(string originalWord)
{
    string wordInPigLatin="";
    signed int i;
    if(isvowel(originalWord[0]))
    {
        wordInPigLatin = originalWord+"yay";
    }
    else
    {
        for(i=0;i<originalWord.length();i++)
        {
            if(isvowel(originalWord[i]))
            {
                wordInPigLatin = originalWord.substr(i);
                wordInPigLatin += originalWord.substr(0,i)+"ay";
                break;
            }
        }
    }
    return wordInPigLatin;
}



void PigLatin::displayText()
{
    freopen("outputText.txt","w",stdout);
    printf("%s",outputText.c_str());
}



int main()
{
    PigLatin pigLationObj;
    pigLationObj.getText();
    pigLationObj.displayText();
    return 0;
}
