<p align="center"><img src="https://i.imgur.com/7YOOfqF.png" align="center" alt="Zodiak" width="250" align="center"></p>

# Zodiak v2
**Zodiak** is a full kernel driver cheat for CS 1.6. The cheat is designed to leave zero traces in the system to avoid detection, so there are no config files or anything else that could leave a trace.

The ESP is implemented by exporting GDI functions, allowing it to work in CS 1.6. The aimbot is created by exploiting the MouHID driver and callling own `MouseClassServiceCallback`.

In the end, it’s similar to ekknod's EC cheat but adapted for CS 1.6, because it uses the same techniques. I hope ekknod won’t mind :3


## General
- Written in C and assembly
- Automatic offsets detection
- Thread context spoofing
- Operates in a single thread


## Limits
- Works only on steam version (pre-25th anniversary build)
- Esp work typically only in windowed fullscreen mode, but for someone it can also work in fullscreen. If someone have two or more monitors it will work in fullscreen.
- Supported only stretched resolution, black bars are not supported (use -stretchaspec or -nofbo)


## Bypass
- Fastcup
- Wargods 
- Any other server side anticheat
- Demochecker


## Features
- **Aimbot** (calling "own" MouseClassServiceCallback)
- **ESP** (GDI kernel drawing without hooks)
- **NoSpread** (Basic and beta version. Held down MOUSE1, aim slowly slide down)


## Usage
1) Disable antivirus because windows detects kdmapper file as virus.
2) Download zip file and extract.
3) Open ESP Fix.reg file under files folder.
4) Open Test ON.bat file and reboot computer.
5) For AWP setting, use Zodiak.rc file
6) Open Zodiak shortcut under main folder. Hack is activated.
7) If you want disable test signing mode, open Test OFF.bat file and reboot computer.


## Note
- This cheat after opened, cannot terminate. You should reboot computer for terminate.
- You only need to open the Zodiak.bat file once. If you open it again, you will get a blue screen error !!!
- For Aim AWP settings, scope mode not working right now. You should use Zodiak.rc configs shoot without scope. (exec Zodiak.rc) (Press F1 key and use MOUSE3 click to shoot with AWP)


### Default Keybinds
 **F6**: Toggle Aimbot
 
 **F7**: Toggle ESP
 
 **F8**: Toggle "Only Enemies" Mode
 
 **F9 or DEL**: Toggle "NoSpread" Mode
 
 **F10 or C**: Aim Rifle and SMG settings (Max. NoRecoil, Aim Position: Head, MinAimFov: 30, MaxAimFov: 100, AimSmooth: 2.5)
 
 **F11 or F**: Pistol settings (Disable NoRecoil, Aim Position: Head, MinAimFov: 60, MaxAimFov: 300, AimSmooth: 2.5)
 
 **F12 or V**: AWP settings (Disable NoRecoil, Aim Position: Chest, AimFov: 100, MaxAimFov: 300, AimSmooth: 2)
 
 **INSERT**: Default settings (Medium NoRecoil, Aim Position: Head and Neck, AimFov: 100, MaxAimSmooth: 4)
 
 **Home**: Adjust FOV+ (Default: 100, once key press increase 5)
 
 **End**: Adjust FOV- (Minumum: 0, once key press decrease 5)
 
 **PgUp**: Adjust Smooth+ (Default: 4, once key press increase 0.5)
 
 **PgDown**: Adjust Smooth- (Minumum: 0, once key press decrease 0.5)
 
 **Numpad +**: Aim Position Up
 
 **Numpad -**: Aim Position Down


## Credits
- Developer Page: https://github.com/3a1/Zodiak
- [Ekknod](https://github.com/ekknod) for the simple and effective GDI implementation, mouse callback trick, and overall for all his publicly available projects, especially the EC project, which I believe has one of the best structures I've seen in external cheats.
- [Unmaewei](https://github.com/unmaewei) for his Full Kernel Driver Cheat. I borrowed some general driver operations from there.
