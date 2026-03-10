# Windows C++ KeyLogger (Educational Project)

A lightweight, invisible, and persistent keylogger written in C++ using the Windows API. This project was developed to explore Windows hooks, system persistence, and low-level keyboard event handling.

## ⚠️ DISCLAIMER
**This project is for EDUCATIONAL PURPOSES ONLY.** Unauthorized use of this software against computers you do not have explicit permission to monitor is illegal. The developer is not responsible for any misuse of this tool. Use this project to learn how to defend against such threats, not to create them.

## 🚀 Features
- **Invisible Execution**: Operates as a Windows Subsystem app; no terminal or console window appears.
- **Low-Level Hooking**: Utilizes `SetWindowsHookEx` (`WH_KEYBOARD_LL`) to capture keystrokes system-wide.
- **Full Symbol Support**: Correctly captures symbols (e.g., `@`, `!`, `"`, `<`) and handles Caps Lock/Shift logic via Unicode translation.
- **Stealthy Logging**: Saves data to a hidden file in `%LOCALAPPDATA%` with the `FILE_ATTRIBUTE_HIDDEN` flag set.
- **Persistence**: Copies itself to `%APPDATA%` and modifies the Registry (`Run` key) to survive system reboots.
- **Special Key Labels**: Identifies and labels `[ENTER]`, `[BACKSPACE]`, `[CTRL]`, `[TAB]`, etc.

## 📂 How to Use
1. **Download**: Navigate to the `x64/Release` folder in this repository and download `myKeyLogger.exe`.
2. **Antivirus**: You must add an **Exclusion** in Windows Security for the folder where you save this file, or it will be deleted immediately.
3. **Run**: Double-click the `.exe`. 
   - The program will instantly disappear.
   - It will install itself to `%APPDATA%\Microsoft\Windows\sys_driver.exe`.
4. **View Logs**: To see the recorded keystrokes, navigate to:
   `%LOCALAPPDATA%\Microsoft\Windows\Updates\data_log.txt`



## 🛠️ Build Information (For Developers)
If you wish to compile the code yourself:
- **Language**: C++
- **IDE**: Visual Studio 2022
- **Linker Settings**: 
    - SubSystem: `Windows (/SUBSYSTEM:WINDOWS)`
    - Entry Point: `mainCRTStartup`

## 🧹 How to Remove
To completely remove the logger from your system, you can use the provided `cleanup.bat` script or follow these manual steps:

### Manual Removal:
1. **Kill Process**: Open **Task Manager** > **Details** and end `sys_driver.exe`.
2. **Delete Registry Key**: 
   - Press `Win + R`, type `regedit`.
   - Go to `HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run`.
   - Delete the value `WindowsDriverUpdate`.
3. **Delete Files**:
   - Delete `%APPDATA%\Microsoft\Windows\sys_driver.exe`.
   - Delete the folder `%LOCALAPPDATA%\Microsoft\Windows\Updates`.



## 📜 License
This project is licensed under the MIT License - see the LICENSE file for details.
