In "Debug Configuration" page in "Debugger" tab untick the "Verify flash downlaod", otherwise for some reason it you will get an error: "Data mismatch found at address 0x08080002 (byte = 0x00 instead of 0x0A)". Which is incomprehensible to me because STM32CubeProgrammer says that in fact at address 0x08080002 there is 0x0A.

[![Debug Configuration](/IMGs/DebugConfigurations.png)]

In "Debug Configuration" page in "Startup" tab don't forget to add "monitor flash mass_erase" or you will get an NMI Handler after flashing NonSecure changes (changing secure part only works).

[![Debug Configuration](/IMGs/DebugConfigurations2.png)]
