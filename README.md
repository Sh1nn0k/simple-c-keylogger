# Simple C Keylogger

A lightweight low-level keyboard event logger written in C for Windows. This project demonstrates how to interface with the Windows API to capture keyboard input globally using system hooks.

## Educational Purpose

This project was developed for **educational and research purposes only**. It aims to demonstrate:
* Interaction with the Windows API (`winuser.h`).
* Usage of Low-Level Hooks (`WH_KEYBOARD_LL`).
* Handling of Virtual-Key Codes and keyboard states (Shift, Caps Lock).
* Event-driven programming with the Windows Message Loop.

## Features

* Global Hooking: Captures keystrokes even when the application is not in focus.
* Case Sensitivity: Correctly handles uppercase and lowercase letters by checking `Shift` and `CapsLock` states.
* Special Key Detection: Identifies and logs non-printable keys such as `[ENTER]`, `[TAB]`, `[BACKSPACE]`, and arrow keys.
* File Logging: Persists captured data to a local file named `klog.log`.

## Technical Implementation Details

### Keyboard Hook
The application utilizes `SetWindowsHookExA` with the `WH_KEYBOARD_LL` parameter. This allows the program to monitor keyboard input events before they reach the target application.

### Character Mapping
Instead of just logging raw scan codes, the project implements `GetCharFromVkCode`, which translates Virtual-Key codes into readable characters, including special symbols (e.g., `!`, `@`, `#`) when the `Shift` key is active.

### Message Loop
To keep the hook active, the program implements a standard Windows `MSG` loop (`GetMessage`), which is required to process the system's hook notifications.

## Prerequisites

* Windows Operating System.
* A C compiler (e.g., MinGW, MSVC).
* Windows API libraries (usually included with standard Windows C compilers).

## Compilation

Using MinGW/GCC:
`gcc main.c -o keylogger.exe -luser32 -lgdi32`
