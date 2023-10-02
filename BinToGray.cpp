#include <windows.h>
#include <string>
#include <iostream>

std::string DecimalToBinary(int decimal) {
    if (decimal == 0)
        return "0";

    std::string binary = "";
    while (decimal > 0) {
        int remainder = decimal % 2;
        binary = std::to_string(remainder) + binary;
        decimal /= 2;
    }

    return binary;
}

std::string binaryToGray(const std::string& binary) {
    std::string gray = "";
    gray += binary[0];  // The first bit remains the same in Gray code

    for (size_t i = 1; i < binary.length(); i++) {
        gray += (binary[i - 1] == '0') ? binary[i] : (binary[i] == '0' ? '1' : '0');
    }

    return gray;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

HWND hMainWindow;
HINSTANCE hMainInstance;

HWND hTxtInput;
HWND hButton;
HWND hLblOutput;

#define IDC_TEXTBOX 1000
#define IDC_BUTTON  1001
#define IDC_STATIC  1002

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MSG msg = { 0 };
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
    wc.lpszClassName = TEXT("GrayToBinWindowsApp");

    if (!RegisterClass(&wc))
        return 1;

    hMainWindow = CreateWindow(
        wc.lpszClassName,
        TEXT("Grey Code to Bin Converter"),
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        0, 0, 640, 480, 0, 0, hInstance, NULL);
    
    hMainInstance = wc.hInstance;

    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_CREATE:
        hMainWindow = hWnd;
        hTxtInput = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT(""),
            WS_VISIBLE | WS_CHILD | ES_CENTER | ES_NUMBER, 
            50, 50, 380, 25,
            hWnd,
            (HMENU)IDC_TEXTBOX, hMainInstance, NULL);
        hButton = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("BUTTON"), TEXT("Convert To Gray"), 
            WS_VISIBLE | WS_CHILD | ES_LEFT, 
            200, 100, 200, 32,
            hWnd,
            (HMENU)IDC_BUTTON, hMainInstance, NULL);
        hLblOutput = CreateWindowEx(WS_EX_STATICEDGE, TEXT("STATIC"), TEXT("Gray Code"),
            WS_VISIBLE | WS_CHILD | ES_CENTER,
            50, 200, 380, 25,
            hWnd,
            (HMENU)IDC_STATIC, hMainInstance, NULL);
        break;
    
    case WM_SIZE:
        // center the button
        RECT rc;
        GetClientRect(hWnd, &rc);
        SetWindowPos(hButton, NULL, (rc.right - rc.left - 200) / 2, 100, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

        // center the label
        SetWindowPos(hLblOutput, NULL, (rc.right - rc.left - 380) / 2, 200, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

        // center the textbox
        SetWindowPos(hTxtInput, NULL, (rc.right - rc.left - 380) / 2, 50, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BUTTON) {
            std::string input;
            GetWindowText(hTxtInput, (TCHAR*)input.c_str(), 100);

            // Convert only binary part to integer if input doesn't start with binary number
            // output the invalid input 
            if (input[0] != '0' && input[0] != '1') {
                SetWindowText(GetDlgItem(hMainWindow, IDC_STATIC), TEXT("Invalid Input: Only Binary accepted"));
                break;
            }

            int decimal = stoi(input, 0, 2);  // Convert only binary part to integer
            input = DecimalToBinary(decimal);   // Only binary part decimal to binary
            input = binaryToGray(input);   // finally convert binary to gray

            SetWindowText(GetDlgItem(hMainWindow, IDC_STATIC), (TCHAR*)input.c_str());
        }
        break;

    case WM_CLOSE:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}