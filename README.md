This project walks you through setting up TrustZone on STM32H562 MCU using STM32CubeIDE and implementing AES encryption using the x-cube-Cryptolib. It also aims to compare the performance of AES encryption in TrustZone and Non-TrustZone configurations.

There are a few folders in this repository:
- TZ2/ - TrustZone project
- NOTZ/ - Non-TrustZone project. The same logic as in TZ2, but without TrustZone configuration (more info below).
- COMMON/ - common code for TrustZone and Non-TrustZone projects. Middlewares and AES code are here.
- IMGs/ - images for README.md

I added the NOTZ (Non-TrustZone) project to test the overhead of TrustZone configuration. The main.c logic is the same as in TZ2 project, but without TrustZone configuration. You can compare the performance of AES encryption in both projects.

Very important notes about setting up the project in STM32CubeIDE:

In "Debug Configuration" page in "Debugger" tab untick the "Verify flash downlaod", otherwise for some reason it you will get an error: "Data mismatch found at address 0x08080002 (byte = 0x00 instead of 0x0A)". Which is incomprehensible to me because STM32CubeProgrammer says that in fact at address 0x08080002 there is 0x0A.

![Debug Configuration](/IMGs/DebugConfigurations.png)

In "Debug Configuration" page in "Startup" tab don't forget to add "monitor flash mass_erase" or you will get an NMI Handler after flashing NonSecure changes (changing secure part only works). I think it's also worth it to use Hardware reset option on the main tab to force RAM reset.

![Debug Configuration](/IMGs/DebugConfigurations2.png)

Hardware setup = STM32H562 by WeAct Studio

![Setup](/IMGs/SETUP.jpg)


Code enables ICACHE (instruction cache) DCACHE (data cache) SWD and some GPIOs for LED control

RAM distribution was changed a little - by default it is 320KB RAM for Secure and NonSecure. I changed it to 48KB for Secure and 592 KB for NonSecure to fit 192KB of AES cipher.

![MMT](/IMGs/MemoryManagementTool.png)
