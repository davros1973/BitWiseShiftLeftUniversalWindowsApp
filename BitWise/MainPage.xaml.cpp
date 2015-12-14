//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"


#include <regex>

using namespace HelloWorld;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

#define BIG_ENDIAN      0
#define LITTLE_ENDIAN   1

unsigned int intGlbNumPlaces = 1;


int TestByteOrder()
{
    short int word = 0x0001;
    char *byte = (char *)&word;
    return(byte[0] ? LITTLE_ENDIAN : BIG_ENDIAN);
}

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();
}

MainPage::~MainPage()
{
}

/*
    ^ means handle declarator - modifies the type specifier to mean 
    that the dclared object should be automatically deleted when the 
    system determines that the object is no longer accessible


    A variable that is declared with the handle declarator behaves like a pointer to the object. 
    However, the variable points to the entire object, cannot point to a member of the object, 
    and it does not support pointer arithmetic. Use the indirection operator (*) to access the object, 
    and the arrow member-access operator (->) to access a member of the object.

    https://msdn.microsoft.com/en-us/library/69ze775t.aspx
    A wide string literal is a null-terminated array of constant wchar_t that is prefixed by 'L' and 
    contains any graphic character except the double quotation mark ("), backslash (\), or 
    newline character. A wide string literal may contain the escape sequences listed
    above and any universal character name.



    TO DO: Make own class for handling validating number input from text boxes
           ... for C++/CX
           ... encapsulating max length, other bounds, radix etc.
           ... and then conversion to required type

           ... not sure if can derive from class?
           ... template class?
           ... library?

*/


void HelloWorld::MainPage::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

    static unsigned char OLDucharNumberToProcess = 0;

    xamlAnswerTextBlock->Text = L"RESULT: ";
    

    int myEndianType = TestByteOrder();
    std::wstring bigEndian = L"This machine appears to be BIG ENDIAN";
    std::wstring littleEndian = L"This machine appears to be LITTLE ENDIAN";

    if (myEndianType == BIG_ENDIAN)
    {
        xamlEndianTextBlock->Text = ref new String(bigEndian.c_str());
    }
    else
    {
        xamlEndianTextBlock->Text = ref new String(littleEndian.c_str());
    }

    // https://msdn.microsoft.com/en-us/library/hh699879.aspx
    // http://www.codeproject.com/Tips/473510/Super-simple-XAML-text-arithmetic-manipulation (not using this streaming method)


    String^ strNumberToProcess = xamlInputField->Text; // property Text - limited by XAML MaxLength property to 8 characters, hint (inputScope) = Number

    const wchar_t  *wchar_tNumberToProcess = strNumberToProcess->Data();  // Method Data - const wchar_t* like std::wstring::c_str()
                                                                          // beginning of data
    std::wstring wstrValidate = std::wstring(wchar_tNumberToProcess);
    xamlRegExTextBlock->Text = L"RegEx RESULT (check if input box is only Binary): ";
    
    std::wsmatch wideMatch;
    std::wregex wrx(L"[0-1]+");
    if (std::regex_match(wstrValidate.cbegin(), wstrValidate.cend(), wideMatch, wrx))
    {
        xamlRegExTextBlock->Text += L"TRUE";

        // http://www.cplusplus.com/reference/cstdlib/strtol/

        int binaryRadix = 2;
        wchar_t * pEnd; // I think it gets modified when a null terminator is encountered?
                        // Not quite sure about String^ and internal string and null terminiator and how that works with this

                        // DAVE ... check how these casts will work in practice and affect arithmetic etc.
        unsigned char ucharNumberToProcess = static_cast<unsigned char>(std::wcstoul(wchar_tNumberToProcess, &pEnd, binaryRadix));

        xamlSizeOfUnChar->Text = L"SizeOf unsigned char = ";
        wchar_t tmpBuff[2];
        errno_t tmpErr = 0;
        int tmpRadix = 10;
        tmpErr = _ultow_s(sizeof ucharNumberToProcess, tmpBuff, 2, tmpRadix);
        xamlSizeOfUnChar->Text += ref new String(tmpBuff) + " byte(s)";

        // if anything other than binary then ulNumberToProcess will be 0

        unsigned char ucharNumPlaces = static_cast<unsigned char>(intGlbNumPlaces);

        // DO STUFF HERE *********************************************************************************



        
        xamlCheckBox->Visibility = Windows::UI::Xaml::Visibility::Visible;
        



        /*  HOW TO DO BELOW??
        if (xamlCheckBox->IsChecked==WHAT?????? - CAN'T IMPLEMENT THE DATATYPE !!!)
        {
            ucharNumberToProcess = OLDucharNumberToProcess | ucharNumPlaces << ucharNumberToProcess;
        }
        else
        { */
            ucharNumberToProcess = ucharNumberToProcess << ucharNumPlaces;
     /*   }
        */
        OLDucharNumberToProcess = ucharNumberToProcess;
        
        // END OF DOING STUFF ****************************************************************************


        // CHECK number size for buffer issues etc. after arithmetic
        // i.e. less than 2^(myBufferSize-1)
        // UPDATE: just the static_cast seems to sort that out ... effectively masks it?
        unsigned long ulProcessedNumber = static_cast<unsigned long>(ucharNumberToProcess);

        // https://msdn.microsoft.com/en-us/library/kk8w4t5t.aspx



        int const myBufferSize = 9; // the 8 binary digits (unicode words) plus one for the \0 null termination

        wchar_t buffer[myBufferSize];

        errno_t zeroIfSuccess = 0;
        int myRadix = 2;

        zeroIfSuccess = _ultow_s(ulProcessedNumber, buffer, myBufferSize, myRadix); // _ultow_s ... s-suffix = security enhanced (buffer size)

        if (zeroIfSuccess == 0)
        {
            std::wstring withPadding = (L"00000000" + std::wstring(buffer));
            withPadding = withPadding.substr(withPadding.length() - 8, 8);

            // nb could have done, just String(buffer) if no padding etc.
            String^ myTest = ref new String(withPadding.c_str());

            xamlAnswerTextBlock->Text += myTest;
        }
        else
        {
            xamlCheckBox->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
            xamlAnswerTextBlock->Text += L"Error";
            xamlInputField->Text = L"";
        }
    }
    else
    {
        // failed regex
        xamlRegExTextBlock->Text += L"FALSE";
        xamlAnswerTextBlock->Text += L"Please only enter 1s and 0s (FIRST box)";
        xamlInputField->Text = L"";
    }
}




void HelloWorld::MainPage::xamlInputNumPlaces_KeyUp(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e)
{
    String^ strNumPlaces = xamlInputNumPlaces->Text; // property Text - limited by XAML MaxLength property to 1 characters, hint (inputScope) = Number

    const wchar_t  *wchar_tNumPlaces = strNumPlaces->Data();  // Method Data - const wchar_t* like std::wstring::c_str()
                                                                          // beginning of data
    std::wstring wstrValidate = std::wstring(wchar_tNumPlaces);
    // check if input box is only Binary

    std::wsmatch wideMatch;
    std::wregex wrx(L"[0-9]");
    if (std::regex_match(wstrValidate.cbegin(), wstrValidate.cend(), wideMatch, wrx))
    {
        // Number is 0 to 9 ... fine
      
        int denaryRadix = 10;
        wchar_t * pEnd; // I think it gets modified when a null terminator is encountered?
                        // Not quite sure about String^ and internal string and null terminiator and how that works with this

                        // DAVE ... check how these casts will work in practice and affect arithmetic etc.
        intGlbNumPlaces = static_cast<unsigned int>(std::wcstoul(wchar_tNumPlaces, &pEnd, denaryRadix));

    }
    else
    {
        xamlInputNumPlaces->Text = L"0";
    }
    xamlInputNumPlaces->SelectAll();

}


void HelloWorld::MainPage::xamlInputNumPlaces_GotFocus(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    xamlInputNumPlaces->SelectAll();
}




/* BELOW adapted FROM:
http://sridharpoduri.com/2012/05/21/implementing-an-ivalueconverter-in-c-cx-for-data-binding/

I did try a separate project, copying and pasting from the website, but still no joy ... 
problems and issues I didn't understanding
*/


Object^ MainPage::Convert(Object^ value, TypeName targetType, Object^ parameter, String^ language)
{
    String^ _grade = "";
    String^ _valueString = "";
    //try parsing the value to int
    int _value = ((Windows::Foundation::IPropertyValue^)value)->GetInt32();
    if (_value < 50)
        _grade = "F";
    else if (_value < 60)
        _grade = "D";
    else if (_value < 70)
        _grade = "C";
    else if (_value < 80)
        _grade = "B";
    else if (_value < 90)
        _grade = "A";
    else if (_value < 100)
        _grade = "A+";
    else if (_value == 100)
        _grade = "SUPER STAR!";
    return _grade;
}

Object^ MainPage::ConvertBack(Object^ value, TypeName targetType, Object^ parameter, String^ language)
{
    int _value = 0;
    String^ _grade = ((Windows::Foundation::IPropertyValue^)value)->GetString();
    if (_grade == "SUPER STAR!")
        _value = 100;
    else if (_grade == "A+")
        _value = 95;
    else if (_grade == "A")
        _value = 85;
    else if (_grade == "B")
        _value = 75;
    else if (_grade == "C")
        _value = 65;
    else if (_grade == "D")
        _value = 55;
    else if (_grade == "F")
        _value = 45;

    return _value;
}
