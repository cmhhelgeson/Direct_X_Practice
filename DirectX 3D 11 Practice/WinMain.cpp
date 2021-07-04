#include <Windows.h>
#include <stdint.h>
#include <unordered_map>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

#define internal static
#define local static
#define global static;

#define REGISTER_MESSAGE(msg){#msg}

typedef int32_t bool32;

global bool32 globalRunning = true;

void MessageToString(UINT message, LPARAM lp, WPARAM wp) {
	std::ostringstream oss;
	oss << "Message: " << message << '\n';
	oss << "   LP: 0x" << std::hex << std::setfill('0') << std::setw(8) << lp;
	oss << "   WP: 0x" << std::hex << std::setfill('0') << std::setw(8) << wp << '\n';
	OutputDebugString(oss.str().c_str());
}

//How to create a window
//Register a Windows Class, then create an instance of that class

/* Windows manages data for your window including messages.
	When you do something, it will put messages into a queue
	When you're ready, you call getmessage to pull message.
	You will do processing on those messages.
	
	TranslateMessage will generate a wm_char message.
	DispatchMessage passes message back to windows 32 side
	Every window has a pointer to a windows 32 procedure.*/

/* What is an HINSTANCE?
	An HINSTANCE is a handle to identify your application.
	Specifically, it's a handle that identifies the executable 
	file produced by your code from other exes in the same
	application?. A pointer to the memory address of the exe
	file. */

/* WinMain: 
	@commands:
	HInstance 1: Handle to the exe
	HInstance 2: Legacy variable from 16-bit Windows.N o longer used. 
	LPSTR: Command Line Arguments as Unicode String
	INT: Will the Application Start Minimized, Maximized, or Normal. 
	Go to MSDN Show Window function.
	@returns:
	Integer Status Code */

internal LRESULT CALLBACK
Win32MainWindowCallback(HWND Window,
	UINT Message,
	WPARAM WParam,
	LPARAM LParam)
{
	MessageToString(Message, LParam, WParam);
	switch (Message) {
	case WM_CLOSE: {
		PostQuitMessage(10);
		globalRunning = false;
		break;
	}
	}

	return DefWindowProc(Window, Message, WParam, LParam);
	
}
     

int CALLBACK WinMain(
	HINSTANCE Instance,
	HINSTANCE Prev_Instance,
	LPSTR Command,
	int Command_Show) {

	/*Remeber to Properties->Advanced->Use MultiByte Character Set */
	const auto class_name = "Direct X Practice";
	WNDCLASSEX window_class = { 0 };
	window_class.cbSize = sizeof(WNDCLASSEX);
	window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	window_class.lpfnWndProc = Win32MainWindowCallback;
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.lpszClassName = class_name;

	RegisterClassEx(&window_class);

	HWND window_handle =
		CreateWindowEx(0, class_name, "Direct X Practice",
			WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
			200, 200, 640, 480,
			nullptr, nullptr, Instance, nullptr);

	ShowWindow(window_handle, SW_MAXIMIZE);

	/* @from http://www.cplusplus.com/forum/beginner/38860/
		GetMessage: "will check the message queue for message, if
		there aren't any messages in the queue it will block.
		Blocking, in this case, would mean that GetMessage will wait
		for a valid message to pop up into the message queue"

		PeekMessage: "Return the first message or return nothing if
		there are no messages...genereally used in video games, or
		applications that need to do things without the user sending
		a message.*/

	MSG msg = { 0 };
	BOOL result;
	while (result = GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}