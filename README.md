# Twilight CRE
Twilight CRE is a kernel driver developed using the dispatch method for [RPM/WPM](https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/) through [IRP](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/wdm/ns-wdm-_irp), This driver was used in almost all our cheats under `Twilight Solutions` until the past few months, when we created a better driver with more legitimate methods and more powerful features, This driver was tested with BattleEye (Escape From Tarkov), specifically, and it worked with all TWS products until we replaced it with our newer version.

### Why Am I Releasing This?
The primary reason for releasing this is to help you with your homework, and of course, because we no longer need it.

### Build
- The driver was developed using WDK 1903, SDK 1903, and Visual Studio 2019
- After installing these tools, switch the build configuration to release mode and disable `Treat Warnings as Errors` in Visual Studio.


### About the driver
The driver uses the dispatch method to communicate with user mode and perform RPM/WPM, You can take a quick introduction [here](https://learn.microsoft.com/en-us/windows-hardware/drivers/gettingstarted/i-o-request-packets), The code contains some protection methods, but I have removed them, You just need to delete some headers.


### Disclaimer
Please note that this is not an undetected driver since it's public now, If you insist on using it, just do not map it with `KdMapper or gdrv`, I will not provide support for this project; it is provided as is, I am willing to share more code about game hacking, as mentioned in earlier repositories.


### Extra
You can check out my [EAC research](https://github.com/Mes2d/EAC), which is valuable if you are interested in game hacking, I have shared the EAC SDK along with some code excerpts from IDA, If you find it useful, please consider supporting my work by starring the repository.
