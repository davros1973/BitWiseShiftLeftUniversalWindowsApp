//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"

using namespace Platform;  // includes TypeName
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Interop;

namespace HelloWorld
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
    /// http://sridharpoduri.com/2012/05/21/implementing-an-ivalueconverter-in-c-cx-for-data-binding/
	/// </summary>
	public ref class MainPage sealed : Windows::UI::Xaml::Data::IValueConverter
	{
	public:
		MainPage();
        virtual ~MainPage();
        virtual Object^ Convert(Object^ value, TypeName targetType, Object^ parameter, String^ language);
        virtual Object^ ConvertBack(Object^ value, TypeName targetType, Object^ parameter, String^ language);

    private:
        void Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

        void xamlInputNumPlaces_KeyUp(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e);
        void xamlInputNumPlaces_GotFocus(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
    };
}
