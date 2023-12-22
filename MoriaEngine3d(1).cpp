#include "framework.h"
#include "MoriaEngine3d(1).h"
#include "ImageLoader.h"
#include <cmath>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// Define a struct for a 2D vector
struct Vec2 {
    float x, y;
};

// Define a struct for a ray
struct Ray {
    Vec2 origin, direction;
};

// Define a struct for a sector
struct Sector {
    float floorHeight, ceilingHeight;
    ImageLoader texture;  // Texture for the sector
};

// Function to cast a ray into a sector
bool castRay(const Ray& ray, const Sector& sector, float& hitDistance) {
    // TODO: Implement raycasting algorithm
    // If the ray hits a wall in the sector, set hitDistance to the distance to the wall and return true
    // If the ray does not hit any wall, return false
    return false;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MORIAENGINE3D1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MORIAENGINE3D1));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MORIAENGINE3D1));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MORIAENGINE3D1);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // Define your sector
        Sector sector;
        sector.floorHeight = 1.0f;  // Set the floor height
        sector.ceilingHeight = 2.0f;  // Set the ceiling height
        sector.texture.load(L"texture.bmp");  // Load the texture

        // Cast a ray for each column of the window
        for (int x = 0; x < ps.rcPaint.right; ++x) {
            // Compute the direction of the ray
            Vec2 direction;
            direction.x = (float)x / ps.rcPaint.right - 0.5f;  // Map x from [0, width] to [-0.5, 0.5]
            direction.y = 1.0f;  // Rays go from top to bottom

            // Create the ray
            Ray ray;
            ray.origin.x = 0.0f;  // Ray starts at the center of the screen
            ray.origin.y = 0.0f;
            ray.direction = direction;

            // Cast the ray
            float hitDistance;
            if (castRay(ray, sector, hitDistance)) {
                // Compute the UV coordinates
                float u = hitPosition.x - std::floor(hitPosition.x);
                float v = (hitPosition.y - sector.floorHeight) / (sector.ceilingHeight - sector.floorHeight);

                // Look up the color in the texture
                COLORREF color = sector.texture.getPixel(static_cast<int>(u * textureWidth), static_cast<int>(v * textureHeight));

                // Draw the wall
                SetPixel(hdc, x, ps.rcPaint.bottom / 2 - wallTop, color);
            }
        }

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
